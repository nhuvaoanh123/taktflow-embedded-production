# Lessons Learned: ThreadX OS Port (2026-03-26/27)

## 1. Stale Object Files — `make clean` doesn't work with escaped paths
**Context:** ThreadX experiments use `FW=../../firmware` in Makefiles. Object files end up at `build/../../firmware/...` which is OUTSIDE the build directory.
**Mistake:** `make clean` only removes `build/`, leaving stale `.o` files in the firmware source tree.
**Fix:** Use `strip_dotdot` in Makefile to remap `../../firmware/X` to `build/firmware/X`. Now `rm -rf build` cleans everything.
**Principle:** Object files must never escape the build directory. Always verify with `nm` that buffer sizes match config.

## 2. RTE_MAX_SIGNALS fallback caused silent buffer overflow
**Context:** `Rte.h` had `#ifndef RTE_MAX_SIGNALS #define 48u` fallback. FZC needs 205 signals. Stale `Rte.o` compiled with 48 → writes to signal 187 corrupted RAM.
**Mistake:** Silent fallback allowed builds to succeed with wrong buffer size.
**Fix:** Changed fallback to `#error "Include Rte_PbCfg.h"`. Now build fails loudly if ECU include path is missing.
**Principle:** Never use silent fallbacks for safety-critical buffer sizes. Fail at compile time.

## 3. TX/RX PduId namespace collision
**Context:** TX and RX PDU IDs both started at 0. When TX auto-pull was added, `Com_RxIndication` matched TX signals (PduId=0) when processing RX frames (PduId=0). ECU_ID always read as 0.
**Mistake:** Single signal config table with shared PduId field for TX and RX.
**Fix:** Split into `txSignalConfig[]` and `rxSignalConfig[]` (AUTOSAR pattern). `Com_MainFunction_Tx` only iterates TX, `Com_RxIndication` only iterates RX.
**Principle:** TX and RX must be in separate namespaces. AUTOSAR does this with separate I-PDU groups.

## 4. FDCAN RX DLC bug — HAL constant not converted
**Context:** STM32G4 HAL returns `rxHeader.DataLength = FDCAN_DLC_BYTES_6 = 0x60000`, not `6`. Code treated it as raw byte count → all frames reported DLC=8 → E2E CRC computed over wrong length → 100% E2E failure.
**Mistake:** Assumed HAL DLC is raw byte count (works for bxCAN, not FDCAN).
**Fix:** `dlcRaw = rxHeader.DataLength >> 16u` to extract actual byte count.
**Principle:** Always check HAL return value encoding. FDCAN HAL uses shifted constants, bxCAN uses raw values.

## 5. Double-call of BSW MainFunctions
**Context:** `Can_MainFunction_Read`, `Com_MainFunction_Tx/Rx` were called from BOTH the experiment's 10ms timer callback AND the RTE runnable table. Each call increments cycle counters → 2x TX rate → 4x bus overload (1610/s vs expected 480/s).
**Mistake:** Copied the bare-metal main loop pattern into ThreadX without removing functions that are now dispatched by the RTE.
**Fix:** Removed duplicate calls from experiment callbacks. BSW MainFunctions dispatched ONLY by `Rte_MainFunction`.
**Principle:** When migrating to RTOS with RTE scheduler, remove all direct BSW MainFunction calls from the application. The RTE table is the single source of dispatch.

## 6. Hand-written Swc_*Com bridge drifts from codegen
**Context:** `Swc_FzcCom_TransmitSchedule` manually called `Com_SendSignal` for each TX signal. When codegen added new signals (OperatingMode split from FaultStatus), the hand-written bridge didn't update → OperatingMode always 0 on CAN.
**Mistake:** Hand-written signal bridging instead of automated codegen.
**Fix:** TX auto-pull in `Com_MainFunction_Tx` reads TX signals from RTE automatically. RX auto-push in `Com_RxIndication` writes to RTE automatically. No hand-written bridge needed.
**Principle:** Never hand-write what codegen generates. Signal routing must be automated end-to-end.

## 7. Legacy Com bridge runnables in ARXML/sidecar
**Context:** `Swc_FzcCom_Receive` and `Swc_FzcCom_TransmitSchedule` were defined in the sidecar and ARXML. The RTE generator included them in the runnable table. `Swc_FzcCom_Receive` overwrote auto-pushed RTE values with stale shadow buffer data → FZC flickering between INIT and RUN.
**Mistake:** Legacy runnables not removed from the codegen pipeline after auto-pull/push replaced them.
**Fix:** Filter out `*Com_Receive` and `*Com_TransmitSchedule` runnables in `reader.py`. Removed from sidecar.
**Principle:** When replacing a mechanism, remove ALL references to the old one — including the scheduling config.

## 8. F413 bxCAN causes ERROR-PASSIVE on 3-ECU bus
**Context:** FZC+CVC (G474 FDCAN) = ERROR-ACTIVE. Add RZC (F413 bxCAN) = ERROR-PASSIVE, 91% throughput drop. Sample point mismatch (87.5% vs 80%) and/or bxCAN transceiver issue.
**Mistake:** Assumed all CAN implementations are interchangeable.
**Fix:** Swapped F413 for a third G474RE → all ERROR-ACTIVE, 963 frames/s matching DBC exactly.
**Principle:** When debugging bus issues, isolate by adding one node at a time. Different CAN peripherals (FDCAN vs bxCAN) may have incompatible timing even at the same baud rate.

## 9. PLATFORM_HIL masks real faults
**Context:** `PLATFORM_HIL` bypassed heartbeat comm guard, SC relay check, E2E alive counter evaluation, and several fault checks. The bus was ERROR-PASSIVE and losing 90% of CVC frames, but the software never noticed.
**Mistake:** Used PLATFORM_HIL as a catch-all bypass instead of fixing root causes.
**Fix:** Removed PLATFORM_HIL, replaced with specific countermeasures: 30s grace period (`CVC_POST_INIT_GRACE_CYCLES=3000`), SC relay default to energized, AUTOSAR deadline monitor.
**Principle:** Never use blanket #ifdef bypasses for testing. Each guard should have its own tunable parameter.

## 10. SC DCAN TX Bit1 Error → Bus-Off → all heartbeats lost
**Context:** SC in normal CAN mode (non-silent). TX of SC_Status (0x013) every 100ms. CVC heartbeat (0x010) sometimes received, FZC (0x011) and RZC (0x012) never received. SC showed ES=0x231 (Error Passive + Stuff Error), then kill=5 (Bus-Off).
**Mistake:** Missing 120Ω termination on the SC end of the CAN bus. Only the Pi's gs_usb had built-in 120Ω on one end. Without dual termination, TX dominant drive caused signal reflections → Bit1 Error (sent recessive, read dominant from reflection) → Error Passive → Bus-Off → all RX killed.
**Why silent mode masked it:** Silent mode suppresses all TX including ACK, so no dominant drive → no reflections → no errors. This hid the termination problem for months.
**Debugging methodology:** Systematic test harness (TEST=0..6) isolating variables one at a time: silent vs normal, TX suppressed vs enabled, DAR on/off, PMD preserve, internal loopback, pure HALCoGen. Confirmed ACK (single-bit dominant) survived reflections but multi-bit TX data did not. Internal loopback TX worked → DCAN logic OK. Added 120Ω → TX works.
**Fix:** Add 120Ω termination at both ends of the CAN bus. SC runs in normal mode (not silent) with TX enabled.
**Principle:** CAN bus MUST have 120Ω termination at both ends. Single termination can work for RX but fails for TX. Always verify bus termination before debugging CAN TX errors. Don't blame firmware when the physics layer is wrong.

## 11. ThreadX timer thread stack overflow — three levels of failure
**Context:** FZC with `THREADX=1`, `Rte_MainFunction()` dispatches 12 SWC runnables from the 1ms timer callback.
**Root cause:** ThreadX timer thread stack too small for the combined call depth of 12 runnables.
**Three failure modes at different stack sizes:**
- **1KB (default):** HardFault → boot-loop. Timer thread dies on first `Rte_MainFunction()` call.
- **4KB:** Boot succeeds, first dispatch cycle works (3 TX frames), then timer thread silently crashes. `Com_MainFunction_Tx` never called again. Symptom: `com=0 TX=3` — looked like an RTE dispatch bug, led to incorrect workaround (direct `Com_MainFunction_Tx` call in Timer_10ms_Callback).
- **8KB:** Stable. All 12 runnables dispatch correctly for 20+ minutes. `com=1449 TX=3194` at 15s — proper rates.
**Misdiagnosis chain:** 4KB fix appeared to work (no boot-loop) → RTE dispatch appeared broken (com=0) → added workaround (direct Com call) → workaround masked the real issue → 8KB revealed the timer crash was the root cause all along.
**Fix:** `TX_TIMER_THREAD_STACK_SIZE=8192` in `tx_user.h`.
**Principle:** Stack overflow in RTOS threads doesn't always cause an obvious crash. Partial overflow can silently kill a thread after the first execution, making the symptom look like a logic bug in the dispatched code. When debugging "function never called" in RTOS context, check thread stack size FIRST.

## 12. ThreadX SysTick 100Hz → all timers 10x slow
**Context:** FZC heartbeat appeared at 500ms instead of 50ms on CAN bus.
**Mistake:** `tx_initialize_low_level.S` configured SysTick at `SYSTEM_CLOCK/100` = 100Hz (10ms tick). ThreadX timers created with `ticks=1` expected 1ms but ran at 10ms. All timer periods scaled 10x.
**Fix:** Changed to `SYSTEM_CLOCK/1000` = 1000Hz (1ms tick).
**Principle:** Verify the RTOS tick rate matches the application's timer assumptions. A 10ms tick with 1-tick timers gives 10ms, not 1ms.

## 13. FDCAN AutoRetransmission=DISABLE → silent after 5 minutes
**Context:** Changed `AutoRetransmission` from `ENABLE` to `DISABLE` to prevent FDCAN HAL stuck in BUSY state.
**What actually happened:** The BUSY state was caused by the timer stack overflow (lesson 11), not auto-retransmission. With `DISABLE`, any lost TX frame (bus contention, arbitration loss) was permanently dropped. Over ~5 minutes, enough frames were lost that the FDCAN effectively went silent.
**Fix:** Restore `AutoRetransmission=ENABLE`. The HAL BUSY issue doesn't occur with 8KB timer stack.
**Principle:** Don't change CAN protocol settings (auto-retransmission, error handling) to work around software bugs. Fix the software bug (stack overflow) instead. CAN auto-retransmission is a fundamental protocol requirement — disabling it creates subtle long-term failures.

## 14. CVC FAULT code path overflows 8KB timer stack
**Context:** SC silent mode (no 0x013 TX) → CVC reads relay=killed → CVC enters FAULT state → timer thread crashes after ~20s. SC normal mode (TEST=5, TX enabled) → CVC reads relay=energized → CVC stays in RUN → stable for 20+ min.
**Root cause:** CVC FAULT code path is heavier than RUN path (Dem reporting, fault logging, safety shutdown). The additional stack usage pushes the 8KB timer thread over the limit. RUN path stays under 8KB.
**Misdiagnosis:** Tried 16KB stack — didn't help because CVC FAULT also triggers other ECUs to go FAULT (cascading: no CVC heartbeat → FZC/RZC timeout → FAULT). The cascade makes ALL ECUs take the heavy code path simultaneously.
**Fix:** Keep SC in normal mode (TEST=5) so CVC receives 0x013 and stays in RUN. The bench SN65HVD230 causes intermittent Error Passive under load, but CVC/FZC/RZC heartbeat reception survives (occasional drops recover within timeout window).
**Principle:** ECU operating state affects stack usage. FAULT/DEGRADED code paths are often heavier than RUN. When debugging RTOS stack issues, test in BOTH normal and fault states. A stack that survives RUN may crash in FAULT.

## 15. Bench CAN transceiver limits — SN65HVD230 on breadboard
**Context:** SC DCAN TX causes Bit1 Error → Error Passive under full bus load (~963 frames/s, ~40% utilization at 500 kbps). Works clean on quiet bus (15 min soak, 0 errors). Fails intermittently under load → selective heartbeat loss (whichever ECU's timing aligns with SC error recovery window).
**Root cause:** Breadboard SN65HVD230 with jumper wires can't sustain clean TX signal at 500k under contention. Signal integrity degrades at high bus load. Not a firmware bug.
**Fix for bench:** SC operates in normal mode (TEST=5) which allows TX but accepts occasional Error Passive. The heartbeat timeout/recovery logic handles brief drops. Production PCB with proper differential routing will eliminate this.
**Principle:** Don't chase firmware bugs when the physics layer is marginal. Characterize the hardware first (quiet bus vs loaded bus), then set firmware expectations accordingly. For bench testing, accept the hardware limitations and design firmware to be resilient.
