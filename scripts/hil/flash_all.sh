#!/bin/bash
# @file       flash_all.sh
# @brief      Build and flash all 4 physical ECUs for HIL bench
# @aspice     SWE.6 — Software Qualification Testing
#
# Usage:
#   ./scripts/hil/flash_all.sh [--build-only]
#
# Flashes in order: SC (TMS570), CVC, FZC, RZC (STM32)
# Note: If multiple Nucleo boards are connected, OpenOCD selects by serial.
#       Set OPENOCD_SERIAL_CVC, OPENOCD_SERIAL_FZC, OPENOCD_SERIAL_RZC env vars.

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
FLAG="${1:-}"

echo "============================================"
echo "  HIL Bench — Flash All Physical ECUs"
echo "============================================"
echo ""

FAIL_COUNT=0

# SC (TMS570) — flash first, it's the safety controller
echo "--- [1/4] Safety Controller (TMS570) ---"
if "${SCRIPT_DIR}/flash_tms570.sh" ${FLAG}; then
    echo "  SC: OK"
else
    echo "  SC: FAILED"
    FAIL_COUNT=$((FAIL_COUNT + 1))
fi
echo ""

# STM32 ECUs — CVC, FZC, RZC
for ecu in cvc fzc rzc; do
    IDX=$(($(echo "cvc fzc rzc" | tr ' ' '\n' | grep -n "^${ecu}$" | cut -d: -f1) + 1))
    echo "--- [${IDX}/4] ${ecu^^} (STM32) ---"
    if "${SCRIPT_DIR}/flash_stm32.sh" "${ecu}" ${FLAG}; then
        echo "  ${ecu^^}: OK"
    else
        echo "  ${ecu^^}: FAILED"
        FAIL_COUNT=$((FAIL_COUNT + 1))
    fi
    echo ""
done

echo "============================================"
if [ ${FAIL_COUNT} -eq 0 ]; then
    echo "  All 4 ECUs flashed successfully"
else
    echo "  ${FAIL_COUNT}/4 ECUs FAILED to flash"
    exit 1
fi
echo "============================================"
