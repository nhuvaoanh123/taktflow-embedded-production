#!/bin/bash
# @file       flash_tms570.sh
# @brief      Build and flash SC (Safety Controller) on TMS570LC4357 via CCS
# @aspice     SWE.6 — Software Qualification Testing
#
# Usage:
#   ./scripts/hil/flash_tms570.sh [--build-only] [--flash-only]
#
# Requires: TI tiarmclang (CCS 20.4.1), UniFlash or CCS debug server

set -euo pipefail

FLAG="${1:-}"

PROJECT_ROOT="$(cd "$(dirname "$0")/../.." && pwd)"
BUILD_DIR="${PROJECT_ROOT}/build/tms570"
ELF="${BUILD_DIR}/sc.elf"

echo "=== TMS570 SC (Safety Controller) ==="

# --- Build ---
if [ "${FLAG}" != "--flash-only" ]; then
    echo "  Building SC..."
    make -f "${PROJECT_ROOT}/firmware/platform/tms570/Makefile.tms570" \
        -C "${PROJECT_ROOT}" \
        -j"$(nproc)"
    echo "  [OK] Build complete"
    tiarmsize "${ELF}" 2>/dev/null || echo "  (tiarmsize not in PATH, skipping size report)"
fi

# --- Flash ---
if [ "${FLAG}" != "--build-only" ]; then
    echo "  Flashing SC via UniFlash CLI..."
    # UniFlash CLI path (typical CCS install)
    UNIFLASH="${UNIFLASH_PATH:-/opt/ti/uniflash/dslite.sh}"
    if [ ! -f "${UNIFLASH}" ]; then
        # Windows fallback
        UNIFLASH="${UNIFLASH_PATH:-C:/ti/uniflash_8.8.0/dslite.bat}"
    fi

    if [ -f "${UNIFLASH}" ]; then
        "${UNIFLASH}" --config "${PROJECT_ROOT}/firmware/platform/tms570/uniflash.ccxml" \
            -f "${ELF}" \
            -v 2>&1 | tail -5
        echo "  [OK] SC flashed and running"
    else
        echo "  [WARN] UniFlash not found at ${UNIFLASH}"
        echo "  Flash manually: CCS → Debug → Load Program → ${ELF}"
    fi
fi
