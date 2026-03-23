# Plan: HIL Mixed Bench — 3 vECU (Laptop) + 4 Physical ECU

**Status**: IN PROGRESS
**Branch**: `feat/hil-mixed-bench`
**Traces**: TSR-005 through TSR-046, SSR-CVC/FZC/RZC/SC, SWR-BCM/ICU/TCU
**Prerequisite**: SIL 16/16 PASS (commit 5dee0e5), plan-hil-test-suite (Phases 1-3 DONE)

## Problem

SIL is complete — all 7 ECUs pass as POSIX binaries on vcan0. The existing HIL
plan (plan-hil-test-suite) tested only 3 simulated ECUs + plant-sim. Now we need
a real mixed HIL bench:

- **Laptop (Ubuntu)**: 3 vECUs (BCM, TCU, ICU) as POSIX Docker containers
- **Physical hardware**: 4 ECUs on real MCU targets
  - CVC → STM32G474RE (Nucleo-64)
  - FZC → STM32G474RE (Nucleo-64)
  - RZC → STM32G474RE (Nucleo-64)
  - SC  → TMS570LC4357 (LaunchPad)
- **CAN bridge**: vcan0 (laptop Docker) ↔ can0 (physical CAN bus via USB-CAN)

This is the first time physical ECUs participate in the system test. The SIL vECU
code for BCM/ICU/TCU is directly reusable — same POSIX binaries, just bridged to
physical CAN instead of all-virtual.

## Topology

```
┌─────────────────────────────────────────────┐
│                 LAPTOP (Ubuntu)              │
│                                              │
│  ┌─────────┐ ┌─────────┐ ┌─────────┐        │
│  │ BCM     │ │ TCU     │ │ ICU     │ vECU   │
│  │ (POSIX) │ │ (POSIX) │ │ (POSIX) │ Docker │
│  └────┬────┘ └────┬────┘ └────┬────┘        │
│       └──────┬─────┘──────────┘              │
│              │ vcan0                         │
│       ┌──────┴──────┐                        │
│       │ CAN Bridge  │ vcan0 ↔ can0           │
│       │ (candump/   │                        │
│       │  cangw)     │                        │
│       └──────┬──────┘                        │
│              │ can0 (USB-CAN adapter)        │
└──────────────┼───────────────────────────────┘
               │ CAN bus 500 kbit/s
    ┌──────────┼──────────────────────┐
    │          │   Physical CAN Bus    │
    │  ┌───┴───┐ ┌───┴───┐ ┌───┴───┐ ┌───┴───┐
    │  │  CVC  │ │  FZC  │ │  RZC  │ │  SC   │
    │  │STM32  │ │STM32  │ │STM32  │ │TMS570 │
    │  │G474RE │ │G474RE │ │G474RE │ │LC4357 │
    │  └───────┘ └───────┘ └───────┘ └───────┘
    └─────────────────────────────────────────┘
```

## What Already Exists

| Component | Location | Status |
|-----------|----------|--------|
| SIL POSIX binaries (all 7 ECUs) | `firmware/platform/posix/` | DONE — 16/16 SIL pass |
| Docker vECU build | `docker/ecu/Dockerfile.ecu` | DONE |
| docker-compose.laptop.yml | `docker/docker-compose.laptop.yml` | DONE — BCM/TCU/ICU + plant-sim + MQTT |
| HIL runner + 26 YAML scenarios | `test/hil/` | DONE (Phases 1-3) |
| STM32 Makefile | `firmware/platform/stm32/Makefile.stm32` | DONE |
| TMS570 Makefile | `firmware/platform/tms570/Makefile.tms570` | DONE |
| OpenOCD config | `firmware/platform/stm32/openocd.cfg` | DONE |
| Waveshare USB-CAN test | `test/hil/test_can_rates.py` | DONE |

## Phases

| # | Phase | Status |
|---|-------|--------|
| 1 | CAN bridge setup (vcan0 ↔ can0) | PENDING |
| 2 | Physical ECU flash scripts | PENDING |
| 3 | Update docker-compose for mixed topology | PENDING |
| 4 | HIL Python test files (ported from SIL) | PENDING |
| 5 | HIL orchestration script (run_hil.sh) | PENDING |
| 6 | Run and validate all tests | PENDING |

---

### Phase 1: CAN Bridge Setup

**What**: Bridge vcan0 (Docker vECUs) ↔ can0 (physical CAN via USB-CAN adapter)
so all 7 ECUs share one logical CAN bus.

**Approach**: Use Linux `cangw` (kernel CAN gateway) for zero-copy forwarding.

**Files**:

| File | Change |
|------|--------|
| `scripts/hil/setup_can_bridge.sh` | NEW — create vcan0, can0, cangw rules |

---

### Phase 2: Physical ECU Flash Scripts

**What**: Scripts to build and flash CVC/FZC/RZC (STM32) and SC (TMS570).

**Files**:

| File | Change |
|------|--------|
| `scripts/hil/flash_stm32.sh` | NEW — build + flash one STM32 ECU via OpenOCD |
| `scripts/hil/flash_tms570.sh` | NEW — build + flash SC via UniFlash/CCS |
| `scripts/hil/flash_all.sh` | NEW — flash all 4 physical ECUs |

---

### Phase 3: Update Docker Compose

**What**: Update docker-compose.laptop.yml for mixed bench — vECUs on vcan0,
plant-sim on vcan0, MQTT on host. Remove QNX relay (not needed — physical
ECUs connect directly via CAN bridge).

**Files**:

| File | Change |
|------|--------|
| `docker/docker-compose.hil.yml` | NEW — mixed bench compose |

---

### Phase 4: HIL Python Test Files

**What**: Port SIL test patterns to HIL. The SIL tests use vcan0 + Docker;
HIL tests use can0 (physical) with tighter timing tolerances.

Key tests to implement:

| Test File | SIL Source | What It Tests |
|-----------|-----------|---------------|
| `test_hil_vsm.py` | `test_vsm_fault_transitions.py` | VSM state machine on physical CVC |
| `test_hil_overtemp.py` | `test_overtemp_hops.py` | Overtemp chain: RZC→CVC→FZC→SC |
| `test_hil_battery.py` | `test_battery_chain.py` | Battery fault chain across physical ECUs |
| `test_hil_heartbeat.py` | (new) | Heartbeat verification on real hardware |
| `test_hil_e2e.py` | (new) | E2E CRC on real CAN bus |
| `test_hil_body.py` | (new) | BCM/ICU/TCU vECU interactions with physical ECUs |

**Key differences from SIL**:
- `CAN_INTERFACE=can0` (physical) instead of `vcan0`
- Timing tolerance: ±5ms (bare-metal) vs ±100ms (VPS SIL)
- SC is real (TMS570) — system can reach RUN state (not stuck in SAFE_STOP)
- MQTT fault injection through laptop mosquitto (bridged)

**Files**:

| File | Change |
|------|--------|
| `test/hil/test_hil_vsm.py` | NEW |
| `test/hil/test_hil_overtemp.py` | NEW |
| `test/hil/test_hil_battery.py` | NEW |
| `test/hil/test_hil_heartbeat.py` | NEW |
| `test/hil/test_hil_e2e.py` | NEW |
| `test/hil/test_hil_body.py` | NEW |
| `test/hil/hil_test_lib.py` | NEW — shared HIL test library (forked from sil_test_lib.py) |

---

### Phase 5: HIL Orchestration Script

**What**: Single script to set up and run the full HIL bench.

Sequence:
1. Set up CAN bridge (vcan0 ↔ can0)
2. Flash physical ECUs (if --flash flag)
3. Start Docker vECUs + plant-sim + MQTT
4. Wait for all ECUs to boot (heartbeat check)
5. Run HIL test suite
6. Collect results + JUnit XML
7. Tear down

**Files**:

| File | Change |
|------|--------|
| `scripts/hil/run_hil.sh` | NEW — full HIL orchestration |

---

### Phase 6: Validate

**What**: Run all tests on real bench, fix failures, document results.

**Expected topology verification**:

| CAN ID | Source | Type | Expected |
|--------|--------|------|----------|
| 0x001 | CVC (physical) | EStop | 10ms |
| 0x010 | CVC (physical) | Heartbeat | 50ms |
| 0x011 | FZC (physical) | Heartbeat | 50ms |
| 0x012 | RZC (physical) | Heartbeat | 50ms |
| 0x013 | SC (physical) | Status | 100ms |
| 0x100 | CVC (physical) | Vehicle_State | 100ms |
| 0x200 | FZC (physical) | Steering_Status | 50ms |
| 0x201 | FZC (physical) | Brake_Status | 50ms |
| 0x300 | RZC (physical) | Motor_Status | 100ms |
| 0x301 | RZC (physical) | Motor_Current | 50ms |
| 0x302 | RZC (physical) | Motor_Temp | 1000ms |
| 0x303 | RZC (physical) | Battery_Status | 500ms |
| 0x360 | BCM (vECU) | Body_Status | 100ms |
| 0x500 | any | DTC_Broadcast | on-event |
| 0x600 | FZC (physical) | FZC_Virtual_Sensors | 50ms |
| 0x601 | RZC (physical) | RZC_Virtual_Sensors | 50ms |

---

## Risk Assessment

| Risk | Mitigation |
|------|------------|
| CAN bridge latency | cangw is kernel-space, <1ms. Measure with test_can_rates.py. |
| STM32 flash failures | OpenOCD with SWD — well-tested on Nucleo boards |
| TMS570 big-endian byte order issues | SC code already handles endianness. DBC is LE for all STM32 signals. |
| SC relay confirmation on TMS570 | SC sends 0x013 SC_Status with relay state — CVC needs this to leave SAFE_STOP |
| Plant-sim on vcan0 vs physical sensors | Plant-sim injects virtual sensor frames (0x600/0x601) via CAN bridge to physical ECUs |
| Timing jitter on laptop Docker | vECU timing less critical (QM); safety-critical timing is on bare-metal (physical ECUs) |

## Files Summary

| File | Change |
|------|--------|
| `scripts/hil/setup_can_bridge.sh` | NEW |
| `scripts/hil/flash_stm32.sh` | NEW |
| `scripts/hil/flash_tms570.sh` | NEW |
| `scripts/hil/flash_all.sh` | NEW |
| `scripts/hil/run_hil.sh` | NEW |
| `docker/docker-compose.hil.yml` | NEW |
| `test/hil/hil_test_lib.py` | NEW |
| `test/hil/test_hil_vsm.py` | NEW |
| `test/hil/test_hil_overtemp.py` | NEW |
| `test/hil/test_hil_battery.py` | NEW |
| `test/hil/test_hil_heartbeat.py` | NEW |
| `test/hil/test_hil_e2e.py` | NEW |
| `test/hil/test_hil_body.py` | NEW |
| `docs/plans/plan-hil-mixed-bench.md` | THIS PLAN |

**Total**: ~14 new files, 0 modifications to existing code.
