# 08 — SC Circuits

**Block**: Safety Controller — DCAN1 (normal mode, TX+RX), kill relay, fault LEDs, WDT, power
**Source**: HWDES Section 5.4

## DCAN1 CAN Interface (Normal Mode, SWR-SC-029)

```
  TMS570LC43x                       SN65HVD230
  +-----------+                      +------------------+
  |           |                      |                  |
  | DCAN1_TX  +-- J10 pin 45 (TX) -->| TXD         CANH |--[CMC]--+-- CAN_H
  |           |                      |                  |         |
  | DCAN1_RX  +-- J10 pin 44 (RX) --<| RXD         CANL |--[CMC]--+-- CAN_L
  |           |                      |                  |         |
  +-----------+  3.3V --[100nF]----->| VCC          GND |  [120R termination]
                                     |                  |         |
                                     | Rs           N/C |       CAN_L
                                     +--+---------------+
                                        |
                                       GND (Rs = GND for full speed)

  DCAN1 runs in normal operation (TEST register NOT modified, SWR-SC-029).
  SC transmits SC_Status (CAN ID 0x013) via mailbox 7 every 500ms.
  SC receives heartbeat/state frames on mailboxes 1-6.
  SN65HVD230 is TI part (same vendor as TMS570).
  120R termination at SC (SC is at one end of the bus).
  J10 pin 44 = DCAN1RX, J10 pin 45 = DCAN1TX (per schematic sprr397.pdf).
  NOT DCAN4 — HALCoGen v04.07.01 mailbox bug on DCAN4.
```

## Kill Relay Circuit

See `04-safety-chain.md` for full schematic. SC GIO_A0 drives IRLZ44N MOSFET gate to control the energize-to-run relay.

## Fault LED Circuit

```
  SC GIO Pins                    LEDs

  GIO_A1 ---[330R]--- LED_CVC (Red, 3mm)    --- GND
  GIO_A2 ---[330R]--- LED_FZC (Red, 3mm)    --- GND
  GIO_A3 ---[330R]--- LED_RZC (Red, 3mm)    --- GND
  GIO_A4 ---[330R]--- LED_SYS (Amber, 3mm)  --- GND

  GIO_B1 ---[330R]--- LED_HB (Green, 3mm)   --- GND  (heartbeat/status)

  LED current: I = (3.3V - 1.8V) / 330R = 4.5 mA.
  Lamp test during startup: all LEDs ON for 500 ms.
```

## External Watchdog Circuit

```
                    TPS3823DBVR
                    +-----------+
  3.3V --[100nF]--->| VDD    MR |--- 3.3V
                    |           |
  GIO_A5 (GPIO) -->| WDI  RESET|---[100nF]--- TMS570 nRST
                    |           |
                    | CT    GND |
                    +---+-------+
                        |
                     [100nF]
                        |
                       GND

  Same configuration as STM32 ECU watchdogs.
  TPS3823 provides independent reset mechanism for faults not caught by lockstep.
  Lockstep detects computation errors; TPS3823 detects hang/timing faults.
```

## Power Supply Design

```
  12V Main Rail (NOT through kill relay)
       |
  [Schottky diode - reverse polarity protection]
       |
  [LM1117-3.3 Linear Regulator]
       |
  3.3V (SC dedicated rail)
       |
       +--- TMS570LC43x (via LaunchPad Vin or direct 3.3V header)
       +--- SN65HVD230 CAN transceiver
       +--- TPS3823 watchdog
       +--- Fault LEDs (5x, through current-limiting resistors)
       +--- Kill relay MOSFET gate circuit

  ALTERNATIVELY: SC powered via LaunchPad USB (5V USB -> onboard 3.3V LDO).

  CRITICAL: SC power is independent of the kill relay.
  When kill relay opens, SC remains powered to:
    - Log DTCs
    - Maintain fault LED indication
    - Continue watchdog feed (if firmware is healthy)
```

## Design Decisions Explained

### Why a separate safety controller (not just software monitoring in the CVC)?

ISO 26262 requires **independence** between the function being monitored and the monitor itself. If the CVC's software monitors its own health, a bug or hardware fault in the CVC could affect both the function AND the monitor simultaneously — defeating the purpose. The SC is a physically separate MCU (different chip, different power supply, different code) that watches the other ECUs from the outside. Even if the CVC completely crashes, the SC still sees the missing heartbeat and cuts power.

### Why TMS570 (not another STM32) for the Safety Controller?

The TMS570LC43x has **lockstep** — two CPU cores running the same code in parallel, with hardware comparison of their outputs every clock cycle. If a cosmic ray, power glitch, or silicon defect causes one core to produce a wrong result, the hardware detects the mismatch instantly and triggers a reset. This provides extremely high fault coverage for computation errors. The STM32G474RE doesn't have lockstep. For ASIL D safety monitoring, lockstep is the gold standard. Additionally, the TMS570 is TI's dedicated automotive safety MCU — it's designed and qualified for exactly this use case.

### Why SN65HVD230 (not TJA1051) for the SC?

Both are 3.3V CAN transceivers and both would work. The SN65HVD230 is a Texas Instruments part — same vendor as the TMS570 MCU. Using same-vendor parts simplifies procurement, ensures tested compatibility, and is a common practice in automotive design. The SN65HVD230's "Rs" pin tied to GND sets it to full-speed mode (slope control disabled), matching our 500 kbps requirement. There's no technical disadvantage vs the TJA1051 for our application.

### Why silent/listen-only mode for the SC's CAN?

In listen-only mode, the SC receives all CAN frames but never transmits or acknowledges. Benefits: (1) **Non-interference** — if the SC has a CAN fault (wrong bit timing, software bug), it cannot corrupt the bus for the CVC/FZC/RZC. (2) **Independent monitoring** — the SC passively observes heartbeat messages from each ECU. If an ECU's heartbeat stops arriving, the SC knows that ECU has failed — without relying on the ECU to report its own failure. (3) **Bus load** — the SC adds zero bus load, leaving full bandwidth for the working ECUs.

### Why is SC power independent of the kill relay?

When the SC detects a fault and opens the kill relay, it must remain powered to: (1) **Log Diagnostic Trouble Codes (DTCs)** — recording what went wrong for post-mortem analysis. (2) **Maintain fault LED indication** — the operator needs to see which ECU faulted. (3) **Decide when to re-enable** — in a real vehicle, some faults are transient (brief sensor glitch). The SC needs to stay alive to evaluate whether it's safe to re-enable the relay. If the SC lost power when the relay opened, all fault information would be lost.

### Why TPS3823 external watchdog on the SC (isn't lockstep enough)?

Lockstep detects **computation errors** (both cores produce different results). But lockstep does NOT detect: (1) **firmware hang** — if both cores get stuck in an infinite loop, they produce the same wrong result (no mismatch). (2) **clock failure** — if the clock stops, both cores stop together (no mismatch). (3) **timing faults** — if the firmware runs but misses its deadline, lockstep doesn't notice. The TPS3823 is a timing watchdog — it requires a periodic toggle from the firmware. If the toggle stops for 1.6 seconds (for any reason), it forces a hard reset. Lockstep + external watchdog = comprehensive fault detection.

### Why the SC uses GIO pins (not specialized peripherals) for LEDs and relay?

The SC's functions are simple: read CAN, toggle LEDs, control one relay, feed the watchdog. These only need basic GPIO (General Purpose Input/Output). GIO on the TMS570 provides direct pin control without complex peripheral configuration. Using specialized peripherals (timers for PWM, DMA, etc.) would add unnecessary complexity for what are essentially on/off signals. The simplicity also reduces the firmware attack surface — less code = fewer bugs = higher safety integrity.

### Why per-ECU fault LEDs (not just one system LED)?

When something goes wrong, you need to know WHICH ECU faulted. A single "system fault" LED tells you something is wrong but not where. With per-ECU LEDs (CVC, FZC, RZC), the operator can immediately identify the failing ECU — critical for debugging during bring-up and for vehicle diagnostics in the field. The amber system LED lights up for faults that aren't ECU-specific (CAN bus-off, power fault, watchdog reset).

## Pin Summary

| # | Function | Pin | Connector | Direction | Net Name | ASIL |
|---|----------|-----|-----------|-----------|----------|------|
| 1 | CAN TX | DCAN1TX | J10 pin 45 (proto header) | OUT | SC_CAN_TX | D |
| 2 | CAN RX | DCAN1RX | J10 pin 44 (proto header) | IN | SC_CAN_RX | D |
| 3 | Kill relay control | GIO_A0 | J3-1 (HDR1) | OUT | SC_KILL_RELAY | D |
| 4 | CVC fault LED | GIO_A1 | J3-2 (HDR1) | OUT | SC_LED_CVC | B |
| 5 | FZC fault LED | GIO_A2 | J3-3 (HDR1) | OUT | SC_LED_FZC | B |
| 6 | RZC fault LED | GIO_A3 | J3-4 (HDR1) | OUT | SC_LED_RZC | B |
| 7 | System fault LED | GIO_A4 | J3-5 (HDR1) | OUT | SC_LED_SYS | B |
| 8 | WDT feed | GIO_A5 | J3-6 (HDR1) | OUT | SC_WDT_WDI | D |
| 9 | Heartbeat LED | GIO_B1 | J12 (onboard) | OUT | SC_LED_HB | QM |

### DCAN1 Configuration Notes

- DCAN1 module enabled via HALCoGen
- Normal mode: TEST register NOT set (SWR-SC-029 — SC transmits SC_Status on mailbox 7)
- Bit timing: 500 kbps with 80% sample point
- Mailboxes: objects 1-6 for RX (heartbeat 0x010-0x012, vehicle state 0x100, torque 0x101, motor current 0x301); mailbox 7 TX-only for SC_Status 0x013
- Edge connector wiring: DCAN1RX on J10 pin 44, DCAN1TX on J10 pin 45 (per schematic sprr397.pdf)

## BOM References

| Component | BOM # |
|-----------|-------|
| TMS570LC43x LaunchPad (LAUNCHXL2-570LC43) | #2 |
| SN65HVD230 module | #7 |
| TPS3823DBVR | #22 |
| SOT-23-5 breakout | #23 |
| 30A automotive relay | #24 |
| IRLZ44N MOSFET | #25 |
| 1N4007 diodes (x2) | #26 |
| LED 3mm red (x3) | #29 |
| LED 3mm green (x1) | #30 |
| LED 3mm amber (x1) | #31 |
| 330 ohm resistors | #51 |
| 100 ohm resistors | #52 |
| 10k resistors | #49 |
| 100nF capacitors | #43 |
| 120 ohm terminator | #9 |
| Common-mode choke | #10 |
| Relay socket | #72 |
