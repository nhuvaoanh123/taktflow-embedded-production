# 05 — CVC Circuits

**Block**: Central Vehicle Computer — CAN, dual AS5048A, OLED, E-stop, WDT, power
**Source**: HWDES Section 5.1

## CAN Transceiver Circuit

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

  120R termination resistor at CVC end of bus.
```

## Dual Pedal Sensor SPI Circuit

```
  STM32G474RE                  AS5048A #1              AS5048A #2
  +-----------+                +----------+            +----------+
  |           |                |          |            |          |
  | SPI1_SCK  +--- PA5 ------>| CLK      |    +------>| CLK      |
  | (AF5)     |                |          |    |       |          |
  | SPI1_MISO +--- PA6 ---<---| DO   VDD |--[100nF]   | DO   VDD |--[100nF]
  | (AF5)     |                |      |   |   |       |      |   |   |
  | SPI1_MOSI +--- PA7 ------>| DI   GND |   |  +--->| DI   GND |   |
  | (AF5)     |                |          |   |  |    |          |   |
  |           |                |     CSn  |   |  |    |     CSn  |   |
  |   GPIO    +--- PA4 ------>|  (act-lo) |   |  |    |  (act-lo) |  |
  |           |     |          +----------+   |  |    +----------+   |
  |   GPIO    +--- PA15 ------+---------------+--+--->               |
  |           |     |                                                |
  +-----------+     |                                                |
                [10k to 3.3V] pull-up on PA4                     [10k to 3.3V]
                                                                 pull-up on PA15

  Both sensors share SPI1 bus (SCK, MISO, MOSI).
  Independent chip-selects (PA4, PA15) with 10k pull-ups.
  Sensors powered from external 3.3V rail with local 100nF decoupling each.
```

## OLED Display I2C Circuit

```
  STM32G474RE                    SSD1306 128x64 OLED
  +-----------+                  +-------------------+
  |           |                  |                   |
  | I2C1_SCL  +--- PB8 ---+---->| SCL      VCC(3.3V)|--[10uF]--[100nF]--3.3V
  | (AF4)     |            |     |                   |
  | I2C1_SDA  +--- PB9 --+|-+-->| SDA          GND  |-- GND
  | (AF4)     |           || |   |                   |
  +-----------+           || |   | RST               |-- GPIO (optional)
                          || |   | ADDR = 0x3C       |
                       [4.7k]|   +-------------------+
                          |[4.7k]
                       3.3V  3.3V

  Pull-ups: 4.7k ohm on SCL and SDA to 3.3V (external).
  I2C speed: 400 kHz (Fast Mode). Address: 0x3C (7-bit).
```

## E-Stop Button Circuit

```
  3.3V (internal pull-up on PC13, ~40k)
    |
    +--- PC13 (EXTI, rising-edge) ---[10k series R]---+--- [100nF] --- GND
    |                                                  |    (RC debounce)
    |                                              [TVS 3.3V]
    |                                                  |
    +--- [NC Push Button] --- GND                     GND

  NC button: fail-safe — broken wire = E-stop activated.
  RC debounce: tau = 10k * 100nF = 1 ms.
```

## Watchdog Circuit

```
                    TPS3823DBVR
                    +-----------+
  3.3V --[100nF]--->| VDD    MR |--- 3.3V (MR tied high = no manual reset)
                    |           |
  PB0 (GPIO) ----->| WDI  RESET|---[100nF]--- STM32 NRST
                    |           |         |
                    | CT    GND |       (open-drain, active-low)
                    +---+-------+
                        |
                     [100nF] (sets timeout = 1.6 sec)
                        |
                       GND

  Firmware toggles PB0 at regular intervals (conditioned on self-checks).
  If PB0 stops toggling for 1.6 sec, TPS3823 pulls RESET low -> MCU resets.
```

## Power Supply Design

```
  12V Main Rail
       |
  [Nucleo Vin pin] ---> Nucleo onboard LDO (LD39050) ---> 3.3V (MCU power)

  External 3.3V rail ---> SPI sensors (AS5048A x2)
                     ---> CAN transceiver (TJA1051T/3)
                     ---> TPS3823 watchdog
                     ---> I2C OLED (SSD1306)
                     ---> Status LEDs

  Power domains separated:
  - MCU powered from Nucleo onboard LDO
  - Peripherals powered from external 3.3V rail
```

## Design Decisions Explained

### Why SPI (not I2C) for the AS5048A pedal sensors?

The AS5048A supports both SPI and I2C. We chose SPI because: (1) **Speed** — SPI runs at up to 10 MHz vs I2C's 400 kHz, giving faster angle readings for pedal position (safety-critical, ASIL D). (2) **Full-duplex** — SPI can send and receive simultaneously. (3) **No address conflicts** — each sensor gets its own chip-select line (PA4 and PA15), so there's no risk of address collision. I2C would require configuring different addresses or using a multiplexer. For ASIL D pedal sensing, the simpler and faster interface is the right choice.

### Why two separate chip-select lines (not sharing one)?

Each AS5048A has its own chip-select (CS) pin: PA4 for pedal sensor #1, PA15 for pedal sensor #2. This lets the MCU talk to each sensor independently — read sensor #1, then sensor #2, without any risk of both responding at the same time. If they shared a CS line, their MISO outputs would fight each other on the bus (electrical contention), potentially corrupting data and damaging the output drivers.

### Why 10k pull-up resistors on the CS lines?

The chip-select is active-low: LOW = selected, HIGH = deselected. During MCU reset or before GPIO initialization, the CS pins are floating (high-impedance). Without pull-ups, the sensors might see a LOW CS and start responding, creating bus contention. The 10k pull-ups hold CS HIGH (deselected) until the MCU explicitly pulls them LOW — ensuring sensors stay quiet during boot.

### Why 100nF decoupling capacitors on each sensor's VDD?

Every digital IC needs a decoupling (bypass) capacitor as close as possible to its power pins. When the IC switches internally (every clock cycle), it draws brief current spikes from the power supply. These spikes create voltage dips on the power rail, which can cause glitches in other circuits. The 100nF capacitor acts as a tiny local battery, providing instant current for these spikes without disturbing the main power rail. Rule of thumb: one 100nF cap per IC, placed within 5mm of the VDD pin.

### Why 4.7k pull-ups on I2C (not 2.2k or 10k)?

I2C is an open-drain bus — the pull-up resistors are what actually drive the signal HIGH. The pull-up value is a trade-off: too low (strong pull-up, 2.2k) = faster edges but more current draw and more susceptibility to noise; too high (weak pull-up, 10k) = slower edges and the signal might not reach valid HIGH levels at higher speeds. 4.7k is the standard value for I2C at 400 kHz (Fast Mode) with short wire lengths (<30cm) and low bus capacitance. Since the OLED is right next to the MCU, 4.7k is ideal.

### Why I2C for the OLED display (not SPI)?

The SSD1306 OLED module supports both I2C and SPI. We chose I2C because: (1) the OLED is ASIL B (status display), not safety-critical — speed isn't critical. (2) I2C uses only 2 pins (SCL + SDA) vs SPI's 4 pins (SCK, MOSI, MISO, CS), conserving GPIO. (3) SPI1 is already used for the pedal sensors — using I2C keeps the buses separate, avoiding any timing interference between safety-critical pedal reads and non-critical display updates.

### Why PC13 for the E-stop (the Nucleo user button)?

PC13 is the Nucleo-64 board's built-in user button (B1). For bench testing, this is convenient — you can press the onboard button to simulate an E-stop without wiring anything. For final integration, the external NC mushroom button replaces B1 via the same pin. PC13 also supports EXTI (External Interrupt), which means the E-stop triggers an interrupt immediately — no polling delay.

### Why the TPS3823 external watchdog (not just the STM32's internal watchdog)?

The STM32 has an internal watchdog (IWDG), but it shares the same silicon as the CPU it's monitoring. If the MCU has a hardware fault (latch-up, power glitch, clock failure), the internal watchdog might also be affected. The TPS3823 is a completely independent IC with its own power supply and oscillator. It monitors the MCU from outside — if the MCU stops toggling PB0 for 1.6 seconds, the TPS3823 pulls the reset line LOW, forcing a hard reset. This two-layer watchdog (internal + external) is an ISO 26262 ASIL D requirement for safety-critical systems.

### Why separate MCU power (Nucleo LDO) and peripheral power (external 3.3V rail)?

If a sensor shorts out and drags the 3.3V rail down, the MCU should still be running (powered by its own LDO) so it can detect the fault and enter a safe state. Sharing one power rail for everything means one shorted sensor kills the MCU too — losing all control. The Nucleo's onboard LDO is rated for the MCU's current draw, while the external 3.3V rail is sized for the total peripheral load.

## Pin Summary

| # | Function | Pin | AF | Direction | Net Name | ASIL |
|---|----------|-----|----|-----------|----------|------|
| 1 | CAN TX | PA12 | AF9 | OUT | CVC_CAN_TX | D |
| 2 | CAN RX | PA11 | AF9 | IN | CVC_CAN_RX | D |
| 3 | SPI1 SCK | PA5 | AF5 | OUT | CVC_SPI1_SCK | D |
| 4 | SPI1 MISO | PA6 | AF5 | IN | CVC_SPI1_MISO | D |
| 5 | SPI1 MOSI | PA7 | AF5 | OUT | CVC_SPI1_MOSI | D |
| 6 | Pedal CS1 | PA4 | — | OUT | CVC_PED_CS1 | D |
| 7 | Pedal CS2 | PA15 | — | OUT | CVC_PED_CS2 | D |
| 8 | I2C1 SCL | PB8 | AF4 | BI | CVC_I2C1_SCL | B |
| 9 | I2C1 SDA | PB9 | AF4 | BI | CVC_I2C1_SDA | B |
| 10 | E-stop | PC13 | — | IN | CVC_ESTOP | B |
| 11 | WDT feed | PB0 | — | OUT | CVC_WDT_WDI | D |
| 12 | LED Green | PB4 | — | OUT | CVC_LED_GRN | QM |
| 13 | LED Red | PB5 | — | OUT | CVC_LED_RED | QM |

## BOM References

| Component | BOM # |
|-----------|-------|
| STM32G474RE Nucleo-64 | #1 |
| TJA1051T/3 module | #6 |
| AS5048A modules (x2) | #13 |
| Diametric magnets (x2) | #14 |
| TPS3823DBVR | #22 |
| SOT-23-5 breakout | #23 |
| SSD1306 OLED | #28 |
| E-stop button | #27 |
| 120 ohm terminator | #9 |
| 100nF capacitors | #43 |
| 10uF capacitors | #45 |
| 10k resistors | #49 |
| 4.7k resistors | #53 |
| 330 ohm resistors | #51 |
| 3.3V TVS diode | #56 |
| Common-mode choke | #10 |
| PESD1CAN TVS | #11 |
