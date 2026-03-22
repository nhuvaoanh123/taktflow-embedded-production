#!/usr/bin/env python3
"""Step-by-step battery voltage signal chain test.

Tests the full chain: plant-sim → 0x601 → RZC SensorFeeder → IoHwAb →
Swc_Battery → 0x303 (Battery_Status) → DTC on undervoltage.

Lessons applied from overtemp test:
- Precondition: wait for CVC RUN
- DTC sniffer starts BEFORE injection (one-shot broadcast)
- Filter DTC by number + ECU source
- Restore voltage between hops
- Use python-can + cantools consistently

Run on SIL host (laptop or VPS) with Docker SIL running:
  python3 test/sil/test_battery_chain.py
"""

import json
import sys
import threading
import time

import can
import cantools
import paho.mqtt.publish as mqtt_pub

DBC_PATH = "gateway/taktflow_vehicle.dbc"
MQTT_HOST = "localhost"
MQTT_TOPIC = "taktflow/command/plant_inject"

CAN_VEHICLE_STATE = 0x100
CAN_VSENSOR       = 0x601
CAN_BATTERY       = 0x303
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

    print("=== Battery Voltage Chain Hop Test ===")
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

    # Hop 1: Plant-sim sends 0x601 with battery voltage
    print("Hop 1: Plant-sim → CAN 0x601 (virtual sensor)")
    decoded = can_recv_decoded(db, bus, CAN_VSENSOR, timeout=3)
    if decoded:
        batt = int(decoded.get("RZC_Virtual_Sensors_BattVoltage_mV", 0))
        check(1, f"0x601 present, batt={batt}mV", True)
    else:
        check(1, "0x601 present", False, "No 0x601 on bus")

    # Hop 2: MQTT injection changes voltage on 0x601
    print("Hop 2: MQTT inject 4000mV → 0x601")
    mqtt_inject("voltage", mV=4000, soc=1)
    time.sleep(2)
    decoded = can_recv_decoded(db, bus, CAN_VSENSOR, timeout=3)
    if decoded:
        batt = int(decoded.get("RZC_Virtual_Sensors_BattVoltage_mV", 0))
        check(2, f"0x601 batt={batt}mV (expect ~4000)",
              batt < 6000, f"batt={batt}mV, expected <6000")
    else:
        check(2, "0x601 after inject", False, "No 0x601")

    # Hop 3: RZC Battery_Status (0x303) reflects injected voltage
    print("Hop 3: RZC → CAN 0x303 (Battery_Status)")
    # Keep injecting to sustain low voltage
    for _ in range(5):
        mqtt_inject("voltage", mV=4000, soc=1)
        time.sleep(0.5)
    decoded = can_recv_decoded(db, bus, CAN_BATTERY, timeout=5)
    if decoded:
        batt = int(decoded.get("Battery_Status_BatteryVoltage_mV", 0))
        level = int(decoded.get("Battery_Status_Level", 2))
        level_names = {0: "critical_UV", 1: "UV_warn", 2: "normal", 3: "OV_warn", 4: "critical_OV"}
        check(3, f"0x303 batt={batt}mV level={level_names.get(level, level)} (expect UV)",
              batt < 6000, f"batt={batt}mV, RZC didn't track injection")
    else:
        check(3, "0x303 present", False, "No Battery_Status on bus")

    # Start DTC sniffer BEFORE Hop 4 (one-shot broadcast)
    dtc_result = {"frame": None}
    dtc_stop = threading.Event()
    def sniff_dtc():
        dtc_bus = can.interface.Bus(channel="vcan0", interface="socketcan")
        while not dtc_stop.is_set():
            msg = dtc_bus.recv(timeout=0.5)
            if msg and msg.arbitration_id == CAN_DTC:
                dtc_result["frame"] = msg
                decoded = db.decode_message(CAN_DTC, msg.data, decode_choices=False)
                dtc_num = int(decoded.get("DTC_Broadcast_Number", 0))
                if dtc_num == 0xE401:  # RZC battery undervoltage DTC
                    break
        dtc_bus.shutdown()
    dtc_thread = threading.Thread(target=sniff_dtc, daemon=True)
    dtc_thread.start()

    # Hop 4: Sustained UV → CVC state change
    print("Hop 4: Sustained battery UV → CVC state change")
    if not stop_hop:
        for _ in range(20):
            mqtt_inject("voltage", mV=4000, soc=1)
            time.sleep(0.5)
        decoded = can_recv_decoded(db, bus, CAN_VEHICLE_STATE, timeout=3)
        if decoded:
            mode = int(decoded.get("Vehicle_State_Mode", 0))
            check(4, f"CVC state={STATE_NAMES.get(mode, mode)} (expect non-RUN)",
                  mode != 1, f"CVC still in RUN after 10s UV")
        else:
            check(4, "0x100 present", False, "No Vehicle_State on bus")

    # Hop 5: DTC 0xE401 (battery UV) on CAN 0x500
    print("Hop 5: DTC 0xE401 (battery UV) on CAN 0x500")
    time.sleep(3)
    dtc_stop.set()
    dtc_thread.join(timeout=3)
    dtc_msg = dtc_result["frame"]
    if dtc_msg:
        decoded = db.decode_message(CAN_DTC, dtc_msg.data, decode_choices=False)
        dtc = int(decoded.get("DTC_Broadcast_Number", 0))
        ecu = int(decoded.get("DTC_Broadcast_ECU_Source", 0))
        ecu_names = {1: "CVC", 2: "FZC", 3: "RZC", 4: "SC", 5: "BCM", 6: "ICU", 7: "TCU"}
        print(f"    DTC=0x{dtc:06X} ECU={ecu_names.get(ecu, ecu)} status=0x{int(decoded.get('DTC_Broadcast_Status', 0)):02X}")
        check(5, f"DTC=0x{dtc:06X} from {ecu_names.get(ecu, ecu)} (expect 0x00E401 from RZC)",
              dtc == 0xE401 and ecu == 3,
              f"DTC=0x{dtc:06X} from {ecu_names.get(ecu, ecu)}")
    else:
        check(5, "0x500 present", False, "No DTC on bus (sniffer caught nothing)")

    # Cleanup: restore normal voltage
    mqtt_inject("voltage", mV=12600, soc=100)
    mqtt_inject("reset")
    bus.shutdown()

    print()
    print(f"=== {passed} passed, {failed} failed ===")
    if stop_hop:
        print(f"Stopped at Hop {stop_hop}")
    sys.exit(1 if failed > 0 else 0)


if __name__ == "__main__":
    main()
