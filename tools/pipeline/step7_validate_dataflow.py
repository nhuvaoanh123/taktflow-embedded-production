#!/usr/bin/env python3
"""
Pipeline Step 7: Data Flow Validation

Enforces strict data flow rules:
  SWC code → Rte_Read/Rte_Write ONLY → Com handles CAN TX/RX

Violations:
  - SWC calling PduR_Transmit (bypasses Com throttle + E2E)
  - SWC calling E2E_Protect/E2E_Check (E2E belongs in Com layer)
  - SWC calling CanIf_Transmit/Can_Write (bypasses entire stack)
  - SWC (non-Com) calling Com_SendSignal (should go through Rte)

Allowed:
  - Swc_*Com.c may call Com_SendSignal (Com bridge SWC)
  - BSW modules (Com.c, PduR.c, CanIf.c) may call anything
  - main.c may call init functions

This enforces the AUTOSAR data flow:
  Sensor → IoHwAb → SWC Rte_Write → Com packs + E2E → PduR → CanIf → CAN
  CAN → CanIf → PduR → Com unpacks + E2E check → Rte_Read → SWC → Actuator

@traces_to Phase 2 architecture: all TX through Com
"""

import sys
import os
import re
import glob

FW_DIR = "firmware/ecu"

# BSW APIs that SWCs must NOT call directly
FORBIDDEN_IN_SWC = [
    ("PduR_Transmit", "bypasses Com throttle and E2E — use Rte_Write + Com"),
    ("PduR_ComTransmit", "bypasses Com — use Rte_Write"),
    ("CanIf_Transmit", "bypasses PduR and Com — use Rte_Write"),
    ("Can_Write", "bypasses entire BSW stack — use Rte_Write"),
    ("Can_Hw_Transmit", "bypasses entire BSW stack — use Rte_Write"),
    ("E2E_Protect", "E2E belongs in Com layer, not SWC — Phase 2 fix"),
    ("E2E_Check", "E2E belongs in Com layer, not SWC — Phase 2 fix"),
]

# Com_SendSignal allowed only in *Com.c bridge SWCs
COM_SEND_ALLOWED_PATTERN = re.compile(r"Swc_\w*Com\w*\.c$")


def is_swc_file(filepath):
    """True if this is an SWC source file (not BSW, not main, not cfg)."""
    basename = os.path.basename(filepath)
    if not basename.startswith("Swc_"):
        return False
    if basename.endswith("_test.c"):
        return False
    return True


def run():
    violations = []
    files_checked = 0

    print("Step 7: Data Flow Validation")
    print()

    for ecu_dir in sorted(glob.glob(os.path.join(FW_DIR, "*", "src"))):
        ecu_name = os.path.basename(os.path.dirname(ecu_dir))

        for src_file in sorted(glob.glob(os.path.join(ecu_dir, "Swc_*.c"))):
            if not is_swc_file(src_file):
                continue

            basename = os.path.basename(src_file)
            files_checked += 1
            is_com_bridge = COM_SEND_ALLOWED_PATTERN.match(basename)

            with open(src_file, 'r', errors='replace') as f:
                lines = f.readlines()

            for line_num, line in enumerate(lines, 1):
                stripped = line.strip()

                # Skip comments
                if stripped.startswith("//") or stripped.startswith("*") or stripped.startswith("/*"):
                    continue

                # Check forbidden BSW API calls
                for api, reason in FORBIDDEN_IN_SWC:
                    if api + "(" in stripped:
                        violations.append({
                            'file': f"{ecu_name}/src/{basename}",
                            'line': line_num,
                            'api': api,
                            'reason': reason,
                            'severity': 'ERROR',
                        })

                # Check Com_SendSignal in non-Com SWCs
                if "Com_SendSignal(" in stripped and not is_com_bridge:
                    violations.append({
                        'file': f"{ecu_name}/src/{basename}",
                        'line': line_num,
                        'api': 'Com_SendSignal',
                        'reason': 'only allowed in *Com.c bridge SWC — use Rte_Write instead',
                        'severity': 'WARNING',
                    })

    # Report
    errors = [v for v in violations if v['severity'] == 'ERROR']
    warnings = [v for v in violations if v['severity'] == 'WARNING']

    print(f"  Files checked: {files_checked}")
    print(f"  Errors:   {len(errors)}")
    print(f"  Warnings: {len(warnings)}")
    print()

    if errors:
        print("  ERRORS (must fix for Phase 2):")
        for v in errors:
            print(f"    {v['file']}:{v['line']} — {v['api']}() — {v['reason']}")
        print()

    if warnings:
        print("  WARNINGS (should fix):")
        for v in warnings:
            print(f"    {v['file']}:{v['line']} — {v['api']}() — {v['reason']}")
        print()

    # Summary per ECU
    ecu_counts = {}
    for v in violations:
        ecu = v['file'].split('/')[0]
        ecu_counts[ecu] = ecu_counts.get(ecu, 0) + 1
    if ecu_counts:
        print("  Per-ECU violation count:")
        for ecu, count in sorted(ecu_counts.items()):
            print(f"    {ecu}: {count}")
        print()

    total = len(violations)
    if total == 0:
        print("PASS — all SWCs use strict Rte_Read/Rte_Write data flow")
        return 0
    else:
        print(f"FAIL — {total} data flow violations found")
        print(f"  These MUST be fixed in Phase 2 (E2E in Com) and Phase 3 (SWC cleanup)")
        return 1


if __name__ == "__main__":
    sys.exit(run())
