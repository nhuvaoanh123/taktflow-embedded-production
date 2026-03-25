# Code Gap Analysis — taktflow-embedded-production

**Date**: 2026-03-25
**Scope**: Full repository audit — source code, tests, build, docs, traceability, SIL, safety
**Status**: READ-ONLY — no fixes applied. Document gaps only.
**Auditor**: Claude Code (automated static + structural analysis)

---

## Summary

| Severity | Count |
|----------|-------|
| CRITICAL | 8 |
| HIGH | 15+ |
| MEDIUM | 25+ |
| LOW | 5+ |
| **Total distinct gaps** | **50+** |

---

## 1. Source Code — Hardware Stubs

### 1.1 STM32 MCAL Drivers — Complete Stubs (CRITICAL)

All five STM32 MCAL drivers are empty stubs. Every function returns `E_OK` or zero
without touching any hardware register. Marked `TODO:HARDWARE` / Phase F2/F3.

| File | Functions stubbed | Severity |
|------|------------------|----------|
| `firmware/platform/stm32/src/Spi_Hw_STM32.c` | Init, Transmit, GetStatus | CRITICAL |
| `firmware/platform/stm32/src/Pwm_Hw_STM32.c` | Init, SetDuty, SetIdle | CRITICAL |
| `firmware/platform/stm32/src/Gpt_Hw_STM32.c` | Init, StartTimer, StopTimer, GetCounter | CRITICAL |
| `firmware/platform/stm32/src/Adc_Hw_STM32.c` | Init, StartConversion, GetResult, GetStatus | CRITICAL |
| `firmware/platform/stm32/src/Dio_Hw_STM32.c` | ReadPin (returns STD_LOW always), WritePin (no-op) | CRITICAL |

**Impact**: Any SWC that calls SPI, PWM, GPT, ADC, or DIO on STM32 is silently doing nothing.
Real hardware will not respond. SIL passes because the stub returns success — HIL will not.

---

### 1.2 TMS570 Safety Self-Tests — Unimplemented (HIGH)

`firmware/platform/tms570/src/sc_hw_tms570.c` contains 8 self-test functions that are
empty stubs with `TODO:HARDWARE` comments. These are safety-critical diagnostics.

| Function | Test type | Line (approx) | Severity |
|----------|-----------|---------------|----------|
| STC self-test | CPU lockstep test | 837 | HIGH |
| PBIST | RAM pattern test | 850 | HIGH |
| Flash CRC check | MCRC module | 862 | HIGH |
| DCAN1 loopback | CAN self-test | 875 | HIGH |
| GPIO readback | Pin verify | 888 | HIGH |
| Lamp test | LED output | 900 | HIGH |
| Watchdog test | WDT verify | 913 | HIGH |
| Incremental flash CRC | Background CRC | 925 | HIGH |

**Impact**: SC/CVC boot-time safety check reports pass without performing any actual test.
Any hardware fault in these subsystems is invisible at startup.

---

### 1.3 Post-Beta Stub — E2E CRC Validation (MEDIUM)

| File | Line | Issue |
|------|------|-------|
| `firmware/ecu/bcm/src/Swc_BcmCan.c` | 95 | `TODO:POST-BETA — implement real E2E CRC-8 validation` — currently always returns `TRUE` |

**Impact**: E2E protection on BCM CAN frames is not functional. Corrupt frames are accepted.

---

### 1.4 rtcan Library TODOs (MEDIUM)

| File | Line | Issue |
|------|------|-------|
| `firmware/lib/rtcan/src/rtcan.c` | 17 | Stack size profiling not implemented |
| `firmware/lib/rtcan/src/rtcan.c` | 184 | CAN filter configuration incomplete |
| `firmware/lib/rtcan/src/rtcan.c` | 476 | Error handling path not implemented |

---

### 1.5 Unity Test Framework — Vendor Stub (MEDIUM)

| File | Issue |
|------|-------|
| `firmware/lib/vendor/unity/unity.h` | Line 8: "TODO:HARDWARE — Replace this minimal stub with the full Unity v2.6.x source" |
| `firmware/lib/vendor/unity/unity_internals.h` | Same notice |
| `firmware/lib/vendor/unity/unity.c` | Same notice |

**Impact**: If the Unity in-tree copy is not the full release, test assertions may behave
differently than expected. Needs confirmation that the vendored copy is complete.

---

## 2. Test Coverage Gaps

### 2.1 SWCs with No Test File (HIGH)

| SWC | Source file | Test file |
|-----|------------|-----------|
| FzcSensorFeeder | `firmware/ecu/fzc/src/Swc_FzcSensorFeeder.c` | MISSING |
| RzcSensorFeeder | `firmware/ecu/rzc/src/Swc_RzcSensorFeeder.c` | MISSING |

**Impact**: Zero unit test coverage for both sensor feeder SWCs. Signal injection path
to FZC and RZC is untested at unit level.

---

### 2.2 Disabled / Ignored Tests (MEDIUM)

| File | Issue |
|------|-------|
| `test/unit/bsw/test_XCP_security_generated.c` | 2× `TEST_IGNORE_MESSAGE` for XCP 32-bit address tests — disabled on 64-bit host |

**Impact**: XCP address-space tests never run in CI (x86_64 host). Gap in XCP security coverage.

---

## 3. Build System Gaps

### 3.1 Warning Suppression per Platform (MEDIUM)

Warnings that should be errors are suppressed globally on three platforms:

| Platform | File | Suppression | Why it's a gap |
|----------|------|-------------|----------------|
| STM32 | `firmware/platform/stm32/Makefile.stm32` | `-Wno-error=unused-parameter -Wno-error=cast-function-type` | ThreadX headers; masks real issues |
| STM32F4 | `firmware/platform/stm32f4/Makefile.stm32f4` | `$(filter-out -Werror) -Wno-unused-parameter` | Removes -Werror entirely for vendor HAL |
| TMS570 | `firmware/platform/tms570/Makefile.tms570` | `-Wno-unused-parameter -Wno-unused-variable -Wno-ti-macros -Wno-ti-pragmas` | HALCoGen output |

**Impact**: Build warnings in platform code are silently ignored. BCM/ICU/TCU pre-existing
-Werror warnings are masked here and never surface in CI.

---

### 3.2 Missing Build Targets (MEDIUM)

| Missing target | Impact |
|----------------|--------|
| `make flash-verify` | No post-flash readback/checksum verification |
| Explicit `make coverage-report` | Coverage only produced in CI, not locally |
| `make codegen-determinism` | Codegen round-trip check only in CI via git diff, not local |

---

### 3.3 BCM / ICU / TCU Pre-existing -Werror Warnings (HIGH)

Per project memory: BCM, ICU, and TCU have pre-existing `-Werror` warnings that fail the
strict build. These modules are either excluded from CI or built with suppressed flags.

**Gap**: Exact warnings not catalogued. No issue log entry. No timeline for resolution.
**Impact**: Three ECU firmware modules cannot be built with the full warning policy.

---

## 4. Traceability Gaps

### 4.1 SWC Annotation Tags — Zero Across All 45+ SWCs (CRITICAL)

Every SWC file is missing mandatory documentation tags in its header:

| Tag | Purpose | Count found |
|-----|---------|------------|
| `@owner` | Responsible developer | 0 / 45+ SWCs |
| `@satisfies SSR-*` | Links SWC to software requirement | 0 / 45+ SWCs |
| `@asil` | ASIL level of this component | 0 / 45+ SWCs |
| `@consumes` | Signals/ports read by this SWC | 0 / 45+ SWCs |
| `@produces` | Signals/ports written by this SWC | 0 / 45+ SWCs |

**Impact**: Cannot produce automated traceability from Safety Goals → SSR → SWC → Code.
Traceability matrix relies entirely on manual cross-referencing. Breaks ISO 26262 §6 requirements.

---

### 4.2 ARXML Phase 2 — SWC Enrichment Incomplete (CRITICAL)

Per contract matrix (`docs/standards/contract-matrix.md` lines 210–221):

| Level | Status |
|-------|--------|
| Level 3: ARXML | MISSING — no SWCs in production ARXML; Phase 2 enrichment not injected |
| Level 4: Codegen | Partial — round-trip check in CI, no determinism contract |
| Level 5: SWC code | MISSING — zero @owner/@satisfies tags |

**Impact**: ARXML describes the interface architecture but does not contain SWC implementations.
Code generation from ARXML to production RTE config is not verified end-to-end.

---

### 4.3 MISRA Suppression ↔ Deviation Register Link (HIGH)

- Suppressions exist inline in source (`// cppcheck-suppress`)
- Deviation register exists at `docs/safety/analysis/misra-deviation-register.md`
- **Gap**: Suppressions in code are not cross-referenced back to the deviation register entries.
  It is impossible to verify all suppressions are justified without manual grep + cross-check.

---

### 4.4 Safety Goals → SSR Trace (HIGH)

| Gap | Location |
|-----|----------|
| No formal trace matrix tool — manual only | `docs/aspice/verification/traceability-matrix.md` note |
| SSR-level links not verified against all Safety Goals | `docs/aspice/software/sw-requirements/SWR-*.md` |

---

## 5. SIL Gaps

### 5.1 SIL Pass Rate: 13/18 = 72% (HIGH)

Five scenarios not passing. Exact scenario IDs not confirmed in audit.
`sil_016` appears absent from the configured scenario list (sil_001–015, sil_017).

| Gap | Detail |
|-----|--------|
| Missing scenario | `sil_016` not present in scenario config |
| Pass rate | 13/18 (72%) — 5 scenarios failing or unimplemented |
| Hop tests | Not implemented for any virtual sensor signal path (per contract matrix) |
| SIL verification report | `docs/aspice/verification/xil/sil-report.md` — only 48 lines, incomplete |

---

### 5.2 Docker Configuration Gaps (MEDIUM)

| File | Issue |
|------|-------|
| `docker/gateway/.gitkeep` | Gateway container definition missing |
| `docker/sil/.gitkeep` | SIL container definition missing |
| Multiple compose variants | `dev`, `gil`, `hil`, `laptop`, `pi`, `sil` — not all validated in CI |

---

## 6. Documentation Structural Gaps

### 6.1 Empty Placeholder Directories (LOW)

17 directories contain only `.gitkeep` with no actual content:

```
docs/api/
docs/aspice/cm/
docs/aspice/hardware/
docs/aspice/plans/
docs/aspice/qa/
docs/aspice/software/
docs/aspice/system/
docs/aspice/traceability/
docs/aspice/verification/
docs/lessons-learned/
docs/plans/
docs/reference/
docs/safety/analysis/
docs/safety/concept/
docs/safety/plan/
docs/safety/requirements/
docs/safety/validation/
```

**Note**: Some of these have sub-files; the `.gitkeep` is the marker, not the only content.
Confirm which are truly empty vs. which have files further down.

---

### 6.2 SIL Verification Report — Incomplete (MEDIUM)

| File | Lines | Status |
|------|-------|--------|
| `docs/aspice/verification/xil/sil-report.md` | 48 | Stub — expected 200+ lines matching unit/integration report depth |
| `docs/aspice/verification/xil/hil-report.md` | Unknown | Not verified complete |
| `docs/aspice/verification/xil/pil-report.md` | Unknown | Not verified complete |

---

## 7. Priority Order for Resolution

Ordered by risk, not effort:

| Priority | Gap | Category |
|----------|-----|----------|
| P1 | Add `@owner` / `@satisfies SSR-*` tags to all 45+ SWCs | traceability |
| P2 | Complete ARXML Phase 2 SWC enrichment + inject into codegen | traceability |
| P3 | Implement TMS570 self-tests (STC, PBIST, Flash CRC, DCAN loopback) | safety |
| P4 | Implement real E2E CRC-8 in Swc_BcmCan (post-beta TODO) | safety |
| P5 | Write unit tests for FzcSensorFeeder + RzcSensorFeeder | test-coverage |
| P6 | Catalogue + fix BCM/ICU/TCU -Werror warnings | build |
| P7 | Resolve 5 failing SIL scenarios; implement hop tests | sil |
| P8 | Link all MISRA suppressions to deviation register entries | safety |
| P9 | Implement STM32 MCAL drivers (Phase F2/F3 hardware bringup) | stub |
| P10 | Replace Unity vendor stub with full Unity v2.6.x | test-coverage |

---

*This document is a read-only gap inventory. No code was modified during this audit.*
