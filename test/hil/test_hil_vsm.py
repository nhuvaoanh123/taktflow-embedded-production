#!/usr/bin/env python3
"""
@file       test_hil_vsm.py
@brief      HIL VSM fault transition test — physical CVC on STM32
@verifies   SWR-CVC-009, SWR-CVC-010, SWR-CVC-011, SWR-CVC-012
@traces_to  SSR-CVC-009, SSR-CVC-010, SG-001, SG-002, SG-006
@ftti       500ms (SG-002 motor control)
@aspice     SWE.6 — Software Qualification Testing
@iso        ISO 26262 Part 4, Section 7 — HSI Verification

Tests CVC state machine transitions on fault injection.
HIL difference from SIL: CVC is physical (STM32), SC is on bus (TMS570),
so system reaches RUN state (not stuck in SAFE_STOP like SIL without SC).

Usage:
    python3 test/hil/test_hil_vsm.py
"""

import sys
import time

import cantools

from hil_test_lib import (
    DBC_PATH,
    CAN_VEHICLE_STATE, CAN_MOTOR_STATUS,
    STATE_NAMES,
    open_bus, can_recv_decoded, poll_signal,
    mqtt_inject, mqtt_reset, wait_cvc_run, reset_and_wait_run,
    verify_normal_operation, HopChecker, print_header,
)


def main():
    db = cantools.database.load_file(DBC_PATH)
    bus = open_bus()
    hc = HopChecker()

    print_header("VSM Fault Transition Hop Test")

    # Precondition: CVC in RUN (SC on bus → should reach RUN)
    if not wait_cvc_run(db, bus):
        print("  [FAIL] CVC did not reach RUN — check SC (TMS570) is on bus")
        bus.shutdown()
        sys.exit(1)
    print()

    # Hop 0: Negative test — no false faults
    print("Hop 0: Normal operation (5s) — CVC stays in RUN")
    ok, detail = verify_normal_operation(db, bus, duration=5.0)
    hc.check(0, "No false fault during normal operation", ok, detail)

    # Hop 1: CVC in RUN
    print("Hop 1: CVC in RUN state")
    decoded = can_recv_decoded(db, bus, CAN_VEHICLE_STATE, timeout=3)
    if decoded:
        mode = int(decoded.get("Vehicle_State_Mode", 0))
        hc.check(1, f"CVC state={STATE_NAMES.get(mode, mode)}", mode == 1,
                 f"state={STATE_NAMES.get(mode, mode)}")
    else:
        hc.check(1, "0x100 present", False, "No Vehicle_State on bus")

    # Hop 2: Overcurrent → MotorFaultStatus on 0x300 (physical RZC)
    print("Hop 2: Overcurrent → MotorFaultStatus on 0x300")
    if not hc.stopped:
        mqtt_inject("overcurrent")
        val, elapsed = poll_signal(
            db, bus, CAN_MOTOR_STATUS, "Motor_Status_MotorFaultStatus",
            lambda v: int(v) == 3, timeout=10.0,
        )
        if elapsed is not None:
            hc.check(2, f"MotorFaultStatus=3 (OVERCURRENT) in {elapsed:.0f}ms", True)
        else:
            hc.check(2, f"MotorFaultStatus={val} (expect 3)", False,
                     f"MotorFaultStatus={val}")

    # Hop 3: CVC state change on motor fault (physical CVC)
    print("Hop 3: CVC state change on overcurrent")
    if not hc.stopped:
        val, elapsed = poll_signal(
            db, bus, CAN_VEHICLE_STATE, "Vehicle_State_Mode",
            lambda v: int(v) != 1, timeout=10.0,
        )
        if elapsed is not None:
            state = STATE_NAMES.get(int(val), val)
            hc.check(3, f"CVC state={state} in {elapsed:.0f}ms (expect non-RUN)", True)
        else:
            hc.check(3, "CVC left RUN", False, "CVC still in RUN")

    # Hop 4: Reset → RUN → brake fault → state change
    print("Hop 4: Reset → RUN → brake fault → state change")
    if not hc.stopped:
        run_ok = reset_and_wait_run(db, bus, timeout=30)
        if not run_ok:
            hc.check(4, "Reset to RUN", False, "CVC did not return to RUN")
        else:
            mqtt_inject("brake_fault")
            val, elapsed = poll_signal(
                db, bus, CAN_VEHICLE_STATE, "Vehicle_State_Mode",
                lambda v: int(v) != 1, timeout=10.0,
            )
            if elapsed is not None:
                state = STATE_NAMES.get(int(val), val)
                hc.check(4, f"CVC state={state} on brake fault in {elapsed:.0f}ms", True)
            else:
                hc.check(4, "CVC left RUN on brake fault", False, "CVC still in RUN")

    # Hop 5: Reset → RUN → battery UV → state change
    print("Hop 5: Reset → RUN → battery UV → state change")
    if not hc.stopped:
        run_ok = reset_and_wait_run(db, bus, timeout=30)
        if not run_ok:
            hc.check(5, "Reset to RUN", False, "CVC did not return to RUN")
        else:
            for _ in range(20):
                mqtt_inject("voltage", mV=5000, soc=1)
                time.sleep(0.5)
            val, elapsed = poll_signal(
                db, bus, CAN_VEHICLE_STATE, "Vehicle_State_Mode",
                lambda v: int(v) != 1, timeout=5.0,
            )
            if val is not None:
                state = STATE_NAMES.get(int(val), val)
                hc.check(5, f"CVC state={state} on battery UV", int(val) != 1,
                         "CVC still in RUN")
            else:
                hc.check(5, "Vehicle_State readable", False, "No 0x100 on bus")

    # Cleanup
    mqtt_reset()
    bus.shutdown()
    sys.exit(hc.summary())


if __name__ == "__main__":
    main()
