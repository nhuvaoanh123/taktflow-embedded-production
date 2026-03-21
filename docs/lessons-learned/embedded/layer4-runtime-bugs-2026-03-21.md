# Lessons Learned: Layer 4 Runtime Integration Bugs — 2026-03-21

## Context

Layer 4 (single ECU POSIX binary) integration testing revealed 3 runtime bugs that passed Layers 1-3 (unit tests + BSW integration). All traced back to one principle: **never hand-write what codegen generates.**

## Bug 1: vcan Loopback Starvation

**Symptom:** CVC doesn't respond to XCP or UDS frames from external processes.
**Root cause:** Linux vcan ignores `CAN_RAW_RECV_OWN_MSGS=0` socket option. CVC receives its own TX frames via loopback, filling the RX buffer. External frames are starved because `CAN_MAX_RX_PER_CALL=32` budget is consumed by own TX loopback.
**Fix:** Software TX ID filter in `Can_Posix.c` — track transmitted CAN IDs, skip matching frames in `Can_Hw_Receive`.
**Principle:** Platform-specific quirks (vcan loopback) only surface at integration level. Unit tests mocked the CAN driver and never tested the real socket.

## Bug 2: PduR Hand-Written Routing Table

**Symptom:** XCP frames reach CanIf but not PduR→XCP.
**Root cause:** CVC `main.c` had a `static` PduR routing table with 13 entries (hand-written before XCP was added). The generated `PduR_Cfg_Cvc.c` had 33 entries with XCP routing. The `static` local table in main.c **shadowed** the generated config.
**Fix:** Replace local routing table with `extern const PduR_ConfigType cvc_pdur_config` pointing to the generated config.
**Principle:** **NEVER hand-write what codegen generates.** If a config is generated, main.c must use `extern` to the generated symbol, not a local copy.

## Bug 3: CanIf Hand-Written TX/RX Tables

**Symptom:** Same as Bug 2 — CanIf had 9 TX + 13 RX entries (hand-written), missing XCP CAN IDs, virtual sensors, and new heartbeat entries.
**Root cause:** Identical to Bug 2 — `main.c` had local `static` CanIf config arrays.
**Fix:** Same pattern — `extern const CanIf_ConfigType cvc_canif_config`.
**Principle:** Same as Bug 2. Applied to CanIf.

## Bug 4: Zombie Process Contamination

**Symptom:** Heartbeat rate 270/s (expected 20/s), Vehicle_State 1347/s (expected 100/s).
**Root cause:** 19 stale CVC processes from previous test runs on the same vcan0 interface. Test scripts used `kill $CVC_PID` which failed silently when the process was backgrounded. Each zombie CVC produced its own heartbeat and Vehicle_State frames.
**Fix:** `sudo killall -9 cvc_posix` before each test. Test scripts must ensure cleanup.
**Principle:** Integration test environments must be clean. Always verify no stale processes before running.

## What Layer 3 Missed

Layer 3 (BSW integration test) mocked `Can_Write` and called `CanIf_RxIndication` directly. It never tested:
1. The real socket receive path (`Can_Hw_Receive` → `CanIf_RxIndication`)
2. The real config wiring (`main.c` → generated configs)
3. Process lifecycle (zombie cleanup)

A Layer 3 socket receive test would have caught Bugs 1-3 earlier. We added one retrospectively (`test_can_socket_rx.py`).

## Pattern: Hand-Written Config Shadowing

| Config | main.c (hand-written) | Generated file | Entries |
|--------|----------------------|----------------|---------|
| CanIf | 9 TX + 13 RX | CanIf_Cfg_Cvc.c | 12 TX + 33 RX |
| PduR | 13 routing | PduR_Cfg_Cvc.c | 33 routing |
| Com | extern (correct!) | Com_Cfg_Cvc.c | — |
| Rte | extern (correct!) | Rte_Cfg_Cvc.c | — |
| Dcm | extern (correct!) | Dcm_Cfg_Cvc.c | — |

Com, Rte, Dcm already used `extern` to generated configs. CanIf and PduR were the holdouts.

**Rule:** After this fix, ALL config structs in main.c must be `extern` to generated files. No exceptions. No hand-written routing tables.

## Key Metrics

- **Time to find:** ~2 hours of debug (strace, XCP readout, DIAG traces)
- **Time to fix:** 5 minutes each (replace static array with extern)
- **Tests that caught it:** Layer 4 full suite (30 tests) and Layer 3 socket test
- **Tests that missed it:** Layer 1-3 unit + BSW integration (mocked Can driver)
