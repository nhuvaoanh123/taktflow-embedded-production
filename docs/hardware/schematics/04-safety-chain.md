# 04 — Safety Chain

**Block**: Kill relay, E-stop, fault LEDs
**Source**: HWDES Sections 5.4.2–5.4.3, 5.1.4

## Kill Relay Circuit

<!-- DECISION: ADR-003 — Energize-to-run relay pattern -->

```
  12V Main Rail
    |
    +---[Relay Coil (12V, 70-120 ohm)]---+
    |                                      |
    |  [1N4007 flyback diode]              |
    |  cathode (+) ---- anode (-)          |
    |     |                |               |
    +-----+                +--- Drain      |
                                |          |
                           [IRLZ44N       ]
                           [ N-MOSFET     ]
                                |          |
                           Source --- GND  |
                                |          |
                           Gate            |
                                |          |
                          [100R series]    |
                                |          |
  SC GIO_A0 ----+--- [10k pulldown] --- GND
                |
            (3.3V output)

  Relay Contact (SPST-NO):
  +-----------+         +-----------+
  | 12V Main  |---NO---+---CLOSED--| 12V Actuator Rail
  | Rail      |   (open when       | (to BTS7960, servos)
  +-----------+    de-energized)   +-----------+
                                        |
                                    [30A Fuse]
```

### Operation

- GIO_A0 = HIGH -> MOSFET ON -> Coil energized -> Relay CLOSED -> Actuator power ON
- GIO_A0 = LOW  -> MOSFET OFF -> Coil de-energized -> Relay OPEN -> Actuator power OFF
- SC power loss -> Gate floats -> 10k pulldown -> MOSFET OFF -> **SAFE STATE**
- SC firmware hang -> TPS3823 reset -> GIO_A0 resets to input -> 10k pulldown -> **SAFE STATE**

### Key Specs

- Dropout timing: < 10 ms from GIO_A0 LOW to relay contacts open
- 1N4007 flyback diode limits coil back-EMF to ~0.7V above rail
- IRLZ44N: VGS(th) < 2V (driven directly by 3.3V GIO output)
- 100R gate resistor limits dI/dt during switching
- 10k pulldown ensures OFF state during all fault conditions

## E-Stop Button Circuit

```
  3.3V (internal pull-up on PC13, ~40k)
    |
    +--- PC13 (EXTI, rising-edge) ---[10k series R]---+--- [100nF] --- GND
    |                                                  |    (RC debounce)
    |                                              [TVS 3.3V]
    |                                                  |
    +--- [NC Push Button] --- GND                     GND

  Resting state: button closed -> PC13 = LOW (connected to GND through button)
  Button pressed: button opens -> PC13 = HIGH (pulled up by internal pull-up)
  Broken wire: PC13 = HIGH (E-stop activated) -- FAIL-SAFE

  Rising edge triggers EXTI interrupt.
  RC debounce: tau = 10k * 100nF = 1 ms.
  TVS diode protects PC13 from static on button wiring.
```

## Fault LED Circuit

```
  SC GIO Pins                    LEDs

  GIO_A1 ---[330R]--- LED_CVC (Red, 3mm)    --- GND
  GIO_A2 ---[330R]--- LED_FZC (Red, 3mm)    --- GND
  GIO_A3 ---[330R]--- LED_RZC (Red, 3mm)    --- GND
  GIO_A4 ---[330R]--- LED_SYS (Amber, 3mm)  --- GND

  GIO_B1 ---[330R]--- LED_HB (Green, 3mm)   --- GND  (heartbeat/status)

  LED current: I = (3.3V - 1.8V) / 330R = 4.5 mA (sufficient for visibility).
  TMS570 GIO pins can source up to 8 mA per pin (4.5 mA is within limits).
  All LEDs OFF at power-up (GIO pins initialized as inputs = high-impedance).
  After init, GIO pins configured as outputs, initialized LOW (LEDs OFF).
  Lamp test during startup: all LEDs ON for 500 ms.
```

## Design Decisions Explained

### Why "energize-to-run" (not "energize-to-stop")?

This is the single most important safety decision in the hardware. "Energize-to-run" means the relay must be actively held closed by the SC firmware. The default (unpowered) state is OPEN = actuators OFF = safe. If the SC loses power, crashes, hangs, or its watchdog resets it, the relay opens automatically. The opposite pattern ("energize-to-stop") would mean actuators are ON by default and you'd need to actively cut power — if the SC fails, actuators keep running uncontrolled. Every safety-critical system uses energize-to-run because it fails to the safe state.

### Why IRLZ44N MOSFET to drive the relay (not a transistor or direct GPIO)?

The relay coil needs ~100-170mA to stay closed. An MCU GPIO pin can only source about 8-20mA — nowhere near enough. The IRLZ44N is a "logic-level" N-channel MOSFET, meaning its gate threshold is low enough (< 2V) to be driven directly by a 3.3V GPIO output. It can switch up to 47A, so 170mA is trivial. A bipolar transistor (like 2N2222) would also work, but MOSFETs waste less power and don't need a base current-limiting resistor.

### Why the 10k pulldown resistor on the MOSFET gate?

When the SC MCU resets or loses power, the GIO_A0 pin becomes a high-impedance input (floating). A floating MOSFET gate can pick up stray voltage from nearby wires and partially turn on the MOSFET — keeping the relay partially energized. The 10k pulldown resistor forces the gate to 0V (GND) whenever the MCU isn't actively driving it HIGH. This guarantees the relay opens during any MCU fault condition. Without it, the relay might stay closed during a crash — defeating the entire safety chain.

### Why 100 ohm series resistor on the gate?

The MOSFET gate acts like a small capacitor (~1800pF for the IRLZ44N). When the GPIO transitions from LOW to HIGH, the gate charges through the wire's inductance, which can cause voltage ringing (oscillation) and high dI/dt current spikes. The 100-ohm resistor limits the charging rate, preventing ringing and reducing electromagnetic interference (EMI). It slightly slows the turn-on time (from nanoseconds to microseconds), which doesn't matter for a relay that takes 5-10ms to close anyway.

### Why 1N4007 flyback diode across the relay coil?

A relay coil is an inductor. When you suddenly cut current through an inductor (MOSFET turns OFF), the inductor generates a voltage spike in the opposite direction (back-EMF) — potentially hundreds of volts. This spike would destroy the MOSFET. The 1N4007 diode is placed "backwards" across the coil (cathode to +12V, anode to MOSFET drain). When the back-EMF spike occurs, the diode conducts and clamps the spike to ~0.7V above the rail — safely dissipating the energy as heat in the coil's resistance. Without this diode, you'd destroy the MOSFET within the first few relay cycles.

### Why NC (Normally Closed) E-stop button (not NO)?

A Normally Closed button maintains a closed circuit when not pressed. Pressing it OPENS the circuit. This is fail-safe: if the wire connecting the button breaks, the circuit opens — same as pressing the button. With a Normally Open button, a broken wire would make the E-stop non-functional (you'd press it and nothing would happen). In safety engineering, you always design so that component failure triggers the safe state.

### Why RC debounce on the E-stop (not just software debounce)?

Mechanical buttons "bounce" — when pressed or released, the contacts make and break several times in milliseconds, generating rapid HIGH-LOW-HIGH-LOW transitions. Software debounce works fine for user buttons, but for a safety-critical E-stop, we add hardware debounce (10k resistor + 100nF capacitor = 1ms time constant) as a first line of defense. The RC filter smooths out the bounces before they reach the MCU pin, preventing false triggers. The software can add additional debounce on top.

### Why 330 ohm resistors for the LEDs?

LEDs have a fixed forward voltage drop (~1.8V for red/amber, ~2.0V for green) and need a current-limiting resistor to prevent them from drawing too much current and burning out. The formula: R = (V_supply - V_LED) / I_desired = (3.3V - 1.8V) / 4.5mA = 330 ohms. This gives ~4.5mA per LED, which is bright enough to see clearly while staying well within the TMS570 GIO pin's 8mA source limit. Lower resistance = brighter but more current; higher resistance = dimmer.

### Why a lamp test at startup (all LEDs ON for 500ms)?

This is standard automotive practice. When you start your car, all dashboard warning lights briefly illuminate — this proves the LEDs and their driving circuits actually work. If an LED were burned out, you'd never know (because it's always off). The 500ms lamp test gives the operator visual confirmation that every fault indicator is functional.

## Pin References

| Function | ECU | Pin | Direction | Net Name |
|----------|-----|-----|-----------|----------|
| Kill relay control | SC | GIO_A0 | OUT | SC_KILL_RELAY |
| CVC fault LED | SC | GIO_A1 | OUT | SC_LED_CVC |
| FZC fault LED | SC | GIO_A2 | OUT | SC_LED_FZC |
| RZC fault LED | SC | GIO_A3 | OUT | SC_LED_RZC |
| System fault LED | SC | GIO_A4 | OUT | SC_LED_SYS |
| Heartbeat LED | SC | GIO_B1 | OUT | SC_LED_HB |
| E-stop input | CVC | PC13 | IN | CVC_ESTOP |

## BOM References

| Component | BOM # |
|-----------|-------|
| 30A automotive relay | #24 |
| IRLZ44N MOSFET | #25 |
| 1N4007 flyback diodes (x2) | #26 |
| E-stop button (NC, mushroom) | #27 |
| LED 3mm red (x5) | #29 |
| LED 3mm green (x5) | #30 |
| LED 3mm amber (x2) | #31 |
| 330 ohm resistors | #51 |
| 100 ohm resistors | #52 |
| 10k ohm resistors | #49 |
| 100nF capacitors | #43 |
| 3.3V TVS diodes | #56 |
| Relay socket | #72 |
