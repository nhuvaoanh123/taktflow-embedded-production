# CAN Bus Physical Topology

**Standard:** CAN 2.0B, ISO 11898-2
**Bit Rate:** 500 kbit/s
**Bus Length:** < 5m (bench), < 20m (vehicle)

## Bus Topology

```
 [CVC]---+---[FZC]---+---[RZC]---+---[SC]---+---[BCM]---+---[ICU]---+---[TCU]
  120R   |           |           |          |           |           |    120R
         |           |           |          |           |           |
       CAN_H ========================================================= CAN_H
       CAN_L ========================================================= CAN_L
```

## Termination

| Location | Resistance | Justification |
|----------|-----------|---------------|
| CVC (bus start) | 120 ohm | First node, central controller |
| TCU (bus end) | 120 ohm | Last node on linear bus |

## ECU CAN Transceivers

| ECU | MCU | CAN Peripheral | Transceiver | Connector |
|-----|-----|----------------|-------------|-----------|
| CVC | STM32G474RE | FDCAN1 | TJA1050 | DB9 pin 2/7 |
| FZC | STM32G474RE | FDCAN1 | TJA1050 | DB9 pin 2/7 |
| RZC | STM32F413ZH | bxCAN1 | TJA1050 | DB9 pin 2/7 |
| SC  | TMS570LC4357 | DCAN1 | TJA1040 | DB9 pin 2/7 |
| BCM | STM32G474RE | FDCAN1 | TJA1050 | DB9 pin 2/7 |
| ICU | STM32G474RE | FDCAN1 | TJA1050 | DB9 pin 2/7 |
| TCU | STM32G474RE | FDCAN1 | TJA1050 | DB9 pin 2/7 |

## Wiring

- **CAN_H:** Yellow, twisted pair with CAN_L
- **CAN_L:** Green, twisted pair with CAN_H
- **GND:** Black, star grounded at vehicle chassis
- **Cable type:** Shielded twisted pair, 120 ohm impedance
- **Max stub length:** 0.3m per node (ISO 11898-2)
- **Shield grounding:** Single-point ground at CVC connector

## Bench Setup Notes

- USB-CAN adapter (PEAK PCAN-USB) connected via DB9 T-connector at CVC end
- External 5V power required for each ECU when not USB-powered
- USB hub power (500mA shared) insufficient for >2 ECUs — use bench PSU
