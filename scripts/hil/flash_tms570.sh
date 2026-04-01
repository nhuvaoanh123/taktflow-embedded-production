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

# --- Verify binary matches current commit (MANDATORY) ---
verify_binary() {
    local elf="$1"
    local head_hash
    head_hash=$(cd "${PROJECT_ROOT}" && git rev-parse --short=8 HEAD 2>/dev/null || echo "unknown")

    if [ ! -f "${elf}" ]; then
        echo "  [FAIL] ${elf} not found. Build first."
        exit 1
    fi

    # Extract GIT_HASH burned into binary
    local binary_hash
    binary_hash=$(strings "${elf}" | grep -oE 'SC Boot \[([a-f0-9]{8})\]' | grep -oE '\[([a-f0-9]{8})\]' | tr -d '[]' || true)

    if [ -z "${binary_hash}" ]; then
        binary_hash=$(strings "${elf}" | grep -oE '\[([a-f0-9]{8})\]' | head -1 | tr -d '[]' || true)
    fi

    if [ -z "${binary_hash}" ]; then
        echo "  [FAIL] Cannot extract GIT_HASH from ${elf}"
        echo "         Binary may be corrupt or built without GIT_HASH."
        echo "         Rebuild: make -f firmware/platform/tms570/Makefile.tms570"
        exit 1
    fi

    if [ "${binary_hash}" != "${head_hash}" ]; then
        echo "  [FAIL] STALE BINARY — git hash mismatch!"
        echo "         Binary:  ${binary_hash}"
        echo "         HEAD:    ${head_hash}"
        echo "         Rebuild: make -f firmware/platform/tms570/Makefile.tms570"
        exit 1
    fi

    echo "  [OK] Binary verified: SC [${binary_hash}]"
}

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
    # Pre-flash: verify binary is from current commit
    verify_binary "${ELF}"

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
        echo "  [OK] SC flashed [$(cd "${PROJECT_ROOT}" && git rev-parse --short=8 HEAD)]"
    else
        echo "  [WARN] UniFlash not found at ${UNIFLASH}"
        echo "  Flash manually: CCS → Debug → Load Program → ${ELF}"
    fi
fi
