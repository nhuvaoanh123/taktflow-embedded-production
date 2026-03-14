# Plan: ARXML Intake to SIL (Pro Workflow Adapted to This Repo)

## Objective

Enable `taktflow-embedded` to accept customer AUTOSAR ARXML input and produce a repeatable path to SIL execution with validation gates similar to professional toolchains.

## What Pros Commonly Do (Research Summary)

1. Validate early, then generate.
- Vector DaVinci workflow explicitly validates ECU configuration for completeness/consistency/correctness before code generation.

2. Keep ARXML round-trip-safe.
- MathWorks ARXML importer emphasizes parsing + XML validation + preserving imported AUTOSAR structure for round-trip export.

3. Use ECU Extract as generation basis.
- AUTOSAR methodology/system template documents define ECU Extract as fully decomposed ECU-specific input for configuration.

4. Separate architecture validation from ECU config.
- dSPACE SystemDesk case material emphasizes architecture validation reports and import openness for schema-compliant ARXML.

5. Standardize virtual ECU interfaces.
- ETAS VECU-BUILDER and COSYM positioning show production teams package/compose vECUs for SIL and CI/CD pipelines, often using standards like FMI/FMU for interoperability.

## Repo-Fit Strategy

Do not replace current onboarding flow immediately.

Use a two-stage architecture:
1. `ARXML -> canonical model -> existing onboard manifest + configs`
2. `manifest/configs -> existing codegen + SIL harness`

This preserves velocity and reuses `tools/onboard` and `test/sil`.

## Target Architecture

1. Ingest Layer
- Input: one or more ARXML files (System, ECU Extract, SWC descriptions).
- Output: canonical JSON model (`build/arxml/model.json`).

2. Validation Layer
- XSD/schema validation.
- Semantic validation (references, duplicates, direction/type mismatches, timing/DLC checks, unsupported constructs).
- Output: machine-readable report + human summary.

3. Transformation Layer
- Convert canonical model to:
  - onboard manifest YAML
  - communication mappings (CAN IDs, signals, cycle/timeout)
  - generated stubs for `Com/CanIf/PduR` compatible with your code style.

4. SIL Bootstrap Layer
- Auto-generate:
  - compose fragment/service config
  - scenario skeletons in `test/sil/scenarios/`
  - baseline verdict checks (startup, heartbeat, timeout, safe-state).

5. CI Layer
- New gates:
  - `arxml:validate`
  - `arxml:generate --dry-run`
  - `sil:smoke:generated`
  - generated artifact drift check.

## Implementation Phases

## Phase 0 - Contract First (1 week)

Deliverables:
- Supported ARXML subset spec (MVP).
- Explicit unsupported list with failure behavior.
- Example customer package structure.

Acceptance:
- Team can answer: "Which ARXML elements are accepted today?" with one versioned document.

## Phase 1 - ARXML Import + Structural Validation (1-2 weeks)

Deliverables:
- `tools/onboard/arxml/importer.py`
- XSD/schema validator command.
- `arxml:validate` CLI output (json + text).

Acceptance:
- Invalid schema files fail with deterministic, line-referenced errors.
- Valid inputs produce canonical model file.

## Phase 2 - Semantic Validation (1-2 weeks)

Deliverables:
- Rule set for:
  - unresolved refs
  - duplicate short-name/path collisions
  - signal length/DLC mismatch
  - direction mismatch (tx/rx)
  - missing timing or timeout defaults.
- Severity model (`error`, `warn`, `info`).

Acceptance:
- Known-bad fixture suite triggers expected diagnostics.
- Validation report is stable across runs.

## Phase 3 - Generator Bridge to Existing Onboard (1 week)

Deliverables:
- `arxml -> onboard manifest` converter.
- Optional DBC or mapping export used by current generator.
- Generated config stubs for shared BSW integration points.

Acceptance:
- For at least one sample ECU, generated output builds through existing `tools/onboard` path.

## Phase 4 - SIL Auto Bootstrap (1 week)

Deliverables:
- Auto-compose service template for new ECU.
- Generated SIL smoke scenarios:
  - normal startup
  - heartbeat timeout
  - communication timeout safe fallback.

Acceptance:
- `run_sil.sh --scenario=<generated-smoke>` passes on baseline environment.

## Phase 5 - CI/CD Hardening (1 week)

Deliverables:
- CI job for validation + generation + SIL smoke.
- Diff guard that blocks manual edits in generated files unless regenerated.
- Artifact publish (validation report + generated manifest + SIL logs).

Acceptance:
- PR fails on ARXML or generation regressions.
- CI artifacts are sufficient for review without local rerun.

## Phase 6 - Safety/Process Integration (ongoing)

Deliverables:
- Trace link convention:
  - ARXML element -> generated config -> test scenario -> verdict report.
- Update ASPICE/verification docs for generated-flow evidence.
- Release checklist for ARXML intake.

Acceptance:
- A reviewer can trace one signal from ARXML to SIL verdict evidence in under 5 minutes.

## Micro Breakdown (Smaller Execution Units)

## M0 - Repo Scaffolding (1-2 days)

Tasks:
1. Create folders:
- `tools/onboard/arxml/`
- `tools/onboard/tests/arxml-fixtures/valid`
- `tools/onboard/tests/arxml-fixtures/invalid`
2. Add CLI placeholder command:
- `python -m tools.onboard arxml validate --input <path>`
3. Add empty output contract docs:
- canonical model schema draft
- validation report schema draft

Done when:
- command runs and returns a deterministic "not implemented yet" with correct CLI args.

## M1 - Structural Parser Skeleton (2-3 days)

Tasks:
1. Implement namespace-safe XML loader.
2. Discover ARXML files from input path.
3. Parse top-level packages and collect raw element index.
4. Emit minimal canonical model with source paths.

Done when:
- one valid sample produces `build/arxml/model.json`.

## M2 - XSD/Schema Gate (2-3 days)

Tasks:
1. Add schema validation entrypoint.
2. Map validation errors to file + line where possible.
3. Emit `validation-structural.json` and text summary.

Done when:
- invalid fixture fails with stable error codes and non-zero exit.

## M3 - Reference Resolver (2-4 days)

Tasks:
1. Build global reference table (short-name + path).
2. Resolve cross-file references.
3. Mark unresolved references as blocking errors.

Done when:
- known broken-reference fixture fails exactly on resolver rule IDs.

## M4 - Semantic Rule Set v1 (3-5 days)

Tasks:
1. Implement rule engine with stable IDs.
2. Add initial blocking rules:
- duplicate IDs
- tx/rx direction mismatch
- signal length vs DLC mismatch
- missing cycle/timeout on required message classes
3. Add warning rules for defaults/fallbacks.

Done when:
- expected rule IDs appear for each invalid semantic fixture.

## M5 - Canonical Model Freeze v1 (2 days)

Tasks:
1. Version canonical model (`modelVersion`).
2. Lock deterministic field ordering.
3. Add serialization tests to prevent format drift.

Done when:
- same input yields byte-stable JSON output in repeated runs.

## M6 - Manifest Bridge v1 (3-4 days)

Tasks:
1. Convert canonical model -> onboard manifest YAML.
2. Map message/signal/timeouts into current resolver expectations.
3. Generate dry-run output preview.

Done when:
- produced manifest is accepted by existing `tools.onboard generate`.

## M7 - Config Stub Generation Bridge (3-5 days)

Tasks:
1. Generate/augment communication mapping artifacts for existing generators.
2. Validate generated `Com/CanIf/PduR`-related stubs compile in one sample ECU path.
3. Add guard comments for generated files.

Done when:
- sample ECU build path completes without manual edits.

## M8 - SIL Bootstrap Templates (2-4 days)

Tasks:
1. Generate compose fragment/service entry for ECU.
2. Generate three smoke scenarios:
- startup
- heartbeat timeout
- comm timeout safe fallback
3. Generate scenario metadata with trace tags.

Done when:
- generated scenarios are accepted by `run_sil.sh` parser.

## M9 - SIL Smoke Execution Gate (2-3 days)

Tasks:
1. Add script target for generated-smoke run.
2. Ensure logs are saved in predictable location.
3. Parse pass/fail summary from verdict output.

Done when:
- pipeline exits non-zero on smoke failure, zero on pass.

## M10 - CI Integration v1 (2-4 days)

Tasks:
1. Add CI job stages:
- structural validate
- semantic validate
- generate dry-run
- generated smoke
2. Publish artifacts (reports + generated outputs + SIL logs).
3. Add branch protection recommendation.

Done when:
- CI blocks merge on any failed stage.

## M11 - Drift Guard (1-2 days)

Tasks:
1. Add generated-file fingerprint/header strategy.
2. Add check that regenerated output matches committed files.

Done when:
- manual edits in generated files fail CI with actionable message.

## M12 - Traceability Report v1 (2-3 days)

Tasks:
1. Generate mapping report:
- ARXML element -> generated file -> scenario ID -> verdict file
2. Export JSON + markdown summary.

Done when:
- one signal can be traced end-to-end from report output alone.

## M13 - Pilot ECU Rollout (3-5 days)

Tasks:
1. Select one representative ECU profile.
2. Run full flow from raw ARXML to SIL smoke.
3. Document deltas, unsupported constructs, and backlog updates.

Done when:
- pilot runs end-to-end with no manual core-config edits.

## M14 - Promote to "Supported (MVP)" (1-2 days)

Tasks:
1. Publish supported subset + known limitations.
2. Tag tool version and freeze MVP behavior.
3. Add onboarding instructions for ARXML customers.

Done when:
- team can onboard new ARXML package via documented command sequence.

## Nano Breakdown (Even Smaller Units)

## N0 - M0 Repo Scaffolding

1. Create `tools/onboard/arxml/__init__.py`.
2. Create `tools/onboard/arxml/validate.py` placeholder.
3. Create `tools/onboard/tests/arxml-fixtures/valid/.gitkeep`.
4. Create `tools/onboard/tests/arxml-fixtures/invalid/.gitkeep`.
5. Add `arxml` subparser in `tools/onboard/cli.py`.
6. Add `validate` subcommand with `--input`.
7. Return exit code `2` for not-implemented path.
8. Add single CLI smoke test command in docs.

## N1 - M1 Structural Parser Skeleton

1. Add function `load_xml(path)`.
2. Add function `discover_arxml(input_path)`.
3. Add namespace map helper.
4. Parse root `AUTOSAR` element.
5. Collect `AR-PACKAGES` paths.
6. Collect file origin metadata per package.
7. Emit minimal JSON with:
- `modelVersion`
- `sourceFiles`
- `packages`
8. Write JSON to `build/arxml/model.json`.

## N2 - M2 Structural Validation

1. Add `validation_report` data class/schema.
2. Add `structural_errors` list in report.
3. Add CLI flag `--report-json`.
4. Add CLI flag `--report-text`.
5. Wire XSD validation call (or placeholder adapter).
6. Map each error to `{code,file,line,message}`.
7. Exit non-zero if any structural error.
8. Add one invalid fixture expecting failure.

## N3 - M3 Reference Resolver

1. Build global element table keyed by canonical path.
2. Build secondary table keyed by short-name.
3. Extract reference fields from model.
4. Attempt resolution against canonical table.
5. If unresolved, append rule `ARXML_REF_001`.
6. If ambiguous short-name match, append `ARXML_REF_002`.
7. Write resolver summary counts.
8. Fail when resolver errors > 0.

## N4 - M4 Semantic Rules v1

1. Create rule registry with stable IDs.
2. Implement `ARXML_SEM_001` duplicate-ID check.
3. Implement `ARXML_SEM_002` tx/rx direction mismatch.
4. Implement `ARXML_SEM_003` signal-length vs DLC check.
5. Implement `ARXML_SEM_004` missing cycle time.
6. Implement `ARXML_SEM_005` missing timeout for RX.
7. Add severity field per finding.
8. Add fixture per rule (one failing case each).
9. Add deterministic sorting of findings.
10. Add summary footer: errors/warnings/info.

## N5 - M5 Canonical Model Freeze v1

1. Add `modelVersion: 1`.
2. Sort all arrays by canonical key before serialize.
3. Serialize with fixed indentation and key order.
4. Add hash output (`sha256`) to report.
5. Add repeat-run test: hash equality.
6. Fail CI on model drift for same fixture input.

## N6 - M6 Manifest Bridge v1

1. Map ECU name into manifest `ecu.name`.
2. Map baudrate into `ecu.can_baudrate`.
3. Map TX messages and cycle times.
4. Map RX messages and timeouts.
5. Map signal name links (`dbc_signal`/`rte_signal` mapping or equivalent internal mapping).
6. Write `manifest.generated.yaml`.
7. Run `tools.onboard generate --dry-run` on generated manifest.
8. Fail if resolver rejects generated manifest.

## N7 - M8 SIL Bootstrap

1. Generate compose snippet for one ECU service.
2. Generate `sil_generated_startup.yaml`.
3. Generate `sil_generated_heartbeat_timeout.yaml`.
4. Generate `sil_generated_comm_timeout.yaml`.
5. Add scenario IDs and `verifies` placeholders.
6. Validate YAML parse for all generated scenarios.
7. Add command example to run each scenario.

## N8 - M10 CI v1

1. Add CI step: structural validate.
2. Add CI step: semantic validate.
3. Add CI step: generate dry-run.
4. Add CI step: generated smoke scenario run.
5. Upload structural/semantic reports as artifacts.
6. Upload generated manifest/model as artifacts.
7. Upload SIL logs as artifacts.
8. Mark job failed on any non-zero step.

## Tooling Choices (Pragmatic)

1. Parser/Model
- Prefer robust XML parser with strict namespace handling.
- Keep canonical model stable and versioned.

2. Validation
- Structural (schema/XSD) + semantic rules are separate stages.
- Keep rule IDs stable for CI and trend tracking.

3. Generation
- Template-driven (existing Jinja pattern already used in onboard tooling).
- Deterministic output ordering to avoid noisy diffs.

4. SIL
- Reuse `test/sil/run_sil.sh` and `verdict_checker.py` directly.
- Generate only what is needed to get first executable feedback.

## Risks and Mitigations

1. ARXML scope explosion
- Mitigation: freeze MVP subset; reject unsupported constructs explicitly.

2. Round-trip fidelity issues
- Mitigation: preserve source references/path metadata in canonical model.

3. False confidence from schema-only checks
- Mitigation: enforce semantic validation gate as mandatory.

4. Generated-code drift
- Mitigation: CI drift check and generated-file headers.

5. SIL pass but real ECU integration fails
- Mitigation: add staged integration tests and timing budget checks after smoke.

## Backlog (First 10 Tasks)

1. Write `docs/private/arxml-supported-subset.md`.
2. Add ARXML fixture set (`valid/invalid`) under `tools/onboard/tests/arxml-fixtures/`.
3. Implement `onboard arxml validate` command.
4. Implement canonical model serializer.
5. Implement semantic rule engine with rule IDs.
6. Implement manifest bridge generator.
7. Generate sample ECU output from one fixture.
8. Add generated SIL smoke scenario templates.
9. Add CI workflow job for validate/generate/sil-smoke.
10. Add traceability mapping report output.

## Exit Criteria for "Level Up Achieved"

All are required:

1. A new ARXML package can be ingested without manual hand-editing of core config files.
2. Validation errors are deterministic and actionable.
3. Generated ECU path can execute SIL smoke automatically.
4. CI blocks invalid ARXML and stale generated artifacts.
5. Evidence trail exists from ARXML input to SIL verdict output.

## Research Links

- Vector DaVinci validation workflow:
  - https://help.vector.com/davinci-configurator-classic/en/current/dvcfg-classic/6.2-SP0/getting-started/workflow/local/check-configuration.html

- MathWorks ARXML importer + validation + round-trip:
  - https://www.mathworks.com/help/autosar/ug/autosar-arxml-importer-tool.html
  - https://www.mathworks.com/help/autosar/ug/round-trip-preservation-of-autosar-elements-and-uuids.html

- AUTOSAR ECU Extract context:
  - https://www.autosar.org/fileadmin/standards/R19-11/CP/AUTOSAR_TPS_SystemTemplate.pdf
  - https://www.autosar.org/fileadmin/standards/R19-11/CP/AUTOSAR_TR_Methodology.pdf

- dSPACE architecture validation and ARXML import/export context:
  - https://www.dspace.com/en/inc/home/products/systems/system_architecture/sd_casestudy_ar-validation.cfm

- ETAS virtual ECU / SIL / CI-CD positioning:
  - https://www.etas.com/ww/en/products-services/software-development-tools/vecu-builder/
  - https://www.etas.com/en/products/cosym-co-simulation-platform.php

- FMI standard for vECU/FMU interchange:
  - https://fmi-standard.org/
  - https://fmi-standard.org/docs/main/
