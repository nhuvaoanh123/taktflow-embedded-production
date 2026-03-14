# 07 — RZC Circuits

**Block**: Rear Zone Controller — CAN, BTS7960, ACS723, NTC, battery, encoder, WDT
**Source**: HWDES Section 5.3

> **Note**: On the RZC, the watchdog WDI pin is PB4 (not PB0). PB0 is used for BTS7960 R_EN on this ECU.

## CAN Transceiver Circuit

Same as CVC (see `05-cvc-circuits.md`) using FDCAN1 on PA12 (TX) and PA11 (RX) with TJA1051T/3. If RZC is at a physical bus end, include 120 ohm termination.

## BTS7960 H-Bridge Motor Driver Interface

```
  STM32G474RE                    BTS7960 Module
  +-----------+                  +------------------+
  |           |                  |                  |
  | TIM1_CH1  +--- PA8 -------->| RPWM             |
  | (AF6)     |   (20 kHz PWM)  |                  |
  | TIM1_CH2  +--- PA9 -------->| LPWM             |
  | (AF6)     |   (20 kHz PWM)  |         M+  M-   |---[Motor]
  |           |                  |                  |
  |   GPIO    +--- PB0 -------->| R_EN             |
  |           |     |            |                  |
  |   GPIO    +--- PB1 -------->| L_EN             |
  |           |     |            |                  |
  |  ADC1_CH3 +--- PA3 ---<-----| R_IS (current)   |
  |           |                  |                  |
  |  ADC1_CH4 +--- PA4 ---<-----| L_IS (current)   |
  |           |                  |                  |
  +-----------+                  | B+  B-           |
                                 +--+-----+---------+
                                    |     |
                                12V Act.  GND
                                Rail

  R_EN, L_EN: 10k pull-down resistors to GND (fail-safe disabled on reset).
  RPWM, LPWM: 20 kHz PWM (above audible), hardware dead-time = 10 us minimum.
  BTS7960 VCC from 12V actuator rail (through kill relay and 30A fuse).
  IS_R, IS_L: current sense outputs for backup monitoring (ACS723 is primary).
```

## ACS723 Current Sensor Circuit

```
                    ACS723LLCTR-20AB-T
                    +------------------+
  Motor supply ---->| IP+          VCC |--- 3.3V ---[100nF]
  (from BTS7960)    |                  |
  Motor return ---<-| IP-         VIOUT|---[1nF]---+--- PA0 (ADC1_CH1)
                    |                  |           |
                    |              GND |     [100nF filter]
                    +------------------+           |
                                                  GND

  Galvanic isolation between high-current path (IP+/IP-) and signal output.
  Sensitivity: 100 mV/A (ACS723LLCTR-20AB-T, 20A variant).
  Zero-current output: VCC/2 = 1.65V (bidirectional sensing).
  Practical range: +/-16.5A before output saturates.
  1nF capacitor: bandwidth filter (80 kHz).
  100nF capacitor: anti-aliasing at ADC input (1.6 kHz cutoff).
  Zener clamp (BZX84C3V3) across ADC input for overvoltage protection.
```

## NTC Temperature Sensing Circuit

```
  3.3V
    |
  [10k, 1% precision fixed resistor]
    |
    +--- PA1 (ADC1_CH2) ---[100nF]--- GND
    |
  [10k NTC (B=3950)]
    |
   GND

  At 25C: R_NTC = 10k, V_out = 1.65V (midscale)
  At 60C: R_NTC = 2.49k, V_out = 0.66V
  At 100C: R_NTC = 0.68k, V_out = 0.21V
  Open NTC: V_out = 3.3V (reads as < -30C -> sensor fault)
  Short NTC: V_out = 0V (reads as > 150C -> sensor fault)

  Second NTC on PA2 (ADC1_CH3) with identical circuit for board temperature.
```

## Battery Voltage Monitoring Circuit

```
  12V Battery Bus
    |
  [47k, 1% resistor]
    |
    +--- PA3 (ADC1_CH4) ---[100nF]--- GND
    |                           |
  [10k, 1% resistor]       [BZX84C3V3 Zener]
    |                           |
   GND                        GND

  Voltage divider: V_out = V_bat * 10k / (47k + 10k) = V_bat * 0.175
  At 12V: V_out = 2.11V
  At 14.4V: V_out = 2.53V
  Max input before Zener clamps: 18.8V
```

## Motor Encoder Interface

```
  Motor Encoder (open-collector outputs)
  +----------+
  |          |
  | Ch A     |---[10k to 3.3V]---[TVS 3.3V]--- PB6 (TIM4_CH1, AF2)
  |          |
  | Ch B     |---[10k to 3.3V]---[TVS 3.3V]--- PB7 (TIM4_CH2, AF2)
  |          |
  | VCC      |--- 5V or 3.3V (per encoder spec)
  |          |
  | GND      |--- GND
  +----------+

  TIM4 configured in encoder mode (both edges, 4x counting).
  Hardware input filter ICF = 0x0F (maximum, 8 samples at fDTS/32).
  10k pull-ups required for open-collector encoder outputs.
  TVS diodes for ESD protection (motor is a noise source).
```

## Watchdog Circuit

Same as CVC (see `05-cvc-circuits.md`) but using **PB4** as WDI GPIO (PB0 used for BTS7960 R_EN).

## Design Decisions Explained

### Why BTS7960 H-bridge (not L298N or a simple MOSFET)?

The BTS7960 is an automotive-grade H-bridge that can handle 43A peak current. The L298N (a common hobby motor driver) maxes out at 2A — far too little for our 12V DC motor (5A typical, 25A stall). A simple MOSFET could switch the motor on/off, but an H-bridge lets you control direction (forward/reverse) and speed (PWM duty cycle). The BTS7960 also has built-in overcurrent protection, over-temperature shutdown, and current sensing outputs (IS_R, IS_L) — safety features that a bare MOSFET doesn't provide.

### Why 20 kHz PWM frequency (not lower like 1 kHz)?

PWM frequency determines how fast the motor driver switches on and off. Below ~15 kHz, the switching is audible — you'd hear an annoying whine from the motor. 20 kHz is above human hearing range, so the motor runs silently. Going much higher (>50 kHz) increases switching losses in the MOSFET and can cause electromagnetic interference (EMI). 20 kHz is the sweet spot: inaudible, efficient, and well within the BTS7960's switching capability.

### Why 10k pulldown resistors on R_EN and L_EN?

R_EN and L_EN are the "enable" pins for the right and left half-bridges. During MCU reset, these GPIO pins are floating (high-impedance). Without pulldowns, stray voltage could partially enable the H-bridge, causing the motor to twitch or spin unexpectedly. The 10k pulldowns force both enable pins LOW during boot = motor disabled = safe state. This is the same fail-safe pattern used for the kill relay MOSFET gate.

### Why ACS723 as the primary current sensor (with BTS7960 IS as backup)?

The ACS723 uses the Hall effect to measure current flowing through the motor path. It's **galvanically isolated** — the high-current motor path (IP+/IP-) is physically separated from the low-voltage signal output (VIOUT). This means a motor short circuit can't damage the MCU through the current sensor. The BTS7960's built-in IS_R/IS_L outputs are a secondary (backup) measurement — they're not isolated and have lower accuracy, but they provide redundant current monitoring. Having two independent current measurements is required for ASIL D motor control: if the ACS723 fails, the BTS7960 IS outputs still detect overcurrent.

### Why 1nF + 100nF filter capacitors on the ACS723 output?

The ACS723's VIOUT output contains high-frequency noise from the PWM switching of the motor driver. The 1nF capacitor sets the sensor's internal bandwidth to ~80 kHz (filtering out switching noise above this frequency). The 100nF capacitor at the ADC input creates a low-pass filter with a cutoff around 1.6 kHz — this is the anti-aliasing filter that prevents high-frequency noise from appearing as false readings in the ADC. Together, they give a clean, accurate current reading.

### Why BZX84C3V3 Zener clamp on the ADC input?

The ADC input on the STM32 is rated for 0-3.3V maximum. If the ACS723 output exceeds 3.3V (due to a fault, power supply glitch, or extreme overcurrent), it could damage the ADC input permanently. The Zener diode clamps the voltage to 3.3V — any excess voltage is shunted to ground through the Zener. This costs $0.15 and prevents potential destruction of a $16 Nucleo board.

### Why voltage divider for NTC temperature sensing (not a current source)?

An NTC thermistor is a resistor whose resistance decreases as temperature increases. The simplest way to measure resistance with an MCU is a voltage divider: put a fixed resistor (10k) in series with the NTC (10k at 25°C), apply 3.3V, and measure the voltage at the middle point with an ADC. At 25°C: both resistors are 10k, so V_out = 3.3V / 2 = 1.65V (midscale ADC). As temperature rises, NTC resistance drops, and V_out drops proportionally. This gives maximum ADC sensitivity around room temperature. A current source would give a more linear response but adds complexity (extra IC) for no real benefit.

### Why 1% precision fixed resistor in the NTC divider?

The fixed resistor is the reference for the measurement. If it's 5% tolerance (meaning it could be 9.5k to 10.5k), your temperature reading would have a 5% uncertainty just from the resistor. A 1% resistor (9.9k to 10.1k) reduces this error to 1%. Since we're measuring motor temperature for safety purposes (overheat detection), accuracy matters.

### Why the 47k/10k voltage divider for battery monitoring (not other ratios)?

The STM32 ADC measures 0-3.3V. The battery voltage can be up to ~14.4V (when charging). We need to scale 14.4V down to below 3.3V. The divider formula: V_out = V_bat × 10k / (47k + 10k) = V_bat × 0.175. At 14.4V: V_out = 2.53V (well within 3.3V ADC range). At 12V: V_out = 2.11V (good ADC resolution). The Zener clamp protects against overvoltage up to 18.8V (3.3V / 0.175). This ratio provides good resolution across the normal battery voltage range while keeping a safety margin.

### Why TIM4 encoder mode (not GPIO polling)?

The STM32's hardware timer can count encoder pulses automatically in "encoder mode" — it counts up for one direction and down for the other, using both edges of both channels (4x counting). This happens in hardware with zero CPU involvement. GPIO polling would require the CPU to check the encoder pins fast enough to not miss any pulses. At 12000 RPM with an 11 PPR encoder, that's 12000 × 11 × 4 / 60 = 8800 counts per second. Hardware encoder mode handles this effortlessly; software polling at this rate would consume significant CPU time and might miss pulses during ISR processing.

### Why 10k pull-ups and TVS diodes on encoder inputs?

Open-collector encoder outputs only pull the signal LOW (to ground). They need an external pull-up resistor to pull the signal HIGH when the encoder output is off. 10k is standard for 3.3V digital inputs. The TVS diodes protect against ESD and voltage spikes — the motor is a significant source of electrical noise, and the encoder wires act as antennas that pick up this noise. Without TVS protection, motor noise spikes could damage the MCU's input pins.

### Why PB4 for WDT (not PB0 like CVC/FZC)?

PB0 is used on the RZC for the BTS7960 R_EN (motor right-enable). Since the WDT feed is just a GPIO toggle, it can use any available pin. PB4 was available and doesn't conflict with any other function on the RZC. This cross-ECU pin difference is documented in pin-mapping.md section 8.2.

## Pin Summary

| # | Function | Pin | AF | Direction | Net Name | ASIL |
|---|----------|-----|----|-----------|----------|------|
| 1 | CAN TX | PA12 | AF9 | OUT | RZC_CAN_TX | D |
| 2 | CAN RX | PA11 | AF9 | IN | RZC_CAN_RX | D |
| 3 | Motor RPWM | PA8 | AF6 | OUT | RZC_MOT_RPWM | D |
| 4 | Motor LPWM | PA9 | AF6 | OUT | RZC_MOT_LPWM | D |
| 5 | Motor R_EN | PB0 | — | OUT | RZC_MOT_REN | D |
| 6 | Motor L_EN | PB1 | — | OUT | RZC_MOT_LEN | D |
| 7 | Encoder A | PB6 | AF2 | IN | RZC_ENC_A | C |
| 8 | Encoder B | PB7 | AF2 | IN | RZC_ENC_B | C |
| 9 | Current sense | PA0 | AN | AN | RZC_CURR_SENSE | A |
| 10 | Motor temp | PA1 | AN | AN | RZC_MOT_TEMP | A |
| 11 | Board temp | PA2 | AN | AN | RZC_BRD_TEMP | QM |
| 12 | Battery voltage | PA3 | AN | AN | RZC_VBAT | QM |
| 13 | BTS7960 IS_R | PB15 | AN | AN | RZC_ISR | A |
| 14 | BTS7960 IS_L | PA4 | AN | AN | RZC_ISL | A |
| 15 | WDT feed | PB4 | — | OUT | RZC_WDT_WDI | D |
| 16 | LED Green | PB5 | — | OUT | RZC_LED_GRN | QM |
| 17 | LED Red | PB3 | — | OUT | RZC_LED_RED | QM |

> **Note**: PB3 (LED Red) is shared with SWO debug trace. If SWO debug is needed on RZC, relocate LED Red to another available pin (e.g., PC10). See pin-mapping.md Section 6.

### Solder Bridge Modifications

| Bridge | Action | Reason |
|--------|--------|--------|
| SB63 | REMOVE | Free PA2 from ST-LINK VCP TX (needed for ADC1_IN3) |
| SB65 | REMOVE | Free PA3 from ST-LINK VCP RX (needed for ADC1_IN4) |

## BOM References

| Component | BOM # |
|-----------|-------|
| STM32G474RE Nucleo-64 | #1 |
| TJA1051T/3 module | #6 |
| BTS7960 H-bridge module | #20 |
| 12V DC motor | #19 |
| Quadrature encoder | #18 |
| ACS723 module | #16 |
| NTC 10k thermistors (x3) | #17 |
| TPS3823DBVR | #22 |
| SOT-23-5 breakout | #23 |
| 100nF capacitors | #43 |
| 1nF capacitors | #44 |
| 10k resistors | #49 |
| 47k resistors | #50 |
| BZX84C3V3 Zener diodes | #55 |
| 3.3V TVS diodes | #56 |
| Common-mode choke | #10 |
| PESD1CAN TVS | #11 |
