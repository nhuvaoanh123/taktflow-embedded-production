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

# SIL (Software-in-the-Loop) Test Suite

Automated multi-ECU integration testing for the Taktflow zonal vehicle platform. All 7 ECUs are compiled for Linux (POSIX/SocketCAN backend), deployed as Docker containers on a virtual CAN bus (vcan0), and exercised by scenario-driven test orchestration with automated verdict checking.

## Standards Compliance

| Standard | Reference | Coverage |
|----------|-----------|----------|
| **ISO 26262** | Part 6, Section 10 | Software integration and testing at ASIL D |
| **ASPICE 4.0** | SWE.5 | SW Component Verification and Integration |
| **IEC 61508-3** | Section 7.4.8 | Software module testing and integration |

## Architecture

```
+---------------------------------------------+
|              SIL Test Runner                 |
|  run_sil.sh -> verdict_checker.py           |
|       | REST API    | MQTT    | CAN         |
+---------------------------------------------+
|           Docker Compose Platform            |
|  +-----+ +-----+ +-----+ +----+            |
|  | CVC | | FZC | | RZC | | SC |  (vECUs)   |
|  +--+--+ +--+--+ +--+--+ +-+--+            |
|     +-------+-------+------+               |
|              vcan0 (SocketCAN)               |
|  +----------+  +-------------+              |
|  |Plant Sim |  |Fault Inject |              |
|  +----------+  +-------------+              |
|  +------+  +------+  +------+              |
|  | BCM  |  | ICU  |  | TCU  |  (sim ECUs)  |
|  +------+  +------+  +------+              |
+---------------------------------------------+
```

### Component Roles

| Component | Description |
|-----------|-------------|
| **CVC** | Central Vehicle Computer -- coordinates all ECUs, state management |
| **FZC** | Front Zone Controller -- steering, front sensors, front lighting |
| **RZC** | Rear Zone Controller -- motor, rear sensors, rear lighting |
| **SC** | Safety Controller -- independent safety monitoring (TMS570 equivalent) |
| **BCM** | Body Control Module -- body electronics, comfort functions |
| **ICU** | Instrument Cluster Unit -- dashboard display, telltales |
| **TCU** | Telematics Control Unit -- cloud connectivity, OTA, MQTT |
| **Plant Sim** | Plant physics simulator -- models vehicle dynamics on CAN |
| **Fault Inject** | REST API for injecting faults (bus-off, sensor stuck, delay) |
| **run_sil.sh** | Test orchestrator -- manages Docker lifecycle, invokes verdict checker |
| **verdict_checker.py** | Evaluates scenario verdicts -- CAN message checks, state transitions, timing |

### Communication Channels

- **vcan0 (SocketCAN)**: All inter-ECU communication via virtual CAN bus
- **MQTT (localhost:1883)**: Telemetry bridge for monitoring and test observation
- **REST API (localhost:8091)**: Fault injection control for test scenarios

## Prerequisites

| Dependency | Version | Purpose |
|------------|---------|---------|
| Docker | 24+ | Container runtime for vECUs |
| Docker Compose | v2+ | Multi-container orchestration |
| Python | 3.10+ | Verdict checker, test orchestration |
| can-utils | any | CAN bus utilities (candump, cansend) |
| vcan kernel module | Linux kernel | Virtual CAN bus interface |

### vcan Kernel Module

The virtual CAN interface requires the `vcan` kernel module. This is available on Linux only. On GitHub Actions runners (ubuntu-latest), use `linux-modules-extra-$(uname -r)` to get the module.

**Check if vcan is available:**

```bash
modprobe --dry-run vcan && echo "vcan module available" || echo "vcan module NOT available"
```

## Quick Start

```bash
# 1. Set up vcan0 (requires root / sudo)
sudo modprobe vcan
sudo ip link add dev vcan0 type vcan
sudo ip link set up vcan0

# 2. Install Python dependencies
pip install -r test/sil/requirements.txt

# 3. Run all SIL scenarios
./test/sil/run_sil.sh

# 4. Run a single scenario
./test/sil/run_sil.sh --scenario=sil_001_normal_startup

# 5. Run with containers kept alive (for debugging)
./test/sil/run_sil.sh --scenario=sil_001_normal_startup --keep

# 6. Override per-scenario timeout
./test/sil/run_sil.sh --timeout=120
```

### Verifying vcan0 Is Working

```bash
# In one terminal, listen on vcan0
candump vcan0

# In another terminal, send a test frame
cansend vcan0 123#DEADBEEF

# You should see the frame appear in the candump output
```

## Scenario List

Each scenario is a YAML file in `test/sil/scenarios/`. Scenarios are executed in alphabetical order by default.

| ID | Name | Tests | Est. Duration |
|----|------|-------|---------------|
| SIL-001 | Normal Startup Sequence | INIT->RUN transition, heartbeats, CAN frames | ~20s |
| SIL-002 | Graceful Shutdown | RUN->SHUTDOWN->SAFE transition, actuator de-energize | ~25s |
| SIL-003 | Emergency Stop | E-stop trigger, FTTI compliance, safe state entry | ~15s |
| SIL-004 | CAN Bus-Off Recovery | Bus-off injection on CVC, recovery within deadline | ~30s |
| SIL-005 | Sensor Stuck-At Fault | Stuck sensor detection, fallback to secondary source | ~25s |
| SIL-006 | Watchdog Timeout Recovery | ECU watchdog trigger, automatic restart, state restore | ~30s |
| SIL-007 | Overcurrent Protection | Motor overcurrent fault, RZC safe-state response | ~20s |
| SIL-008 | Steering Assist Degradation | FZC steering fault, degraded mode engagement | ~25s |
| SIL-009 | Communication Loss (CVC-SC) | CVC-SC link down, SC independent shutdown path | ~20s |
| SIL-010 | Battery Undervoltage | Low voltage simulation, load shedding sequence | ~25s |
| SIL-011 | OTA Update Sequence | TCU firmware update flow, version verification | ~40s |
| SIL-012 | Multi-Fault Cascade | Two simultaneous faults, priority-based safe state | ~35s |
| SIL-013 | Thermal Runaway Protection | Temperature ramp, thermal shutdown sequence | ~30s |
| SIL-014 | SAP QM Notification | Quality event trigger, SAP QM API integration check | ~20s |
| SIL-015 | Full System Stress | All ECUs at max load, timing budget compliance | ~45s |

**Total estimated runtime: approximately 6-8 minutes** (sequential execution, plus Docker build time on first run).

## Scenario YAML Format

Each scenario file follows this structure:

```yaml
name: "Human-readable scenario name"
id: "SIL-NNN"
description: >
  Detailed description of what this scenario tests and why.
verifies:
  - "SWR-BSW-NNN"       # Software requirement IDs
  - "SSR-CVC-NNN"       # Software safety requirement IDs
aspice: "SWE.5"
iso_reference: "ISO 26262 Part 6, Section 10"
timeout_sec: 30

setup:
  - action: reset
    description: "Clear residual state"
  - action: wait
    seconds: 1

steps:
  - action: wait_state
    state: RUN
    timeout: 5
    description: "Wait for system to reach RUN state"
  - action: inject_fault
    fault_type: "bus_off"
    target: "cvc"
    description: "Inject CAN bus-off fault on CVC"
  - action: wait
    seconds: 3
    description: "Allow fault detection and reaction"

verdicts:
  - type: vehicle_state
    expected: SAFE
    within_ms: 2000
    description: "System must reach SAFE state within 2 seconds"
  - type: can_message
    can_id: 0x100
    description: "VehicleState frame indicates SAFE (0x02)"

teardown:
  - action: reset
    description: "Return to clean state"
```

### Verdict Types

| Type | Description | Parameters |
|------|-------------|------------|
| `vehicle_state` | Check system state reached expected value | `expected`, `within_ms` |
| `can_message` | Verify CAN frame presence on vcan0 | `can_id`, optional `data_mask` |
| `can_absence` | Verify CAN frame is NOT present (actuator de-energized) | `can_id`, `absence_ms` |
| `mqtt_message` | Check MQTT topic received expected payload | `topic`, `payload_contains` |
| `timing` | Verify reaction completed within deadline | `event`, `deadline_ms` |
| `fault_api` | Check fault injection API response | `endpoint`, `expected_status` |

## Adding New Scenarios

1. Create a new YAML file in `test/sil/scenarios/` following the naming convention:
   ```
   sil_NNN_short_description.yaml
   ```
   Use the next available number in the sequence.

2. Include the required header fields: `name`, `id`, `description`, `verifies`, `aspice`, `iso_reference`, `timeout_sec`.

3. Link to at least one software requirement ID in the `verifies` field for traceability (ISO 26262 Part 6, ASPICE SWE.5).

4. Define `setup`, `steps`, `verdicts`, and `teardown` sections.

5. Test the scenario in isolation first:
   ```bash
   ./test/sil/run_sil.sh --scenario=sil_NNN_short_description --keep
   ```

6. Verify the scenario passes reliably at least 3 consecutive times before merging.

## Results Output

Each test run creates a timestamped directory under `test/sil/results/`:

```
test/sil/results/
  20260224_020015/
    summary.txt                 # Pass/fail summary with scenario verdicts
    sil_results.xml             # JUnit XML report (CI-compatible)
    verdict_output.log          # Full verdict checker console output
    docker_startup.log          # Docker compose build + up output
    docker_teardown.log         # Docker compose down output
    docker_logs.txt             # Combined container logs
    cvc.log                     # Individual ECU container logs
    fzc.log
    rzc.log
    sc.log
    bcm.log
    icu.log
    tcu.log
    plant-sim.log
    can-gateway.log
    fault-inject.log
    mqtt-broker.log
```

The `results/` directory is gitignored except for the `.gitkeep` file. Results are uploaded as GitHub Actions artifacts on nightly CI runs (retained for 30 days for test results, 90 days for summary reports).

### JUnit XML Format

The verdict checker produces JUnit XML output compatible with GitHub Actions test reporting and standard CI tools. Each scenario maps to a test suite, and each verdict maps to a test case within that suite.

## CI Integration

The SIL test suite runs as a **nightly CI job** (not per-commit, due to the Docker build and multi-container startup overhead).

| Trigger | Schedule | Workflow |
|---------|----------|----------|
| Nightly | 02:00 UTC daily | `.github/workflows/sil-nightly.yml` |
| Manual | On-demand via Actions UI | `.github/workflows/sil-nightly.yml` (workflow_dispatch) |

### CI Environment

- **Runner**: `ubuntu-latest`
- **Timeout**: 30 minutes (whole job)
- **vcan0**: Set up via `modprobe vcan` + `ip link add`
- **Python**: 3.11
- **Docker**: Pre-installed on GitHub-hosted runners
- **Artifacts**: JUnit XML, container logs, summary uploaded after every run

### Relationship to Other CI Workflows

| Workflow | Trigger | Scope |
|----------|---------|-------|
| `misra.yml` | Every push (firmware paths) | MISRA C:2012 static analysis |
| `test.yml` | Every push (firmware paths) | Unit tests + code coverage (per-ECU + BSW) |
| `sil-nightly.yml` | Nightly + manual | Full SIL integration tests (Docker, multi-ECU) |

## Troubleshooting

### vcan0 Issues

**Problem**: `RTNETLINK answers: Operation not supported`
```bash
# The vcan kernel module is not loaded
sudo modprobe vcan
# If modprobe fails, install the extra kernel modules:
sudo apt-get install linux-modules-extra-$(uname -r)
```

**Problem**: `vcan0 already exists`
```bash
# This is fine -- just ensure it is UP
sudo ip link set up vcan0
```

**Problem**: `Cannot find device "vcan0"` inside Docker container
```
# All containers use network_mode: host, so vcan0 must be created on the
# HOST before starting Docker. The can-setup service in docker-compose.yml
# handles this automatically, but requires:
#   cap_add: [NET_ADMIN, SYS_MODULE]
#   volumes: [/lib/modules:/lib/modules:ro]
#   network_mode: host
```

### Docker Issues

**Problem**: `docker compose build` fails with out-of-memory
```bash
# The multi-stage build compiles all 7 ECU binaries. On CI runners with
# limited RAM, build sequentially:
DOCKER_BUILDKIT=0 docker compose -f docker/docker-compose.yml build
```

**Problem**: `port already in use` (1883, 8080, 8090, 8091)
```bash
# Another process is occupying a required port. Check and stop it:
sudo lsof -i :8091
# Or change the port in docker-compose.yml environment variables
```

**Problem**: Containers exit immediately after start
```bash
# Check container logs for the failing service:
docker compose -f docker/docker-compose.yml logs cvc
# Common cause: vcan0 not available (the can-setup service must succeed first)
```

### Timeout Issues

**Problem**: Scenarios fail with timeout waiting for services
```bash
# Increase the health check wait time:
export HEALTH_WAIT_MAX=180

# Or increase per-scenario timeout:
./test/sil/run_sil.sh --timeout=120
```

**Problem**: `wait_state` step times out waiting for RUN state
```
# ECUs may take longer to initialize on slow CI runners.
# Check the scenario timeout_sec value and increase if needed.
# Also check docker logs -- an ECU may have crashed during init:
docker compose -f docker/docker-compose.yml logs sc
```

### General Debugging

**Inspect CAN traffic in real time:**
```bash
candump vcan0
```

**Query fault injection API:**
```bash
curl http://localhost:8091/api/fault/health
curl http://localhost:8091/api/fault/status
```

**Check MQTT messages:**
```bash
mosquitto_sub -h localhost -p 1883 -t '#' -v
```

**Run with containers kept alive for manual inspection:**
```bash
./test/sil/run_sil.sh --scenario=sil_001_normal_startup --keep
# Containers remain running -- inspect, attach, debug
# When done:
docker compose -f docker/docker-compose.yml down
```

## File Structure

```
test/sil/
  apps-web-overview.md                   # This file
  run_sil.sh                  # Test orchestrator (Bash)
  verdict_checker.py          # Verdict evaluation engine (Python)
  requirements.txt            # Python dependencies
  scenarios/                  # Scenario YAML files
    sil_001_normal_startup.yaml
    sil_002_graceful_shutdown.yaml
    ...
  results/                    # Test output (gitignored, except .gitkeep)
    .gitkeep
    <timestamp>/              # Per-run results
      summary.txt
      sil_results.xml
      *.log
```

## Standards Reference

### ISO 26262 Part 6, Section 10 -- Software Integration and Testing

- Integration testing at ASIL D requires: requirements-based testing (++), interface testing (++), fault injection testing (++), and resource usage testing (++)
- Test case derivation by: analysis of requirements (++), equivalence classes (++), boundary value analysis (++), error guessing (++)
- Structural coverage at integration level: statement (++), branch (++), MC/DC (++)
- Each SIL scenario traces to at least one software requirement via the `verifies` field

### ASPICE SWE.5 -- SW Component Verification and Integration

- BP1: Develop integration strategy (Docker Compose multi-ECU platform)
- BP2: Develop test specification based on SW architecture (scenario YAML files)
- BP3: Select test cases based on integration strategy (CAN bus communication, state transitions, fault handling)
- BP4: Perform integration tests (automated via run_sil.sh)
- BP5: Perform regression testing (nightly CI ensures no regressions)
- BP6: Ensure bidirectional traceability (scenario `verifies` field links to requirements)
- BP7: Summarize results (JUnit XML, summary.txt, GitHub Actions job summary)
- BP8: Ensure consistency (verdict checker provides deterministic pass/fail)

### IEC 61508-3, Section 7.4.8 -- Software Module Testing

- Software integration testing using both functional and structural approaches
- Fault injection at integration boundaries (Fault Injection API)
- Automated regression via nightly schedule

