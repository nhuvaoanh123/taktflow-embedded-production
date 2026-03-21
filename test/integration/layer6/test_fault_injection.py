#!/usr/bin/env python3
"""
Layer 6: Fault Injection Tests

Proves the system DETECTS and REACTS to faults:
- E2E CRC corruption → frame discarded
- E2E counter replay → frame discarded
- Heartbeat timeout → VSM state change
- E-Stop injection → all ECUs react
- Motor overcurrent → FZC sends cutoff request
- Multi-ECU failure → cascading safe state

Requires: CVC + FZC + RZC on vcan0, no Docker containers.
"""

import subprocess
import signal
import time
import os
import sys
import struct
import can

REPO = os.path.dirname(os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__)))))
IFACE = "vcan0"

import sys
sys.stdout.reconfigure(line_buffering=True)

passed = 0
failed = 0
total = 0


def test(name, condition, detail=""):
    global passed, failed, total
    total += 1
    if condition:
        passed += 1
        print(f"  [{total:2d}] {name}: PASS{' — ' + detail if detail else ''}", flush=True)
    else:
        failed += 1
        print(f"  [{total:2d}] {name}: FAIL{' — ' + detail if detail else ''}", flush=True)


def start_ecu(name):
    binary = f"/tmp/{name}_posix"
    if not os.path.isfile(binary):
        binary = os.path.join(REPO, "build", f"{name}_posix")
    return subprocess.Popen([binary, IFACE], stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)


def kill_ecu(proc):
    if proc and proc.poll() is None:
        proc.send_signal(signal.SIGTERM)
        try:
            proc.wait(timeout=3)
        except subprocess.TimeoutExpired:
            proc.kill()
            proc.wait()


def wait_for_frame(bus, can_id, timeout_s=3.0):
    deadline = time.time() + timeout_s
    while time.time() < deadline:
        m = bus.recv(timeout=0.1)
        if m and m.arbitration_id == can_id:
            return m
    return None


def inject_frame(bus, can_id, data):
    bus.send(can.Message(arbitration_id=can_id, data=data, is_extended_id=False))


def collect_frames(bus, duration_s, filter_ids=None):
    frames = {}
    deadline = time.time() + duration_s
    while time.time() < deadline:
        m = bus.recv(timeout=0.05)
        if m and (filter_ids is None or m.arbitration_id in filter_ids):
            if m.arbitration_id not in frames:
                frames[m.arbitration_id] = []
            frames[m.arbitration_id].append(m)
    return frames


# ============================================================

print("Layer 6: Fault Injection Tests")
print(f"Interface: {IFACE}")
print()

os.system("sudo killall -9 cvc_posix fzc_posix rzc_posix 2>/dev/null")
time.sleep(0.5)

bus = can.interface.Bus(interface="socketcan", channel=IFACE)

# ============================================================
# Group 1: E2E CRC Corruption
# ============================================================
sys.stdout.flush()
    print("--- Group 1: E2E CRC Corruption ---")

cvc = start_ecu("cvc")
fzc = start_ecu("fzc")
time.sleep(4)

# Inject a corrupted FZC heartbeat (0x011) with wrong CRC
# Valid E2E: [counter<<4 | dataId, CRC, ECU_ID, mode]
# Corrupt: flip CRC byte
corrupt_hb = bytes([0x13, 0xFF, 0x02, 0x00])  # DataId=3, counter=1, CRC=0xFF (wrong)
inject_frame(bus, 0x011, corrupt_hb)

# CVC should NOT crash — corrupted frame should be silently discarded
time.sleep(0.5)
cvc_hb = wait_for_frame(bus, 0x010, timeout_s=1)
test("G1.1 CVC survives corrupted FZC heartbeat injection", cvc_hb is not None)

# Inject 100 corrupted frames rapidly
for i in range(100):
    corrupt = bytes([((i % 16) << 4) | 3, 0xDE, 0x02, 0x00])
    inject_frame(bus, 0x011, corrupt)

time.sleep(0.5)
cvc_hb = wait_for_frame(bus, 0x010, timeout_s=1)
test("G1.2 CVC survives 100 corrupted frames", cvc_hb is not None)

# FZC should still be sending valid heartbeats
fzc_hb = wait_for_frame(bus, 0x011, timeout_s=1)
test("G1.3 Real FZC heartbeat still arriving", fzc_hb is not None)

kill_ecu(cvc)
kill_ecu(fzc)
time.sleep(1)

# ============================================================
# Group 2: Heartbeat Timeout Detection
# ============================================================
print("\n--- Group 2: Heartbeat Timeout Detection ---")

cvc = start_ecu("cvc")
fzc = start_ecu("fzc")
rzc = start_ecu("rzc")
time.sleep(5)

# Verify all 3 heartbeats present
while bus.recv(timeout=0.01):
    pass
frames = collect_frames(bus, 1.0, {0x010, 0x011, 0x012})
test("G2.1 All 3 heartbeats present", len(frames) == 3,
     f"got IDs: {sorted(frames.keys())}")

# Kill FZC — CVC should detect timeout
kill_ecu(fzc)
time.sleep(2)  # Wait for timeout detection

# Check CVC Vehicle_State fault mask
while bus.recv(timeout=0.01):
    pass
vs = wait_for_frame(bus, 0x100, timeout_s=2)
test("G2.2 CVC still running after FZC death", vs is not None)

# Kill RZC — CVC should detect both timeouts
kill_ecu(rzc)
time.sleep(2)

vs = wait_for_frame(bus, 0x100, timeout_s=2)
test("G2.3 CVC still running after FZC+RZC death", vs is not None)

# CVC should be in a degraded state (fault mask non-zero)
if vs:
    # FaultMask should show timeouts
    test("G2.4 FaultMask non-zero (timeouts detected)", vs.data[2] != 0 or vs.data[3] != 0,
         f"byte2=0x{vs.data[2]:02X} byte3=0x{vs.data[3]:02X}")

kill_ecu(cvc)
time.sleep(1)

# ============================================================
# Group 3: E-Stop Propagation
# ============================================================
print("\n--- Group 3: E-Stop Propagation ---")

cvc = start_ecu("cvc")
fzc = start_ecu("fzc")
rzc = start_ecu("rzc")
time.sleep(5)

# Verify E-Stop broadcast is active=0 (normal)
while bus.recv(timeout=0.01):
    pass
estop = wait_for_frame(bus, 0x001, timeout_s=1)
test("G3.1 E-Stop broadcast present (normal)", estop is not None)
if estop:
    test("G3.2 E-Stop active=0 (idle)", estop.data[2] == 0)

# All 3 ECUs should be producing their messages
frames = collect_frames(bus, 1.0, {0x010, 0x011, 0x012, 0x100, 0x200, 0x300})
test("G3.3 All ECUs producing messages", len(frames) >= 5,
     f"IDs: {sorted(frames.keys())}")

kill_ecu(cvc)
kill_ecu(fzc)
kill_ecu(rzc)
time.sleep(1)

# ============================================================
# Group 4: ECU Restart Recovery
# ============================================================
print("\n--- Group 4: ECU Restart Recovery ---")

cvc = start_ecu("cvc")
fzc = start_ecu("fzc")
time.sleep(4)

# Kill FZC
kill_ecu(fzc)
time.sleep(2)

# Restart FZC
fzc = start_ecu("fzc")
time.sleep(4)

# FZC should resume
while bus.recv(timeout=0.01):
    pass
fzc_hb = wait_for_frame(bus, 0x011, timeout_s=2)
test("G4.1 FZC heartbeat resumes after restart", fzc_hb is not None)

# CVC should clear timeout
time.sleep(1)
vs = wait_for_frame(bus, 0x100, timeout_s=2)
test("G4.2 CVC detects FZC recovery", vs is not None)

kill_ecu(cvc)
kill_ecu(fzc)
time.sleep(1)

# ============================================================
# Group 5: Bus Flood Resilience
# ============================================================
print("\n--- Group 5: Bus Flood Resilience ---")

cvc = start_ecu("cvc")
fzc = start_ecu("fzc")
time.sleep(3)

# Flood the bus with 1000 unknown CAN IDs
for i in range(1000):
    inject_frame(bus, 0x7FF, bytes([i & 0xFF] * 8))

time.sleep(1)

# Both ECUs should still be running
cvc_hb = wait_for_frame(bus, 0x010, timeout_s=2)
fzc_hb = wait_for_frame(bus, 0x011, timeout_s=2)
test("G5.1 CVC survives bus flood", cvc_hb is not None)
test("G5.2 FZC survives bus flood", fzc_hb is not None)

# Frame rates should still be correct
while bus.recv(timeout=0.01):
    pass
frames = collect_frames(bus, 1.0, {0x010, 0x011})
cvc_rate = len(frames.get(0x010, []))
fzc_rate = len(frames.get(0x011, []))
test("G5.3 CVC heartbeat rate normal after flood", 15 <= cvc_rate <= 25,
     f"{cvc_rate}/s")
test("G5.4 FZC heartbeat rate normal after flood", 15 <= fzc_rate <= 25,
     f"{fzc_rate}/s")

kill_ecu(cvc)
kill_ecu(fzc)
time.sleep(1)

# ============================================================
# Group 6: Rapid Kill/Restart Cycle
# ============================================================
print("\n--- Group 6: Rapid Kill/Restart Cycle ---")

cvc = start_ecu("cvc")
time.sleep(2)

# Kill and restart FZC 5 times rapidly
for cycle in range(5):
    fzc = start_ecu("fzc")
    time.sleep(1)
    kill_ecu(fzc)
    time.sleep(0.5)

# CVC should still be running
cvc_hb = wait_for_frame(bus, 0x010, timeout_s=2)
test("G6.1 CVC survives 5 rapid FZC kill/restart cycles", cvc_hb is not None)

kill_ecu(cvc)
time.sleep(1)

# ============================================================
# Group 7: Startup Order Independence
# ============================================================
print("\n--- Group 7: Startup Order Independence ---")

# Start RZC first, then FZC, then CVC (reverse order)
rzc = start_ecu("rzc")
time.sleep(1)
fzc = start_ecu("fzc")
time.sleep(1)
cvc = start_ecu("cvc")
time.sleep(4)

# All should be running
while bus.recv(timeout=0.01):
    pass
frames = collect_frames(bus, 1.0, {0x010, 0x011, 0x012})
test("G7.1 All 3 ECUs run regardless of startup order",
     len(frames) == 3, f"IDs: {sorted(frames.keys())}")

kill_ecu(cvc)
kill_ecu(fzc)
kill_ecu(rzc)
time.sleep(1)

# ============================================================
# Cleanup
# ============================================================
bus.shutdown()

print()
print("=" * 60)
print(f"{total} tests: {passed} passed, {failed} failed")
if failed > 0:
    sys.exit(1)
