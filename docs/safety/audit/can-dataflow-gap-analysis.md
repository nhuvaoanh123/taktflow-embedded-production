# CAN Data Flow Gap Analysis — Multi-Perspective Review

**Date:** 2026-03-21
**Scope:** Full CAN data flow from DBC → ARXML → codegen → BSW → SWC → CAN bus → receiver
**Method:** 12 perspectives, each representing a real stakeholder in an automotive program

---

## Perspective 1: System Architect (Top-Down Consistency)

**Question:** Does the architecture enforce a single data flow path with no bypass?

| # | Check | Status | Gap |
|---|-------|--------|-----|
| 1.1 | DBC is single source of truth for CAN | **PASS** | — |
| 1.2 | ARXML generated from DBC (not hand-edited) | **PASS** | — |
| 1.3 | C configs generated from ARXML (not hand-edited) | **PASS** | — |
| 1.4 | SWC→RTE→Com→PduR→CanIf (no layer skip) | **PASS** | Step 7 enforces: 0 violations |
| 1.5 | E2E in one place (Com layer, not SWC) | **PASS** | Phase 2 complete |
| 1.6 | Reverse path: CanIf→PduR→Com→RTE→SWC | **PASS** | — |
| 1.7 | SC uses independent path (no shared AUTOSAR stack) | **PASS** | TMS570, minimal code |
| 1.8 | No backdoor TX path (UART, SPI, debug) | **GAP** | SIL_DIAG `fprintf(stderr)` exists in Com.c — harmless but should be gated by build flag only |
| 1.9 | Configuration round-trip validated | **PASS** | Step 6: E2E DataIDs + CycleTimeMs verified |
| 1.10 | Bus load budget documented | **GAP** | Step 1 checks bus load <75% but no formal bus load allocation table per ECU |

**Priority gaps:**
- **1.10** Bus load allocation table needed — currently only aggregate check, no per-ECU budget showing worst-case burst (e.g., all event messages + cyclics simultaneously)

---

## Perspective 2: Functional Safety Engineer (ISO 26262 Part 6)

**Question:** Does the CAN data flow meet ASIL D requirements for freedom from interference?

| # | Check | Status | Gap |
|---|-------|--------|-----|
| 2.1 | E2E covers all ASIL B+ messages | **GAP** | Motor_Temperature (ASIL A) and Battery_Status (QM) have E2E signals in DBC but no unique DataID — E2E header bytes wasted |
| 2.2 | FTTI budget documented and met | **PASS** | heartbeat-ftti-budget.md, all within budget |
| 2.3 | Safe state on RX timeout (zero-fill) | **PASS** | AUTOSAR ComRxDataTimeoutAction = REPLACE |
| 2.4 | Safe state on E2E failure (discard frame) | **PASS** | Com_RxIndication returns on E2E_STATUS_ERROR |
| 2.5 | No silent data corruption path | **GAP** | E2E discard retains **previous** shadow buffer value — if 1 good frame followed by N bad frames, stale value persists until timeout. No "signal quality" indicator to SWC |
| 2.6 | DFA (Dependent Failure Analysis) documented | **PARTIAL** | dfa.md exists but doesn't cover Com_MainFunction_Tx failure mode (stuck in critical section) |
| 2.7 | Software Unit Verification per Part 6 Table 9 | **GAP** | Com.c E2E integration (Phase 2) has no unit test yet |
| 2.8 | Alive counter rollover handling | **PASS** | 4-bit wraps at 15, MaxDeltaCounter configurable per DBC |
| 2.9 | Data ID uniqueness across bus | **PASS** | Step 1 check 1: no duplicate E2E DataIDs |
| 2.10 | Diagnostic coverage of E2E failures | **GAP** | E2E discard is silent — no DTC raised, no Dem event, no counter exposed to diagnostics |

**Priority gaps:**
- **2.5** Stale signal persistence between E2E failure and timeout — SWC has no way to know signal is degraded
- **2.7** Com.c Phase 2 E2E integration untested
- **2.10** Silent E2E failure — no diagnostic visibility

---

## Perspective 3: ASPICE Assessor (Process & Traceability)

**Question:** Can every CAN signal be traced from requirement to test?

| # | Check | Status | Gap |
|---|-------|--------|-----|
| 3.1 | DBC signals traced to TSR (Satisfies attribute) | **PASS** | 17 messages have Satisfies links |
| 3.2 | ARXML carries ADMIN-DATA traceability | **PASS** | ASIL, Satisfies, CAN_ID in SDGS |
| 3.3 | Generated config traces to ARXML source | **GAP** | Generated files have `/* GENERATED */` header but no ARXML element reference or generation timestamp |
| 3.4 | SWC code traces to SWR via @safety_req | **PARTIAL** | Safety SWCs have @safety_req, non-safety SWCs may not |
| 3.5 | Test traces to requirement | **PARTIAL** | verdict_checker has @traces_to but not all test files |
| 3.6 | Change impact analysis tool | **GAP** | If a DBC signal changes, no automated tool shows which SWCs, tests, and safety arguments are affected |
| 3.7 | Configuration management of DBC | **PASS** | Git-controlled, single source of truth |
| 3.8 | Review evidence for safety-critical changes | **GAP** | No formal review record for Phase 2 Com.c E2E changes — HITL comments exist for Phase 0/1 but not Phase 2 |

**Priority gaps:**
- **3.6** No impact analysis tool — DBC signal rename/removal could silently break consumers
- **3.8** Phase 2 changes to safety-critical Com.c lack formal review evidence

---

## Perspective 4: Embedded Developer (API Usability & Correctness)

**Question:** Is the CAN API easy to use correctly and hard to misuse?

| # | Check | Status | Gap |
|---|-------|--------|-----|
| 4.1 | Signal type safety (compile-time) | **GAP** | Com_SendSignal takes `const void*` — no type check. Passing uint16 to a uint8 signal compiles but corrupts |
| 4.2 | Signal ID validated at runtime | **PASS** | SignalId >= signalCount returns E_NOT_OK |
| 4.3 | PDU overflow protection | **PASS** | BitSize > 16 = no action (documented limitation) |
| 4.4 | Thread safety documented | **PARTIAL** | SchM critical sections used but no @note on which functions are ISR-safe vs task-safe |
| 4.5 | Return value handling enforced | **GAP** | All callers cast to `(void)` — E2E_Protect/PduR_Transmit return values ignored |
| 4.6 | Debug visibility | **PASS** | `com_tx_send_count[]`, `g_dbg_com_tx_skip[]` counters accessible via UART/UDS |
| 4.7 | Error code granularity | **GAP** | Com_RxIndication returns void — caller can't know if E2E check failed vs PDU dropped |
| 4.8 | Configuration constants reviewed | **GAP** | `COM_TX_MAIN_PERIOD_MS = 10u` hardcoded in Com.c — must match BSW timer but no compile-time assert |
| 4.9 | Unused variable warnings | **GAP** | Phase 2 changes left unused variables in Swc_RzcCom.c (pdu[], pdu_info after removing PduR_Transmit) |

**Priority gaps:**
- **4.1** void* signal API allows type mismatch — runtime corruption possible
- **4.8** No static_assert that COM_TX_MAIN_PERIOD_MS matches scheduler config
- **4.9** Dead code from Phase 3 SWC cleanup (unused buffers)

---

## Perspective 5: Test Engineer (Verification Coverage)

**Question:** Are all failure modes tested?

| # | Check | Status | Gap |
|---|-------|--------|-----|
| 5.1 | Nominal TX/RX chain | **PASS** | test_battery_chain, test_scenario_display |
| 5.2 | Heartbeat timeout → state transition | **PASS** | test_vsm_fault_transitions |
| 5.3 | E2E CRC corruption injection | **GAP** | No test injects a corrupted CRC and verifies discard |
| 5.4 | E2E alive counter gap injection | **GAP** | No test injects a skipped counter and verifies detection |
| 5.5 | E2E DataID mismatch injection | **GAP** | No test injects wrong DataID and verifies rejection |
| 5.6 | Bus-off recovery test | **GAP** | No SIL test for bus-off → recovery → message resumption |
| 5.7 | Simultaneous multi-ECU fault | **PARTIAL** | test_vsm tests single-ECU loss, not dual-ECU simultaneous loss |
| 5.8 | DBC error injection (test_gates.py) | **PASS** | 5 error injections + clean DBC test |
| 5.9 | Timing jitter test | **GAP** | No test measures actual TX timing against configured CycleTimeMs |
| 5.10 | Overflow/wrap test | **GAP** | No test for 4-bit alive counter wraparound (14→15→0→1 transition) |

**Priority gaps:**
- **5.3/5.4/5.5** Zero E2E fault injection tests — the core safety mechanism is untested at system level
- **5.6** Bus-off recovery path untested
- **5.10** Counter wraparound untested

---

## Perspective 6: Calibration / Application Engineer (Runtime Tuning)

**Question:** Can CAN parameters be tuned without rebuilding firmware?

| # | Check | Status | Gap |
|---|-------|--------|-----|
| 6.1 | Cycle time configurable at build | **PASS** | Generated from DBC GenMsgCycleTime |
| 6.2 | Cycle time configurable at runtime | **GAP** | CycleTimeMs is `const` in flash — requires rebuild for any timing change |
| 6.3 | E2E DataID configurable at runtime | **GAP** | DataId is `const` — cannot change without rebuild |
| 6.4 | Timeout thresholds configurable | **GAP** | TimeoutMs is `const` — cannot adjust detection sensitivity in field |
| 6.5 | Signal scaling/offset in config | **GAP** | DBC has factor/offset but codegen doesn't generate scaling — raw values only |
| 6.6 | NvM-backed calibration | **GAP** | No NvM integration for CAN parameters — all baked into flash |
| 6.7 | UDS-based parameter read | **PARTIAL** | ReadDataByIdentifier supports some signals but not CAN config parameters |

**Priority gaps:**
- **6.2/6.4** All timing parameters baked into flash — no field tuning without OTA
- **6.5** Signal physical values (°C, mA, RPM) must be converted manually by SWC

---

## Perspective 7: Cybersecurity Engineer (CAN Security)

**Question:** Is the CAN bus protected against adversarial input?

| # | Check | Status | Gap |
|---|-------|--------|-----|
| 7.1 | Unknown CAN ID dropped | **PASS** | CanIf_RxIndication silent discard |
| 7.2 | E2E prevents replay | **PARTIAL** | Alive counter detects replay of same frame, but attacker can increment counter |
| 7.3 | Authentication (SecOC) | **GAP** | No AUTOSAR SecOC — CAN messages not authenticated. Any device on bus can impersonate any ECU |
| 7.4 | UDS access control | **GAP** | No SecurityAccess (0x27) implementation — all UDS services open |
| 7.5 | DTC injection prevention | **GAP** | Any ECU can broadcast DTC on 0x500 — no source verification |
| 7.6 | Bus-off attack mitigation | **GAP** | Deliberate dominant bit attack → bus-off, no mitigation beyond recovery timer |
| 7.7 | Firmware update integrity | **PASS** | OTA spec requires Ed25519/ECDSA signature verification |
| 7.8 | Diagnostic session timeout | **GAP** | No TesterPresent timeout — once diagnostic session opened, no auto-close |

**Priority gaps:**
- **7.3** No SecOC — fundamental CAN vulnerability. Acceptable for development/bench but NOT for production with external bus access
- **7.4** UDS wide open — anyone on the bus can reprogram ECUs

---

## Perspective 8: Production / Manufacturing Engineer (EOL & Flashing)

**Question:** Can the CAN config be validated during end-of-line testing?

| # | Check | Status | Gap |
|---|-------|--------|-----|
| 8.1 | ECU identification via UDS | **PASS** | ReadDID for ECU ID, SW version |
| 8.2 | CAN bus-off detection at EOL | **GAP** | No EOL-specific bus health check service |
| 8.3 | E2E DataID verification at EOL | **GAP** | No UDS service to read back configured E2E DataID table |
| 8.4 | Signal range check service | **GAP** | No built-in boundary value test (send min/max signal, verify echo) |
| 8.5 | Network Management (NM) | **GAP** | No AUTOSAR NM — ECUs have no coordinated wake/sleep. OK for always-on bench but not for vehicle with sleep mode |
| 8.6 | Variant coding | **GAP** | All ECUs have identical CAN config — no variant handling for different vehicle configs |

**Priority gaps:**
- **8.5** No NM layer — power management impossible without it
- **8.3** E2E config not verifiable at EOL — silent misconfiguration risk

---

## Perspective 9: SIL / HIL Test Infrastructure Engineer

**Question:** Does the simulation accurately represent the physical CAN data flow?

| # | Check | Status | Gap |
|---|-------|--------|-----|
| 9.1 | Same Com.c code in SIL and target | **PASS** | Platform abstraction, same source |
| 9.2 | CAN timing accurate in SIL | **GAP** | Docker/POSIX scheduler is non-deterministic — 10ms cycle may jitter ±5ms. Physical CAN timing not representative |
| 9.3 | E2E counters match between SIL and target | **PASS** | Same E2E library, same counter logic |
| 9.4 | Bus-off simulation | **GAP** | POSIX CAN (socketcan) doesn't simulate bus-off — code path untestable in SIL |
| 9.5 | Multi-ECU synchronization | **GAP** | SIL runs all 7 ECUs in sequential Docker threads — no true parallel execution, race conditions masked |
| 9.6 | Plant model fidelity | **PARTIAL** | plant-sim provides sensor values but no motor/brake/steering dynamics model |
| 9.7 | Fault injection framework | **PARTIAL** | REST API can inject signal values but cannot inject CAN-level faults (bit errors, frame loss) |
| 9.8 | Gateway bridge accuracy | **PASS** | Same MQTT→CAN bridge in SIL and Netcup deployment |

**Priority gaps:**
- **9.2** SIL timing is non-representative — timeout tests may pass in SIL but fail on hardware
- **9.5** Sequential SIL masks concurrency bugs (SchM critical sections, RTE race conditions)
- **9.7** No CAN-level fault injection (only signal-level)

---

## Perspective 10: Vehicle Integration Engineer (Physical Bus)

**Question:** Does the CAN bus work correctly when all ECUs are connected?

| # | Check | Status | Gap |
|---|-------|--------|-----|
| 10.1 | Physical layer (120Ω termination) | **GAP** | No documentation of bus topology — which nodes have termination resistors |
| 10.2 | Arbitration under load | **GAP** | No test with all 7 ECUs + external tester simultaneously — potential FIFO overflow |
| 10.3 | EMC immunity | **GAP** | No EMC test plan for CAN transceivers |
| 10.4 | Wake-up / sleep | **GAP** | No NM, no sleep mode, no wake-up pattern |
| 10.5 | Bus-off auto-recovery timing | **PARTIAL** | 10ms recovery timer in experiments but not in production BSW |
| 10.6 | CAN transceiver fault detection | **GAP** | No TJA1050/TJA1040 fault pin monitoring |
| 10.7 | Stub length / cable routing | **GAP** | No bus topology specification (max stub length, cable type) |
| 10.8 | Ground loop prevention | **GAP** | USB hub powered multiple ECUs during bench testing — caused flaky CAN (documented lesson learned) |

**Priority gaps:**
- **10.1/10.7** No physical CAN bus specification — will cause issues at vehicle integration
- **10.2** No full-load arbitration test

---

## Perspective 11: OEM Customer / Homologation Engineer

**Question:** Does the CAN communication meet type-approval requirements?

| # | Check | Status | Gap |
|---|-------|--------|-----|
| 11.1 | ISO 11898 compliance (physical) | **GAP** | No formal CAN compliance test report |
| 11.2 | ISO 14229 UDS compliance | **PARTIAL** | TesterPresent, ReadDTC, ClearDTC work but no full service conformance test |
| 11.3 | ISO 26262 Part 6 SW verification | **GAP** | Com.c E2E path lacks unit test evidence required by Table 9 |
| 11.4 | AUTOSAR conformance | **GAP** | "AUTOSAR-like" — not formally AUTOSAR conformant (no AUTOSAR membership, no conformance test) |
| 11.5 | CAN matrix exchange format | **PASS** | Standard Vector DBC format — importable by any tool |
| 11.6 | Gateway protocol (CAN↔Ethernet) | **GAP** | MQTT bridge is proprietary — no SOME/IP or DoIP for standardized connectivity |
| 11.7 | Cybersecurity compliance (UN R155) | **GAP** | No SecOC, no IDS, no secure boot chain documented |

**Priority gaps:**
- **11.4** AUTOSAR-like is fine for bench but insufficient for production homologation
- **11.7** UN R155 (mandatory in EU since 2024) requires cybersecurity management system

---

## Perspective 12: End User / Fleet Operator (Operational Safety)

**Question:** What happens when things go wrong in the field?

| # | Check | Status | Gap |
|---|-------|--------|-----|
| 12.1 | Safe stop on any CAN fault | **PASS** | Timeout → zero-fill → VSM → SAFE_STOP → brake + center steering |
| 12.2 | Driver notification of fault | **GAP** | ICU dashboard receives fault mask but no HMI specification for warning display |
| 12.3 | Fault logging for post-incident analysis | **PARTIAL** | DTC stored in Dem but no freeze frame data (what were signal values at fault time?) |
| 12.4 | Remote diagnostics | **GAP** | TCU telematics heartbeat exists but no cloud reporting of CAN health metrics |
| 12.5 | Predictive maintenance | **GAP** | No trend monitoring (e.g., increasing E2E failure rate = degrading CAN connector) |
| 12.6 | Recovery procedure documented | **GAP** | No operator manual for "what to do when vehicle enters SAFE_STOP" |
| 12.7 | OTA update of CAN config | **PARTIAL** | OTA framework exists but CAN config is in flash, requires full ECU reflash |

**Priority gaps:**
- **12.2** No HMI specification — driver may not know vehicle is in degraded mode
- **12.3** No freeze frame — post-incident root cause analysis impossible without CAN trace hardware

---

## Summary: Gap Heat Map

| Perspective | Critical | Major | Minor | Total |
|-------------|----------|-------|-------|-------|
| 1. System Architect | 0 | 1 | 1 | 2 |
| 2. Safety Engineer | 1 | 2 | 0 | 3 |
| 3. ASPICE Assessor | 0 | 2 | 1 | 3 |
| 4. Embedded Developer | 1 | 1 | 1 | 3 |
| 5. Test Engineer | 2 | 1 | 0 | 3 |
| 6. Calibration Engineer | 0 | 2 | 1 | 3 |
| 7. Cybersecurity Engineer | 2 | 1 | 0 | 3 |
| 8. Manufacturing Engineer | 0 | 2 | 1 | 3 |
| 9. SIL/HIL Infrastructure | 1 | 2 | 0 | 3 |
| 10. Vehicle Integration | 1 | 2 | 0 | 3 |
| 11. OEM / Homologation | 1 | 2 | 0 | 3 |
| 12. End User / Fleet | 0 | 2 | 1 | 3 |
| **TOTAL** | **9** | **20** | **6** | **35** |

---

## Top 10 Gaps by Risk (across all perspectives)

| Rank | Gap | Perspective | Risk | Fix Effort |
|------|-----|-------------|------|-----------|
| 1 | **No E2E fault injection tests** (5.3-5.5) | Test | Safety mechanism unverified at system level | Medium — write 3 SIL scenarios |
| 2 | **No SecOC / CAN authentication** (7.3) | Cybersecurity | Any device can impersonate any ECU | Large — requires AUTOSAR SecOC stack |
| 3 | **Stale signal on E2E failure** (2.5) | Safety | SWC unaware of degraded signal quality | Small — add signal quality flag |
| 4 | **Silent E2E discard — no DTC** (2.10) | Safety | No diagnostic record of communication fault | Small — add Dem event on E2E failure |
| 5 | **SIL timing non-representative** (9.2) | HIL/SIL | Timeout tests not trustworthy | Medium — add HIL timing validation |
| 6 | **void* signal API** (4.1) | Developer | Type mismatch causes silent corruption | Medium — add typed wrapper macros |
| 7 | **No physical bus specification** (10.1) | Integration | Vehicle integration will fail without spec | Small — document topology |
| 8 | **No HMI fault notification spec** (12.2) | End user | Driver unaware of degraded mode | Small — write HMI spec |
| 9 | **No NM layer** (8.5) | Manufacturing | No sleep/wake = always-on power drain | Large — requires AUTOSAR NM |
| 10 | **Phase 2 Com.c untested** (2.7) | Safety | Safety-critical code without unit tests | Medium — write Com E2E unit tests |

---

## Fix Status (updated 2026-03-21)

| # | Gap | Status | Evidence |
|---|-----|--------|----------|
| 1 | No E2E fault injection tests (5.3-5.5) | **DONE** | test_Com_asild.c: 9 new E2E + quality tests |
| 2 | No SecOC (7.3) | DEFERRED | Cybersecurity — out of scope for bench |
| 3 | Stale signal on E2E failure (2.5) | **DONE** | Com_SignalQualityType enum, Com_GetRxPduQuality() API |
| 4 | Silent E2E discard (2.10) | **DONE** | Dem_ReportErrorStatus on E2E failure, per-PDU E2eDemEventId |
| 5 | SIL timing non-representative (9.2) | OPEN | Requires HIL bench |
| 6 | void* signal API (4.1) | **DONE** | Com_SendSignal_u8/u16/s16 type-safe macros |
| 7 | No physical bus spec (10.1) | **DONE** | docs/aspice/system/can-bus-topology.md |
| 8 | No HMI fault notification (12.2) | **DONE** | docs/aspice/system/hmi-fault-notification.md |
| 9 | No NM layer (8.5) | DEFERRED | Backlog — not needed for bench |
| 10 | Phase 2 Com.c untested (2.7) | **DONE** | test_Com_asild.c: E2E protect/check/quality unit tests |
| 11 | COM_TX_MAIN_PERIOD_MS not asserted (4.8) | **DONE** | Compile-time #error check in Com.c |
| 12 | Dead code from Phase 3 (4.9) | **DONE** | Removed pdu[], pdu_info from Swc_RzcCom, Swc_FzcCom |
| 13 | Bus load allocation (1.10) | **DONE** | docs/aspice/system/can-bus-load-budget.md |
| 14 | No freeze frame (12.3) | DESIGNED | Plan: docs/plans/plan-dem-freeze-frame.md (backlog) |

**Score: 10/14 fixed, 1 designed, 3 deferred.**
