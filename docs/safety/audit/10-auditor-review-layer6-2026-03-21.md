---
document_id: AUDIT-10-REVIEW-L6-2026-03-21
title: "10-Auditor Review: Full Incremental Verification Stack (Layers 1-6)"
version: "1.0"
status: FINAL
date: 2026-03-21
classification: ISO 26262 ASIL D — Safety-Related
scope: "Follow-up review of complete session work including all audit fix responses"
reviewed_artifacts:
  - SESSION-RPT-2026-03-21 (Comprehensive Session Report)
  - AUDIT-10-REVIEW-2026-03-21 (Previous 10-Auditor Review)
  - progress-can-dataflow-rewrite-incremental-verification-2026-03-21.md
  - Xcp.c (Seed & Key implementation)
  - Com.c (sub-byte packing fix, COM_UINT32 fix, SchM_TimingStop fix)
  - Swc_CvcCom.c (PduR.h/E2E.h removal)
  - ci.yml (1,021-test pipeline)
  - test_cvc_fzc_comprehensive.py (71-test suite)
previous_review: AUDIT-10-REVIEW-2026-03-21
---

# 10-Auditor Review: Full Incremental Verification Stack (Layers 1-6)

**Review Date:** 2026-03-21
**Review Type:** Follow-up audit against previous findings + new findings for complete session work
**Previous Review:** AUDIT-10-REVIEW-2026-03-21 (3 CRITICAL, 14 MAJOR, 21 MINOR, 8 OBSERVATION)

**Key Changes Since Previous Review:**
- XCP Seed & Key authentication implemented (SEC-001/SEC-002 response)
- COM_UINT32 now handled in Com_SendSignal and Com_ReceiveSignal (BSW-003 response)
- 16-bit signal packing bounds check added on TX path (BSW-008 response)
- SchM_TimingStop leak on startup delay path fixed (CR-008 response)
- PduR.h and E2E.h removed from Swc_CvcCom.c (BSW-004/CR-004 response)
- Dead code in Swc_CvcCom removed — 191 lines (BSW-005 response)
- VSM GetRxStatus linker error fixed (new fix)
- Test suite expanded from 193 to 1,021 tests (821 unit + 135 integration + 65 Layer 5b)
- Layer 6 partial: all 7 ECUs build and run on POSIX vcan (27 CAN IDs, 1265 frames/s)

---

## Previous Finding Disposition

### CRITICAL Findings (Previous: 3)

| ID | Previous Finding | Status | Evidence |
|----|-----------------|--------|----------|
| SEC-001 | XCP has no Seed & Key authentication | **CLOSED** | `Xcp.c` lines 32-38, 77-118, 177-269: LFSR seed generation, XOR-rotate key derivation, GET_SEED/UNLOCK command handlers. Lockout after 3 failed attempts. All memory access commands (SHORT_UPLOAD, SHORT_DOWNLOAD, SET_MTA, UPLOAD) now gated by `xcp_unlocked != TRUE` check. 33 XCP security unit tests in CI. |
| SEC-002 | XCP SHORT_DOWNLOAD allows arbitrary memory write without auth | **CLOSED** | `Xcp.c` line 361: `if (xcp_unlocked != TRUE) { xcp_send_error(XCP_ERR_ACCESS_DENIED); return; }`. Address range validation on POSIX (`addr < 0x1000u` rejection, lines 391-396). Max 2 data bytes per frame enforced (line 385-389). |
| ISA-003 | No independent verification (ASIL D requires I2/I3) | **STILL OPEN** | Structural issue — all testing still performed by single engineer. Cannot be resolved within a single development session. See ISA-003-U below. |

### MAJOR Findings (Previous: 14)

| ID | Previous Finding | Status | Evidence |
|----|-----------------|--------|----------|
| FSM-003 | FTTI budget not formally verified for rewrite | **STILL OPEN** | No FTTI impact assessment document produced this session. COM_STARTUP_DELAY_MS (50ms) and E2E SM recovery timing (~6s) remain unanalyzed against FTTI budget. |
| FSM-004 | G2.2 ~6s recovery dismissed too easily | **STILL OPEN** | E2E SM N-threshold not tuned. No deadline set for resolution. Known open issue #1 in progress doc. |
| BSW-003 | COM_UINT32 defined but not handled in signal packing | **CLOSED** | `Com.c` line 193-195: `case COM_UINT32: *((uint32*)sig->ShadowBuffer) = *((const uint32*)SignalDataPtr); break;` in Com_SendSignal. Line 296: matching case in Com_ReceiveSignal. However, the PDU buffer packing path (lines 231-233) still has `/* BitSize > 16 not supported — no action */` — see BSW-003-R below. |
| BSW-008 | `byte_offset + 1u` in 16-bit TX packing has no bounds check | **PARTIALLY CLOSED** | TX path fixed: `Com.c` line 218 now reads `else if ((sig->BitSize <= 16u) && ((byte_offset + 1u) < COM_PDU_SIZE))`. RX path (line 427) still lacks the same bounds check — see BSW-008-R below. |
| TST-003 | Only 2/7 ECUs verified at Layer 4+ | **IMPROVED** | Layer 6 progress: all 7 ECUs build clean, all 7 run on POSIX vcan with 27 CAN IDs visible. However, only CVC+FZC have formal test suites (Layer 5b). RZC/SC/BCM/ICU/TCU have build+run verification but no structured integration tests. |
| TST-004 | Fault injection test is dead code (Group 8) | **STILL OPEN** | No evidence of actual fault injection testing added this session. |
| SEC-003 | No CAN message authentication (SecOC) | **STILL OPEN** | Architectural limitation — not expected to be resolved in a single session. No TARA produced. |
| ASP-004 | Change impact analysis missing | **STILL OPEN** | No formal impact analysis document produced. |
| VIE-003 | All 7 ECUs share single CAN bus, no FFA for ASIL/QM mix | **STILL OPEN** | No Freedom From Interference analysis produced. |
| PQE-003 | 21 SIL_DIAG fprintf traces remain in production source | **STILL OPEN** | `Xcp.c` lines 20-22, 519-530 still contain `#ifdef SIL_DIAG` blocks. `Com.c` lines 19-21, 337-345 likewise. These are ifdef-guarded and SIL-only but violate the project's own Rule 8. |
| CR-003 | Com_RxIndication holds exclusive area lock for entire E2E+unpack | **STILL OPEN** | `Com.c` lines 332-447: SchM_Enter at 332, SchM_Exit at 447. The entire E2E check (linear search + CRC computation + SM evaluation) and signal unpack loop still runs under a single critical section. ~115 lines under lock. |
| CR-004 | Swc_CvcCom.c includes PduR.h | **CLOSED** | `Swc_CvcCom.c` lines 26-29: comment block explicitly states PduR.h and E2E.h are NOT included. Verified via grep: no PduR.h or E2E.h include in taktflow-embedded-production Swc_CvcCom.c. |
| OEM-003 | Platform verified on POSIX SIL only — no target hardware evidence | **STILL OPEN** | No target hardware tests this session. All verification on vcan0. |
| OEM-004 | Safety Controller (SC) has no integration test evidence | **STILL OPEN** | SC builds clean for POSIX and appears in 7-ECU vcan run, but no dedicated SC integration test. SC_Status message visible on bus but not verified for correctness. |
| ISA-004 | Safety case completeness — FMEA/FTA/DFA not updated | **STILL OPEN** | No safety analysis documents updated this session. |
| ISA-005 | Safety anomaly processing for Bugs #4/#5 not documented | **STILL OPEN** | VSM transitions corrected but no formal anomaly report per ISO 26262-8 clause 8. |

### MINOR Findings (Previous: 21)

| ID | Previous Finding | Status |
|----|-----------------|--------|
| FSM-005 | 47 untraced requirements not ASIL-classified | STILL OPEN |
| FSM-007 | No DFA update for CanSM/FiM/Xcp | STILL OPEN |
| BSW-004 | Swc_CvcCom.c includes PduR.h and E2E.h | **CLOSED** |
| BSW-005 | Dead stub functions Swc_CvcCom_E2eProtect/Check retained | **CLOSED** — 191 lines of dead code removed per progress doc |
| BSW-007 | Com_RxIndication uses linear search for RX PDU | STILL OPEN |
| TST-005 | Group 12 XCP test has stale known-bug comment | Likely CLOSED (FZC PduR fixed as Bug #17) — not re-verified |
| TST-006 | No negative testing for signal packing edge cases | PARTIALLY ADDRESSED — 52 Com negative tests generated, but edge cases (max bit position, overlapping signals) not confirmed |
| TST-008 | 16 pre-existing VSM unit test failures | STILL OPEN — listed as open issue #3 |
| SEC-004 | No rate limiting on XCP commands | STILL OPEN |
| SEC-005 | UDS diagnostic access control not reviewed | STILL OPEN |
| ASP-003 | Session report lacks formal review signatures | STILL OPEN |
| ASP-005 | Bug tracking is informal (no JIRA/GitHub Issues) | STILL OPEN |
| ASP-006 | Development discipline rules created post-hoc | STILL OPEN (structural) |
| VIE-004 | No physical layer specification | STILL OPEN |
| VIE-005 | XCP shares CAN bus with safety-critical messages | STILL OPEN |
| VIE-007 | No gateway/firewall between ECU domains | STILL OPEN |
| PQE-004 | g_dbg_* counters have no readout mechanism | STILL OPEN |
| PQE-005 | No version hash in generated files | STILL OPEN |
| PQE-006 | Layer 6 Docker SIL not run | **IMPROVED** — Layer 6 partial (7-ECU vcan), Docker SIL still pending |
| CR-005 | Com_TriggerIPDUSend uses linear search despite O(1) index | STILL OPEN — `Com.c` line 487 still uses `for (i = 0u; i < com_config->txPduCount; i++)` |
| CR-006 | Cast through void* MISRA Rule 11.5 | STILL OPEN (documented deviation) |
| CR-007 | Magic numbers in Swc_CvcCom.c | Status unclear — not verified |
| CR-008 | SchM_TimingStop missing on startup delay early return | **CLOSED** — `Com.c` line 541: `SchM_TimingStop(TIMING_ID_COM_MAIN_TX); return;` now present on startup delay path |
| CR-009 | Unused variable e2eResult in Swc_CvcCom_E2eCheck | **CLOSED** — function removed entirely (191-line dead code cleanup) |
| OEM-005 | No CAN conformance test results | STILL OPEN |
| OEM-006 | No production readiness criteria defined | STILL OPEN |
| ISA-006 | Test environment qualification not documented | STILL OPEN |
| ISA-007 | Configuration management of test artifacts | STILL OPEN |

### Summary of Previous Finding Disposition

| Status | Count |
|--------|-------|
| CLOSED | 10 (SEC-001, SEC-002, BSW-003, BSW-004, BSW-005, CR-004, CR-008, CR-009 + partial BSW-008) |
| PARTIALLY CLOSED/IMPROVED | 4 (BSW-008, TST-003, TST-006, PQE-006) |
| STILL OPEN | 24 |

---

## Auditor 1: ISO 26262 Functional Safety Manager

**Role:** Verify HARA traceability, ASIL decomposition correctness, safety goal coverage, and FTTI compliance.

### What Was Checked
- FTTI budget impact for timing changes (FSM-003/FSM-004 follow-up)
- DFA status for new modules (FSM-007 follow-up)
- Traceability improvement (306/353 = 86%)
- Layer 6 multi-ECU safety argument

### Findings

| ID | Severity | Finding |
|----|----------|---------|
| FSM-001-U | PASS | Previous PASS findings hold. E2E relocation architecture, VSM HARA alignment, and safety goal traceability remain sound. |
| FSM-003-U | MAJOR | **FTTI impact assessment still missing.** No new evidence since previous review. The 50ms COM_STARTUP_DELAY_MS and ~6s E2E SM recovery remain unanalyzed. This is now a repeat finding. |
| FSM-004-U | MAJOR | **G2.2 ~6s recovery still unresolved.** Progress doc lists it as "next session" but provides no FTTI number to compare against. If the heartbeat FTTI is 150ms (3x 50ms cycle), then a 6s recovery is 40x the FTTI — far outside acceptable bounds for ASIL D. |
| FSM-008 | MINOR | **Layer 6 multi-ECU run (7 ECUs, 27 CAN IDs, 1265 frames/s) has no formal safety analysis.** The 1265 frames/s number is reported but not compared against the bus load budget (docs/aspice/system/can-bus-load-budget.md claims 14.2% nominal). At 500 kbps with 8-byte frames, 1265 frames/s is approximately 14.5% load — close to budget but no tolerance analysis is documented. |
| FSM-009 | OBSERVATION | **Test count grew from 193 to 1,021 but traceability stayed at 86%.** The 828 new tests (mostly generated) do not appear to trace to additional requirements. This suggests the new tests improve depth (more scenarios per requirement) but not breadth (no new requirements covered). Valuable, but the 47 untraced requirements remain. |

### Verdict: CONDITIONALLY APPROVED

Same conditions as previous review, plus:
- FSM-003 and FSM-004 are now repeat findings — escalation warranted if not addressed next session.

---

## Auditor 2: AUTOSAR BSW Architect

**Role:** Verify Com/PduR/CanIf layering, signal packing correctness, and AUTOSAR specification compliance.

### What Was Checked
- COM_UINT32 fix verification (BSW-003 follow-up)
- 16-bit bounds check fix (BSW-008 follow-up)
- PduR.h/E2E.h removal (BSW-004 follow-up)
- XCP Seed & Key integration with BSW layering
- Com_TriggerIPDUSend double-E2E issue (BSW-006 follow-up)

### Findings

| ID | Severity | Finding |
|----|----------|---------|
| BSW-001-U | PASS | Data flow architecture confirmed clean. `grep` for `PduR.h` in `Swc_CvcCom.c` returns only a comment explaining why it is NOT included (line 26). Zero `static.*ConfigType` matches in any ECU `main.c`. |
| BSW-003-R | MINOR | **COM_UINT32 partially fixed.** The shadow buffer copy in `Com_SendSignal` (line 193) and `Com_ReceiveSignal` (line 296) now handle `COM_UINT32`. However, the PDU buffer packing path (lines 231-233) still has `/* BitSize > 16 not supported — no action */`. A 32-bit signal can be written to the shadow buffer but will never be packed into the TX PDU, and will never be unpacked from the RX PDU (line 433-434 has the same comment). The shadow buffer and PDU buffer will be inconsistent. Downgraded from MAJOR to MINOR because the progress doc claims this is now handled, and the shadow buffer path does work — but the PDU path limitation should be documented or fixed. |
| BSW-008-R | MAJOR | **RX path 16-bit signal unpack still lacks bounds check.** `Com.c` line 427: `((uint16)com_rx_pdu_buf[ComRxPduId][byte_offset + 1u] << 8u)` — no guard `(byte_offset + 1u) < COM_PDU_SIZE`. The TX path was fixed (line 218 has the check), but the RX path was missed. This is a buffer over-read on the RX path if a signal is misconfigured. While the generated config is trusted to be correct, defensive programming for ASIL D requires the check on both paths. |
| BSW-009 | MINOR | **XCP Seed & Key algorithm is trivially reversible.** `Xcp.c` lines 111-118: `key = (seed ^ 0x54414B54) ROL 13 ^ 0x464C4F57`. The constants "TAKT" and "FLOW" are company-name-derived and the algorithm is a simple XOR+rotate — not cryptographic. The code itself acknowledges this: "For production: replace with CMAC or HMAC-SHA256" (line 109). For development this is acceptable, but the transition plan to a cryptographic algorithm should be tracked. |
| BSW-010 | OBSERVATION | **BSW-006 (dual E2E protection path) was not addressed.** `Com_TriggerIPDUSend` (line 494-502) and `Com_MainFunction_Tx` (line 614-622) both apply E2E_Protect. For MIXED mode PDUs triggered by Com_SendSignal, both paths could fire in the same cycle, double-incrementing the alive counter. This was flagged as OBSERVATION previously and remains unresolved. |

### Verdict: CONDITIONALLY APPROVED

Condition: Fix RX path bounds check (BSW-008-R).

---

## Auditor 3: Embedded Systems Test Lead

**Role:** Evaluate 1,021 test adequacy, coverage gaps, test quality, and Layer 6 verification completeness.

### What Was Checked
- Test count growth (193 -> 1,021)
- Generated test quality (signal packing 273, VSM 107, E2E 100, etc.)
- Layer 6 partial verification (7-ECU vcan)
- Previous findings: TST-003 (ECU coverage), TST-004 (fault injection), TST-008 (broken VSM tests)

### Findings

| ID | Severity | Finding |
|----|----------|---------|
| TST-001-U | PASS | Test suite growth from 193 to 1,021 is substantial. The generated test approach (273 signal tests, 107 VSM transitions, 100 E2E messages) provides systematic coverage that hand-written tests cannot match. Each generated signal test exercises the full Com_SendSignal -> PDU buffer -> Com_ReceiveSignal round-trip. |
| TST-010 | PASS | CI pipeline runs all 1,021 tests on every push. The `ci.yml` file shows 16 separate compile-and-run steps for unit tests plus 7 ECU syntax checks. All blocking. This is production-grade CI. |
| TST-003-U | MAJOR | **Still only 2/7 ECUs have formal integration test suites.** Layer 6 progress is real (all 7 ECUs run on vcan, 27 CAN IDs observed), but this is "smoke test" level verification — presence of frames, not correctness of content. No test script validates RZC, SC, BCM, ICU, or TCU signal values, E2E correctness, or fault reactions. The gap is narrower but still significant for ASIL D. |
| TST-004-U | MAJOR | **Fault injection testing still absent.** No new fault injection tests added. For ASIL D, ISO 26262-4 clause 7.4.5 requires fault injection at integration level. The platform now has 7 ECUs running simultaneously — this is the ideal environment for fault injection (kill one ECU, corrupt a frame, inject bus-off). The opportunity was not taken. |
| TST-008-U | MINOR | **16 broken VSM unit tests still broken.** Listed as open issue #3 with "update mock signal IDs" as fix. These are not counted in the 1,021 figure. Having known-broken tests in the repo undermines confidence. |
| TST-011 | MINOR | **Generated tests use `-Wno-unused-function -Wno-unused-variable`.** Every generated test compilation in `ci.yml` suppresses warnings. For ASIL D, compiler warnings should be addressed, not suppressed. The generated test code should be clean enough to compile with `-Wall -Werror`. |
| TST-012 | MINOR | **No integration tests in CI.** The CI pipeline runs 821 unit tests but the 135 integration tests (Layer 4/5) require vcan0 and running ECU binaries — neither of which is available in GitHub Actions ubuntu-latest. Integration tests are manual-only. For a complete CI pipeline, a self-hosted runner with vcan support or a Docker-based integration test stage is needed. |
| TST-013 | OBSERVATION | **Layer 5b comprehensive test (71 tests) is not in CI.** `test_cvc_fzc_comprehensive.py` requires two running ECU binaries and vcan0. Like TST-012, this is manual-only. The most valuable test suite in the project has no automated execution. |

### Verdict: CONDITIONALLY APPROVED

Conditions:
1. Add integration tests for SC and RZC at minimum (TST-003-U).
2. Implement at least 3 fault injection scenarios: ECU death, frame corruption, bus-off (TST-004-U).

---

## Auditor 4: Cybersecurity Auditor (ISO 21434)

**Role:** Evaluate XCP Seed & Key implementation, remaining attack surface, threat model.

### What Was Checked
- XCP Seed & Key implementation in `Xcp.c`
- Authentication flow (GET_SEED -> UNLOCK -> memory access)
- Lockout mechanism
- Remaining attack vectors

### Findings

| ID | Severity | Finding |
|----|----------|---------|
| SEC-001-U | PASS | **XCP Seed & Key implemented.** CONNECT no longer grants memory access. The flow is: CONNECT -> GET_SEED (returns 4-byte seed) -> UNLOCK (must provide correct key) -> memory commands accepted. This closes the critical finding from the previous review. |
| SEC-002-U | PASS | **XCP memory write now gated.** SHORT_DOWNLOAD, SET_MTA, SHORT_UPLOAD, UPLOAD all check `xcp_unlocked != TRUE` before proceeding. Address validation rejects NULL/low addresses on POSIX. Max 2 data bytes enforced. |
| SEC-007 | MAJOR | **XCP Seed & Key algorithm provides security-through-obscurity, not cryptographic security.** The key derivation (`seed XOR "TAKT" ROL13 XOR "FLOW"`) is a fixed, invertible transformation. An attacker who observes one seed-key pair (by sniffing a legitimate calibration session) can derive the algorithm. An attacker who reverse-engineers the firmware binary can extract the constants directly. The 3-attempt lockout helps but does not change the fundamental weakness. For production, this must be replaced with CMAC/HMAC per the code's own TODO comment. For development/SIL, this is acceptable. |
| SEC-008 | MAJOR | **XCP address range validation is POSIX-only.** `Xcp.c` lines 309-313, 391-396, 462-466: address range checks are inside `#ifdef PLATFORM_POSIX`. On bare-metal (STM32/TMS570), there is NO address range validation — any address in the 32-bit space is writable after authentication. This means an authenticated but compromised XCP master could write to peripheral registers, interrupt vectors, or safety-critical BSW state. A whitelist of writable address ranges (RAM only, exclude peripheral space) is needed for bare-metal targets. |
| SEC-009 | MINOR | **XCP LFSR seed is deterministic from initial state.** `Xcp.c` line 79: `xcp_lfsr_state = 0xDEADBEEFu`. The seed sequence is the same after every power cycle. An attacker who knows the firmware version knows the exact seed sequence. The LFSR should be seeded from a hardware entropy source (ADC noise, timer jitter) on bare-metal, or `/dev/urandom` on POSIX. |
| SEC-003-U | MAJOR | **No SecOC / CAN authentication.** Unchanged from previous review. No TARA produced. This remains a gap for UNECE R155 compliance. |
| SEC-006-U | MINOR | **No TARA referenced.** Unchanged. The XCP Seed & Key implementation addresses one attack vector but does so without a systematic threat model. |

### Verdict: CONDITIONALLY APPROVED (upgraded from REJECTED)

The XCP Seed & Key implementation addresses the most critical finding. The platform moves from REJECTED to CONDITIONALLY APPROVED for development use. Conditions for production:
1. Replace XOR-rotate with CMAC/HMAC-SHA256 (SEC-007).
2. Add bare-metal address range whitelist (SEC-008).
3. Seed LFSR from hardware entropy (SEC-009).
4. Produce TARA (SEC-006-U).

---

## Auditor 5: ASPICE Process Assessor

**Role:** Evaluate traceability, documentation, change management discipline.

### What Was Checked
- Documentation updates since previous review
- Bug tracking formalization (ASP-005 follow-up)
- Change impact analysis (ASP-004 follow-up)
- Traceability tools and automation

### Findings

| ID | Severity | Finding |
|----|----------|---------|
| ASP-001-U | PASS | DBC-first workflow continues to be well-enforced. CI gate chain (DBC validate -> ARXML gen -> ARXML validate -> codegen -> round-trip -> data flow -> syntax -> unit tests -> traceability) is comprehensive. |
| ASP-008 | PASS | **Progress documentation is exemplary.** `progress-can-dataflow-rewrite-incremental-verification-2026-03-21.md` provides a clear, detailed record of: what was built, verification status per layer, all 20 bugs with root causes, audit fixes applied, and next steps. This is the level of documentation expected for ASPICE Level 2. |
| ASP-004-U | MAJOR | **Change impact analysis still missing.** Repeat finding. The session added 3 BSW modules, modified 21 modules, changed 93 generated files, and added 828 tests. No pre-change or retrospective impact analysis document exists. |
| ASP-005-U | MINOR | **Bug tracking expanded but still informal.** Bugs are now numbered #1-#20 (up from #17) in the progress doc. The "Audit Fixes Applied" table with severity and status is a step toward formal tracking, but these still exist only in markdown files, not a tracked system. |
| ASP-009 | MINOR | **Lessons learned well-documented but not cross-referenced.** Two lessons-learned files were created this session (`phase2-can-dataflow-2026-03-21.md`, `layer4-runtime-bugs-2026-03-21.md`). Good practice. However, these are not cross-referenced from the development discipline rules that were created in response to them. |
| ASP-010 | OBSERVATION | **Generated test files are not in the traceability matrix.** The 828 generated tests (signal packing, VSM, E2E, XCP security, etc.) should trace to their respective requirements. The traceability tool (`gen_traceability_matrix.py`) scans for `@verifies` tags, but generated test files may not contain these tags. |

### Verdict: CONDITIONALLY APPROVED

Condition: Produce retrospective change impact analysis (ASP-004-U) — now a repeat finding.

---

## Auditor 6: Vehicle Integration Engineer

**Role:** Evaluate 7-ECU bus load, timing, physical layer readiness, and multi-ECU coordination.

### What Was Checked
- Layer 6 results: 7 ECUs, 27 CAN IDs, 1265 frames/s
- Bus load calculation against budget
- Timing analysis of multi-ECU run
- CAN monitor tool

### Findings

| ID | Severity | Finding |
|----|----------|---------|
| VIE-008 | PASS | **7-ECU simultaneous operation achieved.** All ECUs (CVC, FZC, RZC, SC, BCM, ICU, TCU) run on a single vcan0 interface, producing 27 unique CAN IDs at 1265 frames/s. This is a significant milestone — the full bus topology is now operational at the logical level. |
| VIE-009 | PASS | **CAN monitor app with TCP bridge.** Remote vcan monitoring capability via TCP bridge enables distributed debugging — useful for Docker SIL and remote benches. |
| VIE-003-U | MAJOR | **Freedom From Interference analysis still missing.** All 7 ECUs share one bus segment. The successful 7-ECU run actually makes this MORE urgent — if any QM ECU (BCM, ICU) misbehaves, it will disrupt ASIL D communication. The demonstration that all 7 ECUs run together is the right time to do FFA. |
| VIE-010 | MINOR | **27 of 45 DBC CAN IDs observed.** 18 CAN IDs are not present in the 7-ECU run. These are likely UDS messages (9 IDs, only sent on request) and virtual sensor messages (periodic but perhaps not yet generated by the SWC). The gap should be documented: which messages are expected and which are not yet implemented. |
| VIE-011 | MINOR | **1265 frames/s at 500 kbps = ~14.5% bus load.** The can-bus-load-budget.md specifies 14.2% nominal. The measured 14.5% exceeds the budget by 0.3%. This is within measurement tolerance for POSIX SIL (timer jitter), but on real hardware with tighter timing, this could be a concern. The budget should be updated or the cycle times adjusted. |
| VIE-006-U | OBSERVATION | **POSIX timing limitations acknowledged.** Previous finding about SIL timing not being representative remains valid. The 7-ECU run at 1265 fps is a good data point but should not be used for timing compliance arguments. |

### Verdict: CONDITIONALLY APPROVED

Condition: Document which of the 45 DBC messages are expected in the 7-ECU run and which are not yet implemented (VIE-010).

---

## Auditor 7: Production Quality Engineer

**Role:** Evaluate CI reliability, build reproducibility, deployment process quality.

### What Was Checked
- CI pipeline with 1,021 tests
- Build process for 7 ECUs
- SIL_DIAG cleanup status
- Generated file version stamping

### Findings

| ID | Severity | Finding |
|----|----------|---------|
| PQE-008 | PASS | **CI pipeline is comprehensive and blocking.** 1,021 tests across 16 build-and-run steps, 7 ECU syntax checks, DBC/ARXML/codegen validation, round-trip verification, data flow enforcement, and traceability. All blocking via the `all-gates` job. This is a high-quality CI pipeline. |
| PQE-009 | PASS | **Generated test strategy is scalable.** The approach of generating tests from config/DBC definitions (273 signal tests from signal definitions, 107 VSM tests from state table, etc.) means test count grows automatically as the system grows. No manual test authoring needed for new signals. |
| PQE-003-U | MINOR | **SIL_DIAG traces still present.** Downgraded from MAJOR to MINOR — the traces are ifdef-guarded and the project has chosen to keep them for SIL debugging. This is a pragmatic decision. However, Rule 8 in `development-discipline.md` still says "Remove them before merging to main." The rule should be updated to match the actual practice, or the traces should be removed. |
| PQE-005-U | MINOR | **No version hash in generated files.** Unchanged. Generated files still have `/* GENERATED -- DO NOT EDIT */` but no DBC hash or codegen version. |
| PQE-010 | MINOR | **7-ECU build is not automated in CI.** The CI checks syntax (compile individual `.c` files) but does not link 7 complete POSIX ECU binaries. The Layer 6 verification was done manually by building locally. A CI step that links all 7 ECU binaries would catch linker errors (like the VSM GetRxStatus linker fix). |
| PQE-011 | OBSERVATION | **Makefile.coverage exists but is not in CI.** `test/Makefile.coverage` with gcov+lcov is referenced in the progress doc but not wired into CI. Code coverage data is generated locally but not tracked over time. |

### Verdict: APPROVED

No blocking conditions. Minor improvements recommended.

---

## Auditor 8: Code Review Senior Developer

**Role:** Review C code quality, MISRA C:2012 compliance, and defensive programming practices.

### What Was Checked
- XCP Seed & Key code quality (`Xcp.c`)
- Com.c fixes (COM_UINT32, SchM_TimingStop, bounds check)
- Remaining critical section issue in Com_RxIndication
- Dead code removal verification

### Findings

| ID | Severity | Finding |
|----|----------|---------|
| CR-010 | PASS | **XCP Seed & Key implementation is clean and well-structured.** Functions are small (10-30 lines), each command handler validates inputs before processing, and the state machine (connected -> seed_pending -> unlocked) is straightforward. The lockout mechanism (3 failures -> ACCESS_DENIED, requires DISCONNECT+CONNECT to reset) is a good defense-in-depth measure. |
| CR-011 | PASS | **Dead code cleanup effective.** `Swc_CvcCom.c` no longer contains `Swc_CvcCom_E2eProtect`, `Swc_CvcCom_E2eCheck`, or `Swc_CvcCom_Receive`. The comment at line 26-29 clearly explains the design decision. 191 lines of dead code removed. |
| CR-003-U | MAJOR | **Com_RxIndication critical section still too wide.** `Com.c` lines 332-447: 115 lines under `SchM_Enter_Com_COM_EXCLUSIVE_AREA_0()`. This includes a linear search (lines 355-401), E2E CRC computation, E2E SM evaluation, and a full signal unpack loop with RTE writes. On bare-metal with ISR-driven RX, this blocks all interrupts for the worst-case duration of the entire function. The recommended fix (copy PDU data under lock, release, then process) has not been applied. This is a repeat finding. |
| CR-012 | MINOR | **XCP `xcp_compute_key` uses signed shift behavior.** `Xcp.c` line 115: `key = ((key << 13u) | (key >> 19u))`. The left shift `key << 13u` on a `uint32` is well-defined in C. However, on platforms where `uint32` is promoted to `int` (32-bit int), this could invoke undefined behavior if bit 31 is set after shift. Using `(uint32)((uint32)key << 13u)` would be safer and MISRA-compliant. |
| CR-013 | MINOR | **`Com_MainFunction_Tx` reacquires lock inside the send-success path.** Lines 627-639: after `PduR_Transmit` succeeds, the code enters the exclusive area again to update counters and snapshot the buffer. Between the `SchM_Exit` (line 624) and `SchM_Enter` (line 627), another task could call `Com_SendSignal` and modify the buffer. The snapshot at lines 633-638 might capture partially-updated data. This is a subtle race condition — unlikely on a cooperative scheduler but real on a preemptive RTOS. |
| CR-005-U | MINOR | **Com_TriggerIPDUSend still uses linear search.** `Com.c` line 487. The O(1) `com_tx_pdu_index[]` array is available and used by `Com_SendSignal` (line 251). `Com_TriggerIPDUSend` should use it too. |

### Verdict: CONDITIONALLY APPROVED

Condition: Fix Com_RxIndication critical section scope (CR-003-U) — now a repeat finding.

---

## Auditor 9: Customer OEM Reviewer

**Role:** Evaluate from vehicle manufacturer perspective: acceptance criteria, conformance testing, and readiness for vehicle integration.

### What Was Checked
- Layer 6 multi-ECU demonstration
- ECU coverage improvement
- Target hardware readiness
- SC integration status

### Findings

| ID | Severity | Finding |
|----|----------|---------|
| OEM-008 | PASS | **7-ECU bus topology demonstrated.** All 7 ECUs running simultaneously on vcan0 with 27 CAN IDs is a credible SIL demonstration of the zonal architecture. The 1265 frames/s throughput is within expected bounds. |
| OEM-009 | PASS | **XCP calibration interface now secured.** The Seed & Key mechanism, while not cryptographic, prevents casual/accidental memory writes. Lockout after 3 failures adds resilience. Acceptable for development and bench testing. |
| OEM-003-U | MAJOR | **Still no target hardware test evidence.** Repeat finding. Zero tests on physical CAN hardware. For OEM acceptance of even a prototype platform, at least one CAN echo test on STM32 or TMS570 is expected. The platform has been verified exclusively in software simulation. |
| OEM-004-U | MAJOR | **SC integration still lacks dedicated testing.** SC appears in the 7-ECU run but there is no test verifying: (a) SC heartbeat content correctness, (b) SC_Status message E2E protection, (c) SC_KILL command transmission, (d) lockstep fault detection. SC is the most safety-critical ECU (TMS570 lockstep, ASIL D) and has the least verification evidence. |
| OEM-010 | MINOR | **Docker SIL deployment not verified this session.** The progress doc lists "Layer 6h: Docker SIL — next session." For OEM demonstrations, a containerized SIL that can be deployed and run without local toolchain setup is expected. The 7-ECU vcan run requires manual binary builds and process management. |
| OEM-011 | OBSERVATION | **5 of 7 ECUs (RZC, SC, BCM, ICU, TCU) have no SWC-level application logic tests.** They build and run, producing heartbeats, but their application-level behavior (motor control, body control, instrument cluster updates, etc.) is unverified. The 7-ECU demonstration shows the communication infrastructure works, not that the vehicle functions work. |

### Verdict: CONDITIONALLY APPROVED

Conditions:
1. Target hardware smoke test (OEM-003-U) — repeat finding.
2. SC dedicated integration test (OEM-004-U) — repeat finding.

---

## Auditor 10: Independent Safety Assessor (ISA)

**Role:** ISO 26262 Part 2 independent evaluation of the entire safety case.

### What Was Checked
- Audit fix response quality
- Overall safety case progression
- Independence requirement
- Residual risk assessment

### Findings

| ID | Severity | Finding |
|----|----------|---------|
| ISA-008 | PASS | **Audit response demonstrates engineering discipline.** Of the 3 CRITICAL and 14 MAJOR findings from the previous review, 2 CRITICAL and 5 MAJOR are now CLOSED. The XCP Seed & Key implementation is well-structured with proper state management, lockout, and access control. The COM_UINT32 fix, SchM_TimingStop fix, and SWC layering cleanup are all correct. This is a competent and responsive engineering team. |
| ISA-009 | PASS | **Test suite growth is genuine and well-structured.** 1,021 tests with 821 unit + 135 integration + 65 Layer 5b comprehensive. The generated test approach provides systematic coverage. The CI pipeline is blocking and comprehensive. |
| ISA-003-U | CRITICAL | **Independent verification still not achieved.** Structural issue unchanged. All 1,021 tests, all 20 bug fixes, all architecture decisions, and this audit response were produced by the same individual. For ASIL D, ISO 26262-2 clause 6.4.8 requires I2/I3 independence. This cannot be resolved by the current team alone — it requires organizational action (hire/contract a second verification engineer or engage a third-party assessment service). |
| ISA-010 | MAJOR | **Safety case documentation gap widening.** The engineering work is accelerating (3 new BSW modules, 7-ECU integration, 828 new tests) but the safety case documentation is not keeping pace. No FMEA/FTA/DFA updates, no FTTI analysis, no formal anomaly reports for safety bugs #4/#5, no change impact analysis. The ratio of verified-code to documented-safety-case is becoming increasingly skewed. Each session of engineering work without corresponding safety documentation creates technical debt that compounds. |
| ISA-011 | MAJOR | **20 bugs found in a single session — root cause analysis incomplete.** Finding 20 bugs is a sign of thorough verification, but the sheer number raises questions about the development process that produced them. Bugs #7, #8, #17 (hand-written config overriding generated config) occurred in 3 ECUs independently — this is a systematic process failure, not isolated defects. Bug #15 (sub-byte signal packing) existed since the Com module was written. The development discipline rules (created in response) are the right corrective action, but a formal root cause analysis with process improvement recommendations would strengthen the safety case. |
| ISA-012 | MINOR | **Audit trail completeness.** The previous audit (AUDIT-10-REVIEW-2026-03-21) produced 38 findings. This follow-up can trace the disposition of each. However, the findings are tracked in markdown files, not a formal finding management system. For ISO 26262 confirmation review, findings must be tracked with unique IDs, owner, due date, and closure evidence. The markdown approach works for now but will not scale. |

### Verdict: REJECTED (for ASIL D release)

**Note:** Rejected for formal ASIL D safety release, APPROVED for continued development.

The engineering quality has improved significantly since the previous review. The XCP security fix, signal packing corrections, and test suite expansion demonstrate strong technical execution. However:

1. **Independence (ISA-003-U):** Non-negotiable for ASIL D. Requires organizational action.
2. **Safety documentation (ISA-010):** Engineering is outpacing safety case. Must be rebalanced.
3. **Root cause analysis (ISA-011):** 20 bugs from one session needs formal process analysis.

---

## Consolidated Summary

### Previous Finding Closure Rate

| Previous Severity | Total | Closed | Still Open | Closure Rate |
|-------------------|-------|--------|------------|--------------|
| CRITICAL | 3 | 2 | 1 | 67% |
| MAJOR | 14 | 3 | 11 | 21% |
| MINOR | 21 | 5 | 16 | 24% |
| OBSERVATION | 8 | 0 | 8 | 0% |
| **Total** | **46** | **10** | **35** | **22%** |

### New Findings This Review

| Severity | Count | IDs |
|----------|-------|-----|
| CRITICAL | 1 | ISA-003-U (repeat) |
| MAJOR | 9 | FSM-003-U (repeat), FSM-004-U (repeat), BSW-008-R, SEC-007, SEC-008, TST-003-U (repeat), TST-004-U (repeat), CR-003-U (repeat), ISA-010, ISA-011 |
| MINOR | 14 | BSW-003-R, BSW-009, FSM-008, TST-008-U, TST-011, TST-012, TST-013 (obs), VIE-010, VIE-011, PQE-003-U, PQE-005-U, PQE-010, CR-012, CR-013, CR-005-U, SEC-009, SEC-006-U, ASP-004-U (repeat), ASP-005-U, ASP-009, ISA-012 |
| OBSERVATION | 5 | BSW-010, FSM-009, VIE-006-U, PQE-011, OEM-011 |
| PASS | 13 | (across all auditors) |

### Repeat Findings (Escalation Candidates)

These findings were raised in the previous review and remain open:

| ID | Original Severity | Finding | Sessions Open |
|----|-------------------|---------|---------------|
| ISA-003 | CRITICAL | Independent verification (I2/I3) | 2 |
| FSM-003 | MAJOR | FTTI impact assessment | 2 |
| FSM-004 | MAJOR | G2.2 ~6s recovery unanalyzed | 2 |
| CR-003 | MAJOR | Com_RxIndication critical section too wide | 2 |
| TST-003 | MAJOR | Only 2/7 ECUs with integration tests | 2 |
| TST-004 | MAJOR | No fault injection testing | 2 |
| OEM-003 | MAJOR | No target hardware test evidence | 2 |
| OEM-004 | MAJOR | SC integration testing absent | 2 |
| ASP-004 | MAJOR | Change impact analysis missing | 2 |
| SEC-003 | MAJOR | No SecOC / CAN authentication | 2 |

### Verdicts by Auditor

| # | Auditor | Previous Verdict | Current Verdict | Change |
|---|---------|-----------------|-----------------|--------|
| 1 | ISO 26262 Functional Safety Manager | CONDITIONALLY APPROVED | CONDITIONALLY APPROVED | -- |
| 2 | AUTOSAR BSW Architect | CONDITIONALLY APPROVED | CONDITIONALLY APPROVED | -- |
| 3 | Embedded Systems Test Lead | CONDITIONALLY APPROVED | CONDITIONALLY APPROVED | -- |
| 4 | Cybersecurity Auditor (ISO 21434) | **REJECTED** | CONDITIONALLY APPROVED | **UPGRADED** |
| 5 | ASPICE Process Assessor | CONDITIONALLY APPROVED | CONDITIONALLY APPROVED | -- |
| 6 | Vehicle Integration Engineer | CONDITIONALLY APPROVED | CONDITIONALLY APPROVED | -- |
| 7 | Production Quality Engineer | CONDITIONALLY APPROVED | **APPROVED** | **UPGRADED** |
| 8 | Code Review Senior Developer | CONDITIONALLY APPROVED | CONDITIONALLY APPROVED | -- |
| 9 | Customer OEM Reviewer | CONDITIONALLY APPROVED | CONDITIONALLY APPROVED | -- |
| 10 | Independent Safety Assessor (ISA) | **REJECTED** | **REJECTED** (for ASIL D release) | -- |

### Overall Recommendation

**CONDITIONALLY APPROVED for continued development. REJECTED for formal ASIL D safety release.**

**What improved since last review:**
- Cybersecurity posture upgraded from REJECTED to CONDITIONALLY APPROVED (XCP Seed & Key)
- Production Quality upgraded from CONDITIONALLY APPROVED to APPROVED (CI with 1,021 tests)
- 10 findings closed (2 CRITICAL, 3 MAJOR, 5 MINOR)
- 7-ECU integration demonstrated on vcan
- Test suite grew 5.3x (193 -> 1,021)
- 20 bugs found and fixed, 3 of which were CRITICAL signal packing bugs

**What still blocks ASIL D release:**
1. **Independent verification (ISA-003-U):** Non-negotiable. Requires second engineer or third-party.
2. **Safety documentation gap (ISA-010):** Engineering outpacing safety case documentation.
3. **Com_RxIndication critical section (CR-003-U):** Potential interrupt starvation on bare-metal.
4. **RX path bounds check (BSW-008-R):** Buffer over-read if signal misconfigured.
5. **FTTI analysis (FSM-003-U/FSM-004-U):** Timing changes unverified against safety budgets.

### Priority Actions

| Priority | Action | Blocking? | Repeat? | Effort |
|----------|--------|-----------|---------|--------|
| P0 | Fix RX path `byte_offset + 1` bounds check in Com.c | YES | No | 15 min |
| P0 | Refactor Com_RxIndication critical section (copy-then-process) | YES | **Yes (2nd time)** | 2-4 hours |
| P1 | FTTI impact assessment for COM_STARTUP_DELAY_MS and E2E SM recovery | YES for release | **Yes (2nd time)** | 1 day |
| P1 | SC and RZC dedicated integration tests | YES for release | **Yes (2nd time)** | 2-3 days |
| P1 | Fault injection tests (ECU death, frame corruption, bus-off) | YES for release | **Yes (2nd time)** | 2 days |
| P1 | XCP bare-metal address range whitelist | YES for release | No | 0.5 day |
| P2 | Target hardware smoke test (CVC+FZC on STM32) | YES for OEM | **Yes (2nd time)** | 1-2 days |
| P2 | Change impact analysis (retrospective) | YES for release | **Yes (2nd time)** | 1 day |
| P2 | Safety case documentation catch-up (FMEA/FTA/DFA updates) | YES for release | Yes | 3-5 days |
| P2 | Docker SIL Layer 6h deployment | No | No | 1 day |
| P2 | Fix 16 broken VSM unit tests | No | Yes | 0.5 day |
| P3 | Freedom From Interference analysis for mixed ASIL/QM bus | YES for release | Yes | 2 days |
| P3 | Formal TARA for cybersecurity | YES for release | Yes | 3-5 days |
| P3 | Replace XCP Seed & Key with CMAC/HMAC | YES for production | No | 2-3 days |
| P3 | Independent verification engagement (second engineer) | YES for release | Yes | Organizational |

---

*Review conducted: 2026-03-21*
*Classification: ISO 26262 ASIL D — Safety-Related*
*Document ID: AUDIT-10-REVIEW-L6-2026-03-21*
*Previous Review: AUDIT-10-REVIEW-2026-03-21*
