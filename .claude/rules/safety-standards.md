---
paths:
  - "firmware/**/*"
  - "hardware/**/*"
  - "docs/safety/**/*"
  - "docs/aspice/**/*"
---
# ISO 26262 & Automotive Compliance

**Standards**: ISO 26262:2018 (primary), ASPICE 4.0 (OEM req), ISO/SAE 21434 (cybersecurity), MISRA C:2012/2023.
**ASIL D** = S3 (fatal) + E4 (high exposure) + C3 (uncontrollable). Detailed constraints in `asil-d` section below.
**TODO markers**: `// TODO:ISO`, `// TODO:ASIL`, `// TODO:ASPICE`, `// TODO:MISRA`
Retain all safety docs for vehicle lifetime + 15 years.

# ASPICE 4.0

Target: Level 2 min (OEM req), Level 3 for ASIL D.
**Process areas**: SYS.1-5 (system reqs→arch→integration→verification), SWE.1-6 (SW reqs→arch→design→unit verify→integration→verification), HWE.1-4, SUP.1 (QA), SUP.8 (CM), MAN.3 (project mgmt).
**V-model**: every spec has matching verification: SYS.2↔SYS.5, SYS.3↔SYS.4, SWE.1↔SWE.6, SWE.2↔SWE.5, SWE.3↔SWE.4.
ASPICE = process maturity. ISO 26262 = safety integrity. Both required, complementary.

# Safety Lifecycle

**HARA**: item def → hazard ID → S+E+C → ASIL → safety goals + safe states + FTTI.
**Flow**: SG → FSR → TSR → HSR + SSR. Bidirectional traceability at each level.
**Safety case**: claims + argument (GSN/CAE) + evidence. Preliminary → interim → final.
**DFA**: CCF + cascading failures. Identify coupling → mitigate → document.

# Traceability (ISO 26262 + ASPICE)

Bidirectional across V-model: SG ↔ FSR ↔ TSR ↔ SSR/HSR ↔ code ↔ unit tests ↔ integration ↔ system tests.
**Annotations**: `@safety_req`, `@traces_to`, `@verified_by` (source); `@test_case`, `@verifies`, `@method`, `@coverage` (tests).
**ASIL D**: complete (no orphans), verified (independent review), maintained (updated every change). Every matrix row complete for release.
