---
document_id: PIN-MAP
title: "Pin Mapping"
version: "1.0"
status: draft
date: 2026-02-21
---

## Human-in-the-Loop (HITL) Comment Lock

`HITL` means human-reviewer-owned comment content.

**Marker standard (code-friendly):**
- Markdown: `<!-- HITL-LOCK START:<id> -->` ... `<!-- HITL-LOCK END:<id> -->`
- C/C++/Java/JS/TS: `// HITL-LOCK START:<id>` ... `// HITL-LOCK END:<id>`
- Python/Shell/YAML/TOML: `# HITL-LOCK START:<id>` ... `# HITL-LOCK END:<id>`

**Rules:**
- AI must never edit, reformat, move, or delete text inside any `HITL-LOCK` block.
- Append-only: AI may add new comments/changes only; prior HITL comments stay unchanged.
- If a locked comment needs revision, add a new note outside the lock or ask the human reviewer to unlock it.

## Lessons Learned Rule

Every pin mapping topic in this document that undergoes HITL review discussion MUST have its own lessons-learned file in [`hardware/lessons-learned/`](lessons-learned/). One file per pin mapping topic. File naming: `PIN-<topic>.md`.

# Pin Mapping

## 1. Purpose

Complete pin assignment for all physical ECUs (CVC, FZC, RZC, SC). This document serves as the hardware-software interface reference for firmware development and is the authoritative source for which MCU pin connects to which external component. Cross-referenced with the hardware design (HWDES), hardware safety requirements (HSR), and BOM.

## 2. Referenced Documents

| Document ID | Title | Version |
|-------------|-------|---------|
| HWDES | Hardware Design | 1.0 |
| HSR | Hardware Safety Requirements | 1.0 |
| HWREQ | Hardware Requirements | 1.0 |
| SYSARCH | System Architecture | 1.0 |

## 3. Pin Naming Convention

- **Pin Name**: STM32 port/pin (e.g., PA5) or TMS570 GIO/DCAN pin name
- **Nucleo Pin**: Physical pin number on the Nucleo-64 board morpho or Arduino header
- **AF**: Alternate function number for STM32 (e.g., AF5 for SPI1)
- **Direction**: IN (input), OUT (output), BI (bidirectional), AN (analog input)
- **Voltage**: Logic level (3.3V, 5V) or analog range
- **ASIL**: ASIL level of the function using this pin
- **Net Name**: Signal name on the schematic for cross-reference

---

## 4. CVC -- STM32G474RE Nucleo-64

### 4.1 Pin Assignment Table

| # | Function | Peripheral | Pin Name | AF | Nucleo Arduino | Nucleo Morpho | Direction | Voltage | External Component | Net Name | ASIL |
|---|----------|-----------|----------|-----|----------------|---------------|-----------|---------|-------------------|----------|------|
| 1 | CAN TX | FDCAN1 | PA12 | AF9 | D2 | CN10-12 | OUT | 3.3V | TJA1051T/3 TXD | CVC_CAN_TX | D |
| 2 | CAN RX | FDCAN1 | PA11 | AF9 | D10 | CN10-14 | IN | 3.3V | TJA1051T/3 RXD | CVC_CAN_RX | D |
| 3 | Pedal SPI SCK | SPI1 | PA5 | AF5 | D13 | CN10-11 | OUT | 3.3V | AS5048A x2 CLK | CVC_SPI1_SCK | D |
| 4 | Pedal SPI MISO | SPI1 | PA6 | AF5 | D12 | CN10-13 | IN | 3.3V | AS5048A x2 DO | CVC_SPI1_MISO | D |
| 5 | Pedal SPI MOSI | SPI1 | PA7 | AF5 | D11 | CN10-15 | OUT | 3.3V | AS5048A x2 DI | CVC_SPI1_MOSI | D |
| 6 | Pedal CS1 | GPIO | PA4 | -- | A2 | CN7-32 | OUT | 3.3V | AS5048A #1 CSn (10k PU) | CVC_PED_CS1 | D |
| 7 | Pedal CS2 | GPIO | PA15 | -- | -- | CN7-17 | OUT | 3.3V | AS5048A #2 CSn (10k PU) | CVC_PED_CS2 | D |
| 8 | OLED SCL | I2C1 | PB8 | AF4 | D15 | CN10-3 | BI | 3.3V | SSD1306 SCL (4.7k PU) | CVC_I2C1_SCL | B |
| 9 | OLED SDA | I2C1 | PB9 | AF4 | D14 | CN10-5 | BI | 3.3V | SSD1306 SDA (4.7k PU) | CVC_I2C1_SDA | B |
| 10 | E-Stop input | EXTI | PC13 | -- | -- | CN7-23 | IN | 3.3V | NC button + RC debounce + TVS | CVC_ESTOP | B |
| 11 | WDT Feed | GPIO | PB0 | -- | D3 | CN10-31 | OUT | 3.3V | TPS3823 WDI | CVC_WDT_WDI | D |
| 12 | Status LED Green | GPIO | PB4 | -- | D5 | CN10-27 | OUT | 3.3V | Green LED + 330R | CVC_LED_GRN | QM |
| 13 | Status LED Red | GPIO | PB5 | -- | D4 | CN10-29 | OUT | 3.3V | Red LED + 330R | CVC_LED_RED | QM |

**Total pins used: 13** (out of 51 available GPIO on STM32G474RE Nucleo-64)

<!-- HITL-LOCK START:COMMENT-BLOCK-PIN-CVC-ASSIGN -->
**HITL Review (An Dao) — Reviewed: 2026-02-27:** The CVC pin assignment table is comprehensive with 13 pins clearly documented. All critical fields (AF, direction, voltage, ASIL, net name) are present. The dual pedal sensors on SPI1 with separate chip selects (PA4, PA15) support the redundant pedal sensing required by SYS-001. The I2C1 OLED on PB8/PB9 is correctly rated ASIL B (status display, not safety-critical actuation). The E-stop on PC13 (EXTI) enables interrupt-driven detection. The WDT feed on PB0 aligns with the TPS3823 hardware watchdog architecture. The total of 13 out of 51 pins leaves ample room for expansion.
<!-- HITL-LOCK END:COMMENT-BLOCK-PIN-CVC-ASSIGN -->

### 4.2 Pin Conflict Check

| Check | Result |
|-------|--------|
| PA5 used for both SPI1_SCK and onboard LED (LD2)? | CONFLICT -- PA5 is the Nucleo onboard LED. Resolved: use PB4 for status LED instead. SPI1_SCK on PA5 takes priority (ASIL D). Onboard LED LD2 is disabled by removing SB21 solder bridge or accepting it blinks with SPI clock. |
| PA11/PA12 used for both FDCAN1 and USB? | No conflict -- USB D+/D- are on PA11/PA12 but FDCAN1 uses the same pins with AF9. USB is not used simultaneously with CAN. Nucleo ST-LINK uses a separate USB connection. |
| PC13 used for both E-stop and onboard button (B1)? | SHARED -- PC13 is the Nucleo user button B1. For the CVC, B1 doubles as the E-stop input in bench testing. For final integration, an external NC button replaces B1. |
| I2C1 on PB8/PB9 vs PB6/PB7? | Using PB8/PB9 (AF4). PB6/PB7 are reserved for potential TIM4 encoder (not used on CVC). |

<!-- HITL-LOCK START:COMMENT-BLOCK-PIN-CVC-CONFLICT -->
**HITL Review (An Dao) — Reviewed: 2026-02-27:** The pin conflict analysis is thorough. The PA5/SPI1_SCK vs onboard LED LD2 conflict is correctly identified with a practical resolution (SB21 removal or accept LED blinking). The PA11/PA12 FDCAN1 vs USB clarification is important for developers. The PC13 dual-use as E-stop and user button B1 is a pragmatic bench-testing approach. All conflicts have documented resolutions. No unidentified conflicts found.
<!-- HITL-LOCK END:COMMENT-BLOCK-PIN-CVC-CONFLICT -->

### 4.3 Unused Pin Handling

All unused GPIO pins on the CVC shall be configured as GPIO output LOW at initialization. This prevents floating inputs from consuming excess current and prevents unintended peripheral activation.

---

## 5. FZC -- STM32G474RE Nucleo-64

### 5.1 Pin Assignment Table

| # | Function | Peripheral | Pin Name | AF | Nucleo Arduino | Nucleo Morpho | Direction | Voltage | External Component | Net Name | ASIL |
|---|----------|-----------|----------|-----|----------------|---------------|-----------|---------|-------------------|----------|------|
| 1 | CAN TX | FDCAN1 | PA12 | AF9 | D2 | CN10-12 | OUT | 3.3V | TJA1051T/3 TXD | FZC_CAN_TX | D |
| 2 | CAN RX | FDCAN1 | PA11 | AF9 | D10 | CN10-14 | IN | 3.3V | TJA1051T/3 RXD | FZC_CAN_RX | D |
| 3 | Steering servo PWM | TIM2_CH1 | PA0 | AF1 | A0 | CN7-28 | OUT | 3.3V | MG996R signal (orange wire) | FZC_STEER_PWM | D |
| 4 | Brake servo PWM | TIM2_CH2 | PA1 | AF1 | A1 | CN7-30 | OUT | 3.3V | MG996R signal (orange wire) | FZC_BRAKE_PWM | D |
| 5 | Lidar UART TX | USART2 | PA2 | AF7 | A7 | CN10-35 | OUT | 3.3V | TFMini-S RX (green wire) | FZC_LIDAR_TX | C |
| 6 | Lidar UART RX | USART2 | PA3 | AF7 | A2 | CN10-37 | IN | 3.3V | TFMini-S TX (white wire) + TVS | FZC_LIDAR_RX | C |
| 7 | Steering angle SPI SCK | SPI2 | PB13 | AF5 | -- | CN10-30 | OUT | 3.3V | AS5048A CLK | FZC_SPI2_SCK | D |
| 8 | Steering angle SPI MISO | SPI2 | PB14 | AF5 | -- | CN10-28 | IN | 3.3V | AS5048A DO | FZC_SPI2_MISO | D |
| 9 | Steering angle SPI MOSI | SPI2 | PB15 | AF5 | -- | CN10-26 | OUT | 3.3V | AS5048A DI | FZC_SPI2_MOSI | D |
| 10 | Steering angle CS | GPIO | PB12 | -- | -- | CN10-16 | OUT | 3.3V | AS5048A CSn (10k PU) | FZC_STEER_CS | D |
| 11 | Buzzer drive | GPIO | PB4 | -- | D5 | CN10-27 | OUT | 3.3V | 2N7002 gate + 10k PD | FZC_BUZZER | B |
| 12 | WDT Feed | GPIO | PB0 | -- | D3 | CN10-31 | OUT | 3.3V | TPS3823 WDI | FZC_WDT_WDI | D |
| 13 | Status LED Green | GPIO | PB5 | -- | D4 | CN10-29 | OUT | 3.3V | Green LED + 330R | FZC_LED_GRN | QM |
| 14 | Status LED Red | GPIO | PB1 | -- | -- | CN10-24 | OUT | 3.3V | Red LED + 330R | FZC_LED_RED | QM |

**Total pins used: 14** (out of 51 available GPIO)

<!-- HITL-LOCK START:COMMENT-BLOCK-PIN-FZC-ASSIGN -->
**HITL Review (An Dao) — Reviewed: 2026-02-27:** The FZC pin assignment is well-organized with 14 pins. The steering and brake servos on TIM2 CH1/CH2 (PA0/PA1) provide synchronized 50 Hz PWM, which is correct for standard hobby servos. The lidar UART on USART2 (PA2/PA3) requires solder bridge removal (documented in section 9). The steering angle sensor on SPI2 (PB13-PB15) avoids the SPI1/LD2 conflict present on CVC. The buzzer driver through a 2N7002 MOSFET (PB4) is appropriate for a 3.3V GPIO driving an active buzzer. ASIL ratings are correct: D for steering/CAN, C for lidar, B for buzzer, QM for LEDs.
<!-- HITL-LOCK END:COMMENT-BLOCK-PIN-FZC-ASSIGN -->

### 5.2 Pin Conflict Check

| Check | Result |
|-------|--------|
| FZC SPI2 on PB13-PB15 conflict? | No conflict -- SPI2 pins (PB13-PB15) have no onboard peripheral. SPI2 avoids SB21 solder bridge conflict (PA5/LD2) on Nucleo-64. |
| PA2/PA3 used for both USART2 and Nucleo ST-LINK VCP? | CONFLICT -- PA2/PA3 are the Nucleo ST-LINK virtual COM port USART2. Resolved: disconnect SB63/SB65 solder bridges on Nucleo to free PA2/PA3 for lidar UART. Debug printf can use LPUART1 or SWO instead. |
| TIM2_CH1 (PA0) and TIM2_CH2 (PA1) on same timer? | By design -- both servo PWMs on TIM2 for synchronized 50 Hz update. |

<!-- HITL-LOCK START:COMMENT-BLOCK-PIN-FZC-CONFLICT -->
**HITL Review (An Dao) — Reviewed: 2026-02-27:** The PA2/PA3 conflict with ST-LINK VCP is correctly identified as the most significant FZC conflict. The SB63/SB65 removal resolution is documented with the debug alternative (LPUART1 or SWO). The TIM2 CH1/CH2 on same timer is correctly noted as by design. No unidentified conflicts found. The SPI2 pin choice avoids the PA5/LD2 issue, which is a good lesson-learned application from the CVC.
<!-- HITL-LOCK END:COMMENT-BLOCK-PIN-FZC-CONFLICT -->

### 5.3 Unused Pin Handling

All unused GPIO pins configured as GPIO output LOW at initialization.

---

## 6. RZC -- STM32G474RE Nucleo-64

### 6.1 Pin Assignment Table

| # | Function | Peripheral | Pin Name | AF | Nucleo Arduino | Nucleo Morpho | Direction | Voltage | External Component | Net Name | ASIL |
|---|----------|-----------|----------|-----|----------------|---------------|-----------|---------|-------------------|----------|------|
| 1 | CAN TX | FDCAN1 | PA12 | AF9 | D2 | CN10-12 | OUT | 3.3V | TJA1051T/3 TXD | RZC_CAN_TX | D |
| 2 | CAN RX | FDCAN1 | PA11 | AF9 | D10 | CN10-14 | IN | 3.3V | TJA1051T/3 RXD | RZC_CAN_RX | D |
| 3 | Motor PWM (RPWM) | TIM1_CH1 | PA8 | AF6 | D7 | CN10-23 | OUT | 3.3V | BTS7960 RPWM | RZC_MOT_RPWM | D |
| 4 | Motor PWM (LPWM) | TIM1_CH2 | PA9 | AF6 | D8 | CN10-21 | OUT | 3.3V | BTS7960 LPWM | RZC_MOT_LPWM | D |
| 5 | Motor enable R | GPIO | PB0 | -- | D3 | CN10-31 | OUT | 3.3V | BTS7960 R_EN (10k PD) | RZC_MOT_REN | D |
| 6 | Motor enable L | GPIO | PB1 | -- | -- | CN10-24 | OUT | 3.3V | BTS7960 L_EN (10k PD) | RZC_MOT_LEN | D |
| 7 | Encoder A | TIM4_CH1 | PB6 | AF2 | D5 | CN10-17 | IN | 3.3V | Encoder Ch A (10k PU + TVS) | RZC_ENC_A | C |
| 8 | Encoder B | TIM4_CH2 | PB7 | AF2 | -- | CN7-21 | IN | 3.3V | Encoder Ch B (10k PU + TVS) | RZC_ENC_B | C |
| 9 | Current sense | ADC1_IN1 | PA0 | AN | A0 | CN7-28 | AN | 0-3.3V | ACS723 VIOUT (1nF + 100nF + Zener) | RZC_CURR_SENSE | A |
| 10 | Motor temp NTC | ADC1_IN2 | PA1 | AN | A1 | CN7-30 | AN | 0-3.3V | NTC 10k divider (100nF) | RZC_MOT_TEMP | A |
| 11 | Board temp NTC | ADC1_IN3 | PA2 | AN | -- | CN10-35 | AN | 0-3.3V | NTC 10k divider (100nF) | RZC_BRD_TEMP | QM |
| 12 | Battery voltage | ADC1_IN4 | PA3 | AN | A2 | CN10-37 | AN | 0-3.3V | 47k/10k divider (100nF + Zener) | RZC_VBAT | QM |
| 13 | BTS7960 IS_R | ADC2_IN15 | PB15 | AN | -- | CN10-26 | AN | 0-3.3V | BTS7960 R_IS (current sense backup) | RZC_ISR | A |
| 14 | BTS7960 IS_L | ADC2_IN17 | PA4 | AN | -- | CN7-32 | AN | 0-3.3V | BTS7960 L_IS (current sense backup) | RZC_ISL | A |
| 15 | WDT Feed | GPIO | PB4 | -- | D5 | CN10-27 | OUT | 3.3V | TPS3823 WDI | RZC_WDT_WDI | D |
| 16 | Status LED Green | GPIO | PB5 | -- | D4 | CN10-29 | OUT | 3.3V | Green LED + 330R | RZC_LED_GRN | QM |
| 17 | Status LED Red | GPIO | PB3 | -- | D3 | CN10-25 | OUT | 3.3V | Red LED + 330R | RZC_LED_RED | QM |

**Total pins used: 17** (out of 51 available GPIO)

> **Note**: PB3 (RZC LED Red, pin #17) is shared with SWO (Serial Wire Output) debug trace. If SB63/SB65 are removed to free PA2/PA3, SWO on PB3 becomes the recommended alternative debug output. If SWO debug is needed on RZC, relocate LED Red to another available pin (e.g., PC10).

<!-- HITL-LOCK START:COMMENT-BLOCK-PIN-RZC-ASSIGN -->
**HITL Review (An Dao) — Reviewed: 2026-02-27:** The RZC has the most pins (17) of any ECU, reflecting its motor control and sensing complexity. The motor PWM on TIM1 CH1/CH2 (PA8/PA9) with separate enable pins (PB0/PB1) for the BTS7960 H-bridge is correctly configured. The comprehensive ADC inputs (6 channels: current, motor temp, board temp, battery voltage, BTS7960 IS_R, IS_L) provide thorough motor monitoring. The encoder on TIM4 (PB6/PB7) in encoder mode is correct. Note: PB0 is used for motor R_EN on RZC (not WDT as on CVC/FZC), with WDT moved to PB4 -- this cross-ECU pin role difference is correctly documented in section 8.2.
<!-- HITL-LOCK END:COMMENT-BLOCK-PIN-RZC-ASSIGN -->

### 6.2 Pin Conflict Check

| Check | Result |
|-------|--------|
| PA0-PA3 used for both ADC and digital functions? | No conflict -- these pins are dedicated to analog inputs (ADC1). No digital alternate functions assigned. |
| PB0 used for WDT on CVC but Motor Enable on RZC? | No conflict -- PB0 is on different physical ECUs. Each Nucleo board has its own PB0. |
| PB6/PB7 for encoder (TIM4) vs I2C1? | By design -- RZC uses TIM4 encoder mode on PB6/PB7. I2C1 is not used on RZC (no OLED on RZC). |
| PA2/PA3 Nucleo ST-LINK VCP conflict? | PA2/PA3 used for ADC on RZC. SB63/SB65 solder bridges must be removed (same as FZC). Debug via SWO or LPUART1. |
| TIM1 for motor PWM vs TIM4 for encoder? | No conflict -- different timers on different pins. |

<!-- HITL-LOCK START:COMMENT-BLOCK-PIN-RZC-CONFLICT -->
**HITL Review (An Dao) — Reviewed: 2026-02-27:** The conflict checks are thorough. The PA0-PA3 ADC-only usage is confirmed with no digital function conflicts. The PB0 cross-ECU difference is correctly noted as separate physical boards. The PB6/PB7 TIM4 encoder vs I2C1 trade-off is documented (no OLED on RZC). The PA2/PA3 SB63/SB65 removal is shared with FZC. The PB3/SWO conflict for LED Red is noted with PC10 as an alternative -- this should be resolved before final assembly. No unidentified conflicts found.
<!-- HITL-LOCK END:COMMENT-BLOCK-PIN-RZC-CONFLICT -->

### 6.3 Unused Pin Handling

All unused GPIO pins configured as GPIO output LOW at initialization.

---

## 7. SC -- TMS570LC43x LaunchPad (LAUNCHXL2-570LC43)

### 7.1 Pin Assignment Table

| # | Function | Peripheral | Pin Name | LaunchPad Connector | Direction | Voltage | External Component | Net Name | ASIL |
|---|----------|-----------|----------|--------------------|-----------|---------|--------------------|----------|------|
| 1 | CAN TX | DCAN1 | DCAN1TX | J10 pin 45 (proto header) | OUT | 3.3V | SN65HVD230 TXD | SC_CAN_TX | D |
| 2 | CAN RX | DCAN1 | DCAN1RX | J10 pin 44 (proto header) | IN | 3.3V | SN65HVD230 RXD | SC_CAN_RX | D |
| 3 | Kill relay control | GIO | GIO_A0 | J3-1 (HDR1) | OUT | 3.3V | IRLZ44N gate (100R + 10k PD) | SC_KILL_RELAY | D |
| 4 | CVC fault LED | GIO | GIO_A1 | J3-2 (HDR1) | OUT | 3.3V | Red LED + 330R | SC_LED_CVC | B |
| 5 | FZC fault LED | GIO | GIO_A2 | J3-3 (HDR1) | OUT | 3.3V | Red LED + 330R | SC_LED_FZC | B |
| 6 | RZC fault LED | GIO | GIO_A3 | J3-4 (HDR1) | OUT | 3.3V | Red LED + 330R | SC_LED_RZC | B |
| 7 | System fault LED | GIO | GIO_A4 | J3-5 (HDR1) | OUT | 3.3V | Amber LED + 330R | SC_LED_SYS | B |
| 8 | WDT Feed | GIO | GIO_A5 | J3-6 (HDR1) | OUT | 3.3V | TPS3823 WDI | SC_WDT_WDI | D |
| 9 | Heartbeat LED | GIO | GIO_B1 | J12 (onboard LED1) | OUT | 3.3V | Onboard LED (green) | SC_LED_HB | QM |

**Total pins used: 9** (DCAN1: 2 pins, GIO_A: 6 pins, GIO_B: 1 pin)

<!-- HITL-LOCK START:COMMENT-BLOCK-PIN-SC-ASSIGN -->
**HITL Review (An Dao) — Reviewed: 2026-02-27:** The SC pin assignment is minimal (9 pins) reflecting its focused role as a safety monitor. The DCAN1 for CAN communication, GIO_A0 for kill relay control (ASIL D), and GIO_A1-A4 for per-ECU fault LEDs (ASIL B) are correctly prioritized. The TPS3823 WDT on GIO_A5 is consistent with the other ECUs. Using the onboard LED1 (GIO_B1) for heartbeat blink is practical for a demo. The kill relay on GIO_A0 with IRLZ44N MOSFET + 100R gate resistor + 10k pulldown follows safe default logic (relay de-energized on reset = motor power cut).
<!-- HITL-LOCK END:COMMENT-BLOCK-PIN-SC-ASSIGN -->

### 7.2 TMS570LC43x LaunchPad Connector Reference

| Connector | Pins Available | Used By |
|-----------|---------------|---------|
| J10 (proto header, 50-pin) | DCAN1RX (pin 44), DCAN1TX (pin 45), N2HET, ADC, etc. | DCAN1 TX/RX |
| J3 (HDR1 header) | GIO_A0 through GIO_A7 | Kill relay, LEDs, WDT |
| J12 | GIO_B1 (onboard LED1) | Heartbeat blink |
| J1 (XDS110 USB) | Debug/power | USB power + JTAG |

### 7.3 Pin Conflict Check

| Check | Result |
|-------|--------|
| DCAN1 vs DCAN4? | Using DCAN1 (via edge connector J5). DCAN4 NOT used due to HALCoGen v04.07.01 mailbox bug. |
| GIO_A pins: any conflict with onboard peripherals? | J3 header provides direct access to GIO_A0-A7. No conflict with onboard components (onboard LEDs are on GIO_B). |
| GIO_B1 (LED1) used for heartbeat vs other function? | GIO_B1 is the onboard LED1. Used only for heartbeat blink (QM). No conflict. |

<!-- HITL-LOCK START:COMMENT-BLOCK-PIN-SC-CONFLICT -->
**HITL Review (An Dao) — Reviewed: 2026-02-27:** The conflict checks are straightforward given the SC's simple pin usage. Using DCAN1 (not DCAN4) due to the HALCoGen mailbox bug is a critical note -- ensure this is also documented in the lessons-learned. No conflicts between GIO_A and onboard peripherals. The GIO_B1 heartbeat LED has no conflict. No unidentified issues found.
<!-- HITL-LOCK END:COMMENT-BLOCK-PIN-SC-CONFLICT -->

### 7.4 DCAN1 Configuration Notes

1. **DCAN1 module**: Enabled via HALCoGen configuration tool.
2. **Normal mode**: DCAN1 runs in normal operation (SWR-SC-029). TEST register bit 3 is NOT set. SC transmits SC_Status (CAN ID 0x013) via mailbox 7 in addition to receiving.
3. **Bit timing**: Configured for 500 kbps with 80% sample point (see HW Design section 6.3).
4. **Mailboxes**: Configure message objects 1-6 for reception of heartbeat (0x010-0x012), vehicle state (0x100), torque request (0x101), motor current (0x301). Mailbox 7 is TX-only for SC_Status (0x013).
5. **Proto header wiring**: DCAN1RX on J10 pin 44, DCAN1TX on J10 pin 45 (verified from schematic sprr397.pdf page 9 by coordinate analysis). Wire to SN65HVD230 breakout board.

<!-- HITL-LOCK START:COMMENT-BLOCK-PIN-SC-DCAN -->
**HITL Review (An Dao) — Reviewed: 2026-02-27:** The DCAN1 configuration notes are essential for the SC setup. The silent mode (listen-only) is critical for the safety controller's passive monitoring architecture -- it must not interfere with CAN bus traffic. The 500 kbps with 80% sample point matches the STM32 ECUs' CAN configuration. The mailbox configuration (objects 1-15 for specific CAN IDs) is appropriately selective. The note about HALCoGen and the edge connector wiring provides practical build guidance. No gaps identified.
<!-- HITL-LOCK END:COMMENT-BLOCK-PIN-SC-DCAN -->

---

## 8. Alternate Function Summary

### 8.1 STM32G474RE Alternate Function Numbers Used

| AF | Peripheral | Pins |
|----|-----------|------|
| AF1 | TIM2 | PA0 (CH1), PA1 (CH2) -- FZC servos |
| AF2 | TIM4 | PB6 (CH1), PB7 (CH2) -- RZC encoder |
| AF4 | I2C1 | PB8 (SCL), PB9 (SDA) -- CVC OLED |
| AF5 | SPI1 | PA5 (SCK), PA6 (MISO), PA7 (MOSI) -- CVC pedal sensors |
| AF5 | SPI2 | PB13 (SCK), PB14 (MISO), PB15 (MOSI) -- FZC steering angle sensor |
| AF6 | TIM1 | PA8 (CH1), PA9 (CH2) -- RZC motor PWM |
| AF7 | USART2 | PA2 (TX), PA3 (RX) -- FZC lidar UART |
| AF9 | FDCAN1 | PA11 (RX), PA12 (TX) -- all STM32 ECUs |
| AN | ADC1 | PA0-PA3 -- RZC analog inputs (IN1-IN4) |
| AN | ADC2 | PA4 (IN17), PB15 (IN15) -- RZC BTS7960 current sense |

### 8.2 Cross-ECU Pin Commonality

| Pin | CVC Function | FZC Function | RZC Function |
|-----|-------------|-------------|-------------|
| PA11 | FDCAN1_RX | FDCAN1_RX | FDCAN1_RX |
| PA12 | FDCAN1_TX | FDCAN1_TX | FDCAN1_TX |
| PA5 | SPI1_SCK | (unused) | (unused) |
| PA6 | SPI1_MISO | (unused) | (unused) |
| PA7 | SPI1_MOSI | (unused) | (unused) |
| PA4 | SPI1_CS (pedal 1) | (unused) | ADC1_IN5 (BTS7960 IS_L) |
| PB13 | (unused) | SPI2_SCK (steering) | (unused) |
| PB14 | (unused) | SPI2_MISO (steering) | (unused) |
| PB15 | (unused) | SPI2_MOSI (steering) | ADC1_IN15 (BTS7960 IS_R) |
| PB12 | (unused) | SPI2_CS (steering) | (unused) |
| PA0 | (unused) | TIM2_CH1 (steer servo) | ADC1_IN1 (current) |
| PA1 | (unused) | TIM2_CH2 (brake servo) | ADC1_IN2 (motor temp) |
| PA2 | (unused) | USART2_TX (lidar) | ADC1_IN3 (board temp) |
| PA3 | (unused) | USART2_RX (lidar) | ADC1_IN4 (battery V) |
| PB0 | WDT WDI | WDT WDI | Motor R_EN |
| PB4 | Status LED Green | Buzzer drive | WDT WDI |

**Note**: PB0 serves different functions on different ECUs. On CVC and FZC, it feeds the watchdog. On RZC, it controls the motor R_EN (watchdog moved to PB4).

<!-- HITL-LOCK START:COMMENT-BLOCK-PIN-AF-SUMMARY -->
**HITL Review (An Dao) — Reviewed: 2026-02-27:** The alternate function summary and cross-ECU pin commonality tables are excellent reference material. Section 8.1 clearly shows all AF numbers in use, which helps identify potential conflicts at a glance. Section 8.2 is particularly valuable for the BSW team -- it shows which pins serve different functions across ECUs, requiring per-ECU GPIO initialization configuration. The PB0 and PB4 role swap between CVC/FZC and RZC is clearly documented. The AN (ADC1) row confirms PA0-PA4 and PB15 are analog-only on RZC. No gaps identified.
<!-- HITL-LOCK END:COMMENT-BLOCK-PIN-AF-SUMMARY -->

---

## 9. Nucleo-64 Solder Bridge Modifications

The following solder bridge changes are required on the Nucleo-64 boards:

| ECU | Solder Bridge | Action | Reason |
|-----|--------------|--------|--------|
| FZC | SB63 | REMOVE | Free PA2 from ST-LINK VCP TX (needed for lidar UART TX) |
| FZC | SB65 | REMOVE | Free PA3 from ST-LINK VCP RX (needed for lidar UART RX) |
| RZC | SB63 | REMOVE | Free PA2 from ST-LINK VCP TX (needed for ADC1_IN3) |
| RZC | SB65 | REMOVE | Free PA3 from ST-LINK VCP RX (needed for ADC1_IN4) |
| CVC | SB21 | REMOVE (optional) | Disconnect PA5 from onboard LED LD2 (SPI1_SCK interference). Alternatively, accept LED blinking with SPI clock. |

**Note**: After removing SB63/SB65, printf debug output must use LPUART1 (PG7/PG8 on morpho) or SWO (PB3) instead of the default USART2 VCP.

<!-- HITL-LOCK START:COMMENT-BLOCK-PIN-SOLDER-BRIDGE -->
**HITL Review (An Dao) — Reviewed: 2026-02-27:** The solder bridge modification table is critical for build reproducibility. All required modifications are clearly identified per ECU with reasons. The optional SB21 on CVC (PA5/LD2) is appropriately marked as optional. The debug output alternative (LPUART1 or SWO) after SB63/SB65 removal is important practical guidance. Consider adding photos or Nucleo board diagrams showing solder bridge locations in the schematics folder for build documentation. This section should be one of the first references when setting up new boards.
<!-- HITL-LOCK END:COMMENT-BLOCK-PIN-SOLDER-BRIDGE -->

---

## 10. Revision History

| Version | Date | Author | Changes |
|---------|------|--------|---------|
| 0.1 | 2026-02-21 | System | Initial preliminary pin mapping |
| 1.0 | 2026-02-21 | System | Complete pin mapping: CVC (13 pins), FZC (14 pins), RZC (17 pins), SC (9 pins). Added AF numbers, Nucleo header references, voltage levels, net names, ASIL, pin conflict checks, solder bridge modifications, unused pin handling |

