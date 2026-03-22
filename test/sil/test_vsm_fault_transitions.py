#!/usr/bin/env python3
"""Step-by-step VSM fault transition test.

Tests whether CVC transitions to SAFE_STOP when fault signals are
received from FZC/RZC. Each hop isolates one part of the chain.

Lessons applied from overtemp test:
- Precondition: wait for CVC RUN before testing
- Reset faults via MQTT (not curl to fault-inject API)
- Restore state between hops
- Use python-can + cantools for consistent decode

Run on SIL host (laptop or VPS) with Docker SIL running:
  python3 test/sil/test_vsm_fault_transitions.py
"""

import json
import sys
import time

import can
import cantools
import paho.mqtt.publish as mqtt_pub

DBC_PATH = "gateway/taktflow_vehicle.dbc"
MQTT_HOST = "localhost"
MQTT_TOPIC = "taktflow/command/plant_inject"

CAN_VEHICLE_STATE = 0x100
CAN_MOTOR_STATUS  = 0x300
CAN_DTC           = 0x500

STATE_NAMES = {0: "INIT", 1: "RUN", 2: "DEGRADED", 3: "LIMP", 4: "SAFE_STOP", 5: "SHUTDOWN"}


def mqtt_inject(cmd_type, **kwargs):
    payload = {"type": cmd_type}
    payload.update(kwargs)
    mqtt_pub.single(MQTT_TOPIC, json.dumps(payload),
                    hostname=MQTT_HOST, port=1883)


def can_flush(bus):
    while bus.recv(timeout=0) is not None:
        pass


def can_recv_decoded(db, bus, target_id, timeout=3.0):
    can_flush(bus)
    end = time.time() + timeout
    while time.time() < end:
        msg = bus.recv(timeout=0.5)
        if msg and msg.arbitration_id == target_id:
            return db.decode_message(target_id, msg.data, decode_choices=False)
    return None


def wait_for_state(db, bus, expected_state, timeout=10.0):
    """Wait until CVC reaches expected state, return actual state."""
    end = time.time() + timeout
    while time.time() < end:
        decoded = can_recv_decoded(db, bus, CAN_VEHICLE_STATE, timeout=2)
        if decoded:
            mode = int(decoded.get("Vehicle_State_Mode", -1))
            if mode == expected_state:
                return mode
    # Return last seen state
    decoded = can_recv_decoded(db, bus, CAN_VEHICLE_STATE, timeout=2)
    return int(decoded.get("Vehicle_State_Mode", -1)) if decoded else None


def reset_and_wait_run(db, bus, timeout=30.0):
    """Reset all faults and wait for CVC to return to RUN."""
    mqtt_inject("reset")
    mqtt_inject("voltage", mV=12600, soc=100)
    mqtt_inject("clear_temp_override")
    time.sleep(2)
    end = time.time() + timeout
    while time.time() < end:
        decoded = can_recv_decoded(db, bus, CAN_VEHICLE_STATE, timeout=2)
        if decoded and int(decoded.get("Vehicle_State_Mode", 0)) == 1:
            return True
    return False


def main():
    db = cantools.database.load_file(DBC_PATH)
    bus = can.interface.Bus(channel="vcan0", interface="socketcan")
    passed = 0
    failed = 0
    stop_hop = None

    def check(hop, desc, condition, detail=""):
        nonlocal passed, failed, stop_hop
        if stop_hop:
            return
        if condition:
            print(f"  [PASS] Hop {hop}: {desc}")
            passed += 1
        else:
            print(f"  [FAIL] Hop {hop}: {desc} — {detail}")
            failed += 1
            stop_hop = hop
            print(f"  STOP — fix Hop {hop} before testing downstream")

    print("=== VSM Fault Transition Hop Test ===")
    print()

    # Precondition: wait for CVC RUN
    print("Precondition: Waiting for CVC RUN state (up to 30s)...")
    run_seen = False
    deadline = time.time() + 30
    while time.time() < deadline:
        decoded = can_recv_decoded(db, bus, CAN_VEHICLE_STATE, timeout=2)
        if decoded:
            mode = int(decoded.get("Vehicle_State_Mode", 0))
            if mode == 1:
                run_seen = True
                print(f"  [OK] CVC in RUN state")
                break
            else:
                print(f"  ... CVC state={STATE_NAMES.get(mode, mode)}, waiting...")
    if not run_seen:
        print("  [FAIL] CVC never reached RUN")
        bus.shutdown()
        sys.exit(1)
    print()

    # Hop 1: CVC is in RUN
    print("Hop 1: CVC in RUN state")
    decoded = can_recv_decoded(db, bus, CAN_VEHICLE_STATE, timeout=3)
    if decoded:
        mode = int(decoded.get("Vehicle_State_Mode", 0))
        check(1, f"CVC state={STATE_NAMES.get(mode, mode)}", mode == 1,
              f"state={STATE_NAMES.get(mode, mode)}")
    else:
        check(1, "0x100 present", False, "No Vehicle_State on bus")

    # Hop 2: Overcurrent injection → Motor_Status shows fault
    print("Hop 2: Overcurrent injection → MotorFaultStatus on 0x300")
    mqtt_inject("overcurrent")
    time.sleep(3)
    decoded = can_recv_decoded(db, bus, CAN_MOTOR_STATUS, timeout=3)
    if decoded:
        fault = int(decoded.get("Motor_Status_MotorFaultStatus", 0))
        check(2, f"MotorFaultStatus={fault} (expect 3=OVERCURRENT)",
              fault == 3, f"MotorFaultStatus={fault}")
    else:
        check(2, "0x300 present", False, "No Motor_Status on bus")

    # Hop 3: CVC reads motor fault → state change (DEGRADED or SAFE_STOP)
    print("Hop 3: CVC state change on motor fault")
    time.sleep(3)
    decoded = can_recv_decoded(db, bus, CAN_VEHICLE_STATE, timeout=3)
    if decoded:
        mode = int(decoded.get("Vehicle_State_Mode", 0))
        # Accept DEGRADED(2) or SAFE_STOP(4) — both are valid fault reactions
        check(3, f"CVC state={STATE_NAMES.get(mode, mode)} (expect non-RUN)",
              mode != 1, f"CVC still in RUN")
    else:
        check(3, "0x100 present", False, "No Vehicle_State on bus")

    # Hop 4: Reset → back to RUN → brake fault → state change
    print("Hop 4: Reset → RUN → brake fault → state change")
    if not stop_hop:
        run_ok = reset_and_wait_run(db, bus, timeout=30)
        if not run_ok:
            check(4, "Reset to RUN", False, "CVC did not return to RUN after reset")
        else:
            mqtt_inject("brake_fault")
            time.sleep(5)
            decoded = can_recv_decoded(db, bus, CAN_VEHICLE_STATE, timeout=3)
            if decoded:
                mode = int(decoded.get("Vehicle_State_Mode", 0))
                check(4, f"CVC state={STATE_NAMES.get(mode, mode)} after brake fault",
                      mode != 1, f"CVC still in RUN")
            else:
                check(4, "0x100 present", False, "No Vehicle_State on bus")

    # Hop 5: Reset → RUN → battery UV → state change
    print("Hop 5: Reset → RUN → battery UV → state change")
    if not stop_hop:
        run_ok = reset_and_wait_run(db, bus, timeout=30)
        if not run_ok:
            check(5, "Reset to RUN", False, "CVC did not return to RUN after reset")
        else:
            # Sustain low voltage for 10s
            for _ in range(20):
                mqtt_inject("voltage", mV=5000, soc=1)
                time.sleep(0.5)
            decoded = can_recv_decoded(db, bus, CAN_VEHICLE_STATE, timeout=3)
            if decoded:
                mode = int(decoded.get("Vehicle_State_Mode", 0))
                check(5, f"CVC state={STATE_NAMES.get(mode, mode)} after battery UV",
                      mode != 1, f"CVC still in RUN")
            else:
                check(5, "0x100 present", False, "No Vehicle_State on bus")

    # Cleanup
    mqtt_inject("reset")
    mqtt_inject("voltage", mV=12600, soc=100)
    bus.shutdown()

    print()
    print(f"=== {passed} passed, {failed} failed ===")
    if stop_hop:
        print(f"Stopped at Hop {stop_hop}")
    sys.exit(1 if failed > 0 else 0)


if __name__ == "__main__":
    main()
