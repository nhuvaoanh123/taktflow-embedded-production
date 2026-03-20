# Plan: Firmware Rewrite — HARA-Driven Architecture

**Status**: PLANNING
**Date**: 2026-03-20
**Goal**: Rewrite firmware architecture top-down from HARA, not from code bugs.

---

## Traceability Chain

```
HARA (15 hazardous events)
  → Safety Goals (SG-001..SG-008, ASIL A..D)
    → Technical Safety Requirements (TSR)
      → Software Safety Requirements (SSR)
        → Software Requirements (SWR per ECU)
          → DBC (CAN matrix, source of truth)
            → ARXML (system description)
              → Codegen (Com_Cfg, E2E_Cfg, Rte_Cfg, CanIf_Cfg)
                → Firmware (BSW + SWC)
                  → HIL verification
```

Every line of firmware code must trace back through this chain.

---

## Safety Goals Driving the Architecture

| SG | Description | ASIL | FTTI | Safe State | Architecture Impact |
|----|-------------|------|------|------------|-------------------|
| SG-001 | Prevent unintended acceleration | D | 50ms | Torque = 0 | Dual pedal sensors, plausibility check, E2E on Torque_Request |
| SG-002 | Prevent loss of drive torque | B | 500ms | Warn driver | Motor fault detection, DTC broadcast, graceful degradation |
| SG-003 | Prevent unintended/loss of steering | D | 100ms | Center steering | E2E on Steer_Command, steer plausibility, FZC safety SWC |
| SG-004 | Prevent loss of braking | D | 100ms | Max brake | E2E on Brake_Command, brake plausibility, redundant path |
| SG-005 | Prevent unintended braking | A | 200ms | Release brake | Pedal plausibility, brake command validation |
| SG-006 | Detect motor/battery faults | A | 1000ms | Degrade/limp | Overcurrent, overtemp, undervoltage monitoring |
| SG-007 | Detect obstacles reliably | C | 200ms | Emergency brake | Lidar plausibility, sensor disagreement detection |
| SG-008 | Maintain system integrity | C | 500ms | SAFE_STOP | E-Stop, CAN monitoring, heartbeat, SC relay kill |

---

## What HARA Requires of the Firmware

### 1. E2E Protection (SG-001, SG-003, SG-004, SG-008)

Every safety-relevant CAN message MUST have E2E protection:
- **CRC-8** for data integrity
- **Alive counter** for sequence checking (per-PDU, NOT shared)
- **DataID** unique per message (from DBC `E2E_DataID` attribute)

**Current violation**: SWCs share alive counters. DataIDs hardcoded wrong.
**HARA mandate**: Per-PDU E2E state is not optional — ASIL D requires it.

### 2. Timing Compliance (all SGs with FTTI)

Every message MUST be sent within its DBC `GenMsgCycleTime`:
- Torque_Request: 10ms (SG-001 FTTI = 50ms → 5 frames before fault detection)
- Steer_Command: 10ms (SG-003 FTTI = 100ms → 10 frames)
- Heartbeat: 50ms (SG-008 → SC timeout detection within 500ms)

**Current violation**: Bus flooding at 500/s bypasses cycle times.
**HARA mandate**: Timing budget is derived from FTTI, not arbitrary.

### 3. Fault Detection (SG-002, SG-006, SG-007)

Every sensor reading MUST return a valid value or report a fault:
- IoHwAb returns `E_NOT_OK` → SWC MUST set fault flag → DTC broadcast
- Not silently return 0 and pretend everything is fine

**Current violation**: IoHwAb_Init(NULL) → all reads fail → values = 0, no DTC.
**HARA mandate**: Sensor failure = DTC + degraded mode, not silent zero.

### 4. Independent Monitoring (SG-008)

CVC Vehicle State Machine must transition to SAFE_STOP when:
- FZC heartbeat lost for > 500ms
- RZC heartbeat lost for > 500ms
- E-Stop activated (within 100ms FTTI)
- SC relay kill received

**Current violation**: CVC stuck in INIT because heartbeat E2E rejects frames.
**HARA mandate**: VSM transition logic is the core safety mechanism.

---

## Rewrite Phases (HARA-Ordered)

### Phase 0: DBC Audit

Verify every safety-relevant message in DBC has:
- [ ] Correct `GenMsgCycleTime` derived from FTTI
- [ ] Correct `E2E_DataID` unique per message
- [ ] Correct `ASIL` attribute
- [ ] Correct `Owner` (sender ECU)

DBC is the contract between ECUs. If DBC is wrong, everything downstream is wrong.

### Phase 1: Codegen — Generate Everything from DBC

1. **E2E DataIDs**: generate `#define <ECU>_E2E_<MSG>_DATA_ID` for ALL E2E-protected PDUs (TX and RX)
2. **Com CycleTimeMs**: already done — propagate from DBC `GenMsgCycleTime`
3. **Com→RTE binding**: populate `RteSignalId` in `Com_SignalConfigType` for automatic signal routing
4. **Per-PDU E2E config**: generate E2E protect/check configs in `E2E_Cfg_<Ecu>.c`

After Phase 1: all generated configs are correct. No hardcoded values needed in SWC code.

### Phase 2: BSW — E2E in Com Layer

Move E2E protection from SWC to Com:

1. `Com_MainFunction_Tx`: after packing signals, call `E2E_Protect` per PDU
2. Per-PDU alive counter array: `com_e2e_alive[COM_MAX_PDUS]`
3. `Com_MainFunction_Rx`: after unpacking, call `E2E_Check` per PDU, report to Dem on failure
4. Cycle time enforcement: already done

After Phase 2: ALL TX goes through Com → E2E → PduR → CanIf → CAN. No bypass paths.

### Phase 3: SWC — Thin Application Layer

Rewrite each SWC to ONLY use Rte_Read/Rte_Write:

```
Sensor → IoHwAb → SWC Rte_Write → (Com reads RTE automatically) → CAN
CAN → (Com unpacks to RTE automatically) → SWC Rte_Read → Actuator
```

SWC does NOT call Com_SendSignal, PduR_Transmit, E2E_Protect, or CanIf_Transmit.

Per-ECU SWC cleanup:
- **CVC**: VehicleState, Pedal, EStop, Dashboard, Heartbeat, CanMonitor
- **FZC**: Steering, Brake, Lidar, Heartbeat, FzcSafety, FzcCom → simplify to Rte only
- **RZC**: Motor, CurrentMonitor, TempMonitor, Battery, Encoder, Heartbeat, RzcCom → simplify

### Phase 4: IoHwAb — Sensor Abstraction

1. Bench mode: configurable test values when no sensors connected
2. Fault reporting: if real HW read fails → set DTC via `Dem_ReportErrorStatus`
3. SWC reads fault flag from RTE and enters degraded mode

### Phase 5: Can MCAL — Clean Recovery

1. Bus-off recovery in `Can_MainFunction_BusOff` (production code, not experiment hack)
2. Auto-restart with backoff timer (not immediate retry causing 84/s recoveries)
3. Remove all inline FDCAN re-init from experiment timer callbacks

### Phase 6: Verification

Trace every test back to a safety goal:

| Test | Traces To | Checks |
|------|-----------|--------|
| HIL CAN rate test | SG-001..SG-008 (FTTI compliance) | Every cyclic message within ±30% of DBC cycle |
| HIL E2E test | SG-001, SG-003, SG-004 | Per-PDU alive counter, correct DataID, CRC valid |
| HIL heartbeat test | SG-008 | CVC sees FZC+RZC heartbeats, transitions INIT→RUN |
| HIL E-Stop test | SG-008 | E-Stop → SAFE_STOP within 100ms |
| HIL sensor fault test | SG-002, SG-006 | IoHwAb failure → DTC + degraded mode |
| SIL regression | All | 16 scenarios pass |

---

## Order of Execution

1. **Phase 0**: DBC audit (30 min)
2. **Phase 1**: Codegen (2-3 hours)
3. **Phase 2**: Com E2E (3-4 hours)
4. **Phase 3**: SWC rewrite (4-6 hours)
5. **Phase 4**: IoHwAb (1-2 hours)
6. **Phase 5**: Can MCAL (1 hour)
7. **Phase 6**: Verification (2-3 hours)

**Total: 14-20 hours**

---

## Success Criteria

- [ ] Every CAN message rate matches DBC GenMsgCycleTime (±30%)
- [ ] Every E2E-protected PDU has unique DataID from DBC
- [ ] Every E2E alive counter is per-PDU (no sharing)
- [ ] No SWC calls PduR_Transmit, CanIf_Transmit, or E2E_Protect directly
- [ ] IoHwAb bench mode shows non-zero test values
- [ ] CVC reaches RUN state with FZC+RZC on bus
- [ ] SIL suite passes >= 14/16
- [ ] Every firmware function traces to a SWR which traces to a SSR which traces to a SG
