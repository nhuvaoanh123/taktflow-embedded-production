# 03 — CAN Bus

**Block**: CAN topology, transceivers, termination
**Source**: HWDES Section 6

## Bus Topology

```
  [CVC]----100mm----[FZC]----100mm----[RZC]----100mm----[SC]
   |                                                      |
  [120R]                                                [120R]
  term.                                                 term.

  Bus stubs: < 100 mm from main trunk to each ECU transceiver
  Total bus length: ~300 mm main trunk + stubs = ~700 mm total
  (well within 2 m maximum specified in HWR-014)

  Additional taps (T-connection, < 100 mm stub):
  - CANable 2.0 #1: connected to Raspberry Pi 4 (USB)
  - CANable 2.0 #2: connected to development PC (USB) for CAN analyzer
```

## STM32 CAN Transceiver Circuit (CVC/FZC/RZC)

```
  STM32G474RE                TJA1051T/3 Module
  +-----------+              +------------------+
  |           |              |                  |
  | FDCAN1_TX +--- PA12 --->| TXD         CANH |--+--[CMC]--+-- CAN_H (yellow)
  | (AF9)     |              |                  |  |         |
  | FDCAN1_RX +--- PA11 ---<| RXD         CANL |--+--[CMC]--+-- CAN_L (green)
  | (AF9)     |              |                  |  |         |
  +-----------+  3.3V --+-->| VCC          GND |--+-- GND   +--[120R]--+
                        |   |                  |                       |
                    [100nF] | STB          N/C |                    CAN_L
                        |   +------------------+
                       GND        |
                                 GND (STB tied LOW = always active)

  CMC = Common-mode choke (100 uH minimum)
  TVS diodes (PESD1CAN) placed between CMC and bus connectors
  120R termination resistor at CVC end of bus (FZC, RZC: no termination)
```

## TMS570 CAN Transceiver Circuit (SC)

```
  TMS570LC43x                   SN65HVD230
  +-----------+                  +------------------+
  |           |                  |                  |
  | DCAN1_TX  +--- (edge conn)->| TXD         CANH |--[CMC]--+-- CAN_H
  |           |                  |                  |         |
  | DCAN1_RX  +--- (edge conn)<-| RXD         CANL |--[CMC]--+-- CAN_L
  |           |                  |                  |         |
  +-----------+  3.3V --[100nF]->| VCC          GND |  [120R termination]
                                 |                  |         |
                                 | Rs           N/C |       CAN_L
                                 +--+---------------+
                                    |
                                   GND (Rs = GND for full speed)

  DCAN1 TEST register bit 3 = 1 (Silent/Listen-only mode).
  120R termination at SC (SC is at one end of the bus).
  Using DCAN1 via edge connector (NOT DCAN4 due to HALCoGen bug).
```

## CAN Connector Plan

Each ECU connects via a 3-position screw terminal block:

| Terminal | Signal | Wire Color |
|----------|--------|------------|
| 1 | CAN_H | Yellow |
| 2 | CAN_L | Green |
| 3 | GND (CAN reference) | Black |

Daisy-chain wiring: main trunk wire runs along base plate edge, continuous through each terminal block.

## Bit Timing Configuration

| Parameter | STM32G474RE (FDCAN) | TMS570LC43x (DCAN) |
|-----------|--------------------|--------------------|
| Input clock | 170 MHz (PCLK1) | 75 MHz (VCLK1) |
| Prescaler | 17 | 10 (BRP field=9, +1 encoding) |
| Nominal bit rate clock | 10 MHz | 7.5 MHz |
| Time quanta per bit | 20 | 15 |
| TSEG1 | 15 tq | 11 tq (field=10, +1 encoding) |
| TSEG2 | 4 tq | 3 tq (field=2, +1 encoding) |
| SJW | 4 tq | 4 tq (field=3, max for 2-bit BTR field) |
| Sample point | 80% | 80% |
| Actual bit rate | 500.000 kbps | 500.000 kbps |

## Key Notes

- **Common-mode chokes** (100 uH) on every transceiver for EMC compliance
- **PESD1CAN TVS diode arrays** between chokes and bus connectors for ESD protection
- **Bus termination**: 120 ohm at each physical end (CVC and SC)
- **CAN GND**: Reference ground wire runs alongside CAN_H/CAN_L twisted pair

## Design Decisions Explained

### Why CAN bus (not SPI, I2C, UART, or Ethernet)?

CAN (Controller Area Network) was invented specifically for vehicles. Key advantages: (1) **Differential signaling** — uses two wires (CAN_H and CAN_L) that mirror each other, making it immune to electrical noise from motors and actuators. (2) **Multi-master** — any ECU can transmit without a central coordinator. (3) **Built-in error handling** — CAN hardware automatically detects and retransmits corrupted messages. (4) **Priority-based** — lower CAN ID = higher priority, so safety-critical messages always get through first. (5) **Industry standard** — every car on the road uses CAN, so using it makes this project directly relevant to real automotive work. SPI/I2C are for short-distance chip-to-chip communication. Ethernet is overkill for our data rates.

### Why TJA1051T/3 for STM32 ECUs and SN65HVD230 for the Safety Controller?

Both are 3.3V CAN transceivers (the chip that converts the MCU's digital TX/RX signals into the differential CAN_H/CAN_L bus signals). The TJA1051T/3 (NXP) is the most common automotive CAN transceiver — it's robust, widely available, and cheap as a breakout module. The SN65HVD230 (Texas Instruments) is used for the SC because it's the same vendor as the TMS570 MCU (TI), ensuring compatibility. Both operate at 3.3V logic level, matching our MCUs. The "STB" pin (standby) is tied to GND on the TJA1051, keeping it always active — we don't need power-saving standby mode.

### Why common-mode chokes (CMC) on every transceiver?

A common-mode choke filters out high-frequency noise that appears on both CAN_H and CAN_L simultaneously (common-mode noise). This noise comes from motor switching, power supply ripple, and external EMI. The choke blocks common-mode noise while passing the differential CAN signal through unchanged. Without CMCs, noise on the bus can cause bit errors and retransmissions, degrading communication reliability. The 100 uH rating is standard for automotive CAN applications.

### Why PESD1CAN TVS diodes?

TVS (Transient Voltage Suppressor) diodes protect the CAN transceiver from voltage spikes — especially ESD (electrostatic discharge) from touching connectors, and transients from the motor or relay switching. The PESD1CAN is specifically designed for CAN bus: it clamps voltage spikes to safe levels in nanoseconds. Without TVS protection, a single static discharge could permanently damage a CAN transceiver.

### Why 3-position screw terminals for CAN connections?

The three wires are CAN_H, CAN_L, and CAN_GND (ground reference). Screw terminals allow easy connect/disconnect during development and debugging. The alternative (soldered connections) would be more reliable but makes it painful to swap ECUs or reconfigure the bus. For a production vehicle, you'd use automotive connectors (Molex, TE Connectivity).

### Why the same bit timing on both STM32 and TMS570?

Both MCUs must agree on exactly how fast bits are transmitted and when to sample them. If the timing doesn't match, they can't communicate. The prescaler values are different (34 for STM32's 170MHz clock vs 15 for TMS570's 75MHz clock) because they divide different input clocks down to the same 5 MHz "time quantum" clock. From there, both use 10 time quanta per bit with 80% sample point — meaning each bit is sampled at 80% of its duration, which is the sweet spot recommended by CAN specifications for noise immunity.

### Why DCAN1 (not DCAN4) on the TMS570?

The TMS570 has multiple CAN modules. DCAN4 has a known bug in HALCoGen (TI's code generation tool) v04.07.01 where the mailbox configuration doesn't work correctly. DCAN1 works reliably with HALCoGen-generated code. This is a practical lesson: always verify tool support for peripheral modules before committing to a design.

### Why is the SC in "silent/listen-only" mode?

The Safety Controller's job is to MONITOR the other ECUs — not participate in normal communication. In silent mode, the SC receives all CAN frames but never transmits or acknowledges them. This means: (1) if the SC has a CAN fault, it can't disrupt the bus for the other ECUs, (2) the SC can independently verify that heartbeat and status messages are being sent correctly. Think of it as a security camera — it watches everything but doesn't interfere.

### Why daisy-chain wiring (not star topology)?

CAN bus requires a linear bus topology — one continuous wire from end to end, with ECUs tapped off it via short stubs (<100mm). A star topology (all ECUs connected to a central hub) causes signal reflections at the junction points, corrupting data. Our bus runs CVC→FZC→RZC→SC in a line along the base plate edge, with 120-ohm termination at each end.

## Pin References

| ECU | TX Pin | RX Pin | AF | Transceiver |
|-----|--------|--------|-----|-------------|
| CVC | PA12 | PA11 | AF9 | TJA1051T/3 |
| FZC | PA12 | PA11 | AF9 | TJA1051T/3 |
| RZC | PA12 | PA11 | AF9 | TJA1051T/3 |
| SC | DCAN1TX (J5) | DCAN1RX (J5) | — | SN65HVD230 |

## BOM References

| Component | BOM # |
|-----------|-------|
| TJA1051T/3 modules (x3) | #6 |
| SN65HVD230 module | #7 |
| CANable 2.0 (x2) | #8 |
| 120 ohm terminators (x4) | #9 |
| Common-mode chokes (x4) | #10 |
| PESD1CAN TVS diodes (x4) | #11 |
| 22 AWG twisted pair wire | #12 |
| 3-position screw terminals | #63 |
