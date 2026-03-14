# Contributing Guide

---

## Before You Start

1. Read [SETUP.md](SETUP.md) — environment setup
2. Read [BUILD.md](BUILD.md) — how to build
3. Read [TEST.md](TEST.md) — how to test
4. Read [TOOLCHAIN.md](TOOLCHAIN.md) — exact tool versions
5. Read [docs/LEARNING-JOURNEY.md](docs/LEARNING-JOURNEY.md) — understand the system

---

## Workflow

```
fork → branch → change → test → PR → review → merge
```

### 1. Branch Naming

Follow the project naming convention:

| Type | Pattern | Example |
|------|---------|---------|
| Feature | `feat/<short-description>` | `feat/cvc-torque-limiter` |
| Bug fix | `fix/<short-description>` | `fix/rzc-heartbeat-overtransmit` |
| Hotfix | `hotfix/<short-description>` | `hotfix/can-buffer-overflow` |
| Documentation | `docs/<short-description>` | `docs/sil-scenario-authoring` |

### 2. Make Your Change

**Know what you can and cannot edit:**

| Path | Rule |
|------|------|
| `firmware/ecu/*/src/` | ✅ Edit freely |
| `firmware/ecu/*/include/` | ✅ Edit freely |
| `firmware/bsw/*/src/` | ✅ Edit freely |
| `test/sil/scenarios/` | ✅ Edit freely |
| `docs/` | ✅ Edit freely |
| `generated/` | ❌ Never hand-edit — run codegen |
| `firmware/ecu/*/cfg/` | ❌ Never hand-edit — regenerate via tools |
| `arxml/TaktflowSystem.arxml` | ⚠️ Change with care — runs through full codegen pipeline |

### 3. Pre-Commit Checklist

Before pushing, run locally:

```bash
# 1. Build
make -f firmware/platform/posix/Makefile.posix TARGET=<your_ecu>

# 2. Unit tests — must be 0 failures
make -f firmware/platform/posix/Makefile.posix TARGET=<your_ecu> test

# 3. MISRA — must be 0 violations
make -f firmware/platform/posix/Makefile.posix misra

# 4. Coverage — must meet thresholds
make -f firmware/platform/posix/Makefile.posix TARGET=<your_ecu> coverage

# 5. If ARXML or DBC changed: regenerate and commit generated files
python tools/arxml/codegen.py --arxml arxml/TaktflowSystem.arxml --config project.yaml --output generated/
git add generated/

# 6. SIL — run affected scenarios
./test/sil/run_sil.sh --scenario=<relevant_scenario>
```

### 4. Commit Messages

Follow Conventional Commits:

```
<type>(<scope>): <short description>

<body — optional, explain WHY not WHAT>
```

| Type | When |
|------|------|
| `feat` | New feature or capability |
| `fix` | Bug fix |
| `docs` | Documentation only |
| `test` | Adding or fixing tests |
| `refactor` | Code restructure, no behaviour change |
| `chore` | Build scripts, toolchain, CI |
| `safety` | Safety-critical change — always requires body explaining rationale |

Examples:
```
feat(cvc): add torque limiter with ASIL-B monitoring

fix(rzc): heartbeat period corrected from 50ms to 100ms

safety(sc): add dual-channel pedal validation

Rationale: single-channel reading fails ASIL-D SPFM target.
Adds independent ADC read on SC and CVC; cross-checks within 5ms.
Verified against TSR-014.
```

### 5. Pull Request

PR title = commit message format: `type(scope): description`

PR body must include:

```markdown
## What
<!-- What does this change do? -->

## Why
<!-- Why is this change needed? Link to requirement or issue. -->

## Safety Impact
<!-- Does this change affect safety-critical paths? If yes: which SSRs are affected? -->
<!-- If no safety impact: state "No safety-critical paths affected" -->

## Test Evidence
<!-- Which unit tests cover this? Which SIL scenarios pass? -->
<!-- Paste test output or link to CI run. -->

## Checklist
- [ ] Unit tests pass (0 failures)
- [ ] MISRA clean (0 violations or deviation permit added)
- [ ] Coverage thresholds met
- [ ] SIL scenarios pass for affected ECUs
- [ ] Generated files committed (if ARXML changed)
- [ ] Traceability updated (if requirement or test added)
```

### 6. Code Review

All PRs require at least one reviewer.

**ASIL-D code** (CVC, RZC, SC) requires an **independent reviewer** — someone who did not write the changed code. This is an ISO 26262 Part 6 requirement, not a preference.

Reviewers use the checklist in [docs/reference/lessons-learned/process/PROCESS-hitl-review-methodology.md](docs/reference/lessons-learned/process/PROCESS-hitl-review-methodology.md).

---

## Code Standards

### C Code (Firmware)

- MISRA-C:2012 — mandatory. Zero violations. Deviations require permit in [docs/safety/analysis/misra-deviation-register.md](docs/safety/analysis/misra-deviation-register.md).
- Fixed-width integer types only (`uint8_t`, `int32_t`, etc.) — no `int`, `unsigned`, `long`
- No dynamic memory allocation (`malloc`, `calloc`) — ever
- No recursion
- Every function return value checked (MISRA Rule 17.7)
- No `goto` (MISRA Rule 15.1)
- All switch statements have `default` (MISRA Rule 16.4)
- Naming: functions=`camelCase`, constants=`UPPER_SNAKE`, types=`PascalCase_t`

### Python (Test Harnesses / Tooling)

- Type hints on all public functions
- `ruff` for linting: `ruff check tools/ test/`
- Docstrings on all public functions

### Documentation

- Markdown for all docs
- Date + context on new entries (see [docs/.claude/rules/lessons-learned.md](../.claude/rules/lessons-learned.md))
- Link new docs from the relevant `INDEX.md`

---

## Adding a New SIL Scenario

1. Create `test/sil/scenarios/sil_0XX_<name>.yaml` (copy an existing scenario as template)
2. Define: `preconditions`, `stimulus`, `expected`, `timeout_s`
3. Add to `test/sil/run_sil.sh` scenario list
4. Run locally: `./test/sil/run_sil.sh --scenario=sil_0XX_<name>`
5. Commit scenario file + updated script

---

## Adding a New Lesson Learned

After fixing a significant bug or completing a difficult task:

1. Create a new file in the appropriate category:
   ```
   docs/reference/lessons-learned/
     hardware/     -- PCB, toolchain, hardware bring-up
     infrastructure/ -- CAN, AUTOSAR, BSW, Docker
     process/      -- Architecture decisions, team process
     safety/       -- ISO 26262, ASIL, safety analysis
     security/     -- Security hardening
     testing/      -- CI, SIL, unit tests
   ```

2. Use the format:
   ```markdown
   # PROCESS: <Title>

   **Date:** YYYY-MM-DD
   **Context:** <What you were trying to do>
   **Mistake / Observation:** <What went wrong or what you learned>
   **Fix / Solution:** <How it was resolved>
   **Principle:** <The rule to remember>
   ```

3. Add a link from [docs/reference/lessons-learned/reference-lessons-learned-overview.md](docs/reference/lessons-learned/reference-lessons-learned-overview.md)

---

## Getting Help

- Read the relevant lesson-learned file first — most problems have been encountered before
- Check `.github/workflows/` — the CI pipeline is the authoritative source for correct commands
- Open a GitHub issue with label `question`
