---
name: security-review
description: "Audit firmware code for security vulnerabilities, credential leaks, and unsafe patterns"
argument-hint: "[file or directory path]"
user-invocable: true
allowed-tools: "Read, Grep, Glob, Bash(cppcheck *)"
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


# Security Review

Perform a security audit on the specified files or directories. If no argument given, audit `firmware/src/`.

## Checklist

### 1. Banned Functions
Search for unsafe C functions that are prohibited by project rules:
- `gets()`, `strcpy()`, `sprintf()`, `strcat()`, `atoi()`, `system()`
- Report each occurrence with file and line number

### 2. Hardcoded Credentials
Search for patterns indicating hardcoded secrets:
- String literals assigned to variables named password, secret, key, token, api_key
- Base64-encoded strings in source code
- URLs containing credentials

### 3. Input Validation
For each external interface handler (serial, MQTT, BLE, HTTP):
- Verify input length is checked before processing
- Verify buffer bounds are validated
- Verify data is validated against schema/whitelist before use

### 4. Memory Safety
Check for:
- Unchecked malloc/calloc return values
- Missing null pointer checks before dereference
- Buffer operations without size limits
- Use-after-free patterns

### 5. Cryptography
Verify:
- TLS is used for all network communication (no plaintext)
- Certificate verification is not disabled
- OTA update signatures are verified
- Appropriate key sizes and algorithms

### 6. Error Handling
Check that:
- All function return values are checked
- Errors propagate up (not silently swallowed)
- No sensitive data in error messages

## Output Format

Report findings as:
```
[SEVERITY] file:line — description
  Recommendation: how to fix
```

Severity levels: CRITICAL, HIGH, MEDIUM, LOW, INFO

