---
paths:
  - "firmware/**/*"
  - "scripts/**/*"
---
# Security

**Fail-closed**: missing config/creds/validation → REJECT. Unknown state → safe state.
**Creds**: never hardcode/commit. Secure storage. Per-device unique. Rotate on compromise.
**Crypto**: established libs only. TLS 1.2+. ECC 256+. AEAD ciphers. HW RNG. Never disable cert verify.
**Boot**: signature verify, root-of-trust in HW, lock bootloader. Disable JTAG/debug in production.
**Supply chain**: pin versions, verify checksums, audit CVEs, minimize third-party.

# Testing

TDD: `test_<module>.c` (Unity) BEFORE impl, `@verifies` required, hook-enforced.
ASIL D: 100% stmt/branch/MC/DC. Mock at HAL boundary. Fuzz parsers w/ ASan+UBSan.
Bug fix → regression test. Never delete.

# Tool Qualification (ISO 26262-8)

TCL = TI × TD. TCL1: none. TCL2-3: qualify. Pin versions; upgrades re-evaluate.
Compiler/codegen=TCL3, analysis/coverage=TCL2, git/editor=TCL1.
