#!/bin/bash
# @file       flash_stm32.sh
# @brief      Build and flash one STM32 ECU (CVC, FZC, or RZC) via st-flash
# @aspice     SWE.6 — Software Qualification Testing
#
# Usage:
#   ./scripts/hil/flash_stm32.sh cvc [--build-only] [--flash-only] [--serial SERIAL]
#   ./scripts/hil/flash_stm32.sh fzc
#   ./scripts/hil/flash_stm32.sh rzc
#
# Requires: arm-none-eabi-gcc, st-flash (stlink-tools)
#
# Board serial mapping (set via env or --serial):
#   STLINK_CVC=001A00363235510B37333439
#   STLINK_FZC=0027003C3235510B37333439
#   STLINK_RZC=<third board serial>

set -euo pipefail

TARGET="${1:?Usage: $0 <cvc|fzc|rzc> [--build-only|--flash-only] [--serial SERIAL]}"
shift

FLAG=""
SERIAL=""

while [[ $# -gt 0 ]]; do
    case "$1" in
        --build-only) FLAG="build-only" ;;
        --flash-only) FLAG="flash-only" ;;
        --serial)     SERIAL="$2"; shift ;;
        *)            echo "Unknown option: $1"; exit 1 ;;
    esac
    shift
done

PROJECT_ROOT="$(cd "$(dirname "$0")/../.." && pwd)"
BIN="${PROJECT_ROOT}/build/stm32/${TARGET}.bin"
ELF="${PROJECT_ROOT}/build/stm32/${TARGET}.elf"

# Resolve serial from env if not passed
if [ -z "${SERIAL}" ]; then
    VAR="STLINK_${TARGET^^}"
    SERIAL="${!VAR:-}"
fi

echo "=== STM32 ${TARGET^^} ==="

# --- Build ---
if [ "${FLAG}" != "flash-only" ]; then
    echo "  Building ${TARGET}..."
    make -f "${PROJECT_ROOT}/firmware/platform/stm32/Makefile.stm32" \
        TARGET="${TARGET}" \
        -C "${PROJECT_ROOT}" \
        -j"$(nproc)"
    echo "  [OK] Build complete"
    arm-none-eabi-size "${ELF}"
fi

# --- Flash ---
if [ "${FLAG}" != "build-only" ]; then
    if [ ! -f "${BIN}" ]; then
        echo "  [ERROR] ${BIN} not found. Build first."
        exit 1
    fi

    FLASH_ARGS="write ${BIN} 0x08000000"
    if [ -n "${SERIAL}" ]; then
        FLASH_ARGS="--serial ${SERIAL} ${FLASH_ARGS}"
    fi

    echo "  Flashing ${TARGET} via st-flash..."
    if ! st-flash ${FLASH_ARGS} 2>&1; then
        echo "  Retrying with --connect-under-reset..."
        st-flash --connect-under-reset ${FLASH_ARGS} 2>&1
    fi
    echo "  [OK] ${TARGET^^} flashed and verified"
fi
