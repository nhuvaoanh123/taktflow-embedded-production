---
paths:
  - "firmware/**/*"
  - "hardware/**/*"
---
# ASIL D Constraints

Full reference: `docs/reference/asil-d-reference.md`

**SW (Part 6)**: validate all inputs/sensors/indices. E2E (CRC+sequence+dataID+timeout). Redundant safety vars. Program flow + stack monitoring. External watchdog. No dynamic alloc. No recursion without stack proof. Coverage: 100% statement/branch/MC/DC.

**HW (Part 5)**: SPFM≥99%, LFM≥90%, PMHF<10 FIT. Lockstep CPU or monitor. ECC RAM/flash. BIST. Independent voltage/clock monitoring. 1oo2D or 2oo3 architecture.

**Architecture (Part 4)**: FFI (spatial+temporal+communication). MPU/MMU partitioning. WCET analysis. FDTI+FRTI<FTTI. Safe states verified via fault injection.

**Verification**: formal verification recommended. Fault injection at unit+integration. I3 independence for assessment. Back-to-back testing.

**Decomposition**: D+QM, C+A, or B+B. HW metrics stay at original ASIL. DFA required.
