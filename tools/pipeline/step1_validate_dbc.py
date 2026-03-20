#!/usr/bin/env python3
"""
Pipeline Step 1: DBC Validation (12-point ISO 26262 audit)

Input:  gateway/taktflow_vehicle.dbc
Output: PASS/FAIL + issue list
Gate:   STOP if any issue found

Traces to: Phase 0 DBC audit findings, HARA safety goals
"""

import sys
import os
import cantools

DBC_PATH = os.environ.get("DBC_PATH", "gateway/taktflow_vehicle.dbc")

# FTTI budgets from HARA (ms)
FTTI = {
    'EStop_Broadcast': 100, 'CVC_Heartbeat': 500, 'FZC_Heartbeat': 500,
    'RZC_Heartbeat': 500, 'Vehicle_State': 50, 'Torque_Request': 50,
    'Steer_Command': 100, 'Brake_Command': 100, 'Steering_Status': 200,
    'Brake_Status': 200, 'Motor_Status': 500, 'Motor_Current': 1000,
    'Motor_Temperature': 1000, 'SC_Status': 500, 'Motor_Cutoff_Req': 200,
}

# Motor_Temperature accepted deviation (ASIL A, timeout-based)
E2E_DEVIATIONS = {'Motor_Temperature'}


def get_attr(msg, name, default=None):
    try:
        attr = msg.dbc.attributes.get(name)
        if attr is None:
            return default
        return attr.value if hasattr(attr, 'value') else attr
    except Exception:
        return default


def run(dbc_path):
    db = cantools.database.load_file(dbc_path)
    issues = []
    checks_passed = 0
    checks_total = 0

    def check(name, condition, detail=""):
        nonlocal checks_passed, checks_total
        checks_total += 1
        if condition:
            checks_passed += 1
            print(f"  [PASS] {name}")
        else:
            issues.append(f"{name}: {detail}")
            print(f"  [FAIL] {name} -- {detail}")

    print(f"Step 1: DBC Validation ({dbc_path})")
    print(f"  Messages: {len(db.messages)}, Signals: {sum(len(m.signals) for m in db.messages)}")
    print()

    # 1. E2E DataID uniqueness
    e2e_map = {}
    dups = []
    for msg in db.messages:
        e2e = get_attr(msg, 'E2E_DataID')
        if e2e is not None:
            v = int(e2e)
            if v in e2e_map:
                dups.append(f"DataID={v}: {msg.name} and {e2e_map[v]}")
            e2e_map[v] = msg.name
    check("1. E2E DataID uniqueness", len(dups) == 0, f"{len(dups)} duplicates: {dups}")

    # 2. E2E DataID 4-bit range
    over = [f"{n}={v}" for v, n in e2e_map.items() if v > 15]
    check("2. E2E DataID 4-bit range (0-15)", len(over) == 0, f"Overflow: {over}")

    # 3. ASIL messages have E2E (with deviation list)
    missing = []
    for msg in db.messages:
        asil = str(get_attr(msg, 'ASIL', 'QM'))
        if asil != 'QM' and msg.name not in E2E_DEVIATIONS:
            if get_attr(msg, 'E2E_DataID') is None:
                missing.append(f"{msg.name} ASIL={asil}")
    check("3. ASIL messages have E2E", len(missing) == 0, f"Missing: {missing}")

    # 4. GenSigStartValue for ASIL signals
    miss_sv = 0
    for msg in db.messages:
        asil = str(get_attr(msg, 'ASIL', 'QM'))
        if asil != 'QM':
            for sig in msg.signals:
                try:
                    if sig.dbc.attributes.get('GenSigStartValue') is None:
                        miss_sv += 1
                except Exception:
                    miss_sv += 1
    check("4. GenSigStartValue for ASIL signals", miss_sv == 0, f"{miss_sv} missing")

    # 5. FTTI budget
    ftti_fails = []
    for msg in db.messages:
        if msg.name not in FTTI:
            continue
        cycle = int(get_attr(msg, 'GenMsgCycleTime', 0) or 0)
        if cycle == 0:
            continue
        max_delta = int(get_attr(msg, 'E2E_MaxDeltaCounter', 2) or 2)
        t_total = cycle * (max_delta + 1) + 30
        if t_total > FTTI[msg.name]:
            ftti_fails.append(f"{msg.name}: {t_total}ms > {FTTI[msg.name]}ms")
    check("5. FTTI budget compliance", len(ftti_fails) == 0, f"Exceeded: {ftti_fails}")

    # 6. Bus load < 70%
    load = 0
    for m in db.messages:
        c = int(get_attr(m, 'GenMsgCycleTime', 0) or 0)
        if c > 0:
            load += (47 + m.length * 8) * 1.2 * (1000 / c) / 500000 * 100
    check(f"6. Bus load < 70% (actual: {load:.1f}%)", load < 70)

    # 7. Signal overlap
    overlaps = []
    for msg in db.messages:
        used = set()
        for sig in msg.signals:
            for b in range(sig.start, sig.start + sig.length):
                if b in used:
                    overlaps.append(f"{msg.name}.{sig.name} bit {b}")
                    break
                used.add(b)
    check("7. Signal overlap", len(overlaps) == 0, f"Overlaps: {overlaps}")

    # 8. DLC consistency
    dlc_fails = []
    for msg in db.messages:
        mx = max((s.start + s.length for s in msg.signals), default=0)
        if (mx + 7) // 8 > msg.length:
            dlc_fails.append(f"{msg.name}: needs {(mx + 7) // 8}B, has {msg.length}B")
    check("8. DLC consistency", len(dlc_fails) == 0, f"Fails: {dlc_fails}")

    # 9. Signal name uniqueness
    sigs = [s.name for m in db.messages for s in m.signals]
    dupes = len([s for s in set(sigs) if sigs.count(s) > 1])
    check("9. Signal name uniqueness", dupes == 0, f"{dupes} duplicates")

    # 10. Event retransmission for ASIL D events
    event_fails = []
    for msg in db.messages:
        asil = str(get_attr(msg, 'ASIL', 'QM'))
        cycle = int(get_attr(msg, 'GenMsgCycleTime', 0) or 0)
        if asil in ('C', 'D') and cycle == 0:
            fast = get_attr(msg, 'GenMsgCycleTimeFast')
            reps = get_attr(msg, 'GenMsgNrOfRepetition')
            if not fast or not reps:
                event_fails.append(f"{msg.name} ASIL={asil}")
    check("10. Event retransmission for ASIL events", len(event_fails) == 0, f"Missing: {event_fails}")

    # 11. E2E_MaxDeltaCounter present
    has_maxd = any(get_attr(m, 'E2E_MaxDeltaCounter') is not None for m in db.messages)
    check("11. E2E_MaxDeltaCounter attribute defined", has_maxd)

    # 12. Satisfies traceability
    has_sat = sum(1 for m in db.messages if get_attr(m, 'Satisfies'))
    check(f"12. Satisfies traceability ({has_sat} messages linked)", has_sat > 0)

    print()
    print(f"Result: {checks_passed}/{checks_total} checks passed, {len(issues)} issues")
    if issues:
        print("FAIL — fix issues before proceeding to Step 2")
        for i in issues:
            print(f"  {i}")
        return 1
    else:
        print("PASS — DBC validated, proceed to Step 2")
        return 0


if __name__ == "__main__":
    path = sys.argv[1] if len(sys.argv) > 1 else DBC_PATH
    sys.exit(run(path))
