# Plan: foxBMS 2 POSIX vECU (Virtual BMS)

## Goal
Run foxBMS 2 BMS application as a Linux process on the Ubuntu laptop or Netcup VPS, communicating via SocketCAN. No TMS570 hardware needed.

## Architecture

```
┌─────────────────────────────────────┐
│  foxbms-vecu (Linux process)        │
│                                     │
│  ┌─────────┐  ┌──────────────────┐  │
│  │ BMS App │  │ foxBMS Engine    │  │
│  │ bms.c   │  │ database, diag,  │  │
│  │ soa.c   │  │ sysm, algo      │  │
│  └────┬────┘  └────────┬─────────┘  │
│       │                │            │
│  ┌────┴────────────────┴─────────┐  │
│  │ POSIX HAL Layer               │  │
│  │ - can_posix.c (SocketCAN)     │  │
│  │ - os_posix.c (pthreads)       │  │
│  │ - stubs.c (SBC/SPS/PEX/FRAM) │  │
│  └───────────────┬───────────────┘  │
│                  │                  │
└──────────────────┼──────────────────┘
                   │ SocketCAN (vcan0 or can0)
                   │
         ┌─────────┴─────────┐
         │ Plant Model /     │
         │ HIL Bench / Other │
         │ vECUs             │
         └───────────────────┘
```

## Phases

### Phase 1: Identify all HAL dependencies (RESEARCH)
- List every HAL/MCAL function called by foxBMS application code
- Categorize: CAN, GIO, SPI, I2C, RTI, ADC, OS/FreeRTOS, ESM
- Determine which are already stubbed by unit test mocks

### Phase 2: POSIX HAL layer (IMPLEMENT)
- **os_posix.c**: Replace FreeRTOS with pthreads
  - `OS_InitializeOperatingSystem()` → create pthreads
  - `OS_IncrementTimer()` → gettimeofday
  - `OS_EnterTaskCritical()` → pthread_mutex
  - Task delays → usleep/nanosleep
  - Queues → POSIX message queues or pipe
- **can_posix.c**: SocketCAN wrapper
  - `canInit()` → open SocketCAN socket
  - `canTransmit()` → write to socket
  - `CAN_ReadRxBuffer()` → non-blocking read from socket
- **stubs.c**: No-op returns for all missing hardware
  - SBC_Trigger → return OK
  - FRAM_Initialize/Read/Write → static RAM buffer
  - SPS_Initialize/Ctrl → return OK
  - PEX_Initialize/Trigger → return OK
  - ADC/SPI/I2C/GIO → no-op
  - LED → printf

### Phase 3: Build system (IMPLEMENT)
- Makefile.posix for foxBMS vECU
- Compile foxBMS app sources with GCC (x86_64)
- Link with POSIX HAL layer instead of HALCoGen
- Define `FOXBMS_POSIX` to select POSIX code paths

### Phase 4: Integration test (VERIFY)
- Start foxbms-vecu on vcan0
- Send fake cell voltages (0x270), temps (0x280), current (0x521)
- Verify foxBMS sends BMS state (0x220), SOC (0x235) back
- Verify fault detection: send out-of-range voltage → check fault state

### Phase 5: Docker + HIL bench (DEPLOY)
- Dockerfile for foxbms-vecu
- Add to docker-compose alongside CVC/FZC/RZC SIL
- Connect to real CAN bus via canable for HIL testing

## Key foxBMS Source Files to Include

### Application (unchanged)
- `src/app/application/bms/bms.c` — BMS state machine
- `src/app/application/algorithm/` — SOC, SOE, SOF
- `src/app/application/bal/` — balancing
- `src/app/application/plausibility/` — cell checks
- `src/app/application/redundancy/` — redundancy
- `src/app/application/soa/` — safe operating area

### Engine (unchanged)
- `src/app/engine/database/` — database manager
- `src/app/engine/diag/` — diagnostics
- `src/app/engine/sys/` — system state machine
- `src/app/engine/config/` — system config

### Drivers (need POSIX replacement)
- `src/app/driver/can/` — CAN driver → SocketCAN
- `src/app/driver/config/` — CAN message configs (keep)
- `src/app/driver/afe/debug/can/` — debug CAN AFE (keep, reads from CAN)
- `src/app/driver/sbc/` → stub
- `src/app/driver/sps/` → stub
- `src/app/driver/pex/` → stub
- `src/app/driver/fram/` → RAM buffer stub
- `src/app/driver/rtc/` → gettimeofday
- `src/app/driver/led/` → printf

### OS (need POSIX replacement)
- `src/app/task/` — task definitions → pthreads
- `src/os/freertos/` → POSIX OS layer

## Estimated Effort
- Phase 1: 1 hour (code reading)
- Phase 2: 4-6 hours (main work)
- Phase 3: 1 hour (Makefile)
- Phase 4: 1-2 hours (testing)
- Phase 5: 1 hour (Docker)

## Status
- [ ] Phase 1: PENDING
- [ ] Phase 2: PENDING
- [ ] Phase 3: PENDING
- [ ] Phase 4: PENDING
- [ ] Phase 5: PENDING
