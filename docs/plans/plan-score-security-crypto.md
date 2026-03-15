# Plan: Eclipse SCORE Security & Cryptography Module

**Status**: PENDING
**Date**: 2026-03-14
**Target**: Eclipse SCORE (`eclipse-score/inc_security_crypto`)
**Hardware**: NUCLEO-L552ZE (STM32L552ZE, Cortex-M33, TrustZone)

---

## Context

Eclipse SCORE is an open-source automotive middleware platform (BMW, Mercedes-Benz, Accenture,
ETAS, Qorix). Their Security & Cryptography module has 40 requirements and an empty repo.

We propose to be the first implementation — a TrustZone-based HSM reference running on
STM32L552ZE with hardware crypto acceleration.

**But their 40 requirements are not enough.** They cover algorithms and basic API design
but miss critical HSM lifecycle, physical security, compliance, and operational requirements
that any serious automotive or defense deployment needs.

This document extends their requirements with what's missing, then defines the architecture.

---

## Gap Analysis: SCORE's 40 Requirements vs Military-Grade HSM

### What SCORE has (40 requirements)

| Category | Count | Coverage |
|----------|-------|----------|
| Symmetric crypto (AES-CBC/GCM/CCM, ChaCha20) | 5 | Algorithms only |
| Asymmetric crypto (ECDSA, ECDH) | 3 | Algorithms only |
| Hashing (SHA-2, SHA-3, BLAKE3) | 3 | Algorithms only |
| Digital signatures | 2 | Create + verify |
| MAC | 1 | Generic |
| KDF | 1 | Generic |
| RNG | 2 | ChaCha20Rng + entropy source |
| Certificate management | 1 | Generic |
| Key management | 4 | Generate, import, store, delete |
| API design | 2 | Flexible API + algorithm naming |
| Non-functional | 14 | Side-channel, HW accel, SW fallback, etc. |
| TLS | 1 | TLS 1.3 |

### What SCORE is MISSING

---

#### A. Key Lifecycle Management (8 missing requirements)

| ID | Title | Description | Reference |
|----|-------|-------------|-----------|
| SEC_KEY_001 | Key state machine | Keys SHALL transition through defined states: GENERATED → ACTIVE → SUSPENDED → DEACTIVATED → COMPROMISED → DESTROYED. State transitions SHALL be atomic and logged. | NIST SP 800-57 Part 1 |
| SEC_KEY_002 | Key rotation policy | The system SHALL support automated key rotation based on configurable time-based or usage-based policies. Rotation SHALL be atomic (no window of no-valid-key). | ISO/SAE 21434 §8.5 |
| SEC_KEY_003 | Key wrapping for export | Keys SHALL only leave the HSM boundary wrapped (encrypted) using an approved key-wrapping algorithm (AES-KW per RFC 3394 or AES-KWP per RFC 5649). | FIPS 140-3 §4.7 |
| SEC_KEY_004 | Key usage constraints | Each key SHALL have immutable usage attributes set at creation: algorithm binding, permitted operations (encrypt-only, sign-only, wrap-only), maximum usage count, expiry time. | PKCS#11 v3.0 |
| SEC_KEY_005 | Key backup and recovery | The system SHALL support secure key backup to an external key custodian using split-knowledge / dual-control procedures. Recovery SHALL require M-of-N authorization. | FIPS 140-3 §4.7.5 |
| SEC_KEY_006 | Key provenance tracking | Every key SHALL have a provenance record: creation method (generated, imported, derived), source identity, timestamp, creating entity. Provenance SHALL be immutable. | Common Criteria FCS_CKM.1 |
| SEC_KEY_007 | Key isolation per domain | Keys SHALL be isolated per security domain (tenant/ECU). A key in domain A SHALL NOT be accessible to operations in domain B, enforced by hardware (TrustZone/MPU). | EVITA HSM spec §4.3 |
| SEC_KEY_008 | Key agreement protocols | The system SHALL support authenticated key agreement (Station-to-Station, SIGMA) beyond raw ECDH. Unauthenticated ECDH SHALL be flagged as insecure. | NIST SP 800-56A Rev3 |

---

#### B. Zeroization & Emergency Key Destruction (4 missing requirements)

| ID | Title | Description | Reference |
|----|-------|-------------|-----------|
| SEC_ZERO_001 | Commanded zeroization | The system SHALL support immediate destruction of all key material (RAM, flash, registers) upon authenticated command. Zeroization SHALL complete within 100ms. | FIPS 140-3 §4.7.6 |
| SEC_ZERO_002 | Triggered zeroization | The system SHALL automatically zeroize upon detection of: tamper events, N consecutive authentication failures, watchdog timeout, or voltage/clock glitch detection. | Common Criteria FCS_CKM.4 |
| SEC_ZERO_003 | Zeroization verification | After zeroization, the system SHALL verify that all key material storage locations read as zero/random. Verification failure SHALL trigger hardware reset. | FIPS 140-3 IG §4.7.6 |
| SEC_ZERO_004 | Partial zeroization | The system SHALL support selective destruction of individual keys or key domains without affecting other domains. | PKCS#11 C_DestroyObject |

---

#### C. Secure Boot & Firmware Integrity (5 missing requirements)

| ID | Title | Description | Reference |
|----|-------|-------------|-----------|
| SEC_BOOT_001 | Immutable root of trust | The HSM SHALL have an immutable first-stage bootloader in ROM or write-protected flash. This root of trust SHALL verify the integrity of all subsequent boot stages. | NIST SP 800-193 |
| SEC_BOOT_002 | Measured boot | Each boot stage SHALL measure (hash) the next stage and extend a measurement register (similar to TPM PCR). Final measurements SHALL be available for remote attestation. | TCG TPM 2.0 |
| SEC_BOOT_003 | Secure firmware update | HSM firmware updates SHALL be: signed (ECDSA-P256 minimum), version-checked (anti-rollback via monotonic counter), applied atomically (A/B or fail-safe). | UNECE WP.29 R156 |
| SEC_BOOT_004 | Rollback prevention | The system SHALL maintain a hardware-backed monotonic counter that increments on each firmware update. Firmware with a version <= current counter SHALL be rejected. | FIPS 140-3 §4.2.2 |
| SEC_BOOT_005 | Boot attestation report | The system SHALL generate a signed boot attestation report containing: firmware version, boot measurements, security state, tamper status. This report SHALL be available to the host. | ISO/SAE 21434 §15 |

---

#### D. Physical Attack Resistance (5 missing requirements)

| ID | Title | Description | Reference |
|----|-------|-------------|-----------|
| SEC_PHYS_001 | Fault injection detection | The system SHALL detect voltage glitching, clock glitching, and electromagnetic fault injection through redundant computation and sensor monitoring. | Common Criteria AVA_VAN.5 |
| SEC_PHYS_002 | Active shield monitoring | If available, the system SHALL monitor active shield integrity and trigger zeroization upon shield breach detection. On Cortex-M33: use SAU/IDAU + tamper pins. | FIPS 140-3 Level 3 |
| SEC_PHYS_003 | Temperature and voltage monitoring | The system SHALL monitor operating temperature and supply voltage. Operations outside safe range (as per MCU datasheet) SHALL trigger suspension of crypto operations and alert. | AIS-31 §4.2 |
| SEC_PHYS_004 | Debug port lockdown | JTAG/SWD debug access SHALL be permanently disabled in production firmware via hardware fuse (RDP Level 2 on STM32). Debug-enabled builds SHALL be clearly distinguishable. | Common Criteria FPT_PHP.3 |
| SEC_PHYS_005 | Side-channel countermeasures (specific) | Beyond generic "mitigation", the system SHALL implement: constant-time comparisons for all secret-dependent operations, random execution delays (jitter), dummy operations during idle, and masking for AES. | FIPS 140-3 IG §4.5 |

---

#### E. Entropy Source Validation (4 missing requirements)

| ID | Title | Description | Reference |
|----|-------|-------------|-----------|
| SEC_ENT_001 | Entropy source health tests | The TRNG SHALL implement continuous health tests: repetition count test and adaptive proportion test per NIST SP 800-90B §4.3-4.4. Failure SHALL disable RNG output. | NIST SP 800-90B |
| SEC_ENT_002 | Power-on self-test for RNG | At every boot, the RNG subsystem SHALL execute known-answer tests (KAT) for the DRBG and verify TRNG output against stuck-at and bias conditions before any key generation. | FIPS 140-3 §4.9 |
| SEC_ENT_003 | Entropy conditioning | Raw TRNG output SHALL be conditioned through an approved conditioning function (e.g., SHA-256 or AES-CBC-MAC) before use as DRBG seed. Raw output SHALL NOT be used directly. | AIS-31 Class P2 |
| SEC_ENT_004 | Entropy rate declaration | The entropy source SHALL declare its min-entropy rate (bits of entropy per bit of output). This SHALL be validated through NIST SP 800-90B entropy assessment. | NIST SP 800-90B §3 |

---

#### F. Cryptographic Self-Tests (4 missing requirements)

| ID | Title | Description | Reference |
|----|-------|-------------|-----------|
| SEC_TEST_001 | Power-on self-tests (POST) | At boot, the system SHALL execute known-answer tests for EVERY implemented algorithm (AES, ECDSA, SHA-256, etc.). Failure of ANY test SHALL prevent the HSM from entering operational state. | FIPS 140-3 §4.9.1 |
| SEC_TEST_002 | Conditional self-tests | Self-tests SHALL run before first use of each algorithm in a session, after firmware update, and periodically during operation (configurable interval). | FIPS 140-3 §4.9.2 |
| SEC_TEST_003 | Integrity check | The HSM firmware SHALL verify its own integrity (HMAC or digital signature over code + read-only data) at boot and periodically. Integrity failure SHALL trigger safe state. | FIPS 140-3 §4.9.1 |
| SEC_TEST_004 | Pair-wise consistency test | After every asymmetric key pair generation, the system SHALL verify the pair by performing a sign-then-verify (or encrypt-then-decrypt) operation. Failure SHALL destroy the key pair. | FIPS 140-3 §4.9.2 |

---

#### G. Audit & Crypto Logging (4 missing requirements)

| ID | Title | Description | Reference |
|----|-------|-------------|-----------|
| SEC_AUD_001 | Tamper-evident audit log | ALL security-relevant events SHALL be logged: key operations, auth attempts, config changes, self-test results, tamper events. Logs SHALL be integrity-protected (HMAC chain). | Common Criteria FAU_GEN.1 |
| SEC_AUD_002 | Log overflow protection | When audit log storage is full, the system SHALL either: (a) halt crypto operations until logs are exported, or (b) overwrite oldest entries with authenticated notification to host. Configurable policy. | Common Criteria FAU_STG.4 |
| SEC_AUD_003 | Monotonic timestamp | Every audit entry SHALL include a monotonic timestamp derived from a secure, non-resettable counter. The counter SHALL survive power cycles (backed by flash or battery). | ISO/SAE 21434 §15.4 |
| SEC_AUD_004 | Remote log export | Audit logs SHALL be exportable to an external system via authenticated and encrypted channel. Export SHALL NOT expose key material even in log entries. | UNECE R155 §7.2.2 |

---

#### H. Authentication & Access Control (4 missing requirements)

| ID | Title | Description | Reference |
|----|-------|-------------|-----------|
| SEC_AUTH_001 | Mutual authentication | The HSM SHALL mutually authenticate with the host ECU before accepting any command. Authentication SHALL use challenge-response (ECDSA or HMAC-based). | SHE spec §4.2 |
| SEC_AUTH_002 | Role-based access control | The system SHALL enforce RBAC with minimum roles: ADMIN (key provisioning, config), OPERATOR (crypto operations), AUDITOR (log read-only), FACTORY (initial provisioning). | PKCS#11 §6.7 |
| SEC_AUTH_003 | Authentication rate limiting | After N consecutive failed authentication attempts (configurable, default 5), the system SHALL enforce exponential backoff and alert via IDS channel. After M failures (default 10), trigger partial zeroization of the attacking domain. | FIPS 140-3 §4.3.3 |
| SEC_AUTH_004 | Session binding | Authenticated sessions SHALL be bound to a session ID with configurable timeout. All commands outside a valid session SHALL be rejected. Session keys SHALL be ephemeral. | ISO/SAE 21434 §8.6 |

---

#### I. Secure Communication with Host (3 missing requirements)

| ID | Title | Description | Reference |
|----|-------|-------------|-----------|
| SEC_COMM_001 | Encrypted command channel | All commands and responses between host and HSM SHALL be encrypted and authenticated (AES-GCM or ChaCha20-Poly1305). Plaintext command interfaces SHALL NOT exist in production. | EVITA HSM spec §4.5 |
| SEC_COMM_002 | Anti-replay on commands | Every command SHALL include a monotonic sequence number or nonce. The HSM SHALL reject replayed commands. | SHE spec §4.4 |
| SEC_COMM_003 | Transport binding | The communication channel SHALL be bound to the physical transport (SPI/UART/I2C). Channel parameters (speed, addressing) SHALL be locked after initialization. | FIPS 140-3 §4.6 |

---

#### J. Operational Modes & State Machine (3 missing requirements)

| ID | Title | Description | Reference |
|----|-------|-------------|-----------|
| SEC_MODE_001 | HSM operational states | The HSM SHALL operate in defined states: FACTORY → PROVISIONED → OPERATIONAL → SUSPENDED → TERMINATED. Transitions SHALL be one-way (no return from TERMINATED). State SHALL survive power cycles. | FIPS 140-3 §4.2 |
| SEC_MODE_002 | Degraded mode operation | If a self-test or health check fails for a specific algorithm, the HSM SHALL disable ONLY that algorithm and continue operating with remaining healthy algorithms. Full shutdown only if core integrity is compromised. | ISO 26262 ASIL decomposition |
| SEC_MODE_003 | Safe state definition | The system SHALL define and implement a safe state: all pending operations aborted, volatile keys zeroized, audit log flushed, host notified. Safe state SHALL be reachable from any operational state within 50ms. | ISO 26262 §6.4.2 |

---

#### K. Compliance & Certification Readiness (3 missing requirements)

| ID | Title | Description | Reference |
|----|-------|-------------|-----------|
| SEC_CERT_001 | FIPS 140-3 module boundary | The cryptographic module boundary SHALL be clearly defined: which components are inside (crypto algorithms, key storage, RNG, self-tests) and which are outside (host interface, application logic). | FIPS 140-3 §4.1 |
| SEC_CERT_002 | Algorithm validation | All implemented algorithms SHALL be validated against CAVP (Cryptographic Algorithm Validation Program) test vectors. Test vector results SHALL be documented and reproducible. | NIST CAVP |
| SEC_CERT_003 | Security policy document | A formal Security Policy document SHALL be maintained describing: module boundary, security rules, approved algorithms, key management procedures, authentication mechanisms, physical security. | FIPS 140-3 §4.1.1 |

---

### Summary: Gap Count

| Category | SCORE has | Missing | Total needed |
|----------|-----------|---------|-------------|
| Algorithms & crypto ops | 18 | 0 | 18 |
| Key management | 4 | 8 | 12 |
| API & non-functional | 14 | 0 | 14 |
| TLS | 1 | 0 | 1 |
| Zeroization | 0 | 4 | 4 |
| Secure boot & firmware | 0 | 5 | 5 |
| Physical attack resistance | 1 (generic) | 5 | 5 |
| Entropy validation | 1 (generic) | 4 | 4 |
| Cryptographic self-tests | 0 | 4 | 4 |
| Audit & logging | 0 | 4 | 4 |
| Authentication & access | 1 (generic) | 4 | 4 |
| Secure communication | 0 | 3 | 3 |
| Operational states | 0 | 3 | 3 |
| Compliance readiness | 0 | 3 | 3 |
| **TOTAL** | **40** | **47** | **87** |

SCORE has 40 requirements. A proper HSM needs **87**.
They're missing the entire operational, physical, and compliance layer.

---

## Architecture (next section — to be written after plan approval)

Pending your architecture decisions on:

1. TrustZone partitioning — what runs in secure world vs non-secure world
2. Key storage model — flash-based vs OTP vs derived from PUF
3. Host communication protocol — SPI vs UART, message format
4. Rust vs C++ split — crypto in Rust, HAL in C++, or all Rust (Embassy)?
5. Integration with OpenBSW — how does the HSM serve the F413ZH/G474RE ECUs?
6. FIPS 140-3 target level — Level 1 (software only) vs Level 2 (tamper evidence)?

---

## Hardware: STM32L552ZE Crypto Capability Matrix

| Requirement Category | HW Accelerated | SW Fallback Needed |
|---------------------|----------------|-------------------|
| AES-128/256 (CBC/GCM/CCM) | AES peripheral | No |
| ECDSA P-256/P-384 sign/verify | PKA peripheral | No |
| ECDH P-256/P-384 | PKA peripheral | No |
| RSA-2048/4096 | PKA peripheral (modular exp) | Partial — key gen in SW |
| SHA-256 | HASH peripheral | No |
| SHA-384/512 | No HW | Yes — Rust crate |
| SHA-3, BLAKE3 | No HW | Yes — Rust crate |
| ChaCha20-Poly1305 | No HW | Yes — Rust crate |
| EdDSA (Ed25519) | No HW | Yes — Rust crate |
| ML-DSA (post-quantum) | No HW | Yes — Rust crate |
| TRNG (entropy) | RNG peripheral | No |
| Key storage | TrustZone secure flash | No |
| Tamper detection | TAMP pins (5 active + 3 internal) | No |
| Monotonic counter | RTC backup registers (battery-backed) | No |
| Secure debug | RDP Level 2 (irreversible) | No |
| Voltage/clock monitoring | No HW | External or SW watchdog |
