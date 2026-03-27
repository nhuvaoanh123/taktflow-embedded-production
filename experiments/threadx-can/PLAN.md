# ThreadX OS Port — Experiment Progress (2026-03-27)

## Status: 4 ECUs on HIL bench, 3 on ThreadX + 1 bare-metal TMS570

| ECU | Board | OS | CAN | State | Commit |
|-----|-------|----|-----|-------|--------|
| **FZC** | G474RE #1 (SN 001A) | ThreadX | FDCAN | **RUN** | 854ef4d |
| **CVC** | G474RE #2 (SN 0027) | ThreadX | FDCAN | SAFE_STOP | 854ef4d |
| **RZC** | G474RE #3 (SN 0049) | ThreadX | FDCAN | **RUN** | 854ef4d |
| **SC** | TMS570 LaunchPad (XDS110) | Bare-metal | DCAN | Running, FZC/RZC TIMEOUT | 854ef4d |

## Codegen Pipeline (verified on HIL)

| Feature | Status |
|---------|--------|
| TX auto-pull from RTE | DONE — Com_MainFunction_Tx reads RTE, packs PDU |
| RX auto-push to RTE | DONE — Com_RxIndication writes RTE via rteSignalId |
| Split TX/RX signal tables | DONE — no PduId namespace collision |
| E2E SM params from cycle time | DONE — WindowSizeValid/Invalid generated |
| E2E DTC wiring | DONE — per-PDU Dem event on E2E failure |
| E2E recovery event | DONE — INVALID→VALID clears DTC |
| E2E safe defaults | DONE — zero signals on INVALID discard |
| AUTOSAR deadline monitor | DONE — Com_MainFunction_Rx writes COMM_OK/TIMEOUT |
| Legacy Com bridge removal | DONE — filtered in reader.py |
| RTE fallback #error | DONE — compile error if Rte_PbCfg.h missing |
| FDCAN RX DLC fix | DONE — >>16 for HAL constant |
| Double-call fix | DONE — BSW MainFunctions only via RTE |
| Makefile path fix | DONE — strip ../../ for clean builds |

## CAN Bus Sanity (3 G4 boards, SC off)

```
963 frames/s total — matches DBC exactly
All 19 CAN IDs at correct rates
Bus: ERROR-ACTIVE, zero errors
Heartbeat stability: CVC 100%, FZC 95%, RZC 100% over 10s
```

## Blockers for ALL RUN

1. **SC doesn't see FZC/RZC heartbeats** → relay OFF → CVC SAFE_STOP
   - SC DCAN RX works (sees CVC 0x010)
   - Doesn't receive 0x011 (FZC) or 0x012 (RZC)
   - CAN ID filter or acceptance mask issue in SC DCAN config

2. **CVC PLATFORM_HIL removed** — now uses:
   - 30s grace period (`CVC_POST_INIT_GRACE_CYCLES=3000`)
   - SC relay default to energized at init
   - Deadline monitor for comm status
   - After 30s grace: SC relay check activates → if SC relay=OFF → SAFE_STOP

## Next Steps

1. Fix SC DCAN heartbeat reception (CAN ID filter)
2. Verify 4-ECU ALL RUN (CVC+FZC+RZC+SC)
3. Remove PLATFORM_HIL permanently
4. Phase 3: Dem DTC codegen (plan-codegen-gap-closure.md)
5. Port vECUs (BCM/ICU/TCU) to Docker on Pi with matching codegen
