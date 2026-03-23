#!/bin/bash
# @file       setup_can_bridge.sh
# @brief      Set up CAN bridge between vcan0 (Docker vECUs) and can0 (physical)
# @aspice     SWE.6 — Software Qualification Testing
# @iso        ISO 26262 Part 4, Section 7 — HSI Verification
#
# Creates vcan0 for Docker containers, brings up can0 on USB-CAN adapter,
# and sets up bidirectional cangw forwarding so all 7 ECUs share one bus.
#
# Usage:
#   sudo ./scripts/hil/setup_can_bridge.sh [--bitrate 500000] [--device can0]
#
# Requires: can-utils, iproute2, kernel module: can-gw

set -euo pipefail

BITRATE="${1:-500000}"
PHY_DEV="${2:-can0}"
VIRT_DEV="vcan0"

echo "=== HIL CAN Bridge Setup ==="
echo "  Physical:  ${PHY_DEV} @ ${BITRATE} bit/s"
echo "  Virtual:   ${VIRT_DEV}"

# --- Load kernel modules ---
modprobe can
modprobe can_raw
modprobe vcan
modprobe can_gw 2>/dev/null || true

# --- Set up vcan0 (for Docker vECUs) ---
if ip link show "${VIRT_DEV}" > /dev/null 2>&1; then
    echo "  ${VIRT_DEV} already exists, bringing down..."
    ip link set "${VIRT_DEV}" down
    ip link delete "${VIRT_DEV}"
fi
ip link add dev "${VIRT_DEV}" type vcan
ip link set "${VIRT_DEV}" up
echo "  [OK] ${VIRT_DEV} up"

# --- Set up can0 (physical USB-CAN adapter) ---
if ip link show "${PHY_DEV}" > /dev/null 2>&1; then
    ip link set "${PHY_DEV}" down 2>/dev/null || true
fi
ip link set "${PHY_DEV}" type can bitrate "${BITRATE}"
ip link set "${PHY_DEV}" up
echo "  [OK] ${PHY_DEV} up @ ${BITRATE} bit/s"

# --- Set up bidirectional CAN gateway ---
# Clear existing rules
cangw -F 2>/dev/null || true

# vcan0 → can0 (vECU frames reach physical ECUs)
cangw -A -s "${VIRT_DEV}" -d "${PHY_DEV}" -e
echo "  [OK] ${VIRT_DEV} → ${PHY_DEV} gateway"

# can0 → vcan0 (physical ECU frames reach vECUs)
cangw -A -s "${PHY_DEV}" -d "${VIRT_DEV}" -e
echo "  [OK] ${PHY_DEV} → ${VIRT_DEV} gateway"

# --- Verify ---
echo ""
echo "Bridge active. Verify with:"
echo "  candump ${VIRT_DEV} &"
echo "  cansend ${PHY_DEV} 123#DEADBEEF"
echo ""
echo "Tear down with:"
echo "  cangw -F && ip link delete ${VIRT_DEV} && ip link set ${PHY_DEV} down"
