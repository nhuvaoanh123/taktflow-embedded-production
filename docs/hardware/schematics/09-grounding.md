# 09 — Grounding

**Block**: Star ground, analog/digital separation
**Source**: HWDES Section 8

## Star Ground Topology

```
  [Star Ground Point] (single screw terminal or copper bus bar on base plate)
         |
    +----+----+----+----+----+----+----+
    |    |    |    |    |    |    |    |
   PSU  CVC  FZC  RZC   SC  Motor Servo
   GND  GND  GND  GND  GND  GND   GND
              |
         [CAN GND]
         (common ref)
```

## Ground Plane and Separation Rules

### 1. Star Ground

All ground returns converge at a single star ground point (screw terminal or bus bar). This prevents ground loops and ensures all ECUs share the same ground reference.

### 2. Analog/Digital Ground Separation

On each ECU's protoboard, the analog ground (ADC reference, sensor returns) and digital ground (MCU, CAN transceiver) shall be separated by at least 5 mm and connected at a single point near the MCU's VSS/VSSA pins.

### 3. ACS723 Ground Reference

The ACS723 analog output ground (GND pin) shall be connected directly to the RZC's analog ground, with a short, low-impedance trace. The ACS723 power path ground (IP- pin) connects to the motor power ground, which is a separate high-current path.

### 4. Motor Power Ground

The motor power return (from BTS7960 B- to PSU GND) shall use 16 AWG wire and connect directly to the star ground point. This high-current path shall not pass through any signal ground connections.

### 5. CAN Bus Ground

A CAN reference ground wire (CAN_GND) shall run alongside the CAN_H/CAN_L twisted pair. This provides a common-mode reference for all CAN transceivers. CAN_GND connects to the star ground point.

### 6. Decoupling Ground Returns

All decoupling capacitor grounds shall connect to the local ground plane/wire within 5 mm of the capacitor.

## Key Notes

- **Star ground prevents ground loops** — critical for ADC accuracy on RZC (current sensing, temperature, battery voltage)
- **Motor ground is isolated** from signal ground until the star point — prevents motor noise from coupling into ADC readings
- **CAN GND provides common-mode reference** — required by ISO 11898-2 for proper differential signaling
- **16 AWG for power returns, 22 AWG for signal returns** — prevents voltage drop on high-current paths

## Design Decisions Explained

### Why star ground (not daisy-chain ground)?

In a daisy-chain ground, each ECU connects its ground to the next ECU's ground, forming a chain. The problem: current flowing through one ECU's ground wire creates a voltage drop (V = I × R of the wire), which shifts the ground reference for all downstream ECUs. This is called a "ground loop." With motor current (5-25A), these voltage shifts can be millivolts or even volts — enough to corrupt ADC readings and cause CAN communication errors.

Star ground solves this by routing every ground wire directly to one central point (the star point). Each ECU has its own independent ground path. Motor current flows through the motor's ground wire to the star point and back to the PSU — it never flows through any ECU's ground wire. This keeps each ECU's ground reference clean and stable.

### Why separate analog and digital ground on each ECU board?

Digital circuits (MCU, CAN transceiver) switch rapidly between 0V and 3.3V, creating current spikes on the ground plane. These current spikes cause tiny voltage fluctuations on the ground. If the ADC's ground reference shares these fluctuations, the ADC reads noise instead of the actual signal. By separating analog ground (ADC reference, sensor returns) and digital ground (MCU, CAN) on the protoboard and connecting them at one point near the MCU's VSS/VSSA pins, the digital noise stays on the digital side.

This matters most on the RZC, which has 6 ADC channels measuring current, temperature, and battery voltage — all at millivolt precision.

### Why connect analog and digital ground at the MCU's VSS/VSSA pins (not at the star point)?

The MCU internally references its ADC to its VSSA (analog ground) pin. If analog and digital ground are connected far from the MCU (e.g., at the star point on the base plate), the wire between the connection point and the MCU's pins introduces a voltage drop from digital current. Connecting them right at the MCU ensures the ADC ground reference is as clean as possible. The single connection point prevents ground loops between the two planes.

### Why separate the motor power ground from signal ground?

The DC motor draws 5-25A through its ground return. This current flowing through any wire creates a magnetic field and a voltage drop along the wire. If signal ground wires share any path with the motor ground, the motor's magnetic field induces noise into the signal wires, and the motor's ground voltage drop shifts the signal ground reference. By running the motor ground as a dedicated 16 AWG wire directly from the BTS7960 to the star ground point, motor current never flows through any signal path.

### Why a CAN reference ground wire alongside CAN_H/CAN_L?

CAN is a differential bus — it works by measuring the voltage DIFFERENCE between CAN_H and CAN_L. In theory, it doesn't need a ground reference. In practice, without a common ground reference, the common-mode voltage between ECUs can drift outside the transceiver's input range (typically -2V to +7V). The CAN_GND wire ensures all transceivers share the same ground reference, keeping common-mode voltage near 0V. ISO 11898-2 (the CAN physical layer standard) recommends this reference wire.

### Why 16 AWG for power returns and 22 AWG for signal returns?

Wire resistance is inversely proportional to cross-sectional area (thicker = lower resistance). The motor draws up to 25A through its ground return. Using 22 AWG (0.326 mm²) at 25A would create a significant voltage drop: V = I × R = 25A × 0.053 Ω/m × 0.5m = 0.66V. That's 0.66V of ground shift — enough to cause ADC errors and logic-level problems. 16 AWG (1.31 mm²) at 25A: V = 25A × 0.013 Ω/m × 0.5m = 0.16V — much more acceptable. For signals (milliamps), 22 AWG creates negligible voltage drop and is easier to route.

### Why "decoupling caps within 5mm" (why does distance matter)?

Every wire has inductance — it resists changes in current flow. When an IC demands a sudden current spike, the wire's inductance prevents the current from arriving instantly. The decoupling capacitor provides the current locally (from its stored charge), but it can only do this effectively if the loop area between the cap, the IC's VDD pin, and the IC's GND pin is tiny. A 5mm trace has ~5nH of inductance. A 50mm trace has ~50nH — ten times more. More inductance = less effective decoupling = more voltage noise on the IC's power pins.

## BOM References

| Component | BOM # |
|-----------|-------|
| Screw terminal 2-position (x10) | #62 |
| Screw terminal 3-position (x6) | #63 |
| 16 AWG wire (red + black) | #68 |
| 18 AWG wire (red + black) | #67 |
| 22 AWG hookup wire kit | #66 |
| Base plate | #59 |
