---
name: firmware-build
description: "Build, test, and validate firmware. Runs build, tests, static analysis, and size check."
argument-hint: "[debug|release|test|all]"
user-invocable: true
allowed-tools: "Read, Grep, Glob, Bash(make *)"
---

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


# Firmware Build

Build and validate firmware based on argument:
- `debug` — build debug firmware
- `release` — build release firmware
- `test` — run unit tests only
- `all` — full validation (build + test + analysis)
- No argument — defaults to `all`

## Steps for `all` (full validation)

### 1. Clean Build
```bash
make clean
make build-release
```
Report: binary size, build warnings (should be zero)

### 2. Unit Tests
```bash
make test
```
Report: tests passed/failed, coverage percentage

### 3. Static Analysis (if cppcheck available)
```bash
cppcheck --enable=all --error-exitcode=1 firmware/src/
```
Report: issues found by severity

### 4. Size Report
Report firmware binary size breakdown:
- .text (code)
- .data (initialized data)
- .bss (uninitialized data)
- .rodata (read-only data)
- Total vs partition budget

### 5. Security Quick-Check
Grep for banned patterns:
- `gets(`, `strcpy(`, `sprintf(`, `strcat(`, `system(`
- Hardcoded credentials
Report any findings

## Output Summary

```
BUILD:    [PASS/FAIL] — size: XXX bytes (XX% of budget)
TESTS:    [PASS/FAIL] — XX/XX passed, XX% coverage
ANALYSIS: [PASS/FAIL] — XX issues (XX errors, XX warnings)
SECURITY: [PASS/FAIL] — XX banned patterns, XX credential leaks
```

