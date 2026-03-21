#!/usr/bin/env python3
"""
gen_signal_tests.py — Generate Com signal pack/unpack unit tests from DBC + Com_Cfg.

Reads:
  - gateway/taktflow_vehicle.dbc (cantools)
  - firmware/ecu/cvc/cfg/Com_Cfg_Cvc.c (regex parse for signal config table)
  - firmware/ecu/cvc/include/Cvc_Cfg.h (CVC_COM_SIG_* defines)

Generates:
  - test/unit/bsw/test_Com_signals_generated.c   (Unity test file)
  - test/unit/bsw/test_Com_signals_runner.sh      (compile+run script)

For each non-E2E signal (122 signals), generates 3 tests:
  - Normal value (mid-range)
  - Boundary value (max for bit width)
  - Zero value

TX signals: Com_SendSignal → verify PDU buffer bytes
RX signals: build PDU buffer → Com_RxIndication → Com_ReceiveSignal → verify
Sub-byte signals: verify neighboring bits are NOT corrupted
16-bit signals: verify both bytes (little-endian)
Signed signals: test negative values
"""

import cantools
import re
import os
import sys
from pathlib import Path

# ── Paths ──────────────────────────────────────────────────────────────
REPO = Path(__file__).resolve().parent.parent.parent
DBC_PATH = REPO / "gateway" / "taktflow_vehicle.dbc"
CFG_C_PATH = REPO / "firmware" / "ecu" / "cvc" / "cfg" / "Com_Cfg_Cvc.c"
CFG_H_PATH = REPO / "firmware" / "ecu" / "cvc" / "include" / "Cvc_Cfg.h"
OUT_C = REPO / "test" / "unit" / "bsw" / "test_Com_signals_generated.c"
OUT_SH = REPO / "test" / "unit" / "bsw" / "test_Com_signals_runner.sh"


def parse_com_sig_defines(cfg_h_path: Path) -> dict:
    """Parse CVC_COM_SIG_* defines from Cvc_Cfg.h -> {name: int_value}."""
    defines = {}
    pattern = re.compile(r"#define\s+(CVC_COM_SIG_\w+)\s+(\d+)u")
    with open(cfg_h_path, "r") as f:
        for line in f:
            m = pattern.match(line.strip())
            if m:
                defines[m.group(1)] = int(m.group(2))
    return defines


def parse_com_tx_pdu_defines(cfg_h_path: Path) -> dict:
    """Parse CVC_COM_TX_* defines from Cvc_Cfg.h -> {name: int_value}."""
    defines = {}
    pattern = re.compile(r"#define\s+(CVC_COM_TX_\w+)\s+(\d+)u")
    with open(cfg_h_path, "r") as f:
        for line in f:
            m = pattern.match(line.strip())
            if m:
                defines[m.group(1)] = int(m.group(2))
    return defines


def parse_com_rx_pdu_defines(cfg_h_path: Path) -> dict:
    """Parse CVC_COM_RX_* defines from Cvc_Cfg.h -> {name: int_value}."""
    defines = {}
    pattern = re.compile(r"#define\s+(CVC_COM_RX_\w+)\s+(\d+)u")
    with open(cfg_h_path, "r") as f:
        for line in f:
            m = pattern.match(line.strip())
            if m:
                defines[m.group(1)] = int(m.group(2))
    return defines


def parse_signal_config_table(cfg_c_path: Path) -> list:
    """Parse cvc_signal_config[] entries from Com_Cfg_Cvc.c.

    Returns list of dicts: {id, bitPos, bitSize, type_str, pdu_define, is_tx}.
    """
    signals = []
    # Match: { 14u,   16u,     4u, COM_UINT8, CVC_COM_TX_VEHICLE_STATE, ...
    pattern = re.compile(
        r"\{\s*(\d+)u\s*,\s*(\d+)u\s*,\s*(\d+)u\s*,\s*(COM_\w+)\s*,"
        r"\s*(CVC_COM_(?:TX|RX)_\w+)\s*,"
    )
    with open(cfg_c_path, "r") as f:
        for line in f:
            m = pattern.search(line)
            if m:
                sig_id = int(m.group(1))
                bit_pos = int(m.group(2))
                bit_size = int(m.group(3))
                type_str = m.group(4)
                pdu_define = m.group(5)
                is_tx = "_TX_" in pdu_define
                signals.append({
                    "id": sig_id,
                    "bitPos": bit_pos,
                    "bitSize": bit_size,
                    "type_str": type_str,
                    "pdu_define": pdu_define,
                    "is_tx": is_tx,
                })
    return signals


def camel_to_upper_snake(name: str) -> str:
    """Convert CamelCase to UPPER_SNAKE_CASE, preserving existing underscores.

    Examples:
      OperatingMode    -> OPERATING_MODE
      FaultMask        -> FAULT_MASK
      PedalPosition1   -> PEDAL_POSITION_1
      SteerAngleCmd    -> STEER_ANGLE_CMD
      BatteryVoltage_mV -> BATTERY_VOLTAGE_M_V
      BrakeCurrent_mA  -> BRAKE_CURRENT_M_A
      MotorTemp_dC     -> MOTOR_TEMP_D_C
      HeadlightCmd     -> HEADLIGHT_CMD
      TailLightOn      -> TAIL_LIGHT_ON
      DirIsReverse     -> DIR_IS_REVERSE
    """
    # Split on existing underscores first
    parts = name.split("_")
    result_parts = []
    for part in parts:
        # Insert underscore before each uppercase letter that follows a lowercase letter or digit
        expanded = re.sub(r'([a-z])([A-Z])', r'\1_\2', part)
        expanded = re.sub(r'([A-Z]+)([A-Z][a-z])', r'\1_\2', expanded)
        # Split digits from preceding alpha: Position1 -> Position_1
        expanded = re.sub(r'([a-zA-Z])(\d)', r'\1_\2', expanded)
        result_parts.append(expanded.upper())
    return "_".join(result_parts)


def find_com_sig_define(dbc_sig_name: str, msg_name: str, com_sig_defines: dict,
                        is_tx: bool) -> str | None:
    """Map a DBC signal name to its CVC_COM_SIG_* define name."""
    # Convert CamelCase parts to UPPER_SNAKE
    upper_snake = camel_to_upper_snake(dbc_sig_name)
    candidate = "CVC_COM_SIG_" + upper_snake
    if candidate in com_sig_defines:
        return candidate

    # E2E naming: E2E -> E_2_E, CRC8 -> CRC_8
    candidate2 = candidate.replace("E2E", "E_2_E").replace("CRC8", "CRC_8")
    if candidate2 in com_sig_defines:
        return candidate2

    # Fallback: plain uppercase (no CamelCase splitting)
    plain = "CVC_COM_SIG_" + dbc_sig_name.upper()
    if plain in com_sig_defines:
        return plain

    # Try also with E2E transform on plain
    plain2 = plain.replace("E2E", "E_2_E").replace("CRC8", "CRC_8")
    if plain2 in com_sig_defines:
        return plain2

    return None


def is_e2e_signal(sig_name: str) -> bool:
    """Return True if this is an E2E overhead signal (DataID, AliveCounter, CRC8)."""
    low = sig_name.lower()
    return ("e2e_dataid" in low or "e2e_alivecounter" in low or
            "e2e_crc8" in low or "e2e_data_id" in low or
            "e2e_alive_counter" in low or "_crc8" in low.replace("e2e_", ""))


def c_type_for(type_str: str, bit_size: int, is_signed: bool) -> str:
    """Return C type string for a signal."""
    if is_signed:
        return "sint16"
    if type_str == "COM_UINT16" or type_str == "COM_SINT16":
        return "uint16" if not is_signed else "sint16"
    if type_str == "COM_UINT32":
        return "uint32"
    return "uint8"


def main():
    print(f"[gen_signal_tests] Loading DBC: {DBC_PATH}")
    db = cantools.database.load_file(str(DBC_PATH))

    com_sig_defines = parse_com_sig_defines(CFG_H_PATH)
    tx_pdu_defines = parse_com_tx_pdu_defines(CFG_H_PATH)
    rx_pdu_defines = parse_com_rx_pdu_defines(CFG_H_PATH)
    cfg_signals = parse_signal_config_table(CFG_C_PATH)

    # Build lookup: com_sig_id -> cfg entry
    cfg_by_id = {s["id"]: s for s in cfg_signals}

    # Collect test-worthy signals: non-E2E, non-64-bit, CVC is sender or receiver
    test_signals = []
    for msg in db.messages:
        for sig in msg.signals:
            if is_e2e_signal(sig.name):
                continue
            # Skip 64-bit signals (XCP/UDS data blobs)
            if sig.length > 16:
                continue

            # Determine if TX or RX from CVC perspective
            is_tx = (msg.senders and "CVC" in msg.senders)
            is_rx = ("CVC" in [n for r in sig.receivers for n in ([r] if isinstance(r, str) else r)])

            if not is_tx and not is_rx:
                continue

            # Find the CVC_COM_SIG_* define
            define_name = find_com_sig_define(sig.name, msg.name, com_sig_defines, is_tx)
            if define_name is None:
                print(f"  WARN: no CVC_COM_SIG_* for {sig.name}, skipping")
                continue

            sig_id = com_sig_defines[define_name]
            cfg = cfg_by_id.get(sig_id)
            if cfg is None:
                print(f"  WARN: no config table entry for {define_name} (id={sig_id}), skipping")
                continue

            test_signals.append({
                "dbc_sig": sig,
                "msg": msg,
                "define_name": define_name,
                "sig_id": sig_id,
                "cfg": cfg,
                "is_tx": cfg["is_tx"],
            })

    print(f"[gen_signal_tests] Found {len(test_signals)} testable signals")

    # ── Generate C test file ─────────────────────────────────────────
    lines = []
    lines.append("/**")
    lines.append(" * @file    test_Com_signals_generated.c")
    lines.append(" * @brief   Auto-generated Com signal pack/unpack tests")
    lines.append(" *")
    lines.append(" * GENERATED BY tools/test_gen/gen_signal_tests.py -- DO NOT EDIT")
    lines.append(" *")
    lines.append(" * @standard AUTOSAR, ISO 26262 Part 6")
    lines.append(" */")
    lines.append("#include \"unity.h\"")
    lines.append("#include \"Com.h\"")
    lines.append("#include \"Cvc_Cfg.h\"")
    lines.append("")
    lines.append("/* ---- Test infrastructure stubs ---- */")
    lines.append("")
    lines.append("/* Expose internal PDU buffers for verification */")
    lines.append("extern uint8 com_tx_pdu_buf[][8];")
    lines.append("extern uint8 com_rx_pdu_buf[][8];")
    lines.append("")
    lines.append("/* Forward declarations for Com init with CVC config */")
    lines.append("extern const Com_ConfigType cvc_com_config;")
    lines.append("")
    lines.append("void setUp(void) {")
    lines.append("    Com_Init(&cvc_com_config);")
    lines.append("    /* Clear all PDU buffers */")
    lines.append("    for (int i = 0; i < COM_MAX_PDUS; i++) {")
    lines.append("        for (int j = 0; j < 8; j++) {")
    lines.append("            com_tx_pdu_buf[i][j] = 0x00u;")
    lines.append("            com_rx_pdu_buf[i][j] = 0x00u;")
    lines.append("        }")
    lines.append("    }")
    lines.append("}")
    lines.append("")
    lines.append("void tearDown(void) {")
    lines.append("    /* nothing */")
    lines.append("}")
    lines.append("")

    test_names = []

    for ts in test_signals:
        sig = ts["dbc_sig"]
        cfg = ts["cfg"]
        define = ts["define_name"]
        is_tx = ts["is_tx"]
        bit_pos = cfg["bitPos"]
        bit_size = cfg["bitSize"]
        type_str = cfg["type_str"]
        pdu_define = cfg["pdu_define"]
        is_signed = sig.is_signed
        byte_offset = bit_pos // 8
        shift = bit_pos % 8

        # Determine C type
        if is_signed and bit_size > 8:
            c_type = "sint16"
        elif type_str in ("COM_UINT16", "COM_SINT16"):
            c_type = "uint16" if not is_signed else "sint16"
        elif type_str == "COM_UINT32":
            c_type = "uint32"
        else:
            c_type = "uint8"

        # Max raw value for bit width
        max_val = (1 << bit_size) - 1
        # Normal (mid-range) value
        if is_signed and bit_size > 1:
            # For signed signals, mid-range positive
            mid_val = max_val // 4  # stay in positive range
            neg_val = max_val  # all bits set = -1 in two's complement (at raw level)
        else:
            mid_val = max_val // 2 if max_val > 1 else 1
            neg_val = None

        # Clean function name from DBC signal name
        func_name = sig.name

        # ── Helper to generate a single test case ──
        def gen_test(suffix: str, raw_value: int, is_negative: bool = False):
            tname = f"test_Signal_{func_name}_{suffix}"
            test_names.append(tname)
            lines.append(f"void {tname}(void)")
            lines.append("{")

            if is_tx:
                # TX test: SendSignal -> verify PDU buffer
                if is_negative and c_type == "sint16":
                    lines.append(f"    {c_type} val = -1;")
                    raw_for_check = raw_value
                else:
                    if c_type in ("sint16",):
                        lines.append(f"    {c_type} val = ({c_type}){raw_value};")
                    else:
                        lines.append(f"    {c_type} val = {raw_value}u;")
                    raw_for_check = raw_value

                lines.append(f"    Std_ReturnType ret = Com_SendSignal({define}, &val);")
                lines.append(f"    TEST_ASSERT_EQUAL_UINT8(E_OK, ret);")
                lines.append("")

                if bit_size <= 8:
                    if bit_size == 8:
                        # Full byte
                        lines.append(f"    /* Full-byte signal at byte {byte_offset} */")
                        if is_negative:
                            lines.append(f"    TEST_ASSERT_EQUAL_HEX8(0xFFu, com_tx_pdu_buf[{pdu_define}][{byte_offset}]);")
                        else:
                            lines.append(f"    TEST_ASSERT_EQUAL_HEX8(0x{raw_for_check:02X}u, com_tx_pdu_buf[{pdu_define}][{byte_offset}]);")
                    else:
                        # Sub-byte: verify the bits at the right position
                        mask = ((1 << bit_size) - 1) << shift
                        if is_negative:
                            expected = (raw_for_check & ((1 << bit_size) - 1)) << shift
                        else:
                            expected = (raw_for_check & ((1 << bit_size) - 1)) << shift
                        lines.append(f"    /* Sub-byte: {bit_size} bits at bit {bit_pos} (byte {byte_offset}, shift {shift}) */")
                        lines.append(f"    TEST_ASSERT_EQUAL_HEX8(0x{expected:02X}u, com_tx_pdu_buf[{pdu_define}][{byte_offset}] & 0x{mask:02X}u);")

                        # Verify neighboring bits are NOT corrupted (should be zero since we cleared)
                        inv_mask = (~mask) & 0xFF
                        if inv_mask != 0:
                            lines.append(f"    /* Verify neighboring bits not corrupted */")
                            lines.append(f"    TEST_ASSERT_EQUAL_HEX8(0x00u, com_tx_pdu_buf[{pdu_define}][{byte_offset}] & 0x{inv_mask:02X}u);")

                elif bit_size <= 16:
                    # Multi-byte signal: check both bytes (little-endian)
                    mask16 = ((1 << bit_size) - 1) << shift
                    if is_negative and c_type == "sint16":
                        # sint16 -1 as raw uint16 = 0xFFFF, masked to bit_size bits
                        raw_masked = ((1 << bit_size) - 1)
                    else:
                        raw_masked = raw_for_check & ((1 << bit_size) - 1)
                    shifted = raw_masked << shift
                    lo_byte = shifted & 0xFF
                    hi_byte = (shifted >> 8) & 0xFF
                    lo_mask = mask16 & 0xFF
                    hi_mask = (mask16 >> 8) & 0xFF

                    lines.append(f"    /* {bit_size}-bit signal at bit {bit_pos} spanning bytes {byte_offset}-{byte_offset+1} (LE) */")
                    lines.append(f"    TEST_ASSERT_EQUAL_HEX8(0x{lo_byte:02X}u, com_tx_pdu_buf[{pdu_define}][{byte_offset}] & 0x{lo_mask:02X}u);")
                    lines.append(f"    TEST_ASSERT_EQUAL_HEX8(0x{hi_byte:02X}u, com_tx_pdu_buf[{pdu_define}][{byte_offset + 1}] & 0x{hi_mask:02X}u);")

                    # Verify untouched bits in low byte
                    inv_lo = (~lo_mask) & 0xFF
                    if inv_lo:
                        lines.append(f"    /* Verify neighboring bits not corrupted (low byte) */")
                        lines.append(f"    TEST_ASSERT_EQUAL_HEX8(0x00u, com_tx_pdu_buf[{pdu_define}][{byte_offset}] & 0x{inv_lo:02X}u);")
                    inv_hi = (~hi_mask) & 0xFF
                    if inv_hi:
                        lines.append(f"    /* Verify neighboring bits not corrupted (high byte) */")
                        lines.append(f"    TEST_ASSERT_EQUAL_HEX8(0x00u, com_tx_pdu_buf[{pdu_define}][{byte_offset + 1}] & 0x{inv_hi:02X}u);")

            else:
                # RX test: build PDU buffer -> Com_RxIndication -> Com_ReceiveSignal -> verify
                if bit_size <= 8:
                    if bit_size == 8:
                        rx_byte_val = raw_value & 0xFF
                        lines.append(f"    /* RX: inject full byte at offset {byte_offset} */")
                        lines.append(f"    uint8 pdu_data[8] = {{0}};")
                        lines.append(f"    pdu_data[{byte_offset}] = 0x{rx_byte_val:02X}u;")
                    else:
                        rx_byte_val = (raw_value & ((1 << bit_size) - 1)) << shift
                        lines.append(f"    /* RX: inject {bit_size}-bit value at bit {bit_pos} */")
                        lines.append(f"    uint8 pdu_data[8] = {{0}};")
                        lines.append(f"    pdu_data[{byte_offset}] = 0x{rx_byte_val:02X}u;")
                elif bit_size <= 16:
                    raw_masked = raw_value & ((1 << bit_size) - 1)
                    shifted = raw_masked << shift
                    lo = shifted & 0xFF
                    hi = (shifted >> 8) & 0xFF
                    lines.append(f"    /* RX: inject {bit_size}-bit value at bit {bit_pos} (LE) */")
                    lines.append(f"    uint8 pdu_data[8] = {{0}};")
                    lines.append(f"    pdu_data[{byte_offset}] = 0x{lo:02X}u;")
                    lines.append(f"    pdu_data[{byte_offset + 1}] = 0x{hi:02X}u;")

                lines.append(f"    PduInfoType pdu_info = {{ pdu_data, 8u }};")
                lines.append(f"    Com_RxIndication({pdu_define}, &pdu_info);")
                lines.append("")

                # Now read back via Com_ReceiveSignal
                lines.append(f"    {c_type} result = 0;")
                lines.append(f"    Std_ReturnType ret = Com_ReceiveSignal({define}, &result);")
                lines.append(f"    TEST_ASSERT_EQUAL_UINT8(E_OK, ret);")

                if c_type == "uint8":
                    lines.append(f"    TEST_ASSERT_EQUAL_HEX8({raw_value}u, result);")
                elif c_type == "uint16":
                    lines.append(f"    TEST_ASSERT_EQUAL_HEX16({raw_value}u, result);")
                elif c_type == "sint16":
                    if is_negative:
                        lines.append(f"    TEST_ASSERT_EQUAL_INT16(-1, result);")
                    else:
                        lines.append(f"    TEST_ASSERT_EQUAL_INT16({raw_value}, result);")

            lines.append("}")
            lines.append("")

        # Generate 3 tests per signal
        gen_test("normal", mid_val)
        gen_test("max", max_val)
        gen_test("zero", 0)

        # For signed signals: add a negative test
        if is_signed and bit_size > 1:
            gen_test("negative", neg_val, is_negative=True)

    # ── Main runner function ─────────────────────────────────────────
    lines.append("/* ---- Test runner ---- */")
    lines.append("")
    lines.append("int main(void)")
    lines.append("{")
    lines.append("    UNITY_BEGIN();")
    for tname in test_names:
        lines.append(f"    RUN_TEST({tname});")
    lines.append("    return UNITY_END();")
    lines.append("}")

    OUT_C.parent.mkdir(parents=True, exist_ok=True)
    with open(OUT_C, "w", newline="\n") as f:
        f.write("\n".join(lines) + "\n")

    print(f"[gen_signal_tests] Generated {OUT_C} ({len(test_names)} tests)")

    # ── Generate runner shell script ──────────────────────────────────
    sh_lines = []
    sh_lines.append("#!/usr/bin/env bash")
    sh_lines.append("# Auto-generated by tools/test_gen/gen_signal_tests.py -- DO NOT EDIT")
    sh_lines.append("set -euo pipefail")
    sh_lines.append("")
    sh_lines.append("REPO_ROOT=\"$(cd \"$(dirname \"$0\")/../../..\" && pwd)\"")
    sh_lines.append("TEST_DIR=\"${REPO_ROOT}/test/unit/bsw\"")
    sh_lines.append("FW=\"${REPO_ROOT}/firmware\"")
    sh_lines.append("")
    sh_lines.append("# Include paths")
    sh_lines.append("INCLUDES=\"\\")
    sh_lines.append("  -I${FW}/bsw/include \\")
    sh_lines.append("  -I${FW}/bsw/services/Com/include \\")
    sh_lines.append("  -I${FW}/bsw/services/E2E/include \\")
    sh_lines.append("  -I${FW}/bsw/services/Dem/include \\")
    sh_lines.append("  -I${FW}/bsw/services/Det/include \\")
    sh_lines.append("  -I${FW}/bsw/services/Rte/include \\")
    sh_lines.append("  -I${FW}/bsw/services/SchM/include \\")
    sh_lines.append("  -I${FW}/ecu/cvc/include \\")
    sh_lines.append("  -I${FW}/ecu/cvc/cfg/platform_posix \\")
    sh_lines.append("  -I${FW}/lib/vendor/unity\"")
    sh_lines.append("")
    sh_lines.append("# Source files")
    sh_lines.append("SOURCES=\"\\")
    sh_lines.append("  ${TEST_DIR}/test_Com_signals_generated.c \\")
    sh_lines.append("  ${FW}/bsw/services/Com/src/Com.c \\")
    sh_lines.append("  ${FW}/ecu/cvc/cfg/Com_Cfg_Cvc.c \\")
    sh_lines.append("  ${FW}/lib/vendor/unity/unity.c\"")
    sh_lines.append("")
    sh_lines.append("# Stubs (E2E, Dem, Det, Rte, SchM, PduR) — link with test stubs")
    sh_lines.append("STUBS=\"${TEST_DIR}/stubs_bsw.c\"")
    sh_lines.append("")
    sh_lines.append("echo \"=== Compiling test_Com_signals_generated ===\"")
    sh_lines.append("gcc -std=c11 -Wall -Wextra -Werror \\")
    sh_lines.append("    -DUNIT_TEST \\")
    sh_lines.append("    ${INCLUDES} \\")
    sh_lines.append("    ${SOURCES} ${STUBS} \\")
    sh_lines.append("    -o ${TEST_DIR}/test_Com_signals_generated")
    sh_lines.append("")
    sh_lines.append("echo \"=== Running tests ===\"")
    sh_lines.append("${TEST_DIR}/test_Com_signals_generated")
    sh_lines.append("echo \"=== All tests passed ===\"")

    with open(OUT_SH, "w", newline="\n") as f:
        f.write("\n".join(sh_lines) + "\n")
    os.chmod(OUT_SH, 0o755)

    print(f"[gen_signal_tests] Generated {OUT_SH}")
    print(f"[gen_signal_tests] Done. Total: {len(test_signals)} signals, {len(test_names)} tests")


if __name__ == "__main__":
    main()
