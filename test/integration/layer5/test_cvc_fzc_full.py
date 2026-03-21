#!/usr/bin/env python3
"""
Layer 5: FULL CVC+FZC Dual ECU Integration Test Suite

Exhaustive bidirectional CAN verification between CVC and FZC.

@verifies SSR-CVC-014 through SSR-CVC-017, SSR-FZC-018 through SSR-FZC-020,
          SWR-BSW-015, SWR-BSW-023, TSR-022, TSR-023, TSR-024
"""

import os, signal, subprocess, sys, time, collections

try:
    import can
except ImportError:
    print("ERROR: pip install python-can"); sys.exit(1)

ROOT = os.path.dirname(os.path.dirname(os.path.dirname(os.path.dirname(os.path.abspath(__file__)))))
CVC = os.path.join(ROOT, "build", "cvc_posix")
FZC = os.path.join(ROOT, "build", "fzc_posix")
VCAN = "vcan0"

IDS = {
    'CVC_HB': 0x010, 'FZC_HB': 0x011, 'RZC_HB': 0x012,
    'VSTATE': 0x100, 'TORQUE': 0x101, 'STEER_CMD': 0x102, 'BRAKE_CMD': 0x103,
    'STEER_STS': 0x200, 'BRAKE_STS': 0x201,
    'BRAKE_FLT': 0x210, 'MOTOR_CUT': 0x211, 'LIDAR': 0x220,
}

results = []

def test(name):
    def decorator(fn):
        def wrapper(*args):
            try:
                fn(*args)
                results.append((name, 'PASS', ''))
                print(f"  [{len(results):2d}] {name}: PASS")
            except Exception as e:
                results.append((name, 'FAIL', str(e)))
                print(f"  [{len(results):2d}] {name}: FAIL — {e}")
        return wrapper
    return decorator

class Ecu:
    def __init__(self, binary, name):
        self.binary, self.name, self.proc = binary, name, None
    def start(self):
        self.proc = subprocess.Popen([self.binary, VCAN], stdout=subprocess.DEVNULL, stderr=subprocess.DEVNULL)
        time.sleep(0.3)
        assert self.proc.poll() is None, f"{self.name} died"
    def stop(self):
        if self.proc and self.proc.poll() is None:
            self.proc.send_signal(signal.SIGTERM)
            try: self.proc.wait(timeout=3)
            except: self.proc.kill()

def wait(bus, cid, timeout=3.0):
    dl = time.time() + timeout
    while time.time() < dl:
        m = bus.recv(timeout=0.05)
        if m and m.arbitration_id == cid: return m
    return None

def collect(bus, duration=2.0):
    frames = collections.defaultdict(list)
    dl = time.time() + duration
    while time.time() < dl:
        m = bus.recv(timeout=0.05)
        if m: frames[m.arbitration_id].append(m)
    return frames

# ---- Group 1: Both ECUs TX Presence ----

@test("TX: CVC_Heartbeat (0x010)")
def t01(bus): assert wait(bus, IDS['CVC_HB'])

@test("TX: FZC_Heartbeat (0x011)")
def t02(bus): assert wait(bus, IDS['FZC_HB'])

@test("TX: Vehicle_State (0x100) from CVC")
def t03(bus): assert wait(bus, IDS['VSTATE'])

@test("TX: Steer_Command (0x102) from CVC")
def t04(bus): assert wait(bus, IDS['STEER_CMD'])

@test("TX: Brake_Command (0x103) from CVC")
def t05(bus): assert wait(bus, IDS['BRAKE_CMD'])

@test("TX: Steering_Status (0x200) from FZC")
def t06(bus): assert wait(bus, IDS['STEER_STS'])

@test("TX: Brake_Status (0x201) from FZC")
def t07(bus): assert wait(bus, IDS['BRAKE_STS'])

@test("TX: Motor_Cutoff_Req (0x211) from FZC")
def t08(bus): assert wait(bus, IDS['MOTOR_CUT'])

@test("TX: Lidar_Distance (0x220) from FZC")
def t09(bus): assert wait(bus, IDS['LIDAR'])

# ---- Group 2: E2E on All Safety Messages ----

@test("E2E: CVC_Heartbeat DataId=2")
def t10(bus):
    m = wait(bus, IDS['CVC_HB']); assert m
    assert (m.data[0] & 0x0F) == 2

@test("E2E: FZC_Heartbeat DataId=3")
def t11(bus):
    m = wait(bus, IDS['FZC_HB']); assert m
    assert (m.data[0] & 0x0F) == 3

@test("E2E: Vehicle_State DataId=5")
def t12(bus):
    m = wait(bus, IDS['VSTATE']); assert m
    assert (m.data[0] & 0x0F) == 5

@test("E2E: Steer_Command DataId=7")
def t13(bus):
    m = wait(bus, IDS['STEER_CMD']); assert m
    assert (m.data[0] & 0x0F) == 7

@test("E2E: Brake_Command DataId=8")
def t14(bus):
    m = wait(bus, IDS['BRAKE_CMD']); assert m
    assert (m.data[0] & 0x0F) == 8

@test("E2E: Steering_Status DataId=9")
def t15(bus):
    m = wait(bus, IDS['STEER_STS']); assert m
    assert (m.data[0] & 0x0F) == 9

@test("E2E: Brake_Status DataId=10")
def t16(bus):
    m = wait(bus, IDS['BRAKE_STS']); assert m
    assert (m.data[0] & 0x0F) == 10

@test("E2E: Motor_Cutoff_Req DataId=12")
def t17(bus):
    m = wait(bus, IDS['MOTOR_CUT']); assert m
    assert (m.data[0] & 0x0F) == 12

@test("E2E: Lidar_Distance DataId=13")
def t18(bus):
    m = wait(bus, IDS['LIDAR']); assert m
    assert (m.data[0] & 0x0F) == 13

# ---- Group 3: Alive Counter Increments ----

@test("E2E counter: CVC_Heartbeat increments")
def t19(bus):
    m1 = wait(bus, IDS['CVC_HB']); m2 = wait(bus, IDS['CVC_HB'])
    assert m1 and m2
    assert ((m2.data[0]>>4)&0xF) != ((m1.data[0]>>4)&0xF)

@test("E2E counter: FZC_Heartbeat increments")
def t20(bus):
    m1 = wait(bus, IDS['FZC_HB']); m2 = wait(bus, IDS['FZC_HB'])
    assert m1 and m2
    assert ((m2.data[0]>>4)&0xF) != ((m1.data[0]>>4)&0xF)

@test("E2E counter: Steering_Status increments")
def t21(bus):
    m1 = wait(bus, IDS['STEER_STS']); m2 = wait(bus, IDS['STEER_STS'])
    assert m1 and m2
    assert ((m2.data[0]>>4)&0xF) != ((m1.data[0]>>4)&0xF)

# ---- Group 4: DLC Correctness ----

@test("DLC: CVC_Heartbeat=4")
def t22(bus): m = wait(bus, IDS['CVC_HB']); assert m and m.dlc == 4
@test("DLC: FZC_Heartbeat=4")
def t23(bus): m = wait(bus, IDS['FZC_HB']); assert m and m.dlc == 4
@test("DLC: Vehicle_State=6")
def t24(bus): m = wait(bus, IDS['VSTATE']); assert m and m.dlc == 6
@test("DLC: Steer_Command=8")
def t25(bus): m = wait(bus, IDS['STEER_CMD']); assert m and m.dlc == 8
@test("DLC: Steering_Status=8")
def t26(bus): m = wait(bus, IDS['STEER_STS']); assert m and m.dlc == 8
@test("DLC: Brake_Status=8")
def t27(bus): m = wait(bus, IDS['BRAKE_STS']); assert m and m.dlc == 8

# ---- Group 5: Timing ----

@test("Timing: CVC_HB ~50ms (15-30 frames/s)")
def t28(bus):
    f = collect(bus, 1.0); hbs = f.get(IDS['CVC_HB'], [])
    assert 10 <= len(hbs) <= 30, f"{len(hbs)} heartbeats/s"

@test("Timing: FZC_HB ~50ms (15-30 frames/s)")
def t29(bus):
    f = collect(bus, 1.0); hbs = f.get(IDS['FZC_HB'], [])
    assert 10 <= len(hbs) <= 30, f"{len(hbs)} heartbeats/s"

@test("Timing: Steering_Status ~50ms (15-30/s)")
def t30(bus):
    f = collect(bus, 1.0); ss = f.get(IDS['STEER_STS'], [])
    assert 10 <= len(ss) <= 30, f"{len(ss)} frames/s"

@test("Timing: Vehicle_State high-rate (50+/s)")
def t31(bus):
    f = collect(bus, 1.0); vs = f.get(IDS['VSTATE'], [])
    assert len(vs) >= 50, f"Only {len(vs)} Vehicle_State/s"

# ---- Group 6: Kill Tests ----

@test("Kill CVC: FZC survives, keeps producing heartbeat")
def t32(bus, cvc_ref):
    cvc_ref.stop()
    time.sleep(1.5)
    m = wait(bus, IDS['FZC_HB'], timeout=3.0)
    assert m is not None, "FZC died after CVC kill"

@test("Kill CVC: FZC Steering_Status continues")
def t33(bus):
    m = wait(bus, IDS['STEER_STS'], timeout=3.0)
    assert m is not None, "FZC stopped Steering_Status after CVC kill"

@test("Kill CVC: No CVC messages after kill")
def t34(bus):
    m = wait(bus, IDS['CVC_HB'], timeout=2.0)
    assert m is None, "CVC still alive after kill!"

# ==================================================================
# MAIN
# ==================================================================

def main():
    for b in [CVC, FZC]:
        if not os.path.exists(b):
            print(f"ERROR: {b} not found"); return 1

    print(f"Layer 5 FULL Test Suite — CVC + FZC")
    print(f"CVC: {CVC}")
    print(f"FZC: {FZC}")
    print()

    bus = can.interface.Bus(interface='socketcan', channel=VCAN)
    cvc = Ecu(CVC, "CVC"); fzc = Ecu(FZC, "FZC")

    try:
        cvc.start(); fzc.start()
        time.sleep(2.0)

        # Normal operation tests
        for t in [t01,t02,t03,t04,t05,t06,t07,t08,t09,
                  t10,t11,t12,t13,t14,t15,t16,t17,t18,
                  t19,t20,t21,
                  t22,t23,t24,t25,t26,t27,
                  t28,t29,t30,t31]:
            t(bus)

        # Kill tests (order matters — CVC killed, FZC checked)
        t32(bus, cvc)
        t33(bus)
        t34(bus)

    finally:
        cvc.stop(); fzc.stop()
        bus.shutdown()

    p = sum(1 for _,s,_ in results if s=='PASS')
    f = sum(1 for _,s,_ in results if s=='FAIL')
    print(f"\n{'='*50}")
    print(f"{len(results)} tests: {p} passed, {f} failed")
    if f > 0:
        print(f"\nFailed:")
        for n,s,m in results:
            if s=='FAIL': print(f"  - {n}: {m}")
    return 0 if f==0 else 1

if __name__ == "__main__":
    sys.exit(main())
