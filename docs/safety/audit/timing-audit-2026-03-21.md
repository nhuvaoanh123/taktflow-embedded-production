# Timing Audit — ISO 26262 ASIL D

**Date:** 2026-03-21
**Standard:** ISO 26262 Part 4 Section 6.4.3 (FTTI), Part 6 Section 9 (Timing)
**Scope:** Tick rates, FTTI compliance, E2E counter timing, scheduling order, clock sources
**Auditors:** 10 timing-focused roles
**Verdict:** CONDITIONAL PASS — 4 CRITICAL, 3 MAJOR, 6 MINOR, 8 OBSERVATION

---

## Auditor 1: FTTI Analyst

### Finding 1.1 — CRITICAL: 12/20 E2E messages violate FTTI with current MaxDeltaCounter

**File:** `docs/safety/analysis/ftti-complete.md` (Section 3)
**Evidence:** `T_detect = CycleTime * (MaxDeltaCounter + 1) + T_process`

With MaxDeltaCounter=3 globally in the DBC:
- EStop_Broadcast (0x001): T_detect = 50ms vs FTTI = 20ms. **30ms over budget.**
- CVC/FZC/RZC Heartbeats: T_detect = 210ms vs FTTI = 100ms. **110ms over budget.**
- Steering_Status (0x200): T_detect = 210ms vs FTTI = 50ms. **160ms over budget.**
- Brake_Status (0x201): T_detect = 210ms vs FTTI = 50ms. **160ms over budget.**
- Motor_Status/Current/Temp (0x300-302): T_detect = 410ms vs FTTI = 200ms.
- Battery_Status (0x303): T_detect = 4010ms vs FTTI = 1000ms.

**Status:** Documented. Fix plan in Section 4 (reduce MaxDeltaCounter per message). **Not yet implemented in DBC.**

**Verdict:** CRITICAL. FTTI violations are documented but DBC has not been updated. Every minute the system runs with MaxDeltaCounter=3, the E-Stop path is 2.5x over FTTI budget. This is not a paperwork issue -- it means the system cannot detect a stuck E-Stop within the required 20ms.

### Finding 1.2 — MAJOR: Steering_Status and Brake_Status cannot meet FTTI even with MaxDeltaCounter=0

**File:** `docs/safety/analysis/ftti-complete.md` (Section 3.3)
**Evidence:** 50ms cycle * 1 + 10ms = 60ms > 50ms FTTI.

The deviation justification (Section 3.4) relies on plausibility checks (command != feedback within 20ms) as the primary detection mechanism. This is architecturally sound but:
1. The plausibility check is in CVC's Swc_VehicleState, running at 10ms period (sidecar line 100-103). The 2-cycle debounce = 20ms claim is correct.
2. **However**, the plausibility check only works when CVC is sending commands. If CVC is in INIT state (not commanding), the backup E2E path is the only detection mechanism, and it exceeds FTTI.

**Verdict:** MAJOR. The deviation is acceptable for DRIVING state but needs explicit documentation that INIT/SHUTDOWN states have no 50ms feedback detection.

### Finding 1.3 — OBSERVATION: Battery_Status 1010ms vs 1000ms FTTI

The 10ms exceedance on Battery_Status is within thermal inertia margins (battery thermal mass provides seconds of margin). Deviation is reasonable.

**Verdict:** OBSERVATION. Acceptable with current justification.

---

## Auditor 2: AUTOSAR Timing Architect

### Finding 2.1 — MAJOR: CVC dispatches Swc_CvcCom_TransmitSchedule BEFORE Rte_MainFunction

**File:** `firmware/ecu/cvc/src/main.c` lines 416-426 vs 430-434
**Evidence:**
```
/* 10ms block (line 416-426): */
Swc_CvcCom_BridgeRxToRte();
Swc_CvcCom_TransmitSchedule(tick_us / 1000u);  // writes to Com PDU buffers
CanTp_MainFunction();
...

/* 1ms block (line 430-434): */
Rte_MainFunction();  // dispatches Com_MainFunction_Tx at 10ms via RTE
```

The comment at line 412-415 explains this is intentional ("bridge signals BEFORE RTE dispatches Com_MainFunction_Tx"). But this means:

1. `Swc_CvcCom_TransmitSchedule` runs at the START of the 10ms tick
2. `Com_MainFunction_Tx` runs later when RTE dispatches it (also at 10ms)
3. The 10ms check fires first (`>= 10000u`), then the 1ms check fires immediately after

**Problem:** The RTE dispatcher uses `rte_tick_counter` which increments every call to `Rte_MainFunction()`. CVC calls `Rte_MainFunction()` every 1ms (line 430-434). So runnables configured with `period_ms: 10` fire when `rte_tick_counter % 10 == 0`. But the 10ms pre-block at line 416 runs ONCE, then the 1ms block runs 10 times before the next 10ms block. On the FIRST of those 10 calls, `Com_MainFunction_Tx` fires -- this is correct timing. The scheduling is sound but fragile.

**Verdict:** MAJOR. The dispatch order relies on `last_10ms_us` being checked before `last_1ms_us` in the same loop iteration. If code is reordered, TX will use stale data. This needs a documented scheduling order contract in the safety manual. AUTOSAR BSW scheduling normally uses OsAlarm-driven activation, not hand-rolled priority in a polled loop.

### Finding 2.2 — MINOR: BCM calls Bcm_ComBridge_10ms TWICE per tick

**File:** `firmware/ecu/bcm/src/bcm_main.c` lines 238 and 244
**Evidence:**
```
Bcm_ComBridge_10ms();   // line 238: before Rte_MainFunction
Rte_MainFunction();      // line 241
Bcm_ComBridge_10ms();   // line 244: after Rte_MainFunction
```

Calling the bridge twice means Com_SendSignal is called twice per tick for all 6 TX signals. The second call overwrites the first. No functional bug (same data), but:
1. Double E2E counter increment risk if DIRECT mode PDUs are in use
2. Wasted CPU cycles

**Verdict:** MINOR. Not a safety issue (BCM is QM) but wastes bus bandwidth and could confuse E2E counters.

### Finding 2.3 — OBSERVATION: Sidecar defines Can_MainFunction_Read at 1ms for CVC/RZC but 10ms for FZC

**File:** `model/ecu_sidecar.yaml`
- CVC: `Can_MainFunction_Read: period_ms: 1` (line 84-86)
- RZC: `Can_MainFunction_Read: period_ms: 1` (line 364-366)
- FZC: `Can_MainFunction_Read: period_ms: 10` (line 206-208)

CVC and RZC poll CAN every 1ms. FZC polls every 10ms. This means FZC adds up to 10ms latency on RX before E2E/Com processing. For Steer_Command (10ms cycle, 50ms FTTI), this eats 20% of the FTTI budget on reception alone.

**Verdict:** OBSERVATION. 10ms RX polling on FZC is acceptable but worth documenting as a latency contributor.

---

## Auditor 3: E2E Timing Specialist

### Finding 3.1 — CRITICAL: COM_RX_CYCLE_MS is hardcoded to 10ms — wrong for 50ms ECUs

**File:** `firmware/bsw/services/Com/src/Com.c` line 93
**Evidence:**
```c
#define COM_RX_CYCLE_MS   10u
```

This constant is used in `Com_MainFunction_Rx()` at line 690:
```c
if ((com_rx_timeout_cnt[pdu_id] * COM_RX_CYCLE_MS) >= timeout) {
```

For BCM/ICU/TCU, `Com_MainFunction_Rx()` is called every 50ms (BCM main loop at line 235, ICU at line 254). But `COM_RX_CYCLE_MS` is hardcoded to 10ms. This means:

- If `TimeoutMs = 100ms` and actual call period = 50ms:
  - After 2 calls (100ms wall time): `com_rx_timeout_cnt = 2`, calculation: `2 * 10 = 20ms` -- NO timeout triggered
  - After 10 calls (500ms wall time): `com_rx_timeout_cnt = 10`, calculation: `10 * 10 = 100ms` -- timeout triggers
  - **Actual timeout = 500ms, configured timeout = 100ms. 5x late.**

For ASIL-D ECUs (CVC/FZC/RZC) called at 10ms, this is correct. For QM ECUs (BCM/ICU/TCU) at 50ms, **RX deadline monitoring is 5x slower than configured.**

**Verdict:** CRITICAL. `COM_RX_CYCLE_MS` must be derived from `mainFunctionPeriodMs` (which is already in `Com_ConfigType` and correctly set per-ECU). The TX path was fixed (uses `com_main_period_ms` at line 565) but the RX path was missed.

### Finding 3.2 — CRITICAL: E2E TX MaxDeltaCounter set to 15 in Com_TriggerIPDUSend

**File:** `firmware/bsw/services/Com/src/Com.c` line 501 and line 623
**Evidence:**
```c
e2e_cfg.MaxDeltaCounter = 15u;  /* line 501, in Com_TriggerIPDUSend */
e2e_cfg.MaxDeltaCounter = 15u;  /* line 623, in Com_MainFunction_Tx */
```

On the TX side, `MaxDeltaCounter` is set to 15 (the E2E P01 maximum). This value is not used for TX protection (it only affects the alive counter range), but it means:
1. The TX alive counter wraps at 15 (4-bit, correct for E2E P01)
2. **However**, the RX side reads `MaxDeltaCounter` from per-PDU config (`com_config->rxPduConfig[rx_idx].E2eMaxDelta` at line 367). This is correct.

**Verdict:** OBSERVATION. TX MaxDeltaCounter=15 is harmless (it only defines counter range for protection). The RX path correctly uses per-PDU config. But the magic number should be a named constant.

### Finding 3.3 — MINOR: ICU heartbeat counter wraps at 255, not 15

**File:** `firmware/ecu/icu/src/icu_main.c` line 140, 156
**Evidence:**
```c
static uint8 icu_hb_alive_counter = 0u;  /* wraps 0-255 */
...
icu_hb_alive_counter++;  /* uint8 wraps at 255 */
```

E2E P01 alive counter is 4-bit (0-15). The `icu_hb_alive_counter` is `uint8` and wraps at 255. However, the E2E Protect function in `Com_TriggerIPDUSend` writes the alive counter into the PDU, so the local counter is just for tracking sends, not for E2E. The E2E state machine's `Counter` field (line 134, `com_e2e_tx_state[PduId].Counter`) handles the actual 4-bit counter.

**Verdict:** MINOR. The local counter variable is cosmetic but misleading. It should wrap at 15 for clarity, or be removed entirely since E2E manages its own counter.

### Finding 3.4 — MINOR: No E2E on BCM/ICU/TCU heartbeats in main loop

**File:** `firmware/ecu/bcm/src/bcm_main.c` line 173-179, `firmware/ecu/icu/src/icu_main.c` line 148-157
**Evidence:** BCM/ICU/TCU heartbeat functions call `Com_SendSignal()` directly with hardcoded signal IDs. E2E protection is applied in `Com_TriggerIPDUSend()` if the PDU config has `E2eProtected = TRUE`. This is correct -- but the heartbeat is called from the main loop, not from the RTE dispatcher.

**Verdict:** MINOR. Heartbeat bypass of RTE scheduling means WdgM cannot supervise heartbeat execution timing for QM ECUs. Acceptable at QM level but inconsistent with ASIL ECU architecture.

---

## Auditor 4: Real-Time Systems Engineer

### Finding 4.1 — MAJOR: CVC uses cooperative polling, not interrupt-driven scheduling

**File:** `firmware/ecu/cvc/src/main.c` lines 406-462
**Evidence:**
```c
for (;;) {
    Main_Hw_Wfi();
    tick_us = Main_Hw_GetTick();
    if ((tick_us - last_10ms_us) >= 10000u) { ... }
    if ((tick_us - last_1ms_us) >= 1000u)  { ... }
    if ((tick_us - last_100ms_us) >= 100000u) { ... }
}
```

This is a polled superloop. `Main_Hw_Wfi()` on POSIX calls `Sil_Time_Sleep(1000u)` (1ms virtual). On STM32 it would be `__WFI()`. The worst-case scheduling jitter:
- 1ms task: up to 1ms late (bounded by WFI wakeup)
- 10ms task: up to 1ms late (checks happen every WFI wakeup)
- **But**: if the 10ms block takes >1ms to execute, the 1ms Rte_MainFunction is delayed

No WCET budget is documented for any runnable. On STM32 at 72MHz, a single `Com_MainFunction_Tx` iteration over 33 TX PDUs with E2E protect could take 200-500us. Add signal packing, and the 10ms block could exceed 1ms.

**Verdict:** MAJOR. No WCET analysis exists. For ASIL D, ISO 26262 Part 6 Section 9.4.3 requires timing verification including worst-case execution time. The current cooperative scheduler has no overrun detection or preemption.

### Finding 4.2 — OBSERVATION: ICU and TCU use Sil_Time_Sleep for tick timing

**File:** `firmware/ecu/icu/src/icu_main.c` line 250, `firmware/ecu/tcu/src/tcu_main.c` line 193
**Evidence:**
```c
Sil_Time_Sleep(50000u);  /* ICU: 50ms */
Sil_Time_Sleep((uint32)TCU_RTE_PERIOD_MS * 1000u);  /* TCU: 50ms */
```

These ECUs use `Sil_Time_Sleep()` at the TOP of the loop (ICU) or BOTTOM (TCU). This means:
- ICU: sleep FIRST, then process. Total period = sleep + processing time. Actual period > 50ms.
- TCU: process FIRST, then sleep. Total period = processing + sleep. Actual period > 50ms (sidecar says 50ms, but TCU prints "10ms tick" at line 162 -- **contradicts the 50ms sleep**).

**Verdict:** OBSERVATION for ICU (QM). See Finding 4.3 for TCU.

### Finding 4.3 — MINOR: TCU main.c says "10ms tick" but sleeps for 50ms

**File:** `firmware/ecu/tcu/src/tcu_main.c` line 162 vs line 193
**Evidence:**
```c
(void)printf("[TCU] Entering main loop (10ms tick)\n");  /* line 162 */
...
Sil_Time_Sleep((uint32)TCU_RTE_PERIOD_MS * 1000u);  /* line 193, TCU_RTE_PERIOD_MS = 50 */
```

The print says 10ms but `TCU_RTE_PERIOD_MS` is 50 (from sidecar line 479). The heartbeat counter also uses `hb_tick >= 50u` (line 181), which at 50ms/tick = 2500ms -- but the comment says 500ms. This only works if TCU_RTE_PERIOD_MS was recently changed from 10 to 50.

Wait -- `50 ticks * 10ms = 500ms` matches the old 10ms period. With `TCU_RTE_PERIOD_MS = 50`, the actual heartbeat period = `50 ticks * 50ms = 2500ms`. **TCU heartbeat fires every 2.5 seconds, not 500ms.**

**Verdict:** MINOR (QM ECU). But TCU heartbeat rate is wrong: 0.4/s instead of 2/s. The context says "all 6 heartbeats at correct rates" -- this contradicts. The `hb_tick >= 50u` threshold was correct for 10ms period but wrong for 50ms.

**Correction needed:** `hb_tick >= 10u` for 50ms tick (10 * 50ms = 500ms).

---

## Auditor 5: CAN Bus Load Analyst

### Finding 5.1 — OBSERVATION: Bus load estimation

CAN 500kbps, standard frames (8 bytes payload, ~130 bits/frame including stuff bits).

Per the DBC/FTTI analysis, the cyclic messages and their rates:

| Rate | Messages | Frames/s |
|------|----------|----------|
| 10ms | EStop, VehicleState, Torque, Steer, Brake, Lidar | 6 * 100 = 600 |
| 50ms | CVC_HB, FZC_HB, RZC_HB, Steering_Status, Brake_Status, Motor_Cutoff | 6 * 20 = 120 |
| 100ms | Motor_Status, Motor_Current, Motor_Temp | 3 * 10 = 30 |
| 500ms | SC_Status, BCM_HB, ICU_HB, TCU_HB + BCM body msgs (3) | 7 * 2 = 14 |
| 1000ms | Battery_Status | 1 |

Total: ~765 frames/s. At 130 bits/frame: 99,450 bps on 500,000 bps = **~20% bus load**.

**Verdict:** OBSERVATION. 20% bus load is acceptable. Headroom for bursts (DTC broadcasts, UDS diagnostics). No collision concern at this load level.

### Finding 5.2 — OBSERVATION: No CAN message priority analysis

Messages are assigned CAN IDs (lower = higher priority):
- 0x001 EStop_Broadcast (highest priority) -- correct
- 0x010-0x016 Heartbeats -- good, high priority
- 0x100-0x103 Commands -- medium priority
- 0x200-0x303 Status -- lower priority
- 0x400-0x500 Body/DTC -- lowest

This is a sound priority assignment. E-Stop will always win arbitration.

**Verdict:** OBSERVATION. CAN ID assignment is correct for safety priority.

---

## Auditor 6: Safety Timing Assessor

### Finding 6.1 — CRITICAL: EStop_Broadcast FTTI violation is the most dangerous finding

**File:** `docs/safety/analysis/ftti-complete.md` line 34
**Evidence:** SG-011 requires E-Stop propagation within 20ms (2 CAN cycles at 10ms). With MaxDeltaCounter=3, detection takes 50ms. Even with MaxDeltaCounter=0, detection is exactly 20ms (zero margin).

The FTTI analysis (Section 3.2, line 78) shows MaxDeltaCounter=0 yields T_detect = 20ms = FTTI. Zero margin means:
1. Any processing delay pushes past FTTI
2. T_process is assumed as 10ms (one Com_MainFunction_Rx cycle). On CVC with 1ms Can_MainFunction_Read + 10ms Com_MainFunction_Rx, actual T_process could be 11ms.
3. If the E-Stop frame arrives just after CAN polling, add up to 1ms (CVC) or 10ms (FZC) latency.

**Verdict:** CRITICAL. The E-Stop path has zero margin at MaxDeltaCounter=0 and the T_process assumption of 10ms may be optimistic. For SG-011 at ASIL D, this needs either:
(a) Reduce EStop cycle to 5ms (T_detect = 5+10 = 15ms, 5ms margin), or
(b) Hardware E-Stop bypass (hardwired relay) that does not depend on CAN at all.

### Finding 6.2 — MINOR: SC heartbeat timeout differs by 15x between POSIX and TMS570

**File:** `firmware/platform/posix/include/Sc_Cfg_Platform.h` line 17 vs `firmware/platform/tms570/include/Sc_Cfg_Platform.h` line 17
**Evidence:**
- POSIX: `SC_HB_TIMEOUT_TICKS = 150` (150 * 10ms = 1500ms)
- TMS570: `SC_HB_TIMEOUT_TICKS = 10` (10 * 10ms = 100ms)

The POSIX value is 15x the target value, justified by Docker jitter at SIL_TIME_SCALE=10. This means SIL testing does NOT validate the 100ms heartbeat timeout that the target will use. A heartbeat failure that takes 500ms to detect would pass SIL but fail on target.

**Verdict:** MINOR. SIL timing fidelity is inherently limited, but the gap should be documented in the SIL validation report. Consider running SIL at scale=1 for timing-critical tests.

---

## Auditor 7: Multi-ECU Coordination

### Finding 7.1 — MINOR: COM_STARTUP_DELAY_MS is 50ms default -- heartbeat starts before peers

**File:** `firmware/bsw/services/Com/src/Com.c` lines 63-67
**Evidence:**
```c
#ifndef COM_STARTUP_DELAY_MS
#define COM_STARTUP_DELAY_MS  50u
```

After 50ms, all ECUs start transmitting simultaneously. In Docker SIL, containers start sequentially (SC first, then CVC, then FZC...). The SC has a startup grace period of 1500 ticks (15s POSIX). On target hardware, all ECUs power on simultaneously, and the 50ms stagger is the same for all -- no stagger at all.

CAN bus arbitration handles simultaneous TX, so this is not a collision risk. But it means all heartbeats arrive at SC simultaneously every 50ms, creating burst processing load.

**Verdict:** MINOR. Consider per-ECU COM_STARTUP_DELAY_MS offsets (e.g., CVC=50, FZC=55, RZC=60) to spread heartbeat arrivals.

### Finding 7.2 — OBSERVATION: SC startup grace period (15s POSIX, unknown target)

**File:** `firmware/platform/posix/include/Sc_Cfg_Platform.h` line 27
**Evidence:** `SC_HB_STARTUP_GRACE_TICKS = 1500u` (15s on POSIX).

During this 15s window, SC does not kill relay for missing heartbeats. On target hardware, the grace period value is not visible in the TMS570 platform config (need to check). If the target grace period is too short, SC could kill the relay before CVC finishes its self-test sequence.

**Verdict:** OBSERVATION. Verify target platform Sc_Cfg_Platform.h has an appropriate grace period.

---

## Auditor 8: Platform Abstraction Reviewer

### Finding 8.1 — MINOR: RTE tick semantics differ between CVC and BCM/ICU/TCU

**File:** `firmware/bsw/rte/src/Rte.c` line 231, `firmware/bsw/rte/include/Rte.h` line 107
**Evidence:** `Rte_MainFunction()` increments `rte_tick_counter` by 1 each call. Runnable dispatch uses `rte_tick_counter % periodMs == 0`.

- CVC calls `Rte_MainFunction()` every 1ms -> `rte_tick_counter` increments by 1 per ms. A 10ms runnable fires when counter % 10 == 0. **Correct.**
- BCM calls `Rte_MainFunction()` every 50ms (one call per main loop iteration at line 241). `rte_tick_counter` increments by 1 per 50ms. A runnable configured with `period_ms: 50` fires when counter % 50 == 0 -- i.e., every 50th call = **every 2500ms**.

**This is the same class of bug as TCU heartbeat (Finding 4.3).** The RTE dispatcher assumes 1ms tick granularity. When called at 50ms intervals, all period_ms values are effectively multiplied by 50.

Wait -- checking the sidecar: BCM has no explicit runnables section. The main loop directly calls `Bcm_ComBridge_10ms()` and `Rte_MainFunction()`. If BCM's rte_config has runnables at period_ms: 50, they would fire at counter % 50 == 0, which at 50ms call rate = every 2500ms.

**Verdict:** MINOR (BCM/ICU/TCU are QM). But this is a systematic design flaw: the RTE assumes 1ms dispatch but ECUs call it at different rates. Either:
(a) All ECUs must call Rte_MainFunction at 1ms, or
(b) Rte_ConfigType needs a `tickPeriodMs` field that the dispatcher uses to scale.

### Finding 8.2 — OBSERVATION: POSIX timerfd creates+destroys a file descriptor per sleep call

**File:** `firmware/bsw/services/Sil/src/Sil_Time.c` lines 96-117
**Evidence:**
```c
if (wall_us < 2000u) {
    int tfd = timerfd_create(CLOCK_MONOTONIC, 0);
    ...
    close(tfd);
}
```

CVC calls `Sil_Time_Sleep(1000u)` every 1ms via `Main_Hw_Wfi()`. At SIL_TIME_SCALE=10, wall_us = 100us < 2000us, so timerfd is used. That is 1000 timerfd_create/close syscalls per second per ECU, 7 ECUs = 7000 fd operations/s. On a loaded Docker host this could cause scheduling latency.

**Verdict:** OBSERVATION. Consider creating the timerfd once at init and reusing it.

---

## Auditor 9: Clock Source Analyst

### Finding 9.1 — OBSERVATION: Sil_Time_GetTickUs may overflow uint32 after ~71 minutes

**File:** `firmware/bsw/services/Sil/src/Sil_Time.c` lines 129-136
**Evidence:**
```c
uint32 wall_us = (uint32)(...);
return wall_us * sil_scale;
```

`uint32` max = 4,294,967,295. At SIL_TIME_SCALE=10, virtual time runs 10x faster. `wall_us * 10` overflows after wall_us = 429,496,729 = ~429 seconds (7.1 minutes wall clock). After overflow, tick_us wraps and all time deltas in main loops become huge, triggering all tasks simultaneously.

On CVC, `tick_us` is used in subtraction: `(tick_us - last_10ms_us) >= 10000u`. If tick_us wraps from ~4.2B to 0, the subtraction (unsigned) produces a huge value, and all tasks fire. This is actually safe for a single wrap event (tasks fire, then last_*_us is reset to the new wrapped value). But the virtual time value itself is meaningless after overflow.

**Verdict:** OBSERVATION. At scale=1, overflow at 4294 seconds (71 min). At scale=10, overflow at 429s wall (71 min virtual). SIL tests that run >7 minutes at scale=10 will experience a timing discontinuity. The `sil_014_long_duration.yaml` scenario should verify behavior across the wrap point.

### Finding 9.2 — OBSERVATION: CVC POSIX tick uses CLOCK_MONOTONIC -- correct

**File:** `firmware/platform/posix/src/cvc_hw_posix.c` lines 78-81
**Evidence:** `Sil_Time_GetTickUs()` uses `CLOCK_MONOTONIC`. This is immune to NTP adjustments, wall-clock jumps, and DST changes. Correct choice for embedded timing simulation.

**Verdict:** OBSERVATION. Correct.

---

## Auditor 10: Regression Tester

### Finding 10.1 — MINOR: No CI gate for FTTI compliance

**File:** `docs/safety/analysis/ftti-complete.md` Section 5, item 5
**Evidence:** "Update Step 1 DBC validator to check FTTI per safety goal" -- listed as an action item, not yet implemented.

There is no automated check that verifies MaxDeltaCounter values against FTTI budgets. A developer could change a CycleTimeMs or MaxDeltaCounter in the DBC without triggering a FTTI violation alert.

**Verdict:** MINOR. A CI gate should parse the DBC, compute T_detect per message, and fail if any message exceeds its safety goal's FTTI.

### Finding 10.2 — OBSERVATION: SchM_Timing instrumentation exists but no CI consumption

**File:** `firmware/bsw/services/SchM/include/SchM_Timing.h` lines 28-42
**Evidence:** 15 timing IDs are defined (Com_Main_TX/RX, E2E, SWC runnables). `SchM_TimingStart/Stop` record execution duration. But there is no CI test that reads these values and asserts WCET budgets.

**Verdict:** OBSERVATION. Good instrumentation foundation. Needs automated WCET threshold checks.

### Finding 10.3 — OBSERVATION: test_can_rates.py exists in HIL test suite

**File:** `test/hil/test_can_rates.py`
**Evidence:** A CAN frame rate test exists. This could validate heartbeat rates and cycle times. Needs to be verified as part of the CI pipeline.

**Verdict:** OBSERVATION. Verify this test covers all 20 E2E message rates.

---

## Consolidated Findings Table

| # | Severity | Auditor | Finding | File:Line | Status |
|---|----------|---------|---------|-----------|--------|
| 1.1 | **CRITICAL** | FTTI Analyst | 12/20 E2E messages violate FTTI (MaxDeltaCounter=3 not yet fixed in DBC) | ftti-complete.md:64 | OPEN |
| 3.1 | **CRITICAL** | E2E Timing | COM_RX_CYCLE_MS hardcoded 10ms, wrong for 50ms ECUs (5x late timeout) | Com.c:93 | OPEN |
| 6.1 | **CRITICAL** | Safety Assessor | E-Stop path has zero FTTI margin even with MaxDeltaCounter=0 | ftti-complete.md:78 | OPEN |
| 4.3 | **CRITICAL** | RT Engineer | TCU heartbeat fires at 2500ms not 500ms (hb_tick threshold wrong for 50ms period) | tcu_main.c:181 | OPEN |
| 1.2 | **MAJOR** | FTTI Analyst | Steering/Brake feedback FTTI deviation undocumented for INIT state | ftti-complete.md:96 | OPEN |
| 2.1 | **MAJOR** | AUTOSAR Arch | CVC scheduling order fragile, no documented contract | main.c:416 | OPEN |
| 4.1 | **MAJOR** | RT Engineer | No WCET analysis for any runnable (ISO 26262 Part 6 Section 9.4.3) | main.c:406 | OPEN |
| 2.2 | **MINOR** | AUTOSAR Arch | BCM calls ComBridge twice per tick (redundant) | bcm_main.c:238,244 | OPEN |
| 3.3 | **MINOR** | E2E Timing | ICU/TCU/BCM heartbeat counter wraps at 255, not 15 | icu_main.c:140 | OPEN |
| 3.4 | **MINOR** | E2E Timing | QM heartbeat bypass RTE (no WdgM supervision) | bcm_main.c:173 | OPEN |
| 6.2 | **MINOR** | Safety Assessor | SC heartbeat timeout 15x looser on POSIX than target | Sc_Cfg_Platform.h:17 | OPEN |
| 7.1 | **MINOR** | Multi-ECU | No per-ECU startup TX stagger | Com.c:64 | OPEN |
| 8.1 | **MINOR** | Platform | RTE tick semantics wrong for 50ms ECUs (systematic) | Rte.c:231 | OPEN |
| 10.1 | **MINOR** | Regression | No CI FTTI compliance gate | ftti-complete.md:135 | OPEN |
| 1.3 | OBSERVATION | FTTI Analyst | Battery_Status 10ms over FTTI -- acceptable | ftti-complete.md:92 | ACCEPTED |
| 2.3 | OBSERVATION | AUTOSAR Arch | FZC CAN RX polling at 10ms vs CVC at 1ms | ecu_sidecar.yaml:206 | NOTED |
| 5.1 | OBSERVATION | Bus Load | ~20% bus utilization -- adequate headroom | N/A | NOTED |
| 5.2 | OBSERVATION | Bus Load | CAN ID priority assignment correct | N/A | NOTED |
| 7.2 | OBSERVATION | Multi-ECU | SC startup grace period needs target verification | Sc_Cfg_Platform.h:27 | NOTED |
| 8.2 | OBSERVATION | Platform | timerfd created/destroyed per sleep call (1000/s) | Sil_Time.c:97 | NOTED |
| 9.1 | OBSERVATION | Clock Source | uint32 tick overflow at ~71min virtual time | Sil_Time.c:135 | NOTED |
| 9.2 | OBSERVATION | Clock Source | CLOCK_MONOTONIC usage correct | cvc_hw_posix.c:81 | NOTED |
| 10.2 | OBSERVATION | Regression | SchM_Timing exists but no CI WCET checks | SchM_Timing.h:28 | NOTED |
| 10.3 | OBSERVATION | Regression | test_can_rates.py exists, coverage unknown | test/hil/ | NOTED |

---

## Recommendations — Priority Order

### Immediate (before next release)

1. **Fix COM_RX_CYCLE_MS** (Finding 3.1): Replace the hardcoded `#define COM_RX_CYCLE_MS 10u` with runtime `com_main_period_ms` in `Com_MainFunction_Rx()`. This is a one-line fix that corrects RX timeout for all ECUs.

2. **Fix TCU heartbeat tick threshold** (Finding 4.3): Change `hb_tick >= 50u` to `hb_tick >= 10u` in `tcu_main.c` line 181. Also fix the print at line 162 to say "50ms tick". Same pattern may exist in BCM (line 248: `hb_tick >= 50u` at 10ms sleep -- BCM actually sleeps `BCM_RTE_PERIOD_MS * 1000u` = 50ms, so BCM also has wrong heartbeat period: 50 * 50ms = 2500ms instead of 500ms).

3. **Update DBC MaxDeltaCounter values** (Finding 1.1): Apply the per-message MaxDeltaCounter from `ftti-complete.md` Section 4 table. Regenerate all configs.

4. **Address E-Stop FTTI** (Finding 6.1): Either reduce EStop_Broadcast cycle to 5ms, or document the hardware E-Stop bypass as the primary safety mechanism (making the CAN E-Stop a secondary/informational path).

### Short-term (next sprint)

5. **Fix RTE tick semantics for 50ms ECUs** (Finding 8.1): Add `tickPeriodMs` to `Rte_ConfigType`. The dispatcher multiplies `rte_tick_counter` by `tickPeriodMs` before comparing to `periodMs`.

6. **Document CVC scheduling order contract** (Finding 2.1): Add a comment block and a scheduling diagram in the safety manual showing the required order: CAN RX -> signal bridge -> RTE dispatch -> Com TX.

7. **WCET budget analysis** (Finding 4.1): Profile each runnable on target STM32. Document budgets. Add SchM_Timing assertions that flag overruns.

### Medium-term (next milestone)

8. **CI FTTI gate** (Finding 10.1): Script that parses DBC, computes T_detect, and fails if any message exceeds its safety goal FTTI.

9. **SIL timing fidelity tests** (Finding 6.2): Run critical timing tests at SIL_TIME_SCALE=1 to validate that timeout values work at real-time speed.

---

## Overall Verdict

**CONDITIONAL PASS.** The timing architecture is fundamentally sound (correct clock sources, correct CAN priority, reasonable bus load, working E2E infrastructure). However, 4 CRITICAL findings must be resolved before any safety claim:

1. The DBC MaxDeltaCounter values have not been applied -- 60% of E2E messages cannot detect failure within FTTI.
2. COM_RX_CYCLE_MS makes RX deadline monitoring 5x late on QM ECUs -- a latent defect that would mask failures.
3. The E-Stop CAN path has zero FTTI margin -- unacceptable for ASIL D SG-011.
4. TCU (and likely BCM) heartbeat rates are wrong by 5x due to tick period mismatch.

None of these are architectural flaws. All are fixable with targeted changes. But until fixed, the timing safety argument is incomplete.
