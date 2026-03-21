# CAN Bus Load Budget

**Bus:** 500 kbit/s CAN 2.0B
**Budget:** < 75% sustained, < 90% burst (1s window)
**Frame overhead:** 47 bits (SOF, arbitration, control, CRC, ACK, EOF, IFS)

## Per-Message Load

| CAN ID | Message | DLC | Cycle (ms) | Bits/frame | Load (kbit/s) | Sender |
|--------|---------|-----|-----------|------------|---------------|--------|
| 0x001 | EStop_Broadcast | 4 | 10 (event) | 79 | 0.0 (event) | CVC |
| 0x010 | CVC_Heartbeat | 4 | 50 | 79 | 1.58 | CVC |
| 0x011 | FZC_Heartbeat | 4 | 50 | 79 | 1.58 | FZC |
| 0x012 | RZC_Heartbeat | 4 | 50 | 79 | 1.58 | RZC |
| 0x013 | SC_Status | 4 | 100 | 79 | 0.79 | SC |
| 0x014 | ICU_Heartbeat | 4 | 500 | 79 | 0.16 | ICU |
| 0x015 | TCU_Heartbeat | 4 | 500 | 79 | 0.16 | TCU |
| 0x016 | BCM_Heartbeat | 4 | 500 | 79 | 0.16 | BCM |
| 0x100 | Vehicle_State | 6 | 10 | 95 | 9.50 | CVC |
| 0x101 | Torque_Request | 8 | 10 | 111 | 11.10 | CVC |
| 0x102 | Steer_Command | 8 | 10 | 111 | 11.10 | CVC |
| 0x103 | Brake_Command | 8 | 10 | 111 | 11.10 | CVC |
| 0x200 | Steering_Status | 8 | 50 | 111 | 2.22 | FZC |
| 0x201 | Brake_Status | 8 | 50 | 111 | 2.22 | FZC |
| 0x210 | Brake_Fault | 4 | event | 79 | 0.0 | FZC |
| 0x211 | Motor_Cutoff_Req | 4 | 50 | 79 | 1.58 | FZC |
| 0x220 | Lidar_Distance | 8 | 10 | 111 | 11.10 | FZC |
| 0x300 | Motor_Status | 8 | 100 | 111 | 1.11 | RZC |
| 0x301 | Motor_Current | 8 | 100 | 111 | 1.11 | RZC |
| 0x302 | Motor_Temperature | 8 | 100 | 111 | 1.11 | RZC |
| 0x303 | Battery_Status | 6 | 1000 | 95 | 0.10 | RZC |
| 0x350 | Body_Control_Cmd | 4 | 100 | 79 | 0.79 | CVC |
| 0x400 | Light_Status | 4 | 100 | 79 | 0.79 | BCM |
| 0x401 | Indicator_State | 4 | 100 | 79 | 0.79 | BCM |
| 0x402 | Door_Lock_Status | 2 | event | 63 | 0.0 | BCM |
| 0x500 | DTC_Broadcast | 8 | event | 111 | 0.0 | Any |

## Per-ECU Load Summary

| ECU | Cyclic Load (kbit/s) | % of 500 kbit/s | Event Burst (max) |
|-----|---------------------|------------------|-------------------|
| CVC | 44.27 | 8.9% | +7.9 (EStop × 10) |
| FZC | 18.70 | 3.7% | +7.9 (Brake_Fault × 10) |
| RZC | 4.90 | 1.0% | — |
| SC | 0.79 | 0.2% | — |
| BCM | 1.90 | 0.4% | +0.6 (Door_Lock) |
| ICU | 0.16 | <0.1% | — |
| TCU | 0.16 | <0.1% | — |
| **Total** | **70.88** | **14.2%** | **+16.4** |

## Worst-Case Burst (1s window)

All cyclic messages + all events firing simultaneously:
- Cyclic: 70.88 kbit/s
- Events (EStop 10×, Brake_Fault 10×, Door_Lock 1×, DTC 7×): ~16.4 kbit/s burst
- **Peak: 87.3 kbit/s = 17.5% of 500 kbit/s**

**Result: PASS — well within 75% budget.** Even with UDS diagnostic session (additional ~20 kbit/s), total stays under 25%.
