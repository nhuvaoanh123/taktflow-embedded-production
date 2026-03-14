# Test Guide

Four test layers. Run them in order: unit → SIL → PIL → HIL.

```
Layer 4 — HIL        Physical ECU + real I/O + real-time plant  (per release)
Layer 3 — SIL        Docker + vCAN + plant simulator            (every commit in CI)
Layer 2 — PIL        Firmware on real MCU, PC-driven stimuli    (weekly in CI)
Layer 1 — Unit       Host-compiled, no hardware                 (every commit in CI)
```

---

## Layer 1 — Unit Tests

Framework: **Unity** (C) for firmware, **pytest** for Python tooling.

### BSW Unit Tests

```bash
cd firmware/bsw

# Run all BSW module tests
make test

# Run one module
make test-Com_asild
make test-CanIf
make test-NvM

# With coverage report
make coverage
# Output: firmware/bsw/coverage/index.html
open firmware/bsw/coverage/index.html
```

**Coverage thresholds (enforced in CI):**
- Function: ≥ 90%
- Line: ≥ 85%
- Branch: ≥ 75%

### ECU Application Unit Tests

```bash
# Unit tests for one ECU (host-compiled)
make -f firmware/platform/posix/Makefile.posix TARGET=cvc test

# With coverage
make -f firmware/platform/posix/Makefile.posix TARGET=cvc coverage

# All ECUs
for ecu in bcm cvc fzc gw icu rzc; do
  make -f firmware/platform/posix/Makefile.posix TARGET=$ecu test
done
```

### Test Framework Unit Tests

```bash
cd test/framework
make test
make coverage
```

### Python Tooling Tests

```bash
source .venv/bin/activate
cd tools/arxml
pytest tests/ -v
```

### Success Criteria

```
PASS  — All test cases: PASS, coverage thresholds met
FAIL  — Any test case: FAIL or IGNORE, or coverage below threshold
```

Unity outputs: `X Tests Y Failures Z Ignored`
CI fails the build if Failures > 0 or Ignored > 0.

---

## Layer 2 — SIL (Software-in-the-Loop)

All 7 ECUs compiled for POSIX, networked via vCAN in Docker, with a Python plant simulator.

### Run All 18 Scenarios

```bash
source .venv/bin/activate

# Run complete SIL suite
./test/sil/run_sil.sh

# Output: test/sil/results/sil_results.xml (JUnit XML)
```

### Run One Scenario

```bash
./test/sil/run_sil.sh --scenario=sil_001_normal_startup --timeout=60
./test/sil/run_sil.sh --scenario=sil_005_estop_from_bcm --timeout=90
./test/sil/run_sil.sh --scenario=sil_012_rzc_heartbeat_loss --timeout=120
```

### SIL Scenario List

| ID | Name | What It Tests |
|----|------|---------------|
| `sil_001` | Normal startup | All ECUs start, heartbeats detected |
| `sil_002` | Pedal to motor | BCM pedal → CVC motor command end-to-end |
| `sil_003` | E-stop from BCM | E-stop → all motor outputs 0 within 100ms |
| `sil_004` | E-stop from RZC | Redundant e-stop path |
| `sil_005` | BCM heartbeat loss | CVC detects BCM silence, enters safe state |
| `sil_006` | CVC heartbeat loss | RZC detects CVC silence, triggers backup path |
| `sil_007` | CAN bus overload | 500 frames/s → no frame loss on critical signals |
| `sil_008` | NvM write-read | Config written → power cycle → config intact |
| `sil_009` | UDS session | Diagnostic session open, DID read, session close |
| `sil_010` | UDS security access | Seed-key exchange, successful unlock |
| `sil_011` | DTC set and clear | Fault injected → DTC stored → DTC clear → clean |
| `sil_012` | RZC heartbeat overtransmit | RZC sends heartbeat too fast → GW filters |
| `sil_013` | FDCAN TX overflow | 1000 frames burst → TX FIFO recovery |
| `sil_014` | Sensor substitution | Pedal signal lost → substitution value activates |
| `sil_015` | Motor position hold | Motor stopped, position maintained under load sim |
| `sil_016` | Startup sequencing | Correct ECU startup order enforced |
| `sil_017` | 24h endurance (nightly only) | No memory growth, no forwarding degradation |
| `sil_018` | Fault injection full | All fault types injected sequentially |

### SIL with Docker (alternative)

```bash
# Run SIL suite via Docker Compose (CI mode)
docker compose -f docker/docker-compose.sil.yml run sil-harness \
  python test/sil/run_scenarios.py --output results/

# View results
cat test/sil/results/sil_results.xml
```

### Success Criteria

```
PASS  — All scenarios verdict: PASS, JUnit XML: 0 failures
FAIL  — Any scenario: FAIL or TIMEOUT
```

---

## Layer 3 — PIL (Processor-in-the-Loop)

Firmware on a real STM32 board, stimulated from PC via USB-CAN adapter.

### Prerequisites

- STM32G4 board (any Nucleo or custom) flashed with target ECU firmware
- USB-CAN adapter connected to PC and to board CAN bus
- USB-CAN set up per [docs/guides/usb-can-adapter-setup.md](docs/guides/usb-can-adapter-setup.md)

### Run PIL Test Harness

```bash
source .venv/bin/activate

# Run PIL for CVC ECU
python test/pil/run_pil.py \
  --target=cvc \
  --channel=can0 \
  --iterations=100 \
  --output=test/pil/results/pil_cvc.xml

# Pass criteria: ≥99% of iterations PASS
```

### What PIL Tests

- Round-trip latency: stimulus sent → response received (target: < 50ms)
- CAN signal accuracy: pedal value sent matches PWM output within ±2%
- Heartbeat timing: alive counter increments at correct rate
- E-stop response: E-stop frame → motor output 0 within 100ms (measured on real hardware)

See [test/pil/test-pil-overview.md](test/pil/test-pil-overview.md) for full methodology.

### Success Criteria

```
PASS  — ≥99 of 100 iterations PASS, latency p99 < 50ms
FAIL  — <99 iterations PASS or latency p99 ≥ 50ms
```

---

## Layer 4 — HIL (Hardware-in-the-Loop)

Full vehicle subsystem bench: real ECUs, real CAN bus, real I/O, real-time plant simulation.

HIL runs once per release candidate. It is not automated in the push-triggered CI pipeline.

See [docs/reference/lessons-learned/safety/PROCESS-hil-gap-analysis.md](docs/reference/lessons-learned/safety/PROCESS-hil-gap-analysis.md) for what HIL covers and what it does not.

---

## CI Pipeline Overview

| Trigger | Layers Run |
|---------|-----------|
| Every push | Unit (BSW + all ECUs) + MISRA |
| Every PR merge to main | Unit + SIL (scenarios 1–16) |
| Nightly | Unit + SIL (all 18 including endurance) + PIL (if runner available) |
| Release tag | All above + HIL checklist |

CI configuration: `.github/workflows/`

---

## Interpreting Results

### JUnit XML

All layers output JUnit XML to `test/*/results/*.xml`.

```xml
<testsuites>
  <testsuite name="SIL" tests="16" failures="0" errors="0">
    <testcase name="sil_001_normal_startup" time="12.3"/>
    <testcase name="sil_003_estop_from_bcm" time="8.7"/>
    ...
  </testsuite>
</testsuites>
```

### Coverage Report

Open `firmware/bsw/coverage/index.html` in a browser.

- Green = covered lines/branches
- Red = uncovered — must add a test case or document why coverage is not achievable
- Yellow = partially covered branches — check if the uncovered branch is a safety-critical path

### MISRA Report

```bash
# Generate MISRA report
make -f firmware/platform/posix/Makefile.posix misra 2>&1 | tee misra_report.txt
grep -c "error" misra_report.txt  # Must be 0
```

Approved deviations are in [docs/safety/analysis/misra-deviation-register.md](docs/safety/analysis/misra-deviation-register.md). Any violation not listed there is a blocking failure.
