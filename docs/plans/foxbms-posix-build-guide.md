# foxBMS POSIX vECU — Build & Setup Guide

## Prerequisites
- Ubuntu 24.04 (laptop at 192.168.0.158)
- GCC 13.3 (`sudo apt install gcc`)
- Python 3.12 (`sudo apt install python3`)
- foxBMS 2 v1.10.0 cloned at `/home/an-dao/foxbms-2`
- vcan1 interface (`sudo ip link add vcan1 type vcan && sudo ip link set vcan1 up`)
- HALCoGen headers copied from Windows build at `build/app_host_unit_test/include/`

## Directory Structure
```
foxbms-2/
├── posix/                          ← Our POSIX port (not upstream)
│   ├── Makefile                    ← Build system
│   ├── foxbms_posix_main.c         ← Entry point (replaces main.c)
│   ├── hal_stubs_posix.c           ← 80+ HAL function stubs
│   ├── posix_overrides.h           ← Force-included, overrides ARM asm/asserts
│   ├── config_cpu_clock_hz.h       ← CPU clock for FreeRTOS config
│   └── plant_model.py              ← Python script sending fake battery data
├── build/app_host_unit_test/include/ ← HALCoGen headers (from Windows)
└── src/                            ← foxBMS source (upstream + patches)
```

## Source File Patches Applied (on laptop, not committed upstream)

### foxBMS Source Modifications
1. **src/app/driver/sbc/sbc.c** — `SBC_GetState` returns RUNNING, `SBC_SetStateRequest` returns OK on `#ifdef FOXBMS_POSIX`
2. **src/app/driver/rtc/rtc.c** — `RTC_IsRtcModuleInitialized` returns true on POSIX
3. **src/app/driver/can/can.c** — `CAN_IsCurrentSensorPresent/Cc/Ec` return true on POSIX
4. **src/app/task/config/ftask_cfg.c** — Debug trace prints in engine/precyclic init
5. **src/app/engine/sys/sys.c** — SYS_Trigger state trace (first 50 + every 500th call)
6. **src/app/engine/database/database.c** — `DATA_IterateOverDatabaseEntries` made non-static on POSIX

### HALCoGen Header Patches
All `HL_reg_*.h` files in `build/app_host_unit_test/include/` patched:
- Every `#define xxxREG ((type *)0xFFF...)` wrapped with `#ifdef FOXBMS_POSIX` to redirect to RAM buffers
- ~60+ register base macros patched (canREG1-4, systemREG1-2, spiREG1-5, i2cREG1-2, etc.)

### Patch Scripts (stored in /tmp/ on laptop)
- `/tmp/patch_sbc.py` — SBC_GetState bypass
- `/tmp/patch_sbc2.py` — SBC_SetStateRequest bypass
- `/tmp/patch_rtc.py` — RTC init bypass
- `/tmp/patch_ftask.py` — ftask_cfg.c trace prints
- `/tmp/patch_sys2.py` — sys.c state trace
- `/tmp/patch_canreg.py` — CAN register redirect
- `/tmp/patch_all_regs.py` — All register redirects
- `/tmp/patch_can_sensor.py` — Current sensor bypass
- `/tmp/patch_database.py` — Database iterate extern

## Source Files Excluded from Build

| File | Reason |
|------|--------|
| `main.c` | Replaced by foxbms_posix_main.c |
| `fstartup.c` | TMS570 startup assembly |
| `io.c` | Direct GIO register dereference |
| `crc.c` | Hardware CRC peripheral → software stub |
| `spi.c`, `spi_cfg.c` | SPI register access |
| `dma.c` | DMA register access |
| `i2c.c` | I2C register access |
| `fram.c` | SPI FRAM → RAM stub |
| `sps.c`, `sps_cfg.c` | Smart Power Switch (GIO register access) |
| `pex.c`, `pex_cfg.c` | Port Expander (I2C) |
| `htsensor.c` | Humidity/Temp sensor (I2C) |
| `sbc/*` | NXP FS85xx Safety Basis Chip (SPI) |
| `diag.c` | Diagnostics → always-OK stub |
| `os_freertos.c` | FreeRTOS OS wrapper → cooperative stubs |
| `ftask_freertos.c` | FreeRTOS task/queue creation |
| AFE variants | adi, nxp, maxim, ltc, ti, debug/default |
| SOC/SOE/SOH variants | debug, none (keep counting) |
| TS variants | All except epcos b57251v5103j060 |
| ethernet, bal variants | Not needed |

## Key Design Decisions

### No FreeRTOS Scheduler
- Cooperative mode: `while(1)` loop calls cyclic functions at 1ms/10ms/100ms
- FreeRTOS kernel compiled but scheduler never started
- Queue operations go through ring buffers in hal_stubs_posix.c
- `OS_DelayTaskUntil` → `usleep()`
- `OS_GetTickCount` → `clock_gettime(CLOCK_MONOTONIC)`

### FAS_ASSERT = NO_OPERATION
- `posix_overrides.h` sets `FAS_ASSERT_LEVEL = 2` (no-op)
- Without this, ~100+ assertions halt the process on missing hardware
- foxBMS continues past all hardware checks

### DIAG_Handler = Always OK
- `diag.c` excluded, stub returns 0 (DIAG_HANDLER_RETURN_OK)
- Prevents ERROR state transitions from hardware-absent checks

### CAN via SocketCAN
- `canInit()` opens SocketCAN via `FOXBMS_CAN_IF` env var (default: vcan1)
- `canTransmit()` routes to `posix_can_send()` via SocketCAN
- CAN RX: main loop reads SocketCAN → ring buffer → `OS_ReceiveFromQueue`

### Hardware Register RAM Buffers
- All TMS570 register bases (0xFFF7...) redirected to 4KB RAM buffers
- foxBMS writes to RAM instead of hardware — harmless but functional
- ~60 buffers: posix_canreg1, posix_systemreg1, posix_spireg1, etc.

## Build Commands
```bash
cd /home/an-dao/foxbms-2/posix
make clean && make -j4
```

## Run Commands
```bash
# Create vcan1
sudo ip link add vcan1 type vcan
sudo ip link set vcan1 up

# Start plant model (optional — sends fake battery data)
python3 plant_model.py vcan1 &

# Run foxBMS
FOXBMS_CAN_IF=vcan1 timeout 15 ./foxbms-vecu

# Monitor CAN output
candump vcan1 -t z
```

## Reapplying Patches After git checkout
```bash
cd /home/an-dao/foxbms-2
python3 /tmp/patch_sbc.py
python3 /tmp/patch_sbc2.py
python3 /tmp/patch_rtc.py
python3 /tmp/patch_ftask.py
python3 /tmp/patch_can_sensor.py
python3 /tmp/patch_sys2.py
sed -i '57a\#include <stdio.h>' src/app/engine/sys/sys.c
```

## Current Status (2026-03-21)
- **15 CAN message types** transmitted periodically
- **SYS state**: RUNNING (state=5)
- **BMS state**: IDLE (CAN state 3)
- **SOC**: 50% (default counting method)
- **CAN RX**: Working — IVT messages received
- **Blocker**: BMS doesn't transition from IDLE because database queue is stubbed
- **Next**: Fix database passthrough so state requests reach BMS_Trigger
