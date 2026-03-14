# Plan: ARXML -> SIL Feasibility and Failure Modes

## Objective

Assess whether `taktflow-embedded` can realistically evolve to accept ARXML inputs and run generated integrations in SIL with production-like quality gates.

## Feasibility Verdict

## 1) Technical Feasibility: High

Reason:
- The repo already has a stable generation pipeline:
  - `manifest + DBC -> resolve -> generators -> build/verify`
- ARXML can be added as an upstream adapter that emits the existing manifest/canonical model, instead of replacing the current toolchain.

Implication:
- This is an incremental extension, not a rewrite.

## 2) Delivery Feasibility: Medium

Reason:
- AUTOSAR ARXML is broad and vendor-variable.
- Feasible only if we define a strict MVP subset first (ECU Extract + selected communication/config elements).

Implication:
- Scope control is the critical success factor.

## 3) Safety/Process Feasibility: Medium-High

Reason:
- If we add deterministic validation, artifact drift checks, and trace links from ARXML to SIL verdicts, the flow is compatible with ASPICE/ISO-style evidence expectations.

Implication:
- Governance quality depends on CI and traceability design, not just parser correctness.

## Primary Failure Modes

1. ARXML schema/version mismatch (`4.2.x` vs `4.4.x`)
- Effect: parse failures or silent field loss.
- Mitigation: explicit supported version matrix + hard fail on mismatch.

2. Cross-file reference resolution failure
- Effect: incomplete ECU model and wrong generation.
- Mitigation: full reference index with unresolved-ref errors.

3. Vendor extension handling gaps
- Effect: valid customer ARXML rejected or, worse, partially misinterpreted.
- Mitigation: unsupported-extension report + deterministic fail/ignore policy per extension.

4. Signal/DLC/endianness mismatch
- Effect: runtime CAN decode/encode defects despite successful generation.
- Mitigation: semantic validation rules + generated consistency tests.

5. Missing timing semantics (cycle/timeout/defaults)
- Effect: unstable integration, heartbeat or timeout behavior diverges in SIL.
- Mitigation: timing fields mandatory for safety-relevant paths; defaults explicitly logged.

6. Name/path collisions (`SHORT-NAME` ambiguity)
- Effect: wrong signal/PDU mapping.
- Mitigation: canonical path IDs and collision checks as blocking errors.

7. Non-deterministic generation output
- Effect: noisy diffs, CI churn, hard review.
- Mitigation: stable sort/order for all generated artifacts.

8. SIL false confidence from weak smoke tests
- Effect: integration defects escape to bench/HIL.
- Mitigation: include fault-oriented generated scenarios (timeout, loss, safe-state).

9. Traceability discontinuity
- Effect: difficult audit defense and weak evidence chain.
- Mitigation: generate trace map: ARXML element -> generated file -> scenario -> verdict.

## Recommended Constraints for MVP

1. Accept only one declared ARXML version family initially.
2. Support only selected ARXML elements required by current onboard generators.
3. Reject unsupported constructs explicitly (no silent fallback).
4. Treat semantic validation errors as generation blockers.
5. Require generated SIL smoke pass in CI before merge.

## Readiness Criteria (Go/No-Go)

Proceed if all are true:

1. ARXML subset and unsupported list are documented and versioned.
2. Structural + semantic validation commands exist and are deterministic.
3. One sample ECU can complete: ARXML -> generation -> SIL smoke pass.
4. CI enforces validation/generation/drift/smoke gates.
5. Traceability report can show one end-to-end signal path in minutes.

If any are false:
- Keep ARXML path in experimental mode and do not make it default onboarding flow.

## Delivery Estimate (Pragmatic)

- MVP feasibility implementation: ~4-7 weeks
  - 1-2 weeks validation infrastructure
  - 1-2 weeks transformation bridge to existing generator
  - 1 week SIL bootstrap + scenarios
  - 1-2 weeks CI hardening and traceability outputs

## Sources

- Vector validation-first configuration workflow:
  - https://help.vector.com/davinci-configurator-classic/en/current/dvcfg-classic/6.2-SP0/getting-started/workflow/local/check-configuration.html

- MathWorks ARXML importer/validation and round-trip preservation:
  - https://www.mathworks.com/help/autosar/ug/autosar-arxml-importer-tool.html
  - https://www.mathworks.com/help/autosar/ug/round-trip-preservation-of-autosar-elements-and-uuids.html

- AUTOSAR methodology and system template (ECU extract context):
  - https://www.autosar.org/fileadmin/standards/R19-11/CP/AUTOSAR_TR_Methodology.pdf
  - https://www.autosar.org/fileadmin/standards/R19-11/CP/AUTOSAR_TPS_SystemTemplate.pdf

- dSPACE architecture validation/import context:
  - https://www.dspace.com/en/inc/home/products/systems/system_architecture/sd_casestudy_ar-validation.cfm

- Open-source AUTOSAR XML package limitations/scope reality:
  - https://github.com/cogu/autosar/blob/master/apps-web-overview.md

