# 2026-03-23 — HIL Bringup: STM32F4 RZC Silent on CAN Bus

## Context

First HIL mixed-bench bringup — 3 STM32 ECUs (CVC G474, FZC G474, RZC F413) + SC TMS570 on physical CAN bus at 500 kbps. SIL (all POSIX on vcan0) passes 16/16. Goal: get all 4 physical ECUs transmitting.

## Mistakes / Root Causes Found

### 1. Shared `build/stm32/` directory between CVC and FZC targets

**Symptom**: FZC firmware compiled and flashed successfully but never transmitted CAN frames (`0x011` absent). CVC firmware on the same board DID transmit.

**Root cause**: The STM32 Makefile uses `build/stm32/` for all G4 targets. Building CVC then FZC without `make clean` between them reuses BSW `.o` files compiled with CVC's include paths (`-I$(ECU)/cvc/cfg/platform_target`). Make sees the `.o` files as up-to-date (same `.c` source, same timestamp) but the include path changed — the config headers resolve differently per ECU. Result: FZC binary contains BSW objects compiled against CVC config.

**Fix**: Always `make clean` between target builds. Long-term: change `BUILD_DIR` to `build/stm32/$(TARGET)` to isolate per-target.

### 2. Missing `SysTick_Handler` on STM32F4 OSEK build

**Symptom**: RZC F413 firmware flashed and verified, `st-flash` reported success, but firmware never reached `main()`. All BSW init flags (`com_initialized`, `det_initialized`, `rte_initialized`) were 0. CAN registers showed reset defaults (SLEEP=1, BTR unconfigured). Board appeared completely dead.

**Root cause**: `stm32f4xx_it.c` (CubeMX-generated) had `SysTick_Handler` commented out with a note "provided by Os_Port_Stm32_Asm.S". But the OSEK bootstrap OS port does NOT define `SysTick_Handler` — only the ThreadX port does (in `tx_initialize_low_level.S`, compiled only with `THREADX=1`). The startup assembly's vector table had `SysTick_Handler` as a weak alias to `Default_Handler` (infinite loop).

**Chain of failure**:
1. `HAL_Init()` configures SysTick at 1ms
2. SysTick fires → `Default_Handler` → infinite loop
3. `HAL_IncTick()` never called → `HAL_GetTick()` returns 0 forever
4. `Ecu_SystemClock_Config()` calls `HAL_RCC_OscConfig()` → polls `HAL_GetTick()` for PLL lock timeout → times out immediately (tick stuck at 0)
5. `Error_Handler()` called → `__disable_irq(); for(;;){}` → firmware dead
6. `main()` never reached, CAN never initialized

**Fix**: Added `void SysTick_Handler(void) { HAL_IncTick(); }` to `stm32f4xx_it.c`.

**Why SIL worked**: POSIX build uses `clock_gettime(CLOCK_MONOTONIC)` for timing — no SysTick dependency. All `Main_Hw_*` functions are stubs returning `E_OK`.

### 3. Self-test blocking CAN transmission on FZC and RZC

**Symptom**: FZC and RZC firmware ran but never entered BswM RUN mode, so Com never started transmitting periodic frames.

**Root cause**: Self-test functions (`Main_Hw_ServoNeutralTest`, `Main_Hw_SpiSensorTest`, `Main_Hw_UartLidarTest` for FZC; `Main_Hw_Bts7960GpioTest`, `Main_Hw_Acs723ZeroCalTest`, `Main_Hw_NtcRangeTest`, `Main_Hw_EncoderStuckTest` for RZC) check for physical sensors that don't exist on bare Nucleo boards. Any failure returns `FZC_SELF_TEST_FAIL` / `RZC_SELF_TEST_FAIL`, BswM stays in STARTUP, Com never starts.

**Fix**: Wrapped sensor-dependent self-tests in `#ifndef PLATFORM_HIL` and added `#ifdef PLATFORM_HIL self_test_result = PASS` bypass. Build with `HIL=1` flag. Also wrapped the entire `Main_RunSelfTest()` function definition to avoid `-Werror=unused-function`.

**Why SIL worked**: POSIX hw stubs (`fzc_hw_posix.c`, `rzc_hw_posix.c`) return `E_OK` for all self-test functions.

### 4. bxCAN sleep mode deadlock on STM32F4

**Symptom**: After fixing SysTick, RZC reached `main()` but `Can_Hw_Init()` returned `E_NOT_OK`. `can_state` stayed at `CAN_CS_UNINIT`. CAN registers showed SLEEP=1, BTR unconfigured.

**Root cause**: STM32F4 bxCAN starts in SLEEP mode after reset. `HAL_CAN_Init()` sets `INRQ` to enter init mode but does NOT clear `SLEEP` first. The bxCAN won't respond to `INRQ` while asleep — it needs the CAN_RX pin to see 11 consecutive recessive bits to exit sleep (or `AWUM` to be set, which requires init mode to configure).

**Fix**: Bare-metal `Can_Hw_Init()` that enables CAN1 clock, configures GPIO, then writes `MCR` with `SLEEP=0 + INRQ=1` simultaneously, waits for `INAK=1 && SLAK=0`, then configures BTR and filters directly. No HAL dependency for CAN.

**Why SIL worked**: POSIX CAN uses `socket(PF_CAN) + bind()` — no sleep mode, no init mode, immediately active.

### 5. Makefile missing BSW modules (CanSM, FiM, Xcp, SchM_Timing)

**Symptom**: Link errors for `CanSM_Init`, `FiM_Init`, `Xcp_Init`, `SchM_TimingInit` when building for STM32 target.

**Root cause**: New BSW modules were added to the firmware but their include paths and source files were not added to `Makefile.stm32` and `Makefile.stm32f4`.

**Fix**: Added `-I$(BSW)/services/CanSM/include`, `-I$(BSW)/services/FiM/include`, `-I$(BSW)/services/Xcp/include` to CFLAGS, and corresponding `.c` files to `BSW_SRCS`.

### 6. IoHwAb_Hil.c is a replacement, not an addition

**Symptom**: Multiple definition errors when building with `HIL=1` — `IoHwAb_Init`, `IoHwAb_ReadMotorTemp`, etc. defined in both `IoHwAb.c` and `IoHwAb_Hil.c`.

**Root cause**: `IoHwAb_Hil.c` reimplements all IoHwAb functions with CAN-injectable sensor values. It's a full replacement for `IoHwAb.c`, not an addition.

**Fix**: Used Make conditional `$(if $(HIL),IoHwAb_Hil.c,IoHwAb.c)` in BSW_SRCS.

### 7. OpenOCD 0.12.0 can't flash STM32G4 via ST-LINK V2

**Symptom**: `program` command fails with "Cannot identify target as STM32G0/G4/L4..." and "auto_probe failed".

**Root cause**: OpenOCD 0.12.0's `stm32g4x.cfg` reads FLASH_ACR at `0x40022000` during `reset-init` event. The ST-LINK V2 API can't read this register reliably.

**Fix**: Used `st-flash` (stlink-tools v1.8.0) instead of OpenOCD for flashing. SC (TMS570) uses DSLite from CCS.

## Principles

- **SIL ≠ HIL**: POSIX stubs hide all hardware dependencies. Every `Main_Hw_*` function is a potential failure point on real hardware.
- **Interrupt handlers are load-bearing**: A missing `SysTick_Handler` silently kills the entire HAL timing infrastructure. The error manifests as "firmware doesn't boot" with no debug output.
- **Shared build directories cause silent miscompilation**: Two targets with different include paths but shared object files produce binaries that link but don't work.
- **Read the registers**: When firmware appears dead, read MCR/MSR/ESR/BTR directly via debugger. The register values tell exactly what happened.
- **Trace the data flow**: Compare working (SIL) vs broken (HIL) path function by function, from SWC down to hardware register. The break point is always at the platform abstraction boundary.

### 8. STM32F413 HSI clock drift kills CAN bus at 500 kbps

**Symptom**: F413 bxCAN transmits frames but every other CAN node (gs_usb adapter, G474 FDCAN boards) goes error-passive within seconds. Waveshare passive sniffer sees all frames fine. CAN bit timing registers show correct 500 kbps config. Pull-up resistor on CAN_TX didn't help.

**Root cause**: The F413 firmware used **HSI (internal RC oscillator, ±1%)** as PLL source. CAN specification requires **<±0.5% oscillator tolerance** at 500 kbps. At 1% drift, every frame from the bxCAN has subtly wrong bit timing — each bit is ~10ns too long or too short. Active nodes (gs_usb, FDCAN) detect bit errors on every frame, REC climbs to 128+ → error-passive.

**Why it was hard to find**:
- bxCAN TEC stayed low (7-12) because it saw its own frames as valid
- Waveshare passive sniffer doesn't validate bit timing → saw all frames
- CAN registers (MCR, BTR, ESR) all showed correct values
- The old firmware from D: worked because it was flashed alone (no other nodes to detect errors)
- G474 FDCAN uses HSE crystal → works perfectly with gs_usb

**Evidence**:
- CVC (G474/HSE) alone: gs_usb stays ERROR-ACTIVE, all frames perfect
- RZC (F413/HSI) alone: gs_usb goes ERROR-PASSIVE within 5 seconds
- RZC (F413/HSE): gs_usb stays ERROR-ACTIVE, all 6 CAN IDs at perfect rates

**Fix**: Changed PLL source from HSI to HSE bypass (8 MHz from ST-LINK MCO):
```c
// Before (broken):
RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;  // ±1%
RCC_OscInitStruct.PLL.PLLM = 16u;  // 16MHz/16 = 1MHz VCO input

// After (working):
RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;  // ~20ppm
RCC_OscInitStruct.PLL.PLLM = 4u;   // 8MHz/4 = 2MHz VCO input
```

Also caught a PLL math error during the fix: first HSE attempt used M=8,N=192,P=4 → SYSCLK=48MHz (not 96MHz) → APB1=24MHz → CAN ran at 250kbps instead of 500kbps. Verified via register readback before deploying.

**Principle**: Never use internal RC oscillator for CAN. Always use HSE crystal or bypass. CAN is the most clock-sensitive protocol in automotive — ±1% is unacceptable.
