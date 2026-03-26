---
paths:
  - "hardware/**/*"
  - "firmware/src/**/*"
  - "firmware/include/**/*"
---
# Hardware

**HAL**: all HW access through abstraction. One module per peripheral. Platform swap = rewrite HAL only.

**Pins**: single `pin_map.h`, descriptive names, no raw numbers. Unused = input+pull-down.

**Buses**: UART (framing+CRC+timeout), SPI (CPOL/CPHA docs, proper CS), I2C (bus recovery, ACK check). ADC: calibrate, filter, reject out-of-range.
