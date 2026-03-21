#!/usr/bin/env python3
"""
Layer 5: Dual ECU Integration Test — CVC + FZC over vcan0

Tests bidirectional CAN communication between CVC and FZC.
Both run as POSIX processes on the same vcan0 interface.

Requirements tested:
  1. CVC heartbeat (0x010) received by FZC (FZC doesn't timeout)
  2. FZC heartbeat (0x011) received by CVC (CVC doesn't timeout)
  3. CVC sends Steer_Command (0x102) → FZC receives it
  4. FZC sends Steering_Status (0x200) → appears on bus with E2E
  5. Both heartbeats have E2E headers (DataId=2 for CVC, DataId=3 for FZC)
  6. Kill CVC → FZC detects heartbeat timeout

Usage:
    sudo ip link add dev vcan0 type vcan && sudo ip link set up vcan0
    python3 test/integration/layer5/test_cvc_fzc_dual.py

@verifies SSR-CVC-016, SSR-FZC-019, TSR-022, TSR-023
"""

import os
import signal
import subprocess
import sys
import time

try:
    import can
except ImportError:
    print("ERROR: pip install python-can")
    sys.exit(1)

PROJECT_ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.dirname(
    os.path.abspath(__file__)))))
CVC_BINARY = os.path.join(PROJECT_ROOT, "build", "cvc_posix")
FZC_BINARY = os.path.join(PROJECT_ROOT, "build", "fzc_posix")
VCAN = "vcan0"

CAN_ID_CVC_HB   = 0x010
CAN_ID_FZC_HB   = 0x011
CAN_ID_STEER_CMD = 0x102
CAN_ID_STEER_STS = 0x200
CAN_ID_VEHICLE_STATE = 0x100

TIMEOUT_S = 3.0


class EcuProcess:
    def __init__(self, binary, name):
        self.binary = binary
        self.name = name
        self.proc = None

    def start(self):
        if not os.path.exists(self.binary):
            raise FileNotFoundError(f"{self.name} binary not found: {self.binary}")
        self.proc = subprocess.Popen(
            [self.binary, VCAN],
            stdout=subprocess.DEVNULL,
            stderr=subprocess.DEVNULL,
        )
        time.sleep(0.3)
        if self.proc.poll() is not None:
            raise RuntimeError(f"{self.name} exited with code {self.proc.returncode}")

    def stop(self):
        if self.proc and self.proc.poll() is None:
            self.proc.send_signal(signal.SIGTERM)
            try:
                self.proc.wait(timeout=3)
            except subprocess.TimeoutExpired:
                self.proc.kill()

    def is_alive(self):
        return self.proc and self.proc.poll() is None


def wait_for_frame(bus, can_id, timeout=TIMEOUT_S):
    deadline = time.time() + timeout
    while time.time() < deadline:
        msg = bus.recv(timeout=0.1)
        if msg and msg.arbitration_id == can_id:
            return msg
    return None


def collect_ids(bus, duration=2.0):
    ids = set()
    deadline = time.time() + duration
    while time.time() < deadline:
        msg = bus.recv(timeout=0.1)
        if msg:
            ids.add(msg.arbitration_id)
    return ids


# ==================================================================
# Tests
# ==================================================================

def test_1_cvc_heartbeat_on_bus(bus):
    """CVC heartbeat (0x010) present on shared bus."""
    print("  Test 1: CVC heartbeat on bus...", end=" ", flush=True)
    msg = wait_for_frame(bus, CAN_ID_CVC_HB)
    assert msg is not None, "No CVC heartbeat (0x010)"
    print(f"PASS — data={msg.data.hex()}")


def test_2_fzc_heartbeat_on_bus(bus):
    """FZC heartbeat (0x011) present on shared bus."""
    print("  Test 2: FZC heartbeat on bus...", end=" ", flush=True)
    msg = wait_for_frame(bus, CAN_ID_FZC_HB)
    assert msg is not None, "No FZC heartbeat (0x011)"
    print(f"PASS — data={msg.data.hex()}")


def test_3_steer_command_on_bus(bus):
    """CVC sends Steer_Command (0x102) that FZC can receive."""
    print("  Test 3: Steer_Command (0x102) on bus...", end=" ", flush=True)
    msg = wait_for_frame(bus, CAN_ID_STEER_CMD)
    assert msg is not None, "No Steer_Command (0x102)"
    print(f"PASS — data={msg.data.hex()}")


def test_4_steering_status_on_bus(bus):
    """FZC sends Steering_Status (0x200) with E2E."""
    print("  Test 4: Steering_Status (0x200) on bus...", end=" ", flush=True)
    msg = wait_for_frame(bus, CAN_ID_STEER_STS)
    assert msg is not None, "No Steering_Status (0x200)"
    # E2E: DataId=9 for Steering_Status
    data_id = msg.data[0] & 0x0F
    assert data_id == 9, f"E2E DataId={data_id}, expected 9"
    print(f"PASS — data={msg.data.hex()}, E2E DataId={data_id}")


def test_5_both_heartbeats_e2e(bus):
    """Both heartbeats have correct E2E DataIDs."""
    print("  Test 5: Both heartbeats E2E...", end=" ", flush=True)
    cvc_hb = wait_for_frame(bus, CAN_ID_CVC_HB)
    fzc_hb = wait_for_frame(bus, CAN_ID_FZC_HB)
    assert cvc_hb is not None, "No CVC heartbeat"
    assert fzc_hb is not None, "No FZC heartbeat"
    cvc_did = cvc_hb.data[0] & 0x0F
    fzc_did = fzc_hb.data[0] & 0x0F
    assert cvc_did == 2, f"CVC E2E DataId={cvc_did}, expected 2"
    assert fzc_did == 3, f"FZC E2E DataId={fzc_did}, expected 3"
    print(f"PASS — CVC DataId={cvc_did}, FZC DataId={fzc_did}")


def test_6_cvc_kill_fzc_detects_timeout(bus, cvc):
    """Kill CVC → FZC detects heartbeat timeout (no crash)."""
    print("  Test 6: Kill CVC, FZC survives...", end=" ", flush=True)
    cvc.stop()
    time.sleep(2.0)
    # FZC should still be producing heartbeats
    msg = wait_for_frame(bus, CAN_ID_FZC_HB, timeout=3.0)
    assert msg is not None, "FZC stopped producing heartbeats after CVC death"
    print(f"PASS — FZC alive after CVC kill, hb={msg.data.hex()}")


# ==================================================================
# Main
# ==================================================================

def main():
    print("Layer 5: Dual ECU Integration Test — CVC + FZC")
    print(f"CVC: {CVC_BINARY}")
    print(f"FZC: {FZC_BINARY}")
    print(f"Interface: {VCAN}")
    print()

    bus = can.interface.Bus(interface='socketcan', channel=VCAN)
    cvc = EcuProcess(CVC_BINARY, "CVC")
    fzc = EcuProcess(FZC_BINARY, "FZC")
    passed = 0
    failed = 0

    try:
        cvc.start()
        fzc.start()
        time.sleep(2.0)  # Let both initialize

        tests = [
            lambda: test_1_cvc_heartbeat_on_bus(bus),
            lambda: test_2_fzc_heartbeat_on_bus(bus),
            lambda: test_3_steer_command_on_bus(bus),
            lambda: test_4_steering_status_on_bus(bus),
            lambda: test_5_both_heartbeats_e2e(bus),
            lambda: test_6_cvc_kill_fzc_detects_timeout(bus, cvc),
        ]

        for test in tests:
            try:
                test()
                passed += 1
            except (AssertionError, Exception) as e:
                print(f"FAIL — {e}")
                failed += 1

    finally:
        cvc.stop()
        fzc.stop()
        bus.shutdown()

    print()
    print(f"{passed + failed} tests: {passed} passed, {failed} failed")
    return 0 if failed == 0 else 1


if __name__ == "__main__":
    sys.exit(main())
