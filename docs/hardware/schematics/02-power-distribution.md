# 02 — Power Distribution

**Block**: 12V rail, buck converters, fuses, kill relay gating
**Source**: HWDES Section 7

## Power Distribution Diagram

```
  [12V Bench PSU 10A]
         |
    [SB560 Schottky] -- reverse polarity protection
         |
    [10A ATC Fuse] -- main rail fuse
         |
    ===== 12V MAIN RAIL (16 AWG) =====
         |         |         |         |         |         |
     [LM2596]  [LM2596]  [Nucleo]  [Nucleo]  [Nucleo]  [LaunchPad]
     12->5V    12->3.3V   Vin(CVC)  Vin(FZC)  Vin(RZC)  Vin(SC)
      |          |
    5V Rail    3.3V Rail
    (3A PTC)   (1A PTC)
      |          |
    TFMini     Sensors
    RPi4       Transceivers
               Watchdogs
               LEDs
         |
    [Kill Relay Coil Circuit] (SC controlled)
         |
    [30A ATC Fuse]
         |
    ===== 12V ACTUATOR RAIL (16 AWG) =====
         |              |              |
    [BTS7960]       [6V Reg #1]    [6V Reg #2]
    Motor Driver    Steering Servo  Brake Servo
    (25A peak)      (2A peak)       (2A peak)
         |              |              |
    [DC Motor]     [MG996R]        [MG996R]
                   [3A Fuse]       [3A Fuse]
```

## Power Budget

| Domain | Voltage | Current (typical) | Current (peak) | Source |
|--------|---------|-------------------|----------------|--------|
| CVC logic | 3.3V | 230 mA | 250 mA | Nucleo LDO |
| FZC logic | 3.3V | 194 mA | 220 mA | Nucleo LDO |
| RZC logic | 3.3V | 162 mA | 180 mA | Nucleo LDO |
| SC logic | 3.3V | 150 mA | 200 mA | LaunchPad LDO |
| External 3.3V rail | 3.3V | 74 mA | 100 mA | LM2596 buck |
| External 5V rail | 5V | 700 mA | 1340 mA | LM2596 buck |
| Motor (12V actuator rail) | 12V | 5 A | 25 A | Kill relay |
| Servos (6V from 12V act.) | 6V | 1 A | 5 A | 6V regulators |
| Kill relay coil | 12V | 100 mA | 170 mA | Main rail |
| **Total from 12V supply** | **12V** | **~3.5 A** | **~8.5 A** | |

Peak current (8.5A) is within the 10A PSU rating. Motor stall current (25A) is transient (<100 ms) before overcurrent protection triggers.

## Key Notes

- **Reverse polarity**: SB560 Schottky at PSU entry (~0.5V drop at full load)
- **PTC fuses**: Resettable PTC on 5V rail (3A) and 3.3V rail (1A) for overcurrent protection
- **Actuator isolation**: Kill relay gates ALL actuator power — SC-controlled fail-safe
- **Servo fuses**: 3A fast-blow glass fuses on each 6V servo feed
- **Wire gauge**: 16 AWG for 12V main rail and actuator rail, 22 AWG for signal/logic

## Design Decisions Explained

### Why a bench power supply instead of a battery?

For a development platform, a bench PSU is safer and more practical. You can set exact voltage/current limits, see real-time current draw, and it won't run out of charge during testing. The current-limiting feature also protects your circuits: if there's a short, the PSU limits current instead of dumping 100+ amps like a battery would. A 12V battery can be added later for field demos — the circuit is the same.

### Why LM2596 buck converters (not linear regulators like 7805)?

A linear regulator (like 7805) converts 12V to 5V by burning the excess 7V as heat: P = 7V × I. At 1A, that's 7 watts of pure heat — you'd need a heatsink. A buck converter (switching regulator like LM2596) is ~85% efficient, wasting much less power as heat. The LM2596 can deliver 3A, runs cool, and costs $3 as a module. The downside is electrical noise (switching noise), but for our digital circuits and robust sensors, it's fine.

### Why separate 5V and 3.3V external rails?

Different components need different voltages. The TFMini-S lidar and Raspberry Pi need 5V. The sensors (AS5048A), CAN transceivers, and watchdog ICs need 3.3V. Running everything from one voltage would require individual level shifters or risk damaging 3.3V components with 5V.

### Why PTC (resettable) fuses on the logic rails?

PTC fuses (polyfuses) automatically reset after the fault clears — unlike glass fuses which blow once and need manual replacement. For the 5V and 3.3V logic rails, faults are usually transient (brief short during debugging, loose wire). A resettable fuse lets you fix the problem and keep working without hunting for replacement fuses. The 3A PTC on the 5V rail and 1A PTC on the 3.3V rail are sized to protect the LM2596 modules from overcurrent.

### Why glass tube fuses (not PTC) for the servo power?

Servos can draw high peak current (up to 2.5A each). PTCs respond slowly to overcurrent — they might let a fault persist long enough to damage the servo or 6V regulator. A fast-blow glass fuse opens within milliseconds at >3A, providing faster protection. For actuators, you WANT the circuit to stay broken until you investigate — not auto-reset.

### Why 6V regulated servo power (not direct 12V or 5V)?

Standard hobby servos (MG996R) are rated for 4.8V-7.2V. At 5V they work but have reduced torque. At 12V they would burn out immediately. 6V gives near-maximum torque while staying safely within the servo's rating. The 6V comes from the 12V actuator rail (through the kill relay), so cutting the relay also cuts servo power — which is correct for safety.

### Why 16 AWG wire for power rails and 22 AWG for signals?

Wire gauge determines how much current the wire can safely carry. 16 AWG handles ~22A continuously — plenty for the motor (5A typical, 25A peak stall). 22 AWG handles ~7A — far more than any signal needs (milliamps), and it's thin enough to route easily. Using thick wire for signals wastes space; using thin wire for power causes voltage drop and heat. The 18 AWG in between is for moderate-current runs (ECU power feeds, ~250mA per board).

### Why is the power budget (8.5A peak) under the PSU rating (10A)?

The power budget table shows worst-case simultaneous current draw. The 8.5A peak includes motor stall (25A for <100ms), which is transient — the BTS7960's overcurrent protection kicks in before the PSU trips. In normal operation, total draw is ~3.5A. Having 1.5A of headroom (10A - 8.5A) provides margin for components that draw slightly more than spec, cold-start inrush currents, and measurement uncertainty.

## BOM References

| Component | BOM # |
|-----------|-------|
| 12V 10A bench PSU | #34 |
| LM2596 buck modules (x2) | #35 |
| SB560 Schottky | #36 |
| ATC fuse holders (x2) | #37 |
| ATC fuse 10A | #38 |
| ATC fuse 30A | #39 |
| Glass tube fuse 3A (x4) | #40 |
| Glass tube fuse holders (x2) | #41 |
| 6V regulator modules (x2) | #42 |
| Resettable PTC 3A (x2) | #58 |
| 16 AWG wire | #68 |
| 18 AWG wire | #67 |
