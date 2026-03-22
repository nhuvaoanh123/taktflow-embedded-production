# SIL Test Suite — 10-Role Audit

**Date:** 2026-03-22
**Scope:** `test/sil/test_overtemp_hops.py`, `test_vsm_fault_transitions.py`, `test_battery_chain.py`
**Method:** Each test reviewed from 10 distinct professional perspectives

---

## 1. Safety Engineer (ISO 26262 Part 4/6)

| Finding | Sev | Test | Detail |
|---------|-----|------|--------|
| **No FTTI timing assertion** | HIGH | All 3 | Tests check that the correct state is reached but never measure HOW LONG it took. ISO 26262 requires FTTI verification. Overtemp FTTI = 500ms. Add `t_start = time.time()` before injection, `t_end` at state change, assert `< 500ms`. |
| **No negative test (fault NOT present → no false transition)** | MED | All 3 | Tests only inject faults and check reaction. Never verify that normal operation does NOT trigger a fault. Add Hop 0: "5s of normal operation → CVC stays in RUN". |
| **Overtemp Hop 5 accepts SAFE_STOP(4) but also string "SAFE_STOP"** | LOW | Overtemp | Line 208: `vs in ("SAFE_STOP", 4)` — mixing string and int comparison. The `decode_choices=False` returns int, so the string branch is dead code. Harmless but misleading. |

## 2. Test Engineer (ISTQB / Automotive SPICE SWE.6)

| Finding | Sev | Test | Detail |
|---------|-----|------|--------|
| **No test IDs or traceability** | HIGH | All 3 | No `@verifies`, `@traces_to`, or test case IDs. Cannot trace to requirements (SWR/SSR). Add comment headers linking each hop to SWR-xxx. |
| **No JUnit/xUnit output** | MED | All 3 | Print-only verdicts. CI needs machine-readable output. The `verdict_checker.py` has JUnit support but these hop tests don't use it. Add `--junit` flag or integrate with verdict_checker. |
| **Hop numbering gaps in battery test** | LOW | Battery | Hops are 1,2,3,4,5 now (fixed from 1,2,5,9). Good. |
| **No test execution timestamp** | LOW | All 3 | Add ISO 8601 timestamp at start of output for audit trail. |

## 3. AUTOSAR Architect

| Finding | Sev | Test | Detail |
|---------|-----|------|--------|
| **Tests bypass AUTOSAR layering** | INFO | All 3 | Tests inject via MQTT→plant-sim→CAN, which is correct for SIL (plant model replaces real sensors). The AUTOSAR stack (Com→PduR→CanIf→Can) is exercised end-to-end. Good. |
| **DBC is test oracle, not ARXML** | LOW | All 3 | Tests decode CAN frames using `taktflow_vehicle.dbc`. In a full AUTOSAR workflow, the ARXML model is the source of truth and DBC is derived. If DBC drifts from ARXML, tests pass but system is wrong. Mitigate: add CI check that DBC matches ARXML. |
| **`FAULT_API` constant unused** | LOW | Overtemp | Line 23: `FAULT_API = "http://localhost:8091"` is declared but never used. Dead code. Remove. |

## 4. Embedded Firmware Developer

| Finding | Sev | Test | Detail |
|---------|-----|------|--------|
| **Overtemp: `import threading` inside function body** | LOW | Overtemp | Line 148: `import threading` mid-function. Move to top-level imports for clarity. |
| **`can_flush` is aggressive** | MED | All 3 | `can_flush` drains ALL buffered frames before each read. On a busy bus with 7 ECUs, this discards hundreds of frames. If the target frame arrives during flush, it's lost. Better: read with timeout and filter, don't pre-flush. |
| **No bus error handling** | LOW | All 3 | If vcan0 goes down mid-test (e.g., container crash), `bus.recv` throws `can.CanError`. No try/except around CAN reads. Test crashes with unhelpful traceback. |

## 5. HIL/SIL Validation Engineer

| Finding | Sev | Test | Detail |
|---------|-----|------|--------|
| **No deterministic timing** | HIGH | All 3 | `time.sleep(3)` between hops is arbitrary. On a slow machine, 3s may not be enough. On a fast machine, it wastes time. Replace with polling loops with timeout (like the precondition already does). |
| **VSM test Hop 3 accepts any non-RUN state** | MED | VSM | Line 158: `mode != 1` — accepts DEGRADED, LIMP, SAFE_STOP, or SHUTDOWN. Per HARA, overcurrent from RUN should go to SAFE_STOP (via DEGRADED escalation). The test should track the expected escalation path. |
| **Battery test Hop 4 accepts any non-RUN** | MED | Battery | Same issue. Battery UV from RUN should go to LIMP (BATT_WARN) or SAFE_STOP (BATT_CRIT). The test should check which. |
| **No CAN bus load measurement** | LOW | All 3 | High bus load can cause frame drops. Should log frame count and bus utilization during test. |

## 6. ASPICE Process Auditor (SWE.5/SWE.6)

| Finding | Sev | Test | Detail |
|---------|-----|------|--------|
| **No documented test specification** | HIGH | All 3 | Tests are code-only. SWE.6 requires a test specification document mapping test cases to requirements. The test scripts ARE the executable spec, but a companion doc with preconditions, expected results, and traceability is missing. |
| **No pass/fail criteria documented outside code** | MED | All 3 | The pass criteria (e.g., "MotorFaultStatus==4") are embedded in code. An ASPICE auditor needs them in a reviewable document. Add structured comments or a test spec YAML. |
| **No test environment specification** | MED | All 3 | Tests assume Docker SIL on Ubuntu with vcan0. No documentation of required containers, versions, or configuration. Add a `test/sil/README.md` with environment setup. |

## 7. Cybersecurity Engineer (ISO 21434)

| Finding | Sev | Test | Detail |
|---------|-----|------|--------|
| **MQTT injection has no authentication** | INFO | All 3 | MQTT topic `taktflow/command/plant_inject` accepts commands without auth. In SIL this is fine. In HIL with real network, this is an attack vector. Document that MQTT auth is required for HIL/production. |
| **No input validation on injected values** | LOW | All 3 | Tests inject `temp_c=110`, `mV=4000` etc. No test for out-of-range injection (e.g., `temp_c=999999`). Add robustness test: inject absurd values, verify system doesn't crash. |

## 8. Systems Engineer

| Finding | Sev | Test | Detail |
|---------|-----|------|--------|
| **No recovery verification** | HIGH | Overtemp + Battery | Tests inject fault and check reaction, but never verify the system RECOVERS after fault clears. The overtemp test's cleanup sends `clear_temp_override` + `inject_temp(25)` but never checks CVC returns to RUN. Add recovery hop. |
| **VSM test resets between hops — masks cascading failures** | MED | VSM | Hops 4-5 reset to RUN before each fault. This prevents testing whether multiple faults cascade correctly. Add a compound fault hop: overcurrent + brake fault simultaneously. |
| **No inter-ECU coordination test** | LOW | All 3 | Tests check CVC and RZC independently. No test verifies that FZC, SC, BCM see the fault status correctly (e.g., SC should detect motor cutoff and keep relay energized during SAFE_STOP). |

## 9. Quality Manager (OEM Review)

| Finding | Sev | Test | Detail |
|---------|-----|------|--------|
| **No test report generation** | HIGH | All 3 | Tests print to stdout. No HTML/PDF report, no screenshots, no trace logs saved. OEM review requires a test report with: date, environment, DUT version (git hash), results, deviations. |
| **No git hash in output** | MED | All 3 | Test output doesn't include the firmware version under test. Add `git rev-parse HEAD` at start of output. |
| **No repeat/soak test** | LOW | All 3 | Each test runs once. No option to repeat N times to check for intermittent failures (we had several during development). Add `--repeat N` flag. |

## 10. DevOps / CI Engineer

| Finding | Sev | Test | Detail |
|---------|-----|------|--------|
| **Tests require manual Docker setup** | HIGH | All 3 | Must manually start all containers before running tests. Should have a `docker compose up` wrapper or pytest fixture that handles lifecycle. |
| **No pip requirements file** | MED | All 3 | Tests depend on `python-can`, `cantools`, `paho-mqtt`. No `requirements.txt` or `pyproject.toml` in `test/sil/`. |
| **Hardcoded `localhost`** | LOW | All 3 | MQTT_HOST = "localhost". Won't work if tests run in a different container or on a remote host. Use environment variable with fallback. |
| **No timeout on entire test** | MED | VSM | VSM test can take >120s (30s reset×3 + injection waits). No overall timeout. CI job could hang. Add `signal.alarm()` or pytest-timeout. |

---

## Summary Matrix

| Role | Critical | High | Medium | Low | Info |
|------|----------|------|--------|-----|------|
| 1. Safety Engineer | 0 | 1 | 1 | 1 | 0 |
| 2. Test Engineer | 0 | 1 | 1 | 2 | 0 |
| 3. AUTOSAR Architect | 0 | 0 | 0 | 2 | 1 |
| 4. Firmware Developer | 0 | 0 | 1 | 2 | 0 |
| 5. HIL/SIL Validation | 0 | 1 | 2 | 1 | 0 |
| 6. ASPICE Auditor | 0 | 1 | 2 | 0 | 0 |
| 7. Cybersecurity | 0 | 0 | 0 | 1 | 1 |
| 8. Systems Engineer | 0 | 1 | 1 | 1 | 0 |
| 9. Quality Manager | 0 | 1 | 1 | 1 | 0 |
| 10. DevOps/CI | 0 | 1 | 2 | 1 | 0 |
| **Total** | **0** | **8** | **11** | **12** | **2** |

## Top 5 Action Items (by impact)

1. **Add FTTI timing assertions** — measure injection-to-reaction latency, assert < FTTI budget (Safety + Validation)
2. **Add test IDs + traceability** — link each hop to SWR/SSR requirements (Test Engineer + ASPICE)
3. **Add recovery verification** — after fault, verify system returns to RUN (Systems + Safety)
4. **Add negative test (Hop 0)** — confirm normal operation doesn't trigger false faults (Safety)
5. **Replace `time.sleep` with polling loops** — deterministic timing, no arbitrary waits (Validation + DevOps)
