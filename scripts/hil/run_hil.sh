#!/bin/bash
# @file       run_hil.sh
# @brief      HIL mixed bench orchestration — full test run
# @aspice     SWE.6 — Software Qualification Testing
# @iso        ISO 26262 Part 4, Section 7 — HSI Verification
#
# Orchestrates the full HIL test sequence:
#   1. Set up CAN bridge (vcan0 ↔ can0)
#   2. Flash physical ECUs (optional, --flash)
#   3. Start Docker vECUs + plant-sim + MQTT
#   4. Wait for all ECUs to boot
#   5. Run HIL test suite
#   6. Collect results
#   7. Tear down
#
# Usage:
#   sudo ./scripts/hil/run_hil.sh [--flash] [--scenarios-only] [--tests-only]
#
# Options:
#   --flash         Build and flash all 4 physical ECUs before testing
#   --scenarios-only  Run only YAML scenario tests (hil_runner.py)
#   --tests-only    Run only Python hop tests (test_hil_*.py)
#   --no-teardown   Leave vECUs running after tests
#   --channel CHAN  CAN channel (default: can0)
#
# Requires: docker, docker-compose, can-utils, python3, arm-none-eabi-gcc (if --flash)

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
PROJECT_ROOT="$(cd "${SCRIPT_DIR}/../.." && pwd)"
RESULTS_DIR="${PROJECT_ROOT}/test/hil/results"

# Defaults
DO_FLASH=false
SCENARIOS_ONLY=false
TESTS_ONLY=false
NO_TEARDOWN=false
CAN_CHANNEL="can0"

# Parse args
while [[ $# -gt 0 ]]; do
    case "$1" in
        --flash)        DO_FLASH=true ;;
        --scenarios-only) SCENARIOS_ONLY=true ;;
        --tests-only)   TESTS_ONLY=true ;;
        --no-teardown)  NO_TEARDOWN=true ;;
        --channel)      CAN_CHANNEL="$2"; shift ;;
        *)              echo "Unknown option: $1"; exit 1 ;;
    esac
    shift
done

echo "============================================"
echo "  Taktflow HIL Mixed Bench"
echo "  $(date -u '+%Y-%m-%dT%H:%M:%SZ')"
echo "  CAN: ${CAN_CHANNEL}"
echo "============================================"
echo ""

mkdir -p "${RESULTS_DIR}"
FAIL_COUNT=0

# --- Step 1: CAN Bridge ---
echo "=== Step 1: CAN Bridge Setup ==="
"${SCRIPT_DIR}/setup_can_bridge.sh" 500000 "${CAN_CHANNEL}"
echo ""

# --- Step 2: Flash Physical ECUs (optional) ---
if [ "${DO_FLASH}" = true ]; then
    echo "=== Step 2: Flash Physical ECUs ==="
    "${SCRIPT_DIR}/flash_all.sh" || {
        echo "[FAIL] Flash failed — aborting"
        exit 1
    }
    echo ""
    echo "Waiting 5s for physical ECUs to boot..."
    sleep 5
else
    echo "=== Step 2: Flash — SKIPPED (use --flash to enable) ==="
fi
echo ""

# --- Step 3: Start Docker vECUs ---
echo "=== Step 3: Start Docker vECUs + Plant-Sim + MQTT ==="
cd "${PROJECT_ROOT}"
docker compose -f docker/docker-compose.hil.yml up --build -d 2>&1 | tail -10
echo ""
echo "Waiting 10s for vECUs to boot..."
sleep 10

# --- Step 4: Verify all ECUs on bus ---
echo "=== Step 4: Verify ECU presence on CAN bus ==="
echo "  Listening on ${CAN_CHANNEL} for 5s..."
SEEN_IDS=$(timeout 5 candump -t z "${CAN_CHANNEL}" 2>/dev/null | \
           awk '{print $3}' | sort -u | head -20 || true)
echo "  CAN IDs seen: ${SEEN_IDS}"

# Check critical IDs
for ID_NAME in "010:CVC_HB" "011:FZC_HB" "012:RZC_HB" "013:SC_Status" "100:Vehicle_State" "360:BCM_Body"; do
    ID="${ID_NAME%%:*}"
    NAME="${ID_NAME##*:}"
    if echo "${SEEN_IDS}" | grep -qi "${ID}"; then
        echo "  [OK] ${NAME} (0x${ID})"
    else
        echo "  [WARN] ${NAME} (0x${ID}) NOT SEEN"
    fi
done
echo ""

# --- Step 5: Run Tests ---
echo "=== Step 5: Run HIL Tests ==="
export CAN_INTERFACE="${CAN_CHANNEL}"
export MQTT_HOST="localhost"

# 5a: Python hop-by-hop tests
if [ "${SCENARIOS_ONLY}" = false ]; then
    echo "--- Python Hop Tests ---"
    cd "${PROJECT_ROOT}"
    for test_file in test/hil/test_hil_*.py; do
        test_name=$(basename "${test_file}" .py)
        echo ""
        echo ">>> ${test_name}"
        if python3 "${test_file}" 2>&1 | tee "${RESULTS_DIR}/${test_name}.log"; then
            echo "  [PASS] ${test_name}"
        else
            echo "  [FAIL] ${test_name}"
            FAIL_COUNT=$((FAIL_COUNT + 1))
        fi
    done
    echo ""
fi

# 5b: YAML scenario tests via hil_runner.py
if [ "${TESTS_ONLY}" = false ]; then
    echo "--- YAML Scenario Tests ---"
    cd "${PROJECT_ROOT}"
    if python3 test/hil/hil_runner.py \
        --channel "${CAN_CHANNEL}" \
        --interface socketcan \
        --mqtt-host localhost \
        2>&1 | tee "${RESULTS_DIR}/hil_scenarios.log"; then
        echo "  [PASS] YAML scenarios"
    else
        echo "  [FAIL] YAML scenarios"
        FAIL_COUNT=$((FAIL_COUNT + 1))
    fi
    echo ""
fi

# --- Step 6: Results ---
echo "============================================"
echo "  HIL Test Results"
echo "============================================"
echo "  Results in: ${RESULTS_DIR}"

if [ ${FAIL_COUNT} -eq 0 ]; then
    echo "  Status: ALL PASS"
else
    echo "  Status: ${FAIL_COUNT} FAILED"
fi
echo "============================================"
echo ""

# --- Step 7: Teardown ---
if [ "${NO_TEARDOWN}" = false ]; then
    echo "=== Step 7: Teardown ==="
    cd "${PROJECT_ROOT}"
    docker compose -f docker/docker-compose.hil.yml down 2>&1 | tail -3
    echo "  vECUs stopped"
else
    echo "=== Step 7: Teardown — SKIPPED (--no-teardown) ==="
    echo "  Stop manually: docker compose -f docker/docker-compose.hil.yml down"
fi

exit ${FAIL_COUNT}
