---
document_id: WIRE-LOG
title: "Physical Wiring Log"
version: "1.1"
status: active
date: 2026-03-03
---

# Physical Wiring Log

Real-world wire connections for the bench setup. Updated as wiring changes.

## Bench Setup

- **Nucleo CVC**: SN `0027003C3235510B37333439` — COM7
- **Nucleo FZC**: SN `0049002D3235510C37333439` — COM8
- **Both powered via USB** from same PC (shared ground)
- **2 breadboards** — one per TJA1051 module

---

## TJA1051 Module Pin Layout (single-row, both boards identical)

```
Row 1: UCC    (3.3V power)
Row 2: GND    (ground)
Row 3: CTX    (CAN TX input from MCU)
Row 4: CRX    (CAN RX output to MCU)
Row 5: CANH   (CAN bus high)
Row 6: CANL   (CAN bus low)
Row 7: S      (standby — tie to GND)
Row 8: NC     (no connect)
```

---

## CAN Bus (F2 Bring-Up)

### Board 1: Nucleo CVC → TJA1051 #1

| Wire Color | From | To | Breadboard Row |
|---|---|---|---|
| Red | Nucleo 3V3 | UCC | Row 1 |
| White | Nucleo GND | GND | Row 2 |
| Green | CN10 pin 14 (PA12) | CTX | Row 3 |
| Blue | CN10 pin 16 (PA11) | CRX | Row 4 |
| White | Nucleo GND (or jumper from row 2) | S | Row 7 |

### Board 2: Nucleo FZC → TJA1051 #2

| Wire Color | From | To | Breadboard Row |
|---|---|---|---|
| Red | Nucleo 3V3 | UCC | Row 1 |
| White | Nucleo GND | GND | Row 2 |
| Green | CN10 pin 14 (PA12) | CTX | Row 3 |
| Blue | CN10 pin 16 (PA11) | CRX | Row 4 |
| White | Nucleo GND (or jumper from row 2) | S | Row 7 |

### CAN Bus Wires (between boards)

| Wire Color | From | To |
|---|---|---|
| Yellow | Board 1 CANH (row 5) | Board 2 CANH (row 5) |
| Violet | Board 1 CANL (row 6) | Board 2 CANL (row 6) |

### Termination (120 ohm resistors)

| Location | Value | Breadboard Rows |
|---|---|---|
| Board 1 (CVC) | 120 ohm | Row 5 (CANH) ↔ Row 6 (CANL) |
| Board 2 (FZC) | 120 ohm | Row 5 (CANH) ↔ Row 6 (CANL) |

---

## Notes

- CN10 even pins are on the **outer row** (away from MCU) on these boards
- Pin 1 is marked on the board silkscreen — count down to find pin 14 (7th) and pin 16 (8th)
- TJA1051 S (standby) pin MUST be tied to GND for normal operation
- Total bus termination: 60 ohm (two 120 ohm in parallel)
- Resistors have no polarity — either leg in either row
