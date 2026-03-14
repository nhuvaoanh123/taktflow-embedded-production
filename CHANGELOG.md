# Changelog

All notable changes to this project are documented here.
Format follows [Keep a Changelog](https://keepachangelog.com/en/1.0.0/).
Version numbering follows [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

---

## [Unreleased]

### Added
- `docs/LEARNING-JOURNEY.md` — 10-stage guided reading path for automotive students
- `docs/CURRICULUM.md` — 2-year automotive embedded curriculum with exercises
- `docs/hardware/` — Hardware docs: BOM, pin-mapping, wiring-log, schematics
- `test/mil|pil|sil/` — Test overview documents for all simulation layers
- `docs/plans/plan-arxml-to-sil-feasibility-and-failure-modes.md`
- `docs/plans/plan-arxml-to-sil-pro-workflow.md`
- `SETUP.md`, `BUILD.md`, `TEST.md`, `TOOLCHAIN.md`, `CONTRIBUTING.md` — Developer onboarding

---

## [1.0.0] — 2026-03-10

### Added
- arxmlgen code generation pipeline (Phase 1: config loader, ARXML reader, data model, template engine, CLI)
- `docs/arxmlgen/` — User guide, architecture, API reference, test report
- `docs/lessons-learned/arxmlgen.md` — Cross-ECU E2E data ID propagation issue
- `docs/reference/threadx-local-reference-map.md` — ThreadX reference archive
- `docs/safety/analysis/os-fmea.md` — OS Stack FMEA (ISO 26262 Part 6/9)
- `docs/plans/plan-os-threadx-bootstrap.md` — ThreadX OSEK bootstrap plan
- Production repo migration complete — all ASPICE, safety, and reference docs migrated from development repo
- Full AUTOSAR BSW stack (7 ECUs: BCM, CVC, FZC, GW, ICU, RZC, SC)
- SIL environment: Docker Compose, 18 automated scenarios, JUnit XML output
- CI pipelines: push-triggered unit + SIL, nightly endurance, HIL preflight nightly
- MISRA-C pipeline with zero-violation gate
- Traceability automation: requirement → test → verdict in CI

### Architecture
- 7-ECU vehicle subsystem (BCM, CVC, FZC, GW, ICU, RZC, SC)
- ASIL-D: CVC (motor controller), RZC (redundancy), SC (safety controller)
- ARXML-driven code generation for RTE stubs and COM configuration
- POSIX build target for all ECUs (SIL without hardware)

---

## Version Numbering Policy

| Change | Version Bump | Example |
|--------|-------------|---------|
| Breaking API or protocol change | MAJOR | `2.0.0` |
| New ECU, new safety feature, new CI layer | MINOR | `1.1.0` |
| Bug fix, doc update, toolchain update | PATCH | `1.0.1` |

Breaking changes must include a migration note explaining what callers/integrators must update.
