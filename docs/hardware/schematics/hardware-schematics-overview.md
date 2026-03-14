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

# Schematics

ASCII wiring diagrams and circuit references for the physical platform, organized per subsystem.

> **Canonical source**: The comprehensive hardware design is in `docs/aspice/hardware-eng/hw-design.md` (HWDES). These files are extracted for quick per-block reference during assembly and firmware development.

## Index

| # | File | Block | Content |
|---|------|-------|---------|
| 01 | [01-system-block-diagram.md](01-system-block-diagram.md) | System Overview | Top-level block diagram: power rails, ECUs, CAN bus, cloud |
| 02 | [02-power-distribution.md](02-power-distribution.md) | Power Distribution | 12V rail, buck converters, fuses, kill relay gating, power budget |
| 03 | [03-can-bus.md](03-can-bus.md) | CAN Bus | Bus topology, transceivers (TJA1051/SN65HVD230), termination, bit timing |
| 04 | [04-safety-chain.md](04-safety-chain.md) | Safety Chain | Kill relay, E-stop button, fault LEDs |
| 05 | [05-cvc-circuits.md](05-cvc-circuits.md) | CVC Circuits | CAN, dual AS5048A SPI, OLED I2C, E-stop, WDT, power |
| 06 | [06-fzc-circuits.md](06-fzc-circuits.md) | FZC Circuits | CAN, AS5048A SPI, servo PWM, TFMini UART, buzzer, WDT |
| 07 | [07-rzc-circuits.md](07-rzc-circuits.md) | RZC Circuits | CAN, BTS7960 H-bridge, ACS723 current, NTC temp, battery V, encoder, WDT |
| 08 | [08-sc-circuits.md](08-sc-circuits.md) | SC Circuits | DCAN1 listen-only, kill relay control, fault LEDs, WDT, power |
| 09 | [09-grounding.md](09-grounding.md) | Grounding | Star ground topology, analog/digital separation |

## Cross-References

- **Pin assignments**: `hardware/pin-mapping.md`
- **Bill of materials**: `hardware/bom.md` (74 items, procurement tracker, validation chronicle)
- **Hardware design (full)**: `docs/aspice/hardware-eng/hw-design.md`
