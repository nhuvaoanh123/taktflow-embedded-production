---
document_id: AUDIT-10-REVIEW-2026-03-21
title: "10-Auditor Review: CAN Data Flow Rewrite"
version: "1.0"
status: FINAL
date: 2026-03-21
classification: ISO 26262 ASIL D — Safety-Related
scope: "Independent multi-discipline review of SESSION-RPT-2026-03-21"
reviewed_artifact: SESSION-RPT-2026-03-21 (Comprehensive Session Report)
---

# 10-Auditor Review: CAN Data Flow Rewrite

**Review Date:** 2026-03-21
**Artifact Under Review:** SESSION-RPT-2026-03-21 — Comprehensive Session Report: CAN Data Flow Rewrite
**Source Files Inspected:** `Com.c`, `main.c` (CVC), `Swc_CvcCom.c`, `Xcp.c`, `test_cvc_fzc_comprehensive.py`

---

## Auditor 1: ISO 26262 Functional Safety Manager

**Role:** Verify HARA traceability, ASIL decomposition correctness, safety goal coverage, and FTTI compliance.

### Findings

| ID | Severity | Finding |
|----|----------|---------|
| FSM-001 | PASS | E2E relocation from SWC to Com BSW layer correctly removes safety mechanism implementation from application code, consistent with ISO 26262-6 Table 1 (separation of concerns). |
| FSM-002 | PASS | VSM transitions SC_KILL->SHUTDOWN and MOTOR_CUTOFF->DEGRADED are now HARA-aligned. Bug #4 and #5 were genuine safety defects with correct root cause analysis and traceability (SG-008/HE-012, SG-003/HE-005). |
| FSM-003 | MAJOR | **FTTI budget not formally verified for the rewrite.** The session report references FTTI (docs/safety/analysis/heartbeat-ftti-budget.md exists) but the rewrite introduced a 50ms COM_STARTUP_DELAY_MS and changed E2E SM timing (G2.2 recovery ~6s). Neither change has a documented impact assessment against the FTTI budget. ISO 26262-4 clause 6.4.3 requires FTTI verification for any change to fault detection/reaction timing. |
| FSM-004 | MAJOR | **G2.2 known failure dismissed too easily.** The ~6s E2E recovery time is labeled "LOW" severity and "does not affect safety." This is incorrect without analysis. If the FTTI for heartbeat loss is <6s (common for ASIL D steering/braking), then a 6s recovery window could violate the safety goal. The report acknowledges the E2E SM N-threshold needs tuning but provides no deadline or acceptance criterion. |
| FSM-005 | MINOR | **Traceability gap: 47 untraced requirements (13.6%).** For ASIL D, ISO 26262-8 clause 6.4.3 requires 100% bidirectional traceability for safety-relevant requirements. While the report states these are "primarily BCM/ICU/TCU application-level," no evidence is provided that all 47 are truly QM-only. A requirement-by-requirement ASIL classification of the untraced items is needed. |
| FSM-006 | OBSERVATION | Safety goals SG-001 through SG-012 are referenced but the HARA document itself (`docs/safety/concept/hara.md`) was not included in the session report evidence package. An assessor would require direct access to verify the claim that all VSM transitions now align. |
| FSM-007 | MINOR | **No Dependent Failure Analysis (DFA) update.** Three new BSW modules (CanSM, FiM, Xcp) were added. ISO 26262-9 clause 7 requires DFA for any new element in the safety architecture. The existing DFA (`docs/safety/analysis/dfa.md`) likely predates these modules. |

### Verdict: CONDITIONALLY APPROVED

Conditions:
1. Formal FTTI impact assessment for COM_STARTUP_DELAY_MS and E2E SM recovery timing (FSM-003, FSM-004).
2. ASIL classification of all 47 untraced requirements (FSM-005).
3. DFA update for CanSM, FiM, Xcp (FSM-007).

---

## Auditor 2: AUTOSAR BSW Architect

**Role:** Verify Com/PduR/CanIf layering, signal packing correctness, and AUTOSAR specification compliance.

### Findings

| ID | Severity | Finding |
|----|----------|---------|
| BSW-001 | PASS | Data flow architecture is correct: SWCs use only `Com_SendSignal`/`Com_ReceiveSignal`. No SWC-to-PduR or SWC-to-E2E calls found (verified by grep). This matches AUTOSAR Com SWS layering. |
| BSW-002 | PASS | Sub-byte signal packing in `Com.c` lines 194-219 uses correct mask-and-shift. The fix for Bug #15/#16 is sound: `(buffer & ~(mask << shift)) | (value << shift) & mask)`. |
| BSW-003 | MAJOR | **COM_UINT32 type defined in `Com.h` (line 39) but not handled in Com_SendSignal or Com_ReceiveSignal.** Both functions have switch statements that handle COM_UINT8, COM_BOOL, COM_UINT16, COM_SINT16, and fall through to `return E_NOT_OK` for COM_UINT32. The `BitSize > 16` path in signal packing is a no-op comment (`/* not supported — no action */`, lines 220-222, 419-421). If any signal is configured as 32-bit, it will silently fail. The session report claims "32-bit signals byte-order correct" (G3.9-G3.12 PASS) but the code cannot pack them. Either the tests are not testing 32-bit signals through Com_SendSignal, or the tests pass through a different path. |
| BSW-004 | MINOR | **`Swc_CvcCom.c` still includes `PduR.h` (line 27) and `E2E.h` (line 23).** While the E2E functions are now stubs, including these headers in an SWC violates the AUTOSAR layering principle. The data flow gate checks for function calls but not header includes. An SWC should only include `Com.h` and `Rte.h` for communication. |
| BSW-005 | MINOR | **`Swc_CvcCom_E2eProtect` and `Swc_CvcCom_E2eCheck` are retained as stub functions** ("for API compatibility," lines 144-148, 178-183). Dead API functions are technical debt. In AUTOSAR, the SWC port interface should be updated to remove these. They also contain unused variables (`e2eResult` at line 161) which would trigger MISRA warnings. |
| BSW-006 | OBSERVATION | **Dual E2E protection path.** `Com_TriggerIPDUSend` (line 480-488) applies E2E protection, AND `Com_MainFunction_Tx` (lines 599-607) also applies E2E protection. If a DIRECT-mode PDU triggers `Com_TriggerIPDUSend` via `Com_SendSignal`, the alive counter increments in `Com_TriggerIPDUSend`. If the same PDU is also processed by `Com_MainFunction_Tx` (MIXED mode), the counter increments again. This could cause double-increment, which would trigger E2E WRONG_SEQUENCE on the receiver. Verify that DIRECT-only PDUs are excluded from MainFunction iteration (lines 541-543 do this, but MIXED mode PDUs are vulnerable). |
| BSW-007 | MINOR | **Com_RxIndication performs linear search for RX PDU config** (lines 341-387: `for (rx_idx = 0u; rx_idx < com_config->rxPduCount; rx_idx++)`). The TX path has O(1) lookup via `com_tx_pdu_index[]` (line 240), but RX does not. For 45 messages this is not a performance issue, but it is an architectural inconsistency. On the RX ISR path, deterministic execution time is preferred. |
| BSW-008 | MAJOR | **`byte_offset + 1u` access in 16-bit signal packing (lines 217-219, 412-413) has no bounds check against COM_PDU_SIZE.** If a signal is configured with BitPosition near the end of the PDU (e.g., bit 56 in an 8-byte PDU), `byte_offset + 1` would be 8, causing a buffer overrun. The code trusts the generated config to be correct, but defensive programming for ASIL D requires runtime bounds checking. |

### Verdict: CONDITIONALLY APPROVED

Conditions:
1. Resolve COM_UINT32 dead path or document it as intentional limitation with ASIL impact (BSW-003).
2. Add bounds check for multi-byte signal packing (BSW-008).
3. Remove PduR.h and E2E.h includes from Swc_CvcCom.c (BSW-004).

---

## Auditor 3: Embedded Systems Test Lead

**Role:** Evaluate test coverage, TDD discipline, test quality, and identify missing test scenarios.

### Findings

| ID | Severity | Finding |
|----|----------|---------|
| TST-001 | PASS | TDD discipline is evident. Bug #2 (CanSM NULL init) and Bug #3 (L1 counter reset) were caught by writing tests first. Session report documents the TDD flow correctly. |
| TST-002 | PASS | Layered verification (L1 through L5b) is a strong methodology. The 6-layer approach with mandatory pass-before-proceed is well-structured. |
| TST-003 | MAJOR | **Only 2 of 7 ECUs verified at Layer 4+.** CVC and FZC passed Layer 5b, but RZC, SC, BCM, ICU, TCU have no Layer 4+ verification. The report acknowledges RZC may have the same hand-written config bug (#7/#17). For an ASIL D platform with 7 ECUs, 28% coverage at integration level is insufficient. SC is especially concerning as the safety controller. |
| TST-004 | MAJOR | **Test script (`test_cvc_fzc_comprehensive.py`) has dead code in Group 8 (lines 451-472).** The brake fault injection test acknowledges it cannot inject faults due to E2E protection, then does nothing meaningful. The test simply checks "VSM state before fault injection" which is already verified in Group 1. There is no actual fault injection test. Fault injection is a mandatory verification activity for ISO 26262 ASIL D (Part 4, clause 7.4.5). |
| TST-005 | MINOR | **Group 12 XCP test (line 577-581) documents a known bug in the test comments** ("KNOWN BUG: FZC main.c uses hand-written PduR config") rather than marking the test as XFAIL or skipping with a defect reference. If FZC PduR was already fixed per Bug #17 in the session report, this test comment is stale. If not fixed, the test should be an expected failure, not a misleading PASS/FAIL. |
| TST-006 | MINOR | **No negative testing for signal packing edge cases.** The test suite verifies correct packing but does not test: (a) signal at maximum bit position (bit 60 in 8-byte PDU), (b) overlapping signal definitions (config error), (c) signal write with value exceeding bit width (e.g., writing 0xFF to a 4-bit signal). These are boundary conditions that ASIL D requires. |
| TST-007 | OBSERVATION | **Test timing tolerances are generous.** G5.1 allows CVC_Heartbeat rate of 15-25/s for a 50ms (20/s) cycle. That is +/-25% tolerance. For ASIL D timing verification, 10% tolerance would be expected. On POSIX this may be unavoidable, but it should be documented as a SIL limitation with a plan for tighter verification on target hardware. |
| TST-008 | MINOR | **16 pre-existing VSM unit test failures** (open issue #3). Stale mock signal IDs from a previous refactor. Unit tests are supposed to be the foundation of the verification pyramid. Having 16 broken unit tests undermines confidence in the entire test suite. These should be fixed before claiming "192/193 tests passing" — the 16 broken tests are simply not counted. |
| TST-009 | OBSERVATION | **No mutation testing or coverage metrics reported.** The report claims 30 Com unit tests but does not report line coverage, branch coverage, or MC/DC coverage. ISO 26262 Part 6 Table 12 recommends MC/DC for ASIL D. |

### Verdict: CONDITIONALLY APPROVED

Conditions:
1. Extend Layer 4+ verification to all 7 ECUs, prioritizing SC and RZC (TST-003).
2. Implement actual fault injection testing — at minimum via bypassing E2E or using valid E2E-protected fault frames from the real FZC process (TST-004).
3. Fix the 16 broken VSM unit tests (TST-008).

---

## Auditor 4: Cybersecurity Auditor (ISO 21434)

**Role:** Evaluate XCP access control, CAN authentication gaps, and threat model coverage.

### Findings

| ID | Severity | Finding |
|----|----------|---------|
| SEC-001 | CRITICAL | **XCP has no Seed & Key authentication.** `Xcp.c` implements SHORT_DOWNLOAD (write memory) with only a connection check (`xcp_connected == FALSE` at line 340). Any CAN participant can send XCP_CMD_CONNECT and then freely write to arbitrary memory addresses. This is a direct code execution/calibration tampering vector. ISO 21434 clause 7.4.2 requires threat analysis for diagnostic/calibration interfaces. For a vehicle platform, XCP DOWNLOAD without authentication is a critical vulnerability. |
| SEC-002 | CRITICAL | **XCP SHORT_DOWNLOAD allows arbitrary memory write.** Combined with SEC-001, an attacker on the CAN bus can overwrite safety-critical variables (E2E counters, VSM state, fault masks) by targeting their memory addresses. This bypasses all E2E protection and could cause unintended vehicle behavior. The XCP module has no address range restriction — any address is writable. |
| SEC-003 | MAJOR | **No CAN message authentication (SecOC).** The platform relies entirely on E2E P01 (CRC-8 + alive counter) for message integrity. E2E P01 provides protection against communication errors and simple replay, but not against a malicious actor. CRC-8 has only 256 possible values — brute-forceable in <1 second on CAN. ISO 21434 and UNECE R155 require authentication for safety-relevant messages in connected vehicles. |
| SEC-004 | MINOR | **No rate limiting on XCP commands.** A flood of XCP CONNECT requests could consume processing time in the Com/PduR/CanIf chain, potentially causing deadline misses for safety-relevant PDU processing. |
| SEC-005 | MINOR | **UDS diagnostic access control not reviewed.** The session report mentions 9 UDS messages but the Dcm module's security access implementation was not included in the evidence package. UDS SecurityAccess (0x27) is a standard attack surface. |
| SEC-006 | OBSERVATION | **No threat model referenced.** The session report does not reference an ISO 21434 TARA (Threat Analysis and Risk Assessment). For an ASIL D platform, cybersecurity threats that can violate safety goals must be analyzed per ISO 21434 clause 15 (interaction between cybersecurity and functional safety). |

### Verdict: REJECTED

Mandatory before release:
1. Implement XCP Seed & Key or disable XCP DOWNLOAD entirely (SEC-001, SEC-002).
2. Document and accept (or mitigate) the CAN authentication gap via TARA (SEC-003).
3. Add XCP memory address range restrictions at minimum (SEC-002).

---

## Auditor 5: ASPICE Process Assessor

**Role:** Evaluate traceability completeness, documentation quality, and change management discipline.

### Findings

| ID | Severity | Finding |
|----|----------|---------|
| ASP-001 | PASS | The DBC-first workflow (DBC -> ARXML -> Codegen -> Generated C) is well-defined and enforced via CI gates. The round-trip check (Gate 5) is a strong process control. |
| ASP-002 | PASS | @satisfies traceability tags in generated code enable automated matrix generation. This is ASPICE SWE.1 BP4 compliant. |
| ASP-003 | MINOR | **Session report is comprehensive but lacks formal review and approval signatures.** ASPICE SUP.4 requires documented review records with reviewer identification. The report status is "REVIEW" but no reviewers are listed. |
| ASP-004 | MAJOR | **Change impact analysis missing.** The rewrite touched 21 BSW modules, added 3 new modules, changed 93 generated files, and modified 2 VSM transitions. ASPICE MAN.3 BP7 requires documented impact analysis before changes of this magnitude. The session report documents what was done but not the pre-change impact assessment. |
| ASP-005 | MINOR | **Bug tracking is informal.** 17 bugs are documented in the session report with sequential numbers (#1-#17) but there is no reference to a formal defect tracking system (JIRA, GitHub Issues). ASPICE SUP.9 requires defect management with status tracking, priority classification, and resolution verification. |
| ASP-006 | MINOR | **Development discipline rules (development-discipline.md) were created during the session in response to bugs.** While proactive, this means the rules did not exist when the bugs were introduced. The process should ensure rules are reviewed and disseminated to all developers, not just codified post-hoc. |
| ASP-007 | OBSERVATION | **No formal verification report.** The session report combines implementation description and verification results. ASPICE SWE.4/SWE.5 require separate verification reports with pass/fail criteria defined before execution. |

### Verdict: CONDITIONALLY APPROVED

Conditions:
1. Produce a formal change impact analysis, even if retrospective (ASP-004).
2. Transfer bugs #1-#17 to a tracked defect management system (ASP-005).
3. Create a standalone verification report with pre-defined acceptance criteria (ASP-007).

---

## Auditor 6: Vehicle Integration Engineer

**Role:** Evaluate CAN bus load, timing, physical layer considerations, and multi-ECU coordination.

### Findings

| ID | Severity | Finding |
|----|----------|---------|
| VIE-001 | PASS | 45 messages on a 500 kbps bus is well within capacity. Even with worst-case 8-byte DLCs at shortest cycle times (10ms), theoretical maximum load is ~35%, which is acceptable. |
| VIE-002 | PASS | Heartbeat cycle times (50ms) with 3x timeout detection (150ms) provide reasonable fault detection latency for a development platform. |
| VIE-003 | MAJOR | **All 7 ECUs share a single CAN bus.** The DBC shows 45 messages including safety-critical (E-Stop, Brake, Steering) and QM (Body Control, HVAC) on the same bus segment. ISO 26262 Part 4 clause 7.4.3 requires freedom from interference between ASIL and QM elements. A misbehaving QM ECU (BCM, ICU) could cause bus-off, affecting ASIL D communication. CanSM L1/L2 recovery mitigates but does not eliminate this risk. Physical bus segmentation or at minimum a documented FFA (Freedom From Interference) argument is required. |
| VIE-004 | MINOR | **No CAN bus termination or physical layer specification.** The session report focuses exclusively on the logical (protocol) layer. For production, 120-ohm termination, cable length limits, transceiver specifications, and EMC requirements must be documented. |
| VIE-005 | MINOR | **XCP shares the CAN bus with safety-critical messages.** XCP request/response pairs (8 messages) could contribute to bus load spikes during calibration sessions. If a calibration engineer connects while the vehicle is in RUN mode, XCP traffic could delay safety-critical heartbeats. Priority-based arbitration helps (XCP IDs 0x550+ are lower priority than heartbeats 0x010+), but burst behavior should be analyzed. |
| VIE-006 | OBSERVATION | **POSIX SIL timing is not representative of target hardware.** The test suite tolerates +/-25% timing variance (G5.1: 15-25 heartbeats/s for a 20/s target). On real hardware with CAN bus arbitration, interrupt latency, and scheduler jitter, timing behavior will differ significantly. The SIL results should not be taken as evidence of timing compliance on target. |
| VIE-007 | MINOR | **No gateway/firewall between ECU domains.** CVC coordinates all zones, but there is no evidence of message filtering between front (FZC) and rear (RZC) zones. A compromised RZC could directly affect FZC-consumed messages (e.g., spoofing CVC heartbeats). |

### Verdict: CONDITIONALLY APPROVED

Conditions:
1. Document Freedom From Interference argument for mixed ASIL/QM bus (VIE-003).
2. Analyze XCP impact on safety-critical message latency (VIE-005).

---

## Auditor 7: Production Quality Engineer

**Role:** Evaluate build reproducibility, CI reliability, and deployment process quality.

### Findings

| ID | Severity | Finding |
|----|----------|---------|
| PQE-001 | PASS | 9 CI gates with blocking enforcement is a solid pipeline. The round-trip check (DBC -> ARXML -> C -> verify) is particularly valuable for catching configuration drift. |
| PQE-002 | PASS | Data flow enforcement gate (grep-based check for SWC->PduR and SWC->E2E calls) is simple, reliable, and catches the exact class of defect that caused Bugs #7-#9. |
| PQE-003 | MAJOR | **21 SIL_DIAG `fprintf` traces remain in production source files.** The development discipline (Rule 8) states "Remove them before merging to main." Grep confirms 21 files contain `#ifdef SIL_DIAG` blocks including `Com.c` (lines 19-21, 324-330), `main.c` (lines 18-19, 450-460), `Swc_CvcCom.c` (lines 29-35), `Xcp.c` (lines 20-22, 318-329), and multiple SWC files. These are ifdef-guarded so they do not affect non-SIL builds, but they indicate incomplete cleanup and violate the project's own stated discipline. |
| PQE-004 | MINOR | **Volatile debug counters (`g_dbg_*`) are production instrumentation but have no retention or readout mechanism.** `Com.c` has 5 volatile debug arrays (lines 44-46, 59, 86). These consume ~1KB RAM (5 arrays * COM_MAX_PDUS * 4 bytes). On resource-constrained ECUs, this should be justified. If these are meant for production diagnostics, they should be accessible via UDS ReadDID, not just debugger. |
| PQE-005 | MINOR | **No version stamping.** Generated files have `/* GENERATED -- DO NOT EDIT */` headers but no version hash linking them to the specific DBC/ARXML/codegen version that produced them. If a build uses stale generated files, there is no way to detect this from the binary. |
| PQE-006 | MINOR | **Layer 6 (Docker SIL) not run this session.** The CI pipeline includes syntax checks and unit tests but the session report shows Layer 6 was skipped. If Docker SIL is the primary deployment target, the full deployment pipeline should be validated as part of any major rewrite. |
| PQE-007 | OBSERVATION | **Build artifacts not checksummed.** No evidence of SHA-256 checksums for `cvc_posix`, `fzc_posix` binaries used in testing. Reproducible builds require artifact verification. |

### Verdict: CONDITIONALLY APPROVED

Conditions:
1. Clean up SIL_DIAG traces or formally accept them as permanent SIL instrumentation (PQE-003).
2. Add version hash to generated files for traceability (PQE-005).

---

## Auditor 8: Code Review Senior Developer

**Role:** Review C code quality, MISRA C:2012 compliance, and defensive programming practices.

### Findings

| ID | Severity | Finding |
|----|----------|---------|
| CR-001 | PASS | Defensive NULL checks on all public API entry points (`Com_Init`, `Com_SendSignal`, `Com_ReceiveSignal`, `Com_RxIndication`). Det_ReportError called correctly with module ID and API ID. |
| CR-002 | PASS | SchM_Enter/Exit exclusive areas used consistently around shared data access in Com.c. Critical sections properly matched (every Enter has a corresponding Exit on all paths). |
| CR-003 | MAJOR | **`Com_RxIndication` holds exclusive area lock for the entire E2E check + signal unpacking + RTE write operation (lines 318-433).** This is ~115 lines of code under a single critical section including an E2E CRC computation, a linear search, and N signal unpack iterations. On a bare-metal system with ISR-driven RX, this blocks all interrupts for the duration. Worst-case execution time could exceed the 1ms task period. The lock should be minimized: copy PDU data under lock, release, then process. |
| CR-004 | MAJOR | **`Swc_CvcCom.c` still includes `PduR.h` (line 27).** Beyond the layering violation noted by BSW-004, this means the SWC has access to `PduR_Transmit` at compile time. The data flow gate only checks for calls, not includes. A developer could accidentally introduce a direct PduR call without a CI failure. The include should be removed to enforce layering at compile time, not just at grep time. |
| CR-005 | MINOR | **`Com_TriggerIPDUSend` uses linear search** (lines 472-509: `for (i = 0u; i < com_config->txPduCount; i++)`) despite the O(1) `com_tx_pdu_index[]` being available. This function is called from `Com_SendSignal` for DIRECT mode, so it is on the hot path. Should use the index. |
| CR-006 | MINOR | **Cast through `void*` in `Com_SendSignal`** (lines 177-187). `*((uint8*)sig->ShadowBuffer) = *((const uint8*)SignalDataPtr)` — this is a type-pun through void pointer, which is MISRA C:2012 Rule 11.5 (advisory). The project documents a DEV-001 deviation for AUTOSAR void* patterns, but this should be explicitly listed in the deviation register for this specific usage. |
| CR-007 | MINOR | **Magic numbers in `Swc_CvcCom.c`.** TX table (line 43-49) uses raw hex CAN IDs (`0x001u`, `0x010u`, `0x100u`) and raw Data IDs. These should reference named constants from the generated config for maintainability. If the DBC changes a CAN ID, this table must be manually updated — violating the "never hand-write what codegen generates" rule. |
| CR-008 | MINOR | **`Com_MainFunction_Tx` has a missing `SchM_TimingStop` on early return** (line 527-528). The startup delay path returns without calling `SchM_TimingStop(TIMING_ID_COM_MAIN_TX)`, which was started at line 517. This could corrupt WCET instrumentation data. |
| CR-009 | OBSERVATION | **Variable `e2eResult` declared but unused** in `Swc_CvcCom_E2eCheck` (line 161). This would trigger MISRA C:2012 Rule 2.2 (no dead code) if MISRA analysis is run on SWC files. The `(void)e2eResult;` suppression at line 182 is a workaround, not a fix. |

### Verdict: CONDITIONALLY APPROVED

Conditions:
1. Reduce Com_RxIndication critical section scope (CR-003).
2. Remove PduR.h include from Swc_CvcCom.c (CR-004).
3. Fix SchM_TimingStop missing on startup delay early return (CR-008).

---

## Auditor 9: Customer OEM Reviewer

**Role:** Evaluate from vehicle manufacturer perspective: acceptance criteria, conformance to platform requirements, and readiness for vehicle integration.

### Findings

| ID | Severity | Finding |
|----|----------|---------|
| OEM-001 | PASS | The DBC-first architecture with 45 messages, E2E P01 protection on 16 safety-relevant messages, and 7-ECU zonal topology is a reasonable platform architecture for a development vehicle. |
| OEM-002 | PASS | E-Stop broadcast mechanism (0x001, 10ms cycle) with independent safety controller (SC on TMS570 lockstep) demonstrates defense-in-depth. |
| OEM-003 | MAJOR | **Platform verified on POSIX SIL only — no target hardware test evidence.** The session report documents 193 tests on vcan0 (virtual CAN). Zero tests on physical CAN hardware (STM32, TMS570). For OEM acceptance, at least a basic sanity test on target hardware showing heartbeat exchange, E2E protection, and bus-off recovery on real CAN transceivers is expected. POSIX SIL cannot verify CAN bit timing, bus-off recovery timing, transceiver wake/sleep, or interrupt latency. |
| OEM-004 | MAJOR | **Safety Controller (SC) has no integration test evidence.** SC is ASIL D and runs on TMS570 lockstep — the most safety-critical ECU. The session report mentions SC_Status and SC heartbeat in the DBC but provides no evidence that SC firmware was compiled, loaded, or tested during this session. The VSM transition SC_KILL->SHUTDOWN was corrected in CVC code but the SC side of this interaction is unverified. |
| OEM-005 | MINOR | **No CAN conformance test results.** OEMs typically require ISO 11898-1 conformance testing (bit timing, error handling, bus-off behavior) on real hardware. CanSM bus-off tests on vcan are necessary but not sufficient. |
| OEM-006 | MINOR | **No production readiness criteria defined.** The report recommends "proceed to RZC config verification" and "Layer 6 Docker SIL" but does not define what constitutes "production ready." A gate review checklist with quantitative criteria (code coverage %, MISRA violations, test pass rate, FTTI compliance, etc.) is needed. |
| OEM-007 | OBSERVATION | **Body control signals (Group 4.6) are all zeros.** The test passes because "no body requests" is the expected state, but this means the body control path (BCM) is entirely untested. An OEM would expect at least one end-to-end body control scenario (headlight on/off command -> BCM actuation -> status feedback). |

### Verdict: CONDITIONALLY APPROVED

Conditions:
1. Provide target hardware test evidence for at least CVC+FZC on physical CAN (OEM-003).
2. SC integration test plan and evidence (OEM-004).
3. Define production readiness gate criteria (OEM-006).

---

## Auditor 10: Independent Safety Assessor (ISA)

**Role:** ISO 26262 Part 2 independent evaluation of the entire safety case.

### Findings

| ID | Severity | Finding |
|----|----------|---------|
| ISA-001 | PASS | The overall approach of the rewrite is sound. Moving E2E protection from SWC to BSW layer is the correct architectural decision per AUTOSAR and eliminates a systemic violation (31 data flow errors to 0). The session demonstrated engineering rigor with 17 bugs found and fixed, including 2 safety-relevant VSM defects. |
| ISA-002 | PASS | The 6-layer verification strategy demonstrates structured progression from unit to integration. The discipline of "each layer passes before proceeding" is ISO 26262 Part 6 Table 9 (verification) compliant in principle. |
| ISA-003 | CRITICAL | **No independent verification.** All testing was performed by the same engineer who wrote the code. ISO 26262-2 clause 6.4.8 requires independence in verification for ASIL D (independence category I2 or I3). The session report, the code, and the tests all appear to be authored by one person. For ASIL D, at minimum I2 (different person in same organization) is required for software verification. This audit itself does not satisfy I2/I3 requirements as it is reviewing a report, not independently executing verification activities. |
| ISA-004 | MAJOR | **Safety case completeness.** The session report documents the CAN data flow rewrite but the overall safety case requires: (a) Safety Plan with verification milestones, (b) Safety Analysis results (FMEA, FTA, DFA) updated for new architecture, (c) Confirmation measures, (d) Assessment of residual risk. References to these documents exist (`docs/safety/plan/`, `docs/safety/analysis/`) but there is no evidence they were updated to reflect the rewrite. |
| ISA-005 | MAJOR | **Confirmation review of safety anomalies not documented.** Bugs #4 (SC_KILL->SAFE_STOP) and #5 (MOTOR_CUTOFF->SAFE_STOP) are safety anomalies per ISO 26262-8 clause 8. They require: (a) impact analysis on fielded/released versions, (b) root cause classification, (c) corrective action effectiveness verification, (d) assessment of whether the anomaly existed in released artifacts. The session report documents the fix but not the formal anomaly process. |
| ISA-006 | MINOR | **Test environment qualification not documented.** The POSIX SIL on vcan0 is used as the primary verification environment. ISO 26262-8 clause 9.4 requires qualification of software tools used in verification. Is vcan0 behavior representative enough? What are the known limitations? The test script uses `time.sleep()` and `bus.recv(timeout=)` which are non-deterministic — can they produce false passes? |
| ISA-007 | MINOR | **Configuration management of test artifacts.** The test script `test_cvc_fzc_comprehensive.py` references binaries at `build/cvc_posix` and `build/fzc_posix`. These are build artifacts, not configuration-managed items. There is no evidence that the binary tested is the same binary built from the reviewed source code. A CI pipeline that builds and tests in the same run would address this, but the session report describes manual test execution. |

### Verdict: REJECTED (for ASIL D release)

**Note:** Rejected for formal ASIL D release, not for development progress. The engineering work is sound and the platform is progressing well. However, the following are mandatory for any safety release:

1. Independent verification by a second engineer (ISA-003).
2. Complete safety case update (ISA-004).
3. Formal safety anomaly processing for Bugs #4 and #5 (ISA-005).

---

## Consolidated Summary

### Finding Statistics

| Severity | Count | Auditors |
|----------|-------|----------|
| CRITICAL | 3 | SEC-001, SEC-002, ISA-003 |
| MAJOR | 14 | FSM-003, FSM-004, BSW-003, BSW-008, TST-003, TST-004, SEC-003, ASP-004, VIE-003, PQE-003, CR-003, CR-004, OEM-003, OEM-004, ISA-004, ISA-005 |
| MINOR | 21 | FSM-005, FSM-007, BSW-004, BSW-005, BSW-007, TST-005, TST-006, TST-008, SEC-004, SEC-005, ASP-003, ASP-005, ASP-006, VIE-004, VIE-005, VIE-007, PQE-004, PQE-005, PQE-006, CR-005, CR-006, CR-007, CR-008, CR-009, OEM-005, OEM-006, ISA-006, ISA-007 |
| OBSERVATION | 8 | FSM-006, BSW-006, TST-007, TST-009, SEC-006, VIE-006, OEM-007, PQE-007 |
| PASS | 16 | (across all auditors) |

### Verdicts by Auditor

| # | Auditor | Verdict |
|---|---------|---------|
| 1 | ISO 26262 Functional Safety Manager | CONDITIONALLY APPROVED |
| 2 | AUTOSAR BSW Architect | CONDITIONALLY APPROVED |
| 3 | Embedded Systems Test Lead | CONDITIONALLY APPROVED |
| 4 | Cybersecurity Auditor (ISO 21434) | **REJECTED** |
| 5 | ASPICE Process Assessor | CONDITIONALLY APPROVED |
| 6 | Vehicle Integration Engineer | CONDITIONALLY APPROVED |
| 7 | Production Quality Engineer | CONDITIONALLY APPROVED |
| 8 | Code Review Senior Developer | CONDITIONALLY APPROVED |
| 9 | Customer OEM Reviewer | CONDITIONALLY APPROVED |
| 10 | Independent Safety Assessor (ISA) | **REJECTED** (for ASIL D release) |

### Overall Recommendation

**CONDITIONALLY APPROVED for continued development. REJECTED for formal ASIL D safety release.**

The CAN data flow rewrite represents significant engineering progress. The architectural decision to relocate E2E into the Com BSW layer is correct, the 17 bugs found-and-fixed demonstrate rigorous verification, and the 6-layer test methodology is well-structured.

However, three blocking issues prevent approval for safety release:

1. **XCP security (SEC-001/002):** Arbitrary memory write without authentication is unacceptable on any safety-critical platform. This must be mitigated before any vehicle testing.

2. **Independent verification (ISA-003):** ASIL D requires I2/I3 independence. All current verification was self-verified.

3. **Incomplete ECU coverage (TST-003/OEM-003/OEM-004):** Only 2/7 ECUs verified at integration level, and zero tests on target hardware.

### Recommended Priority Actions

| Priority | Action | Blocking? | Effort |
|----------|--------|-----------|--------|
| P0 | Disable XCP SHORT_DOWNLOAD or add Seed & Key | YES | 1 day |
| P0 | Fix Com_RxIndication critical section scope | YES | 0.5 day |
| P0 | Add bounds check for multi-byte signal packing | YES | 0.5 day |
| P1 | Extend Layer 4+ to RZC, SC (at minimum) | YES for release | 2-3 days |
| P1 | FTTI impact assessment for timing changes | YES for release | 1 day |
| P1 | Remove PduR.h/E2E.h from Swc_CvcCom.c | No | 0.5 hour |
| P1 | Fix SchM_TimingStop on startup delay path | No | 0.5 hour |
| P1 | Handle COM_UINT32 or document as limitation | No | 0.5 day |
| P2 | Target hardware smoke test (CVC+FZC on STM32) | YES for OEM | 1-2 days |
| P2 | Clean up SIL_DIAG traces per Rule 8 | No | 0.5 day |
| P2 | Fix 16 broken VSM unit tests | No | 1 day |
| P2 | Safety anomaly processing for Bugs #4/#5 | YES for release | 1 day |
| P3 | DFA update for new BSW modules | YES for release | 1 day |
| P3 | Freedom From Interference analysis for mixed bus | YES for release | 2 days |
| P3 | Formal TARA for cybersecurity | YES for release | 3-5 days |

---

*Review conducted: 2026-03-21*
*Classification: ISO 26262 ASIL D — Safety-Related*
*Document ID: AUDIT-10-REVIEW-2026-03-21*
