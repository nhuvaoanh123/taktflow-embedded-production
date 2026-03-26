## Human-in-the-Loop (HITL) Comment Lock

`HITL` means human-reviewer-owned comment content.

**Marker standard (code-friendly):**
- Markdown: `<!-- HITL-LOCK START:<id> -->` ... `<!-- HITL-LOCK END:<id> -->`
- C/C++/Java/JS/TS: `// HITL-LOCK START:<id>` ... `// HITL-LOCK END:<id>`
- Python/Shell/YAML/TOML: `# HITL-LOCK START:<id>` ... `# HITL-LOCK END:<id>`

**Rules:**
- AI must never edit, reformat, move, or delete text inside any `HITL-LOCK` block.
- Append-only: AI may add new comments/changes only; prior HITL comments stay unchanged.
- If a locked comment needs revision, add a new note outside the lock or ask the human reviewer to unlock it.

# Deliverable Artifact Catalog (Automotive ALM) + How To Build Each

Last updated: 2026-02-21
Scope: ASPICE + ISO 26262 + ISO/SAE 21434 + UNECE R155/R156 style projects.

Format:
- `Artifact`: deliverable name
- `How to create`: practical method, minimum content, and acceptance check

## 1. Program and Project Management Artifacts

- `Project charter`: Define business case, scope, objectives, constraints, governance, and success criteria; approve by sponsor and engineering lead.
- `Scope statement`: List in-scope/out-of-scope items, interfaces, variants, and exclusions; baseline and control changes through CR process.
- `Assumptions and constraints log`: Record technical, schedule, supplier, and compliance assumptions with owner/date and validation trigger.
- `Integrated project plan`: Build WBS with dependencies, milestones, resources, budget, and gates; map tasks to process areas and owners.
- `Milestone plan`: Set gate dates, entry/exit criteria, review participants, and required evidence for each gate.
- `Release plan`: Define release train, branch strategy, freeze windows, qualification activities, and artifact bundle needed for each release.
- `Resource and staffing plan`: Map required skills to roles and FTE loading; include backup and key-person risk mitigation.
- `Budget and cost baseline`: Estimate labor/tooling/hardware/cloud/compliance costs; approve baseline and track variance monthly.
- `Communication plan`: Define meeting cadence, decision forums, escalation path, and stakeholder reporting format.
- `Risk register`: Capture risk statement, probability/impact, trigger, mitigation, contingency, and owner; review weekly.
- `Issue log`: Track active blockers with severity, due date, owner, and resolution evidence.
- `Decision log`: Record architectural/process decisions with context, alternatives, rationale, and impacts.
- `Governance and review calendar`: Schedule design/safety/cyber/test reviews with mandatory attendees and exit criteria.
- `Lessons learned log`: Capture what worked/failed, root cause, and action for next increment.

## 2. Requirements Management Artifacts

- `Stakeholder requirements specification`: Capture customer/business/regulatory needs as unique IDs with source, rationale, and acceptance criteria.
- `System requirements specification`: Derive measurable system-level requirements with units, bounds, and verification method.
- `Software requirements specification`: Decompose system requirements into software behavior, timing, diagnostics, and error handling constraints.
- `Hardware requirements specification`: Define electrical, thermal, safety, MCU/peripheral, and watchdog requirements with tolerances.
- `Non-functional requirements`: Specify latency, throughput, memory, startup time, availability, safety integrity, and security targets.
- `Interface requirements specification`: Define producer/consumer, signal encoding, timing, handshake, fault behavior, and versioning rules.
- `Requirements attributes model`: Standardize fields (priority, ASIL, status, owner, release, verification link) in ALM tool.
- `Requirements baseline records`: Snapshot approved requirement set per release and keep signed change history.
- `Bidirectional traceability matrix`: Link stakeholder -> system -> HW/SW -> design -> code -> test -> defects; ensure no orphan links.

## 3. System and Software Architecture Artifacts

- `Item definition`: Describe item purpose, boundaries, operating scenarios, external interfaces, assumptions, and misuse conditions.
- `System context diagram`: Draw external actors/systems and data/control flows with trust and safety boundaries.
- `System architecture document`: Define subsystems, responsibilities, allocations, redundancy, and communication topology.
- `E/E architecture`: Describe ECU topology, network domains/zones, gateways, and power/reset/watchdog architecture.
- `ECU allocation matrix`: Map functions/requirements to ECUs and justify allocation decisions and safety independence.
- `Software architecture document`: Define SW components, layers, APIs, scheduling model, and error-handling concepts.
- `Component decomposition and responsibilities`: Break each component into units with explicit contracts and ownership.
- `Interface control document (ICD)`: Freeze interface fields, units, ranges, endianness, timing, invalid values, and backward compatibility policy.
- `State machine specifications`: Define states, transitions, guards, entry/exit actions, and failure transitions.
- `Timing and performance budget`: Allocate end-to-end timing budget by task/hop and keep margin accounting.
- `Resource budget`: Set RAM/flash/CPU/bus load budgets and thresholds for warning/fail.
- `Variant and feature model`: Define feature toggles, compile/runtime variants, and compatibility constraints per product variant.

## 4. Vehicle Network and Diagnostics Artifacts

- `CAN/LIN/Ethernet topology diagram`: Document network nodes, channels, baud rates, gateways, and termination strategy.
- `CAN message matrix`: List IDs, DLC, sender, receivers, cycle/event timing, timeout, and packing definitions.
- `Signal dictionary`: Define each signal type, scaling, unit, range, default, invalid value, and safety classification.
- `E2E protection specification`: Define CRC polynomial, alive counter, data ID, freshness checks, and error reactions.
- `Diagnostic concept`: Define on-board/off-board diagnostics architecture, sessions, fault memory, and service responsibilities.
- `UDS service specification`: Define supported service IDs, subfunctions, preconditions, NRC behavior, and timing (P2/P2*).
- `DTC definition list`: Assign DTC IDs, trigger logic, debounce, status bits, storage/clear policy, and service mapping.
- `OBD PID mapping`: Map OBD modes/PIDs to internal signals including conversion and unsupported behavior.
- `Network timing and bus load analysis`: Calculate utilization and worst-case latency; verify under normal and fault traffic.

## 5. Detailed Design and Implementation Artifacts

- `Detailed software design specs`: For each unit define algorithm, interfaces, data structures, constraints, and test points.
- `Detailed hardware design specs`: Capture schematic/PCB constraints, IO maps, power tree, EMC considerations, and diagnostics hooks.
- `Algorithm design notes`: Provide equations, assumptions, calibration parameters, numerical limits, and failure behavior.
- `Source code repositories`: Use protected branches, review rules, signed commits/tags, and traceable requirement IDs in commits.
- `Coding guidelines and compliance profile`: Tailor coding standard (e.g., MISRA subset), define mandatory/advisory rules and waivers.
- `Code review records`: Store reviewer checklist, defects found, disposition, and closure evidence per merge.
- `Static analysis reports`: Run configured analyzers, triage findings, document suppressions/justifications, and trend counts.
- `Build scripts and toolchain definitions`: Version compiler/linker options, dependency versions, and reproducible build steps.
- `CI/CD pipeline definitions`: Encode build/test/security/quality gates with artifacts published per run.
- `Binary and image manifest`: Record binary hash, compiler version, config set, and linked source revision.
- `Configuration files per ECU/variant`: Maintain parameter sets with controlled versioning and compatibility checks.

## 6. Verification and Validation Artifacts

- `Verification strategy`: Define verification levels, methods (review/analysis/test), environment, and pass criteria.
- `Validation strategy`: Define user/mission-level validation scenarios against intended use and misuse cases.
- `Test plan`: Set scope, resources, schedule, entry/exit criteria, and defect workflow for each test level.
- `Test specifications`: Define objective, setup, preconditions, stimulus, expected results, and tolerances.
- `Test cases`: Write atomic, repeatable cases with unique IDs and requirement references.
- `Test procedures`: Provide executable sequence and operator/script instructions for deterministic runs.
- `Test data sets`: Curate input vectors, boundary/robustness data, and golden references.
- `Unit test reports`: Summarize pass/fail by module, uncovered functions, and defect links.
- `Integration test reports`: Show interface-level behavior, timing results, and interoperability defects.
- `System test reports`: Demonstrate end-to-end scenarios and system-level acceptance evidence.
- `MIL/SIL/PIL/HIL plans and reports`: Define each xIL scope, tooling, scenario matrix, and comparative findings.
- `Coverage report`: Report statement/branch/MC/DC coverage with justification for uncovered code.
- `Static analysis and coding rule compliance report`: Aggregate rule violations, waivers, and residual risk statement.
- `Regression test results`: Track suite history, changed tests, failures, and fixed regressions by build.
- `Defect reports`: Record reproducible steps, severity, root cause, fix version, and verification evidence.
- `Defect trend and closure report`: Analyze inflow/outflow/aging/reopen rates and readiness implications.

## 7. Functional Safety Artifacts (ISO 26262)

- `Functional safety management plan`: Define roles, competence, confirmation measures, and independence requirements.
- `Safety lifecycle plan`: Map activities/work products across concept to decommissioning with gate criteria.
- `HARA`: Identify hazards, operational situations, hazardous events; rate S/E/C and assign ASIL.
- `Safety goals and ASIL assignment`: Formulate top-level safety intent per hazard with unique IDs and rationale.
- `Functional Safety Concept (FSC)`: Define functional safety mechanisms and safe-state strategy at system level.
- `Technical Safety Concept (TSC)`: Allocate technical safety mechanisms to HW/SW architecture elements.
- `Functional safety requirements (FSR)`: Derive verifiable requirements from safety goals/FSC.
- `Technical safety requirements (TSR)`: Decompose FSR into implementation-oriented technical requirements.
- `Hardware safety requirements (HSR)`: Define hardware diagnostics, fault handling, and architectural metrics constraints.
- `Software safety requirements (SSR)`: Define software-side safety behavior, timing, diagnostics, and fallback logic.
- `Safety analyses (FMEA/FTA/DFA/FMeda)`: Analyze single/common/dependent failures and diagnostic coverage.
- `Hardware metrics (SPFM/LFM/PMHF)`: Compute metrics from failure rates and diagnostic assumptions, including confidence level.
- `Safety verification plan and report`: Verify each safety requirement with method, result, and nonconformance handling.
- `Safety validation plan and report`: Validate achieved safety in representative operational scenarios.
- `Tool confidence/qualification evidence`: Assess tool impact/error-detection and provide qualification rationale/tests.
- `Confirmation review and audit reports`: Perform independent reviews/audits and track closure of findings.
- `Functional safety assessment records`: Record assessor conclusions, residual risks, and release recommendation.
- `Safety case`: Build claim-argument-evidence structure proving acceptable residual safety risk.

## 8. Cybersecurity Artifacts (ISO/SAE 21434, UNECE R155)

- `Cybersecurity management plan`: Define governance, responsibilities, lifecycle activities, and compliance checkpoints.
- `Cybersecurity item definition`: Describe item assets, interfaces, operational environment, and attack surfaces.
- `Asset inventory`: Catalog assets (ECUs, keys, data, services) with confidentiality/integrity/availability criticality.
- `Threat analysis and risk assessment (TARA)`: Identify threat scenarios, attack paths, impact/feasibility, and risk treatment.
- `Cybersecurity goals`: Define risk-reduction goals traceable to TARA outcomes.
- `Cybersecurity concept`: Define controls (prevention/detection/response/recovery) and architecture placement.
- `Cybersecurity requirements`: Specify verifiable security requirements for HW/SW/backend interfaces.
- `Security architecture and trust boundaries`: Document roots of trust, key flows, privilege boundaries, and secure boot/update chain.
- `Security verification plan`: Define security test scope, methods, tooling, and acceptance thresholds.
- `Security test reports`: Execute fuzzing/pentest/robustness tests; capture vulnerabilities and exploitability.
- `Vulnerability management process and records`: Run PSIRT flow: intake, triage, CVSS-like scoring, fix, disclosure decision.
- `Incident response plan and logs`: Define incident workflow and keep immutable timeline/evidence records.
- `Post-production monitoring records`: Collect fleet telemetry/threat intel and document decisions/actions.
- `Cybersecurity case/compliance evidence set`: Build argument with evidence for regulatory/type-approval compliance.

## 9. Software Update and OTA Artifacts (UNECE R156, ISO 24089)

- `Software update management policy`: Define governance, approval authority, rollback authority, and audit obligations.
- `SUMS process definition`: Document end-to-end update lifecycle from package creation to fleet verification.
- `Software configuration index per vehicle/variant`: Map every software item/version to applicable variants.
- `Software bill of materials (SBOM)`: Generate component/dependency inventory with versions, licenses, and vulnerability references.
- `Update package specification`: Define payload content, metadata, preconditions, and installation logic.
- `Compatibility and dependency matrix`: Specify required versions/order and blockers for unsafe combinations.
- `Signing and integrity verification evidence`: Record key IDs, signatures, hash algorithms, and verification results.
- `Rollout and rollback strategy`: Define staged rollout, stop conditions, and safe rollback triggers.
- `Update validation and safety impact assessment`: Prove update does not violate safety/security/system constraints.
- `Campaign execution logs`: Track distribution, install attempts, failures, retries, and operator actions.
- `Fleet update status reports`: Report adoption and exception status by region/variant/VIN segment.

## 10. Quality, Supplier, and Production Artifacts

- `Supplier requirements and interface agreements`: Flow down technical/compliance requirements and acceptance criteria.
- `SOW and acceptance criteria`: Define supplier deliverables, objective acceptance tests, and due dates.
- `Supplier status reports`: Track maturity, quality metrics, and action items at agreed cadence.
- `Change request records (internal and supplier)`: Capture requested change, impact, approval decision, and implementation evidence.
- `Problem resolution records (8D/CAPA)`: Record containment, root cause, corrective/preventive actions, and effectiveness checks.
- `APQP/PPAP package (if production scope)`: Prepare planning, process capability, control plans, and submission evidence.
- `Production readiness checklist`: Verify design/process/test/service readiness before SOP or pilot.
- `Manufacturing test and end-of-line test specs`: Define production test limits, traceability, and reject/rework logic.
- `Field issue and warranty analysis records`: Analyze field returns, failure patterns, and corrective action priorities.

## 11. Configuration and Release Management Artifacts

- `Configuration management plan`: Define item identification, baselining, change control, status accounting, and audits.
- `Baseline definition records`: Record exact contents of each approved baseline with immutable identifiers.
- `Versioning scheme definition`: Define semantic/build numbering, branch mapping, and compatibility semantics.
- `Branching and merge policy`: Define branch roles, protection rules, review gates, and merge criteria.
- `Release readiness checklist`: Verify all required artifacts/tests/compliance approvals before release.
- `Release notes`: Summarize features, fixes, known issues, and compatibility impacts.
- `Delivered artifact manifest`: List all delivered binaries/docs/checksums and destination recipients.
- `Reproducible build evidence`: Prove same source+config reproduces identical binary hash or explain variance.
- `Archive and retention records`: Define long-term storage, retrieval index, and retention durations per regulation/contract.

## 12. Operations and Service Artifacts

- `Service diagnostics guide`: Explain DTC interpretation, diagnostic workflow, tools, and repair decision paths.
- `Service and maintenance procedures`: Define periodic checks, software service actions, and safety precautions.
- `Troubleshooting handbook`: Provide symptom -> probable cause -> test -> fix decision trees.
- `Field monitoring dashboard definitions`: Define KPIs/alerts for fleet health, faults, and update performance.
- `Incident and recall playbook`: Define triggers, escalation, legal/compliance communication, and remediation steps.
- `End-of-life and decommissioning plan`: Define support sunset, key revocation, and secure disposal steps.

## 13. Assessment and Compliance Artifacts

- `ASPICE process definitions and tailoring`: Document process model, tailoring decisions, roles, and expected work products.
- `Process deployment evidence`: Show actual project usage: templates, records, reviews, and measured compliance.
- `KPI and measurement reports`: Track planned vs actual schedule/quality/process performance indicators.
- `Internal audit reports`: Document audit scope, findings, nonconformities, and corrective action status.
- `External assessment evidence package`: Collect objective evidence for assessor interviews and rating justification.
- `Compliance matrix (standard clause -> evidence artifact)`: Map each required clause to exact artifact location and status.

## Minimum Mandatory Set (Practical Baseline)

- `Requirements baseline with full traceability`: Approve requirements and prove end-to-end bidirectional links to tests and results.
- `System and software architecture baseline`: Freeze architecture views and interfaces with change control.
- `Implementation baseline with CI/CD and code quality evidence`: Ensure reproducible build + review + static analysis + tracked defects.
- `Verification baseline (plans, cases, results, coverage)`: Provide complete evidence that requirements were verified.
- `Functional safety baseline (HARA through safety case)`: Show complete safety lifecycle chain and residual risk argument.
- `Cybersecurity baseline (TARA through monitoring)`: Show cybersecurity engineering lifecycle and post-production monitoring path.
- `Update/SUMS baseline (package and campaign evidence)`: Prove controlled and safe update process with trace logs.
- `Configuration and release baseline`: Prove exactly what was released and under which approvals.

## Research Basis (used to structure this guide)

- Automotive SPICE PAM 4.0 process/work-product framing (VDA QMC public package page and PAM PDF)
- ISO 26262 parts structure (functional safety lifecycle and supporting processes)
- ISO/SAE 21434 structure (cybersecurity lifecycle)
- UNECE R155 and R156 regulatory intent (cybersecurity and software update management)

Where standards text is not publicly detailed, "How to create" guidance above is an implementation-oriented inference aligned to those frameworks.

Reference links:
- https://vda-qmc.de/en/automotive-spice/
- https://www.vda-qmc.de/fileadmin/redakteur/Publikationen/Automotive_SPICE/AutomotiveSPICE_PAM_4.0_ENG.pdf
- https://www.iso.org/publication/PUB200262.html
- https://www.iso.org/standard/70918.html
- https://unece.org/transport/vehicle-regulations-wp29/standards/addenda-1958-agreement-regulations-141-160

