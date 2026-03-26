# Taktflow Embedded — ISO 26262 ASIL D Zonal Vehicle Platform

A complete automotive functional safety project built from scratch: 7 ECUs, AUTOSAR-like BSW, ASIL D safety lifecycle, full ASPICE 4.0 documentation, and a [live SIL demo](https://sil.taktflow-systems.com) you can interact with right now.

```
                         CAN Bus (500 kbps, E2E-protected)
     ┌──────────────┬──────────────┬──────────────┬──────────────┐
     │              │              │              │              │
┌────┴────┐   ┌────┴────┐   ┌────┴────┐   ┌────┴────┐   ┌─────┴─────┐
│   CVC   │   │   FZC   │   │   RZC   │   │   SC    │   │  vECUs    │
│ STM32   │   │ STM32   │   │ STM32   │   │ TMS570  │   │  Docker   │
│         │   │         │   │         │   │lockstep │   │           │
│ Pedals  │   │Steering │   │ Motor   │   │Watchdog │   │BCM ICU TCU│
│ State   │   │ Brake   │   │Current  │   │Kill Rly │   │Lights    │
│ Machine │   │ LiDAR   │   │ Temp    │   │Heartbeat│   │Dashboard │
│ Display │   │ Buzzer  │   │Encoder  │   │Plausib. │   │Telemetry │
└─────────┘   └─────────┘   └─────────┘   └─────────┘   └───────────┘
  ASIL D         ASIL D        ASIL D        ASIL D         QM
                                                │
                                    ┌───────────┘
                                    │
                              ┌─────┴─────┐
                              │ Raspberry  │
                              │  Pi GW     │
                              │            │
                              │ CAN→MQTT   │
                              │ Edge ML    │
                              │ Fault Inj. │
                              └─────┬──────┘
                                    │ MQTT/TLS
                              ┌─────┴──────┐
                              │  AWS IoT   │
                              │  Grafana   │
                              └────────────┘
```

## Key Numbers

| Metric | Value |
|--------|-------|
| ECUs | **7** (4 physical + 3 simulated) |
| Safety Goals | **8** (ASIL A through D) |
| Unit Tests | **1,067** (100% statement, branch, MC/DC) |
| C Source Files | **278** across all ECUs |
| BSW Modules | **18** (AUTOSAR-like: MCAL, ECUAL, Services, RTE) |
| Safety Requirements | **548** (SG→FSR→TSR→SSR→HSR→SWR) |
| Traceability Links | **440+** bidirectional (requirement→code→test) |
| MISRA Violations | **0** (CI blocking, 2 documented deviations) |
| ASPICE Documents | **70+** work products |
| Hazardous Events | **20** analyzed (4 ASIL D, 6 C, 4 B, 4 A, 2 QM) |

## Live Demo

**[sil.taktflow-systems.com](https://sil.taktflow-systems.com)** — All 7 ECUs running in Docker with real CAN bus simulation.

- Real-time telemetry via WebSocket (motor, steering, brake, battery, LiDAR)
- Fault injection: trigger ASIL D scenarios (overcurrent, steering fault, E-stop)
- SAP QM integration: faults automatically generate quality notifications
- Edge ML anomaly detection running on simulated sensor data
- ECU heartbeat monitoring with timeout detection

## What This Demonstrates

### ISO 26262 ASIL D — Full Safety Lifecycle
- **HARA** with 20 hazardous events across 6 operational situations
- **8 Safety Goals** with defined safe states and FTTI budgets
- **Safety requirements flow-down**: SG → FSR (25) → TSR (51) → SSR (81) → code → tests
- **FMEA**: 50 failure modes analyzed
- **DFA**: Common-cause and cascading failure analysis
- **Hardware metrics**: SPFM ≥ 99%, LFM ≥ 90%, PMHF < 10 FIT
- **Diverse redundancy**: ST Cortex-M4F (zone ECUs) + TI Cortex-R5F lockstep (safety controller)

### ASPICE 4.0 — Process Maturity Level 2-3
- **V-Model** specification ↔ verification across SYS.1-5, SWE.1-6, HWE.1-3
- **Configuration management** (SUP.8): baselines, branching strategy, change requests
- **Quality assurance** (SUP.1): QA plan, independent review
- **Project management** (MAN.3): decision log with tiered ADRs, risk register, weekly status

### AUTOSAR-Like BSW Stack
```
APPLICATION SWCs (Swc_Pedal, Swc_Motor, Swc_Steering, Swc_Brake, ...)
         ↕ Rte_Read / Rte_Write
RTE (signal buffers, port connections, runnable scheduling)
         ↕
SERVICES (Com, Dcm, Dem, WdgM, BswM, E2E)
         ↕
ECU ABSTRACTION (CanIf, PduR, IoHwAb)
         ↕
MCAL (Can, Spi, Adc, Pwm, Dio, Gpt)
         ↕
HARDWARE (STM32G474RE / TMS570LC43x)
```

### Independent Safety Controller
The SC runs on a **TI TMS570 with dual Cortex-R5F lockstep cores** — a completely different architecture from the zone ECUs:
- **~400 LOC**, flat bare-metal (no RTOS, no BSW) for maximum simplicity
- Monitors heartbeats from CVC, FZC, RZC (100ms timeout)
- Cross-checks torque vs. current for plausibility
- Controls energize-to-run kill relay (fails safe on power loss)
- External TPS3823 watchdog: fed only when all checks pass

### Test-Driven Development
Every source file has tests **written before the implementation** (enforced by hook):
- **1,067 unit tests** using Unity framework
- 100% statement, branch, and MC/DC coverage for safety paths
- Boundary value analysis, fault injection, NULL pointer tests
- Test code is 4x the size of production code (39,040 vs 9,799 LOC)

### CI/CD Pipeline
- Every commit: compile (debug + release), unit tests, MISRA check
- Coverage gates: Function ≥ 90%, Line ≥ 85%, Branch ≥ 75%
- MISRA C:2012 blocking — zero new violations allowed
- SIL nightly: full 7-ECU system test

## ECU Details

### Physical ECUs (Real Hardware)

| ECU | MCU | Role | Key Features |
|-----|-----|------|-------------|
| **CVC** | STM32G474RE | Central Vehicle Computer | Pedal sensing (SPI), state machine, CAN master, OLED display |
| **FZC** | STM32G474RE | Front Zone Controller | Steering servo, brake servo, LiDAR (UART), auto-brake on CAN timeout |
| **RZC** | STM32G474RE | Rear Zone Controller | Motor H-bridge (PWM), overcurrent protection (>10A), thermal derating |
| **SC** | TMS570LC43x | Safety Controller | Lockstep CPU, heartbeat monitor, kill relay, external watchdog |

### Simulated ECUs (Docker + SocketCAN)

| ECU | Role |
|-----|------|
| **BCM** | Body Control Module — lights, indicators, door locks |
| **ICU** | Instrument Cluster — dashboard gauges, DTC display |
| **TCU** | Telematics — UDS diagnostics, OBD-II, cloud telemetry |

All simulated ECUs run the same BSW stack on Linux with a POSIX MCAL backend.

## CAN Communication

- **CAN 2.0B** at 500 kbps, 24% worst-case bus load
- **E2E protection** on all safety-critical messages: CRC-8 + 4-bit alive counter + 100ms Rx timeout
- **31 message types**, 16 with E2E protection
- Key timing: pedal-to-motor < 12ms, E-stop propagation < 12ms, SC kill relay < 102ms

## Project Structure

```
taktflow-embedded/
├── firmware/
│   ├── cvc/, fzc/, rzc/, sc/      — Physical ECU firmware (src/, include/, cfg/, test/)
│   ├── bcm/, icu/, tcu/           — Simulated ECU firmware
│   └── shared/bsw/                — AUTOSAR-like BSW (18 modules)
│       ├── mcal/                  — Can, Spi, Adc, Pwm, Dio, Gpt
│       ├── ecual/                 — CanIf, PduR, IoHwAb
│       ├── services/              — Com, Dcm, Dem, WdgM, BswM, E2E
│       └── rte/                   — Runtime Environment
├── docker/                        — SIL environment (docker-compose, Caddyfile)
├── gateway/                       — Raspberry Pi edge gateway
│   ├── dashboard/                 — Live telemetry dashboard (vanilla JS)
│   ├── sap_qm_mock/              — SAP QM mock API
│   └── models/                    — ML anomaly detection models
├── hardware/                      — Pin mappings, BOM, schematics
├── test/mil/, sil/, pil/          — xIL testing
├── docs/
│   ├── safety/                    — ISO 26262 (15 documents)
│   │   ├── concept/               — Item definition, HARA, safety goals
│   │   ├── analysis/              — FMEA, DFA, hardware metrics
│   │   └── requirements/          — FSR, TSR, SSR, HSR
│   ├── aspice/                    — ASPICE 4.0 (25+ work products)
│   │   ├── system/                — SYS.1-3: requirements, architecture
│   │   ├── software/              — SWE.1-2: SW requirements, architecture
│   │   ├── verification/          — SWE.4-6: test plans, results, xIL
│   │   └── traceability/          — Bidirectional traceability matrix
│   └── plans/                     — Master plan (source of truth)
└── .github/workflows/             — CI: tests, MISRA, SIL nightly
```

## Build & Run

```bash
# Build all firmware (POSIX/SIL target)
make build

# Run all 1,067 unit tests
make test

# Start the SIL environment (all 7 ECUs + gateway + dashboard)
cd docker && docker compose up

# MISRA C:2012 analysis
make misra
```

## Hardware BOM

4 microcontroller boards, sensors, actuators, CAN adapters — full BOM in [`hardware/`](hardware/):
- 3x STM32G474RE Nucleo-64
- 1x TI TMS570LC43x LaunchPad (dual lockstep Cortex-R5F)
- 2x CANable 2.0 USB-CAN adapters
- Sensors: AS5048A (SPI angle), ACS723 (current), NTC (temp), TFMini-S (LiDAR), encoder
- Actuators: BTS7960 H-bridge, servo motors, relay, buzzer
- Total budget: $537–$937

## Safety Requirements Chain (Example)

```
SG-001: Prevent unintended acceleration (ASIL D)
  └── FSR-001: Motor shall stop within 100ms of pedal release
      └── TSR-001: CVC shall send zero torque on CAN within 10ms
          └── SSR-CVC-001: Swc_Pedal shall detect pedal release within one cycle
              └── test_pedal_release_detected_within_10ms() — PASS
```

Every safety goal follows this chain. Every test traces back to a requirement. Every requirement traces forward to a test.

## Documentation Index

| Area | Documents | Status |
|------|-----------|--------|
| ISO 26262 Safety | 15 (concept, plan, analysis, requirements, validation) | Draft |
| ASPICE Work Products | 25 (SYS, SWE, HWE, SUP, MAN) | Draft |
| Verification Reports | 14 (unit, integration, xIL) | Planned |
| Hardware Engineering | 3 (requirements, design, verification) | Draft |
| **Total** | **~70** | |

Full document registry: [`docs/INDEX.md`](docs/INDEX.md)

## Tech Stack

| Layer | Technology |
|-------|-----------|
| MCUs | STM32G474RE (Cortex-M4F, 170 MHz), TMS570LC43x (Cortex-R5F lockstep, 300 MHz) |
| Language | C11 (MISRA C:2012 compliant) |
| Build | GNU Make + arm-none-eabi-gcc / ti-cgt-arm |
| Test | Unity 2.6.0 + CMock |
| Static Analysis | cppcheck + MISRA addon |
| SIL Runtime | Docker + SocketCAN (vcan0) |
| Gateway | Python (Raspberry Pi 4) |
| Cloud | AWS IoT Core (MQTT/TLS) |
| Dashboard | Vanilla JS + WebSocket |
| ML | scikit-learn (anomaly detection, sub-ms inference) |
| CI/CD | GitHub Actions |

## Author

**Ngoc An Dao** — Embedded Systems / Functional Safety / ASPICE

Built as a portfolio project demonstrating end-to-end automotive development: from HARA and safety concept through architecture, implementation, and verification — with every artifact traceable and every claim backed by evidence.

## License

This project is proprietary. All rights reserved.
