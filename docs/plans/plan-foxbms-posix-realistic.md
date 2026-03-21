# foxBMS POSIX vECU — Realistic Simulation Plan

## Current Stubs vs Realistic Replacements

### Category 1: Must Be Realistic (affects BMS behavior)

| Component | Current Stub | Realistic Implementation | Priority |
|-----------|-------------|------------------------|----------|
| **Contactor feedback** | `SPS_GetChannelPexFeedback` returns 0 (OPEN) | Return CLOSED when foxBMS requests close. Track contactor state in plant model. | P1 — blocks NORMAL state |
| **Contactor close** | `SPS_RequestContactorState` no-op | Set internal state, plant model tracks open/close | P1 |
| **Cell voltages** | None sent (plant model sends IVT only) | Plant sends 0x270 with muxed cell voltages (3.7V ± noise) | P2 — needed for plausibility |
| **Cell temperatures** | None sent | Plant sends 0x280 with muxed temps (25°C ± noise) | P2 |
| **Current sensor** | Plant sends 0A static | Dynamic current based on contactor state and load model | P3 |
| **Pack voltage** | Plant sends 22.2V static | Dynamic based on cell voltages × string count | P3 |
| **Interlock** | `ILCK_SetStateRequest` stubbed | Return interlock CLOSED, simulate interlock feedback | P2 |
| **DIAG_Handler** | Always returns OK | Allow actual error detection but filter hardware-absent errors | P4 |

### Category 2: Keep As Stubs (hardware-only, no BMS impact)

| Component | Why Stub Is OK |
|-----------|---------------|
| SPI drivers | No SPI hardware on POSIX, all SPI peripherals stubbed at higher layer |
| I2C drivers | No I2C hardware |
| DMA | No DMA |
| LED | Printf debug output instead |
| FRAM | RAM buffer (data lost on restart, acceptable for simulation) |
| PEX (port expander) | Only used for contactor feedback → handled by SPS stub |
| HT sensor | Temperature from CAN instead |
| RTC | System clock via `clock_gettime` |
| CRC | Software CRC (already implemented) |
| Hardware registers | RAM buffers (60+ register bases redirected) |
| FAS_ASSERT | NO_OPERATION level (hardware checks will always fail) |

### Category 3: Should Be Realistic But Can Wait

| Component | Current State | Future Work |
|-----------|--------------|-------------|
| **SOC estimation** | Counting from 50% with 0A current → stays at 50% | Dynamic current → SOC changes over time |
| **Balancing** | `BAL_Trigger` runs but no cell equalization | Simulate cell voltage changes from balancing |
| **SOF (State of Function)** | Calculates but all cells identical | Vary cell impedance for realistic power limits |
| **IMD (Insulation Monitoring)** | Set to "none" | Not needed for battery simulation |

## Implementation Steps

### Step 1: Contactor state tracking (P1)
**Goal**: foxBMS requests contactor close → SPS stub tracks state → feedback returns CLOSED

**Changes**:
- `hal_stubs_posix.c`: `SPS_RequestContactorState` stores requested state per channel
- `hal_stubs_posix.c`: `SPS_GetChannelPexFeedback` returns stored state (CLOSED if requested)
- No plant model change needed — contactor control is internal to foxBMS

**Test**: BMS transitions IDLE → STANDBY → PRECHARGE → NORMAL

### Step 2: Interlock feedback (P2)
**Goal**: Interlock always reports CLOSED

**Changes**:
- Check `ILCK_Trigger` and `ILCK_SetStateRequest` — ensure interlock feedback returns CLOSED
- May need to set `cont_contactorStates` interlock entry to CLOSED

**Test**: No interlock error in SYS state machine

### Step 3: Cell voltage injection from plant model (P2)
**Goal**: Plant model sends realistic 0x270 messages with 6 cells at 3.7V

**Changes**:
- `plant_model.py`: Add 0x270 cell voltage messages with correct mux/encoding
- Mux byte selects cell group (0 = cells 0-3, 1 = cells 4-5)
- 4 voltages per message, 13-bit resolution, big-endian
- Add small random noise (±10mV) for realism

**Test**: foxBMS database shows cell voltages, plausibility passes

### Step 4: Cell temperature injection (P2)
**Goal**: Plant sends 0x280 with 25°C temperatures

**Changes**:
- `plant_model.py`: Add 0x280 temperature messages
- Similar mux/encoding as voltages

**Test**: foxBMS temperature data populated

### Step 5: Dynamic current model (P3)
**Goal**: Current changes based on contactor state and simulated load

**Changes**:
- `plant_model.py`: Track contactor state from foxBMS CAN TX (0x240)
- When contactors closed: simulate 10A discharge current
- When contactors open: 0A
- Update IVT current (0x521) accordingly

**Test**: SOC decreases during discharge (0x235 changes)

### Step 6: Dynamic pack voltage (P3)
**Goal**: Pack voltage = sum of cell voltages, varies with current (IR drop)

**Changes**:
- `plant_model.py`: Calculate pack voltage from cell model
- V_pack = N_cells × V_cell - I × R_internal
- Update IVT voltage (0x522) accordingly

**Test**: Pack voltage in 0x233 matches model

### Step 7: Fault injection scenarios (P4)
**Goal**: Test foxBMS response to realistic fault conditions

Scenarios:
1. **Overvoltage**: Set one cell to 4.5V → foxBMS opens contactors
2. **Undervoltage**: Set one cell to 2.5V → foxBMS opens contactors
3. **Overtemperature**: Set one sensor to 60°C → foxBMS warns, 80°C → opens
4. **Overcurrent**: Set current to 200A → foxBMS opens contactors
5. **Cell imbalance**: Set cells to 3.5V, 3.7V, 3.9V → balancing activates
6. **Sensor failure**: Send invalid flag in 0x270 → foxBMS detects missing cell

**Test**: Each fault → correct foxBMS response on 0x220

### Step 8: Restore selective DIAG_Handler (P4)
**Goal**: DIAG_Handler returns real results for software checks, OK for hardware checks

**Changes**:
- Classify DIAG IDs: software (keep) vs hardware (suppress)
- `DIAG_Handler` stub checks ID range and returns real result for software IDs

**Test**: Software faults detected, hardware-absent errors suppressed

## Architecture

```
┌─────────────────────────────────────────────┐
│  plant_model.py (Python)                     │
│  ┌─────────────┐  ┌──────────────────────┐  │
│  │ Cell Model   │  │ Contactor Feedback   │  │
│  │ 6× 3.7V ±   │  │ Track open/close     │  │
│  │ noise        │  │ from foxBMS 0x240    │  │
│  └──────┬───────┘  └──────────┬───────────┘  │
│         │ 0x270/280/521/522    │ contactor    │
│         ▼                      │ state        │
└─────────┼──────────────────────┼─────────────┘
          │ SocketCAN (vcan1)    │
┌─────────┼──────────────────────┼─────────────┐
│  foxbms-vecu (C)               │              │
│  ┌──────┴───────┐  ┌──────────┴───────────┐  │
│  │ CAN RX       │  │ SPS Stub             │  │
│  │ → Database   │  │ contactor feedback    │  │
│  │ → BMS Logic  │  │ from plant state      │  │
│  └──────────────┘  └─────────────────────┘  │
│  ┌──────────────┐  ┌─────────────────────┐  │
│  │ BMS State    │  │ CAN TX               │  │
│  │ Machine      │──│ 0x220 State          │  │
│  │ SOC/SOE/SOF  │  │ 0x235 SOC            │  │
│  └──────────────┘  │ 0x240 Contactors     │  │
│                    └─────────────────────┘  │
└─────────────────────────────────────────────┘
```

## Status
- [x] Step 1: PENDING (contactor feedback)
- [ ] Step 2: PENDING
- [ ] Step 3: PENDING
- [ ] Step 4: PENDING
- [ ] Step 5: PENDING
- [ ] Step 6: PENDING
- [ ] Step 7: PENDING
- [ ] Step 8: PENDING
