# Plan: E2E Fault Detection Hardening

**Status:** DRAFT
**Created:** 2026-03-26
**Author:** Claude + An Dao

## Overview

Wire E2E supervision state machine results into the fault reporting
chain so that CAN communication failures trigger DTCs, fault masks,
safe defaults, and vehicle state transitions. Currently E2E checks
run but failures are silently counted — no SWC can react.

## Phase Table

| Phase | Name | Status | Risk |
|-------|------|--------|------|
| 1 | DTC event wiring | PENDING | ZERO |
| 2 | Recovery event | PENDING | ZERO |
| 3 | Safe defaults on INVALID | PENDING | LOW |
| 4 | Fault mask propagation | PENDING | MEDIUM |

## Phase 1: DTC Event Wiring

### Tasks
- [ ] Extend sidecar `dtc_events` with per-PDU E2E failure event IDs
- [ ] Update `Com_Cfg.c.j2` — map `E2eDemEventId` from sidecar DTC
- [ ] Regenerate all ECU configs
- [ ] Verify: pull one ECU off bus → `Dem_ReportErrorStatus` called

### Files Changed
- `model/ecu_sidecar.yaml` — add E2E DTC events per ECU
- `tools/arxmlgen/templates/com/Com_Cfg.c.j2` — emit DTC event ID
- `firmware/ecu/*/cfg/Com_Cfg_*.c` — regenerated

### DONE Criteria
- E2E failure on any RX PDU calls `Dem_ReportErrorStatus` with correct event ID
- DTC stored in Dem (visible via UDS ReadDTCInfo)

## Phase 2: Recovery Event

### Tasks
- [ ] Track previous SM state per PDU in `Com.c`
- [ ] On transition INVALID → VALID: call `Dem_ReportErrorStatus(PASSED)`
- [ ] Unit test: SM recovery clears DTC

### Files Changed
- `firmware/bsw/services/Com/src/Com.c` — add prev_sm_state tracking

### DONE Criteria
- ECU removed then reconnected → DTC clears after smValid consecutive OKs

## Phase 3: Safe Defaults on INVALID

### Tasks
- [ ] On E2E INVALID discard: zero all signal shadow buffers for that PDU
- [ ] Also zero the RTE signals (via auto-push with value 0)
- [ ] Unit test: sustained E2E failure → signal reads return 0

### Files Changed
- `firmware/bsw/services/Com/src/Com.c` — zero shadow + RTE on discard

### DONE Criteria
- Remove one ECU → receiving ECU sees 0 for all signals from that PDU
- Reconnect → signals resume with fresh values

## Phase 4: Fault Mask Propagation

### Tasks
- [ ] Define fault bit constants per RX PDU (e.g., FAULT_CAN_FZC, FAULT_CAN_RZC)
- [ ] On E2E INVALID: write fault bit to RTE fault mask signal
- [ ] On recovery: clear fault bit
- [ ] Verify: CVC VehicleState transitions to DEGRADED on zone ECU loss

### Files Changed
- `firmware/ecu/*/include/*_Cfg.h` — fault bit defines (may already exist)
- `firmware/bsw/services/Com/src/Com.c` — write fault bits to RTE
- `firmware/ecu/cvc/src/Swc_VehicleState.c` — already reads fault signals

### DONE Criteria
- Remove FZC from bus → CVC transitions DEGRADED within 500ms
- Remove both FZC+RZC → CVC transitions SAFE_STOP
- Reconnect → CVC returns to RUN after smValid window

## Testing Plan

### Unit Tests
- `test_Com_E2E_DtcReport.c` — E2E failure triggers Dem call
- `test_Com_E2E_Recovery.c` — INVALID→VALID clears DTC
- `test_Com_E2E_SafeDefault.c` — signal zeroed on sustained failure

### HIL Tests
- Remove FZC USB cable → verify CVC state change on UART
- Reconnect → verify recovery
- Remove RZC → verify CVC dual-timeout → SAFE_STOP

## Open Questions
- Should safe defaults be 0 or configurable per-signal init values?
  **Recommendation:** 0 for now, add init_value to codegen later.
