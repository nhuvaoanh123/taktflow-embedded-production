# Learning Journey — Automotive Embedded Stack

**Audience:** Automotive engineering students, junior embedded engineers, or anyone new to functional-safety embedded systems development.

**Goal:** Understand the full stack — from physical hardware and CAN bus, through AUTOSAR BSW, safety architecture, simulation, and CI/CD pipelines — by reading real project documentation in a structured order.

---

## How to Use This Guide

Read the stages in order. Each stage builds on the previous. Links point to documents in this repository. At the end of each stage you should be able to answer the **checkpoint questions**.

---

## Stage 1 — The System: What Are We Building?

Understand the vehicle system, the hardware, and why this project exists before touching any code.

| Step | Document | What You Learn |
|------|----------|---------------|
| 1.1 | [docs/aspice/sys/system-architecture.md](aspice/sys/system-architecture.md) | Vehicle system decomposition, all ECUs, their roles |
| 1.2 | [docs/aspice/sys/can-message-matrix.md](aspice/sys/can-message-matrix.md) | Every CAN signal, who sends, who receives |
| 1.3 | [docs/aspice/sys/interface-control-doc.md](aspice/sys/interface-control-doc.md) | ECU-to-ECU interfaces and contracts |
| 1.4 | [docs/hardware/taktflow-embedded-hardware-overview.md](hardware/taktflow-embedded-hardware-overview.md) | Physical boards, MCU selection |
| 1.5 | [docs/hardware/bom.md](hardware/bom.md) | Bill of materials — what chips and boards are used |
| 1.6 | [docs/hardware/schematics/01-system-block-diagram.md](hardware/schematics/01-system-block-diagram.md) | Top-level hardware block diagram |
| 1.7 | [docs/hardware/schematics/03-can-bus.md](hardware/schematics/03-can-bus.md) | CAN bus topology |
| 1.8 | [docs/hardware/schematics/04-safety-chain.md](hardware/schematics/04-safety-chain.md) | Safety chain circuit — how e-stop works physically |

**Checkpoint:** Can you name all ECUs, what each controls, and how they communicate?

---

## Stage 2 — Functional Safety: Why It Matters

Understand ISO 26262 fundamentals through real project safety documents.

| Step | Document | What You Learn |
|------|----------|---------------|
| 2.1 | [docs/safety/concept/item-definition.md](safety/concept/item-definition.md) | What the "item" is under ISO 26262 |
| 2.2 | [docs/safety/concept/hara.md](safety/concept/hara.md) | Hazard Analysis and Risk Assessment — ASIL assignment |
| 2.3 | [docs/safety/concept/safety-goals.md](safety/concept/safety-goals.md) | Top-level safety goals derived from HARA |
| 2.4 | [docs/safety/concept/functional-safety-concept.md](safety/concept/functional-safety-concept.md) | How safety goals flow into functional requirements |
| 2.5 | [docs/safety/analysis/fmea.md](safety/analysis/fmea.md) | Failure Mode and Effects Analysis |
| 2.6 | [docs/safety/analysis/asil-decomposition.md](safety/analysis/asil-decomposition.md) | How ASIL-D is decomposed across the system |
| 2.7 | [docs/reference/lessons-learned/safety/SYS-001-dual-pedal-sensing.md](reference/lessons-learned/safety/SYS-001-dual-pedal-sensing.md) | Real case study: dual pedal sensing safety design |
| 2.8 | [docs/safety/plan/safety-plan.md](safety/plan/safety-plan.md) | How safety activities are planned and tracked |

**Checkpoint:** What is an ASIL rating? What safety goal covers the brake system? What does FMEA tell you that HARA doesn't?

---

## Stage 3 — Software Requirements and Architecture

How system requirements flow into software requirements and then architecture.

| Step | Document | What You Learn |
|------|----------|---------------|
| 3.1 | [docs/aspice/sys/stakeholder-requirements.md](aspice/sys/stakeholder-requirements.md) | What the customer actually wants |
| 3.2 | [docs/aspice/sys/system-requirements.md](aspice/sys/system-requirements.md) | System-level requirements (SYS.2) |
| 3.3 | [docs/aspice/swr/SWR-ALL.md](aspice/swr/SWR-ALL.md) | All software requirements — the full SWR set |
| 3.4 | [docs/aspice/swa/sw-architecture.md](aspice/swa/sw-architecture.md) | Software architecture — component decomposition |
| 3.5 | [docs/aspice/swa/bsw-architecture.md](aspice/swa/bsw-architecture.md) | AUTOSAR BSW layering — how BSW, RTE, SWC fit together |
| 3.6 | [docs/aspice/swa/vecu-architecture.md](aspice/swa/vecu-architecture.md) | Virtual ECU architecture for SIL simulation |
| 3.7 | [docs/aspice/traceability/traceability-matrix.md](aspice/traceability/traceability-matrix.md) | Requirement → component → test traceability |

**Checkpoint:** Can you trace a stakeholder requirement all the way to a software component and a test case?

---

## Stage 4 — The Hardware Bring-Up Journey (Learn From Real Mistakes)

Read how the hardware was actually brought up — includes real debugging war stories.

| Step | Document | What You Learn |
|------|----------|---------------|
| 4.1 | [docs/hardware/pin-mapping.md](hardware/pin-mapping.md) | GPIO, CAN, UART pin assignments per MCU |
| 4.2 | [docs/hardware/wiring-log.md](hardware/wiring-log.md) | Physical wiring decisions and changes |
| 4.3 | [docs/reference/lessons-learned/hardware/PROCESS-stm32-cubemx-bringup.md](reference/lessons-learned/hardware/PROCESS-stm32-cubemx-bringup.md) | STM32 CubeMX bring-up step by step |
| 4.4 | [docs/reference/lessons-learned/hardware/PROCESS-stm32-uart-debug-bringup.md](reference/lessons-learned/hardware/PROCESS-stm32-uart-debug-bringup.md) | Getting UART debug output working |
| 4.5 | [docs/reference/lessons-learned/hardware/PROCESS-halcogen-tms570-gui-setup.md](reference/lessons-learned/hardware/PROCESS-halcogen-tms570-gui-setup.md) | TMS570 HALcogen setup — safety MCU toolchain |
| 4.6 | [docs/reference/lessons-learned/infrastructure/PROCESS-stm32g4-fdcan-bringup.md](reference/lessons-learned/infrastructure/PROCESS-stm32g4-fdcan-bringup.md) | FDCAN peripheral bring-up on STM32G4 |
| 4.7 | [docs/reference/lessons-learned/infrastructure/PROCESS-tms570-blinky-debugging.md](reference/lessons-learned/infrastructure/PROCESS-tms570-blinky-debugging.md) | First firmware on TMS570 — debugging approach |
| 4.8 | [docs/reference/lessons-learned/infrastructure/PROCESS-can-bus-bring-up.md](reference/lessons-learned/infrastructure/PROCESS-can-bus-bring-up.md) | Getting two nodes talking on CAN |

**Checkpoint:** What are the first things to verify when CAN traffic is not appearing? What does HALcogen generate?

---

## Stage 5 — AUTOSAR BSW and the Communication Stack

How AUTOSAR COM, PDU Router, and the RTE wire everything together.

| Step | Document | What You Learn |
|------|----------|---------------|
| 5.1 | [docs/reference/lessons-learned/infrastructure/PROCESS-com-sendsignal-vs-pdur-transmit.md](reference/lessons-learned/infrastructure/PROCESS-com-sendsignal-vs-pdur-transmit.md) | When to use `Com_SendSignal` vs `PduR_Transmit` |
| 5.2 | [docs/reference/lessons-learned/infrastructure/PROCESS-com-rx-timeout-sensor-feeder.md](reference/lessons-learned/infrastructure/PROCESS-com-rx-timeout-sensor-feeder.md) | Handling RX timeouts — what happens when a sensor goes silent |
| 5.3 | [docs/reference/lessons-learned/infrastructure/PROCESS-stale-can-shadow-buffers.md](reference/lessons-learned/infrastructure/PROCESS-stale-can-shadow-buffers.md) | Why stale data in shadow buffers causes subtle faults |
| 5.4 | [docs/reference/lessons-learned/infrastructure/PROCESS-fdcan-tx-fifo-overflow.md](reference/lessons-learned/infrastructure/PROCESS-fdcan-tx-fifo-overflow.md) | TX FIFO overflow under load — detection and fix |
| 5.5 | [docs/reference/lessons-learned/infrastructure/PROCESS-can-silence-detection.md](reference/lessons-learned/infrastructure/PROCESS-can-silence-detection.md) | Detecting CAN bus silence — watchdog pattern |
| 5.6 | [docs/reference/lessons-learned/infrastructure/PROCESS-heartbeat-hardening.md](reference/lessons-learned/infrastructure/PROCESS-heartbeat-hardening.md) | Heartbeat monitoring — the alive counter pattern |
| 5.7 | [docs/reference/lessons-learned/infrastructure/PROCESS-rzc-heartbeat-overtransmit.md](reference/lessons-learned/infrastructure/PROCESS-rzc-heartbeat-overtransmit.md) | What happens when a node transmits heartbeat too fast |

**Checkpoint:** What is a PDU? What is the difference between a signal and a PDU? What triggers a COM RX timeout?

---

## Stage 6 — Safety-Critical Logic and E-Stop

How safety-critical code is written and what happens when things go wrong.

| Step | Document | What You Learn |
|------|----------|---------------|
| 6.1 | [docs/safety/requirements/sw-safety-reqs.md](safety/requirements/sw-safety-reqs.md) | Software safety requirements — ASIL-D constraints |
| 6.2 | [docs/safety/requirements/technical-safety-reqs.md](safety/requirements/technical-safety-reqs.md) | Technical safety requirements — implementation rules |
| 6.3 | [docs/reference/lessons-learned/safety/PROCESS-sc-safety-critical-dataflow-state-transition-investigation.md](reference/lessons-learned/safety/PROCESS-sc-safety-critical-dataflow-state-transition-investigation.md) | Deep dive: SC safety-critical data flow investigation |
| 6.4 | [docs/reference/lessons-learned/infrastructure/PROCESS-estop-recovery-startup-sequencing.md](reference/lessons-learned/infrastructure/PROCESS-estop-recovery-startup-sequencing.md) | E-stop recovery — startup sequencing after a fault |
| 6.5 | [docs/reference/lessons-learned/infrastructure/PROCESS-fault-injection-demo.md](reference/lessons-learned/infrastructure/PROCESS-fault-injection-demo.md) | How fault injection works in simulation |
| 6.6 | [docs/reference/lessons-learned/safety/PROCESS-hil-gap-analysis.md](reference/lessons-learned/safety/PROCESS-hil-gap-analysis.md) | What the HIL bench cannot cover and why |
| 6.7 | [docs/safety/analysis/dfa.md](safety/analysis/dfa.md) | Dependent Failure Analysis — common cause failures |

**Checkpoint:** What is a safe state? What sequence must an ECU follow after detecting a fault? What is dependent failure analysis?

---

## Stage 7 — Testing: Unit, SIL, MIL, PIL, HIL

The full test pyramid — from isolated unit tests to hardware-in-the-loop.

| Step | Document | What You Learn |
|------|----------|---------------|
| 7.1 | [docs/reference/lessons-learned/testing/PROCESS-bsw-tdd-development.md](reference/lessons-learned/testing/PROCESS-bsw-tdd-development.md) | TDD for BSW — how to write tests before the code |
| 7.2 | [test/mil/test-mil-overview.md](../test/mil/test-mil-overview.md) | Model-in-the-Loop — testing logic in Simulink/Python models |
| 7.3 | [test/sil/test-sil-overview.md](../test/sil/test-sil-overview.md) | Software-in-the-Loop — firmware compiled for POSIX, tested in Docker |
| 7.4 | [test/pil/test-pil-overview.md](../test/pil/test-pil-overview.md) | Processor-in-the-Loop — firmware on real MCU, stimuli from PC |
| 7.5 | [docs/reference/lessons-learned/infrastructure/PROCESS-sil-demo-integration.md](reference/lessons-learned/infrastructure/PROCESS-sil-demo-integration.md) | How the SIL environment was integrated end-to-end |
| 7.6 | [docs/reference/lessons-learned/infrastructure/PROCESS-bsw-hardening-sil-timing.md](reference/lessons-learned/infrastructure/PROCESS-bsw-hardening-sil-timing.md) | Timing issues discovered in SIL that were invisible in unit tests |
| 7.7 | [docs/reference/lessons-learned/safety/PROCESS-sil-hil-divergence-preflight.md](reference/lessons-learned/safety/PROCESS-sil-hil-divergence-preflight.md) | When SIL and HIL disagree — root causes and fixes |
| 7.8 | [docs/reference/lessons-learned/testing/PROCESS-sil-nightly-ci.md](reference/lessons-learned/testing/PROCESS-sil-nightly-ci.md) | Running SIL in CI — nightly regression pipeline |
| 7.9 | [docs/reference/lessons-learned/testing/PROCESS-ci-test-hardening.md](reference/lessons-learned/testing/PROCESS-ci-test-hardening.md) | Making CI tests reliable and flake-free |

**Checkpoint:** What is the difference between SIL and PIL? When does a bug only appear in HIL and not in SIL?

---

## Stage 8 — MISRA-C and Code Quality

Why MISRA-C exists and how compliance is enforced in CI.

| Step | Document | What You Learn |
|------|----------|---------------|
| 8.1 | [docs/safety/analysis/misra-deviation-register.md](safety/analysis/misra-deviation-register.md) | Which MISRA rules are deviated from and why |
| 8.2 | [docs/reference/lessons-learned/infrastructure/PROCESS-misra-pipeline.md](reference/lessons-learned/infrastructure/PROCESS-misra-pipeline.md) | Setting up MISRA checking in CI — toolchain and config |
| 8.3 | [docs/aspice/sqa/qa-plan.md](aspice/sqa/qa-plan.md) | Full quality assurance plan — metrics, gates, reviews |

**Checkpoint:** What is MISRA Rule 14.4? What does a deviation permit? What blocks a merge in this project's CI?

---

## Stage 9 — ARXML and Code Generation

How the system model (ARXML) drives code generation — the modern automotive workflow.

| Step | Document | What You Learn |
|------|----------|---------------|
| 9.1 | [docs/arxmlgen/user-guide.md](arxmlgen/user-guide.md) | Quick start — what arxmlgen does and how to run it |
| 9.2 | [docs/arxmlgen/architecture.md](arxmlgen/architecture.md) | How the generator is designed: config loader, data model, template engine |
| 9.3 | [docs/plans/plan-arxml-to-sil-feasibility-and-failure-modes.md](plans/plan-arxml-to-sil-feasibility-and-failure-modes.md) | What can go wrong when going ARXML → SIL |
| 9.4 | [docs/plans/plan-arxml-to-sil-pro-workflow.md](plans/plan-arxml-to-sil-pro-workflow.md) | Professional ARXML-to-SIL workflow — the full pipeline |
| 9.5 | [docs/lessons-learned/arxmlgen.md](../lessons-learned/arxmlgen.md) | Real bug: E2E data ID propagation issue in cross-ECU signals |

**Checkpoint:** What is an ARXML file? What does arxmlgen generate from it? What is a cross-ECU E2E data ID conflict?

---

## Stage 10 — Process, Traceability, and ASPICE

How the engineering process is managed — from requirements to audit evidence.

| Step | Document | What You Learn |
|------|----------|---------------|
| 10.1 | [docs/reference/process-playbook.md](reference/process-playbook.md) | Full development process playbook |
| 10.2 | [docs/guides/traceability-guide.md](guides/traceability-guide.md) | How traceability is built and maintained |
| 10.3 | [docs/reference/lessons-learned/process/PROCESS-traceability-automation.md](reference/lessons-learned/process/PROCESS-traceability-automation.md) | Automating traceability in CI |
| 10.4 | [docs/reference/lessons-learned/process/PROCESS-architecture-decisions.md](reference/lessons-learned/process/PROCESS-architecture-decisions.md) | How architectural decisions are recorded (ADRs) |
| 10.5 | [docs/reference/lessons-learned/process/PROCESS-commercial-sil-vs-custom-sil-fidelity.md](reference/lessons-learned/process/PROCESS-commercial-sil-vs-custom-sil-fidelity.md) | When to use commercial SIL tools vs. building your own |
| 10.6 | [docs/reference/lessons-learned/process/PROCESS-cross-document-consistency.md](reference/lessons-learned/process/PROCESS-cross-document-consistency.md) | Keeping 240+ documents consistent across a project |

**Checkpoint:** What is ASPICE? What is the difference between a verification and a validation? What makes a traceability link "broken"?

---

## Quick Reference

| Topic | Jump to |
|-------|---------|
| All safety documents | [docs/safety/INDEX.md](safety/INDEX.md) |
| All ASPICE documents | [docs/aspice/](aspice/) |
| All lessons learned | [docs/reference/lessons-learned/](reference/lessons-learned/) |
| ARXML codegen | [docs/arxmlgen/](arxmlgen/) |
| Hardware schematics | [docs/hardware/schematics/](hardware/schematics/) |
| Test overviews | [test/](../test/) |
| Master project plan | [docs/plans/master-plan.md](plans/master-plan.md) |
| Safety reference (ASIL-D) | [docs/reference/asil-d-reference.md](reference/asil-d-reference.md) |
