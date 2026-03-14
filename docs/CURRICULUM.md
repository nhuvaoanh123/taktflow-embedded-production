# Automotive Embedded Systems — 2-Year Curriculum

**Target:** Engineering students (EE, CE, ME) entering automotive embedded systems development.
**Duration:** 24 months, 4 quarters per year, ~10 hrs/week self-study.
**Outcome:** Junior-to-mid embedded engineer capable of contributing to a production AUTOSAR/ISO 26262 project.

---

## Prerequisites (Before You Start)

| Skill | Minimum Level |
|-------|--------------|
| C programming | Can write structs, pointers, arrays without help |
| Linux command line | `ls`, `cd`, `grep`, `make` — comfortable |
| Git basics | `commit`, `branch`, `merge` understood |
| Basic electronics | Voltage, current, resistance, Ohm's law |
| Digital logic | AND/OR/NAND, flip-flops, clocks |

If any of these are missing, fix them first — they are not covered here.

---

## Curriculum Map

```
Year 1                              Year 2
┌──────────────────────────────┐   ┌──────────────────────────────┐
│ Q1: C Mastery + Toolchain    │   │ Q5: ISO 26262 Functional Safety│
│ Q2: Hardware + CAN + Protocols│  │ Q6: ASPICE + Process          │
│ Q3: AUTOSAR Architecture     │   │ Q7: ARXML + Code Generation   │
│ Q4: Testing Pyramid + SIL    │   │ Q8: Capstone Project          │
└──────────────────────────────┘   └──────────────────────────────┘
```

---

---

# YEAR 1 — FOUNDATIONS

---

## Q1 — C Mastery, Embedded Toolchain, and Bare-Metal Programming

**Goal:** Write production-quality embedded C. Set up a real toolchain. Run firmware on hardware.

---

### Month 1 — Advanced C for Embedded

#### Topics
- Memory layout: stack, heap, `.bss`, `.data`, `.text` — what goes where and why
- Pointers to functions, pointers to structs, const-correctness
- Volatile: why it exists, when to use it, what breaks without it
- Bit manipulation: masks, shifts, set/clear/toggle, bitfields
- Fixed-width integer types (`uint8_t`, `int32_t`) — why `int` is forbidden in embedded
- Endianness: little-endian vs big-endian, byte swapping
- Inline functions vs macros: trade-offs in embedded contexts

#### Reading
- **Book:** *Programming Embedded Systems in C and C++* — Michael Barr (Ch. 1–4)
- **Book:** *Make: AVR Programming* — Elliot Williams (Ch. 1–3, concepts only)
- **Online:** [Embedded Artistry — Practical C Patterns](https://embeddedartistry.com)
- **Reference:** MISRA-C:2012 Guidelines summary (free summary available online)

#### Exercises

**EX-1.1 — Memory Map**
Write a C program. Use `objdump -t` to find where each variable lives (stack vs `.bss` vs `.data`). Write down what you find. Add `volatile` to one variable, observe how the compiler output changes with `gcc -O2`.

**EX-1.2 — Bit Manipulation Library**
Write a header-only library `bits.h` with these macros/inlines:
- `BIT_SET(reg, bit)`, `BIT_CLR(reg, bit)`, `BIT_TOGGLE(reg, bit)`, `BIT_READ(reg, bit)`
- `BITS_SET(reg, mask)`, `BITS_CLR(reg, mask)`
- Write 20 unit tests using Unity that verify each function on 8-bit, 16-bit, 32-bit values.

**EX-1.3 — Fixed-Width Types**
Take any open-source embedded project (e.g., FreeRTOS). Find 5 places where `int` or `unsigned` is used where a fixed-width type should be. Write the corrected version and explain why the original is risky.

**EX-1.4 — Endianness**
Write `uint32_to_can_bytes(uint32_t value, uint8_t *buf)` and `can_bytes_to_uint32(uint8_t *buf)`. Test that encoding then decoding gives back the original value on both little- and big-endian hosts (use `__BYTE_ORDER__` to detect).

---

### Month 2 — Toolchain, Build Systems, and Debugging

#### Topics
- GCC cross-compilation: `arm-none-eabi-gcc`, flags, `-O0` vs `-O2` vs `-Os`
- Linker scripts: `MEMORY`, `SECTIONS`, placement of `.text`, `.data`, `.bss`, vectors
- Makefiles: variables, targets, pattern rules, dependency tracking
- CMake basics for embedded
- OpenOCD + GDB: flash, breakpoint, watchpoint, memory inspect
- UART as debug output: `printf` over UART, semihosting
- Static analysis: Cppcheck, `-Wall -Wextra -Werror`

#### Reading
- **Book:** *The Definitive Guide to ARM Cortex-M3/M4 Processors* — Joseph Yiu (Ch. 1–3)
- **Online:** [Interrupt Blog — Demystifying Firmware Linker Scripts](https://interrupt.memfault.com/blog/how-to-write-linker-scripts-for-firmware)
- **Online:** [Memfault Blog — GDB for Embedded](https://interrupt.memfault.com/blog/gdb-for-firmware-1)

#### Exercises

**EX-2.1 — Linker Script from Scratch**
Starting from a blank STM32 project (no CubeMX), write a minimal linker script that places:
- `.text` in Flash at `0x08000000`
- `.data` copied from Flash to RAM at `0x20000000`
- `.bss` zero-initialised in RAM
- Stack pointer set to top of RAM
Verify it boots by toggling an LED.

**EX-2.2 — Makefile Build System**
Write a `Makefile` that:
- Cross-compiles all `.c` files in `src/`
- Has separate `debug` and `release` targets (different optimisation flags)
- Runs Cppcheck as part of `make lint`
- Has `make flash` that calls OpenOCD
- Rebuilds only changed files (dependency tracking via `.d` files)

**EX-2.3 — GDB Debugging Session**
Introduce a deliberate bug: a null pointer dereference triggered after 5 button presses. Use GDB to:
- Set a breakpoint at the fault handler
- Inspect the stack frame
- Find the program counter at the time of fault
- Identify the line of source code

Write a short report: what you typed in GDB and what you found.

---

### Month 3 — Bare-Metal Peripheral Programming

#### Topics
- GPIO: input, output, alternate function, pull-up/pull-down
- UART: baud rate calculation, TX/RX, interrupt-driven vs polling
- SPI and I2C: master/slave, clock polarity, addressing
- Timers: PWM generation, input capture, periodic interrupts
- DMA: why it exists, channel configuration, half-transfer vs full-transfer
- Interrupt priorities: NVIC, preemption, tail-chaining
- Low-power modes: sleep, stop, standby — trade-offs

#### Reading
- **Datasheet:** STM32G4 Reference Manual (RM0440) — GPIO chapter + USART chapter
- **Book:** *Embedded Systems with ARM Cortex-M Microcontrollers* — Yifeng Zhu (Ch. 5–9)
- **Online:** [STM32 Bare Metal Series — Low Byte Productions](https://www.youtube.com/c/LowByteProductions)

#### Exercises

**EX-3.1 — UART Driver Without HAL**
Write a UART driver for STM32 from scratch (register access only, no HAL):
- `uart_init(uint32_t baud)`
- `uart_putc(char c)`
- `uart_getc(void)` — blocking
- `uart_puts(const char *s)`
Verify by echoing characters from a terminal.

**EX-3.2 — Interrupt-Driven Ring Buffer**
Extend EX-3.1: UART RX driven by interrupt, storing bytes in a ring buffer. Implement:
- `ring_buf_push(ring_buf_t *rb, uint8_t byte)`
- `ring_buf_pop(ring_buf_t *rb, uint8_t *byte)`
- Overflow handling: drop oldest or return error
Write 15 unit tests for the ring buffer (host-compiled, no hardware needed).

**EX-3.3 — PWM Motor Control**
Generate a PWM signal at 20 kHz. Read a potentiometer via ADC. Map ADC reading (0–4095) to duty cycle (0–100%). Test by varying pot and measuring PWM on oscilloscope or logic analyser.

**EX-3.4 — DMA UART Transfer**
Use DMA to send a 256-byte buffer over UART without CPU involvement. Measure CPU load with and without DMA (use a GPIO toggle to time CPU usage).

---

### Q1 Mini-Project: Sensor Logger

**Description:** Build a standalone data logger on STM32:
- Read temperature from an I2C sensor (e.g., LM75 or TMP102) every 100ms
- Log readings to UART in CSV format: `timestamp_ms,temperature_c`
- If temperature exceeds threshold, toggle an LED at 5 Hz
- Ring buffer must not overflow even if UART is slow
- All drivers written bare-metal (no HAL)
- Makefile build, Cppcheck clean, zero compiler warnings

**Assessment criteria:**
- [ ] Ring buffer implementation correct (unit tested)
- [ ] No busy-wait loops (interrupt or DMA driven)
- [ ] Correct baud rate, no framing errors
- [ ] Code compiles clean with `-Wall -Wextra -Werror`
- [ ] I2C timing verified on logic analyser

---

---

## Q2 — CAN Bus, Automotive Protocols, and Hardware Bring-Up

**Goal:** Understand automotive communication deeply. Get two real nodes talking on CAN.

---

### Month 4 — CAN Bus Deep Dive

#### Topics
- CAN frame structure: SOF, arbitration ID, RTR, IDE, DLC, data, CRC, ACK, EOF
- Arbitration: how priority works, dominant vs recessive bits
- Bit timing: prescaler, TSEG1, TSEG2, SJW — how to calculate
- Error detection: bit error, stuff error, CRC error, form error, ACK error
- Error confinement: error-active, error-passive, bus-off states
- CAN FD: increased payload (64 bytes), higher data-phase bitrate, BRS, ESI bits
- Extended IDs (29-bit) vs standard (11-bit)
- Filters and masks: hardware acceptance filters
- Tools: `candump`, `cansend`, `cangen`, SocketCAN on Linux

#### Reading
- **Book:** *A Comprehensible Guide to Controller Area Network* — Wilfried Voss
- **Standard:** CAN 2.0A/B specification (free online from Bosch)
- **Online:** [Kvaser CAN Bus Guide](https://www.kvaser.com/can-protocol-tutorial/)
- **Reference:** [docs/hardware/schematics/03-can-bus.md](hardware/schematics/03-can-bus.md) — this project's CAN topology
- **Lesson:** [docs/reference/lessons-learned/infrastructure/PROCESS-can-bus-bring-up.md](reference/lessons-learned/infrastructure/PROCESS-can-bus-bring-up.md)

#### Exercises

**EX-4.1 — CAN Frame Decoder**
Write a Python script that reads raw CAN frames from `candump` output and decodes:
- Frame type (standard/extended/remote/error)
- Arbitration ID in hex and binary
- Data bytes as hex and as decimal
- DLC

**EX-4.2 — Bit Timing Calculator**
Write a C function `can_calc_timing(uint32_t clk_hz, uint32_t bitrate, can_timing_t *out)` that calculates:
- Prescaler
- TSEG1, TSEG2
- SJW
Verify against TI's CAN timing calculator for 500 kbps at 80 MHz.

**EX-4.3 — CAN Error Injection**
Using two STM32 nodes on a real CAN bus:
- Send a continuous stream from Node A
- Disconnect the CAN terminator resistor
- Observe: does Node A go bus-off? How many error frames appear before?
Write: what you observed in `candump` and what the theory predicts.

**EX-4.4 — SocketCAN on Linux**
Set up a virtual CAN bus (`vcan0`) on Linux. Write a C program that:
- Sends 100 frames per second on `vcan0`
- Receives frames and prints them to stdout
- Uses `SO_TIMESTAMP` to measure jitter
Measure and report average and max jitter over 10,000 frames.

---

### Month 5 — Automotive Protocol Stack

#### Topics
- **UDS (ISO 14229):** diagnostic services, session types (default, extended, programming), SID 0x10–0x3E, security access (0x27), DID read/write (0x22/0x2E), DTC read (0x19)
- **XCP:** calibration and measurement, ODT, DAQ, STIM — what calibration engineers actually do
- **LIN:** master/slave scheduling, LIN frames vs CAN frames, LIN diagnostics
- **Ethernet (automotive):** 100BASE-T1, SOME/IP, DDS, why Ethernet is coming to vehicles
- **OBD-II:** standardised diagnostic connector, Mode 01–09, what a scan tool reads
- **EnOcean / other wireless:** brief awareness — where wireless is used in vehicles vs buildings

#### Reading
- **Book:** *Automotive Embedded Systems Handbook* — Nicolas Navet (Ch. 3–5)
- **Standard:** ISO 14229-1:2020 summary (search for "UDS tutorial" — Vector and PEAK have good free summaries)
- **Online:** [Vector — Introduction to UDS](https://www.vector.com/int/en/know-how/protocols/uds-unified-diagnostic-services/)
- **Online:** [CSS Electronics — CAN Bus Explained](https://www.csselectronics.com/pages/can-bus-simple-intro-tutorial)

#### Exercises

**EX-5.1 — UDS Session Simulator**
Write a Python UDS simulator over SocketCAN that responds to:
- `0x10 0x01` (default session request) → `0x50 0x01`
- `0x10 0x03` (extended session) → `0x50 0x03`
- `0x22 0xF1 0x90` (read VIN) → `0x62 0xF1 0x90` + 17-byte VIN
- `0x27 0x01` (security access seed request) → `0x67 0x01` + 4-byte seed
Test using `isotpsend` / `isotprecv` from the `can-utils` package.

**EX-5.2 — DTC Decoder**
Write a C function that decodes a 3-byte DTC (per ISO 14229) into:
- System (powertrain / chassis / body / network)
- Subsystem code
- Fault code
- Status byte breakdown (confirmed, pending, test failed, etc.)
Test with 10 real DTC values from an OBD-II scan.

**EX-5.3 — LIN Frame Parser**
Write a Python parser for LIN frame logs (can be simulated as byte arrays). Parse:
- Sync byte (`0x55`)
- Protected ID (calculate parity bits from frame ID)
- Data bytes
- Checksum (classic vs enhanced)

---

### Month 6 — Hardware Bring-Up and Signal Integrity

#### Topics
- PCB bring-up methodology: power rail sequencing, current measurement, thermal check
- Oscilloscope skills: triggering, cursors, protocol decode (UART, SPI, I2C, CAN)
- Logic analyser: when to use vs oscilloscope, sampling rate vs resolution trade-offs
- Signal integrity: termination resistors, impedance matching, ground planes
- EMC basics: what causes radiated emissions, filtering, shielding
- Common failure modes: floating inputs, shared ground issues, capacitor choice
- Debug techniques: binary search isolation, process of elimination

#### Reading
- **Book:** *Hardware Hacker* — Andrew Bunnie Huang (Ch. 1–3)
- **Reference:** [docs/hardware/taktflow-embedded-hardware-overview.md](hardware/taktflow-embedded-hardware-overview.md)
- **Reference:** [docs/hardware/schematics/01-system-block-diagram.md](hardware/schematics/01-system-block-diagram.md)
- **Lesson:** [docs/reference/lessons-learned/hardware/PROCESS-stm32-cubemx-bringup.md](reference/lessons-learned/hardware/PROCESS-stm32-cubemx-bringup.md)
- **Lesson:** [docs/reference/lessons-learned/infrastructure/PROCESS-stm32g4-fdcan-bringup.md](reference/lessons-learned/infrastructure/PROCESS-stm32g4-fdcan-bringup.md)

#### Exercises

**EX-6.1 — Power-On Checklist**
Read [docs/hardware/bom.md](hardware/bom.md) and [docs/hardware/schematics/02-power-distribution.md](hardware/schematics/02-power-distribution.md). Write a 15-step power-on procedure for bringing up a new board for the first time, including: what to measure, in what order, and what values indicate success or failure.

**EX-6.2 — CAN Termination Experiment**
On a real CAN bus with two nodes:
- Measure bus voltage with both, one, and no termination resistors
- Capture `candump` output in each case
- Measure frame error rate in each case
Write a report: what is the minimum termination needed for reliable communication at 500 kbps over 30cm of wire?

**EX-6.3 — Wiring Log**
Read [docs/hardware/wiring-log.md](hardware/wiring-log.md). Write your own wiring log for a 3-node CAN system you build (even on a breadboard). Include: wire colours, pin numbers, connector types, changes made and why.

---

### Q2 Mini-Project: Two-Node CAN System

**Description:** Build a CAN system with two STM32 nodes:
- Node A: reads a potentiometer, sends CAN frame every 50ms with value (0–100%)
- Node B: receives frame, drives LED brightness via PWM proportional to value
- Both nodes implement heartbeat: send alive counter every 1s, detect if other node silent for >3s → fault LED
- UDS diagnostic support on Node A: respond to ReadDataByIdentifier (DID 0x1000 = pot value)
- Written bare-metal (no HAL), FDCAN peripheral, SocketCAN logging via USB adapter

**Assessment criteria:**
- [ ] Correct CAN bit timing at 500 kbps (verified on oscilloscope)
- [ ] Heartbeat timeout detection works (pull CAN connector, fault LED lights within 3s)
- [ ] UDS response correct (tested with `isotpsend`)
- [ ] No bus-off events in 1 hour of operation
- [ ] Wiring log documented

---

---

## Q3 — AUTOSAR Architecture and BSW

**Goal:** Understand and work inside a real AUTOSAR-layered stack.

---

### Month 7 — AUTOSAR Fundamentals

#### Topics
- AUTOSAR Classic Platform architecture: ASW, RTE, BSW, MCAL
- Layering rules: what is allowed to call what — and why
- RTE: ports, interfaces, runnable entities, inter-ECU communication
- COM stack: signal → PDU → frame → physical — the full chain
- BSW modules: COM, PDU Router, CAN Interface, CAN Driver, OS, NvM, Dem, Dcm
- SWC types: application, sensor-actuator, ECU abstraction
- System description: ARXML as the single source of truth
- Vendor toolchains: Vector DaVinci, EB Tresos, AUTOSAR Builder (awareness only)

#### Reading
- **Standard:** AUTOSAR Explained — *AUTOSAR Classic Platform Overview* (free PDF from autosar.org)
- **Book:** *AUTOSAR Compendium Part 1: Application & RTE* — Jörg Rech
- **Reference:** [docs/aspice/swa/bsw-architecture.md](aspice/swa/bsw-architecture.md)
- **Reference:** [docs/aspice/swa/sw-architecture.md](aspice/swa/sw-architecture.md)
- **Lesson:** [docs/reference/lessons-learned/infrastructure/PROCESS-com-sendsignal-vs-pdur-transmit.md](reference/lessons-learned/infrastructure/PROCESS-com-sendsignal-vs-pdur-transmit.md)

#### Exercises

**EX-7.1 — AUTOSAR Stack Diagram**
Draw (on paper or draw.io) the full AUTOSAR stack for one ECU in this project (pick BCM or CVC). For each BSW module you place, write one sentence: what it does and which module calls it and which module it calls.

**EX-7.2 — COM Signal Trace**
Pick a signal from [docs/aspice/sys/can-message-matrix.md](aspice/sys/can-message-matrix.md). Trace it from the physical CAN frame all the way to the application SWC that consumes it. Write down every module it passes through, the API called at each boundary, and the data type at each stage.

**EX-7.3 — RTE Port Definition**
Write an AUTOSAR port interface definition for a temperature sensor SWC:
- Sender port: `TemperatureSender` — sends `float32` temperature value every 100ms
- Receiver port: `TemperatureReceiver` — receives same value, with `NEVER_RECEIVED` init value
Write it as an XML fragment (mimic ARXML format). Bonus: check if your definition matches the AUTOSAR schema.

---

### Month 8 — AUTOSAR COM Stack and Diagnostics

#### Topics
- `Com_SendSignal` vs `PduR_Transmit`: when each is appropriate
- COM callbacks: `Com_RxIndication`, `Com_TxConfirmation`
- PDU routing tables: why a gateway ECU needs special routing configuration
- RX timeout monitoring: `ComSignalRxTimeoutSubstitution` — safe default values
- Dem (Diagnostic Event Manager): events, DTCs, event status, storage conditions
- Dcm (Diagnostic Communication Manager): UDS session management, security access, DID handling
- NvM (Non-Volatile Memory Manager): block management, job queuing, write scheduling
- WdgM (Watchdog Manager): checkpoint sequencing, watchdog triggering

#### Reading
- **Reference:** [docs/aspice/sys/can-message-matrix.md](aspice/sys/can-message-matrix.md)
- **Lesson:** [docs/reference/lessons-learned/infrastructure/PROCESS-com-rx-timeout-sensor-feeder.md](reference/lessons-learned/infrastructure/PROCESS-com-rx-timeout-sensor-feeder.md)
- **Lesson:** [docs/reference/lessons-learned/infrastructure/PROCESS-stale-can-shadow-buffers.md](reference/lessons-learned/infrastructure/PROCESS-stale-can-shadow-buffers.md)
- **Lesson:** [docs/reference/lessons-learned/infrastructure/PROCESS-heartbeat-hardening.md](reference/lessons-learned/infrastructure/PROCESS-heartbeat-hardening.md)
- **Lesson:** [docs/reference/lessons-learned/infrastructure/PROCESS-nvm-posix-buffer-overflow.md](reference/lessons-learned/infrastructure/PROCESS-nvm-posix-buffer-overflow.md)

#### Exercises

**EX-8.1 — COM Timeout Handler**
Write a bare-metal simulation (host C code) of COM RX timeout monitoring:
- Signal has a 200ms timeout
- If not refreshed within 200ms, substitute value activates (`0xFF`)
- Implement periodic tick (1ms), `signal_refresh()`, `signal_get()`
- Write 10 test cases: normal refresh, late refresh, never refreshed, substitution value correct

**EX-8.2 — DTC State Machine**
Implement a DTC status byte manager per ISO 14229 Annex D:
- Bits: `testFailed`, `testFailedThisOperationCycle`, `pendingDTC`, `confirmedDTC`, `testNotCompletedSinceLastClear`, `testFailedSinceLastClear`, `testNotCompletedThisOperationCycle`, `warningIndicatorRequested`
- Implement transitions for: test pass, test fail, operation cycle start, DTC clear
Write 20 unit tests verifying all bit transitions.

**EX-8.3 — NvM Write Strategy**
Read [docs/reference/lessons-learned/infrastructure/PROCESS-nvm-posix-buffer-overflow.md](reference/lessons-learned/infrastructure/PROCESS-nvm-posix-buffer-overflow.md). Write a design doc (1 page) explaining: why NvM writes are asynchronous in AUTOSAR, what can go wrong if you call `NvM_WriteBlock` too frequently, and how to design around it.

---

### Month 9 — AUTOSAR OS and Scheduling

#### Topics
- AUTOSAR OS (OSEK-based): tasks, alarms, events, resources
- Task types: basic tasks vs extended tasks
- Scheduling: fixed-priority preemptive, scheduling tables
- Stack usage analysis: why stack overflow is a silent killer
- BSW main functions: `*_MainFunction()` scheduling discipline
- OSEK/VDX: how the AUTOSAR OS descended from OSEK
- ThreadX (Azure RTOS): an alternative RTOS used in newer platforms — threads, queues, semaphores
- Context switching cost: why task count matters for CPU load

#### Reading
- **Standard:** OSEK/VDX OS Specification v2.2.3 (free from osek-vdx.org)
- **Reference:** [docs/plans/OSEK_OS_SPEC.md](plans/OSEK_OS_SPEC.md)
- **Reference:** [docs/plans/plan-os-threadx-bootstrap.md](plans/plan-os-threadx-bootstrap.md)
- **Reference:** [docs/reference/threadx-local-reference-map.md](reference/threadx-local-reference-map.md)
- **Book:** *Real-Time Concepts for Embedded Systems* — Qing Li with Caroline Yao (Ch. 1–6)

#### Exercises

**EX-9.1 — OSEK Task Set Design**
Design the task set for a motor controller ECU with these runnables:
- `CurrentSenseRunnable` — must run every 1ms
- `SpeedControlRunnable` — every 10ms
- `DiagnosticsRunnable` — every 100ms
- `NvMWriteRunnable` — every 1000ms
Assign priorities (justify each), identify potential priority inversions, and calculate worst-case CPU load.

**EX-9.2 — Stack Overflow Detection**
Implement a stack canary mechanism:
- Fill each task stack with `0xDEADBEEF` at startup
- Add `stack_check()` that verifies canaries and reports remaining headroom
- Deliberately create a stack overflow (deep recursion) and verify detection
Test that your canary fires before the overflow corrupts adjacent memory.

**EX-9.3 — ThreadX Semaphore vs OSEK Resource**
Write a 1-page comparison: OSEK `GetResource/ReleaseResource` vs ThreadX mutex/semaphore. Cover: priority inheritance, ceiling priority, what happens on deadlock, which is safer for ASIL-B and above (with justification).

---

### Q3 Mini-Project: Mini AUTOSAR Stack

**Description:** Implement a minimal AUTOSAR-inspired software architecture on STM32:
- Two SWCs: `PedalSensor_SWC` (reads ADC) and `MotorControl_SWC` (drives PWM)
- Minimal RTE layer: port-based communication between SWCs (no direct calls)
- Minimal COM layer: pack/unpack CAN signal from RTE port value
- OSEK-style task scheduler: two tasks at 10ms and 100ms, fixed priority
- Heartbeat watchdog: if 100ms task misses 3 consecutive deadlines → safe state (PWM = 0)
- Host-compiled test build: test the RTE and COM logic without hardware

**Assessment criteria:**
- [ ] SWCs do not call each other directly (only through RTE ports)
- [ ] COM signal encoding matches DBC spec (verified with candump + cantools)
- [ ] Watchdog fires correctly (testable by blocking the 100ms task)
- [ ] Host-compiled unit tests pass for RTE port logic
- [ ] No global variables accessed without protection (mutex or task-level isolation)

---

---

## Q4 — Testing Pyramid: Unit, SIL, MIL, PIL, HIL

**Goal:** Build and run every layer of the automotive test pyramid. Understand what each layer catches and what it misses.

---

### Month 10 — Unit Testing and MISRA-C

#### Topics
- Unity test framework: `TEST_ASSERT_*` macros, test runners, suites
- CppUTest: C++ test framework for C code, `CHECK`, `EQUAL` macros
- Test doubles: stubs, fakes, mocks — when each is appropriate
- Code coverage: line, branch, MC/DC — why MC/DC is required for ASIL-D
- MISRA-C:2012: key rules, rule categories (mandatory/required/advisory)
- Static analysis tools: Cppcheck, PC-lint, SonarQube for embedded
- CI integration: running tests and static analysis on every commit

#### Reading
- **Book:** *Test-Driven Development for Embedded C* — James W. Grenning
- **Reference:** [docs/safety/analysis/misra-deviation-register.md](safety/analysis/misra-deviation-register.md)
- **Lesson:** [docs/reference/lessons-learned/testing/PROCESS-bsw-tdd-development.md](reference/lessons-learned/testing/PROCESS-bsw-tdd-development.md)
- **Lesson:** [docs/reference/lessons-learned/infrastructure/PROCESS-misra-pipeline.md](reference/lessons-learned/infrastructure/PROCESS-misra-pipeline.md)
- **Lesson:** [docs/reference/lessons-learned/testing/PROCESS-ci-test-hardening.md](reference/lessons-learned/testing/PROCESS-ci-test-hardening.md)

#### Exercises

**EX-10.1 — TDD Ring Buffer**
Using TDD (write test first, then code): implement a thread-safe ring buffer for CAN frames:
- `can_buf_push(can_frame_t *frame)` — returns `E_OK` or `E_OVERFLOW`
- `can_buf_pop(can_frame_t *out)` — returns `E_OK` or `E_EMPTY`
- `can_buf_size(void)` — current count
Write all tests first (minimum 25), then implement. Achieve 100% branch coverage (verify with `gcov`).

**EX-10.2 — MISRA-C Fixes**
Take the code from Q1/Q2 mini-projects. Run Cppcheck with MISRA enabled. Fix all violations. For each violation you cannot fix, write a formal deviation permit (1 paragraph: rule ID, why it fires, why it is safe here).

**EX-10.3 — Mock the Hardware**
Take `uart_putc` from EX-3.1. Write a mock implementation (`mock_uart.c`) that records calls in a buffer. Write a test for a higher-level `logger_write(msg)` that verifies exactly the right bytes were sent to UART. No hardware required.

---

### Month 11 — SIL and MIL

#### Topics
- SIL (Software-in-the-Loop): compile firmware for POSIX/Linux, run in Docker
- MIL (Model-in-the-Loop): test control logic in Python/Simulink before writing C
- Differences between SIL and target: timing, floating point, memory, peripheral absence
- Virtual CAN (`vcan0`): how to wire SIL nodes together on Linux
- Docker for embedded testing: reproducibility, CI integration, parallel runs
- Fault injection in SIL: how to inject missing signals, corrupted data, timing anomalies
- Plant models: simulating the physical environment (motor, sensor, actuator)

#### Reading
- **Reference:** [test/sil/test-sil-overview.md](../test/sil/test-sil-overview.md)
- **Reference:** [test/mil/test-mil-overview.md](../test/mil/test-mil-overview.md)
- **Reference:** [docs/aspice/swa/vecu-architecture.md](aspice/swa/vecu-architecture.md)
- **Lesson:** [docs/reference/lessons-learned/infrastructure/PROCESS-posix-vcan-porting.md](reference/lessons-learned/infrastructure/PROCESS-posix-vcan-porting.md)
- **Lesson:** [docs/reference/lessons-learned/infrastructure/PROCESS-sil-demo-integration.md](reference/lessons-learned/infrastructure/PROCESS-sil-demo-integration.md)
- **Lesson:** [docs/reference/lessons-learned/infrastructure/PROCESS-bsw-hardening-sil-timing.md](reference/lessons-learned/infrastructure/PROCESS-bsw-hardening-sil-timing.md)
- **Lesson:** [docs/reference/lessons-learned/infrastructure/PROCESS-plant-sim-dbc-encoding-mismatch.md](reference/lessons-learned/infrastructure/PROCESS-plant-sim-dbc-encoding-mismatch.md)

#### Exercises

**EX-11.1 — POSIX Port**
Take the Q3 mini-project firmware. Add a POSIX build target:
- Replace `uart_putc` with `write(STDOUT_FILENO, ...)`
- Replace CAN peripheral with SocketCAN (`socket(PF_CAN, ...)`)
- Replace `SysTick` with `SIGALRM` or `timerfd`
The firmware should run on Linux and send/receive real CAN frames on `vcan0`.

**EX-11.2 — Docker SIL Environment**
Write a `Dockerfile` and `docker-compose.yml` that:
- Builds and runs the POSIX firmware in a container
- Sets up `vcan0` inside the container (requires `--privileged`)
- Runs a Python plant simulator that generates sensor signals and consumes actuator commands
- Outputs pass/fail verdict to stdout as JUnit XML

**EX-11.3 — Fault Injection**
Extend EX-11.2: add a fault injector container that, after 30 seconds, stops sending the pedal position signal. Verify that the firmware detects the timeout within 200ms and enters safe state. Add this as an automated test case in the JUnit XML output.

---

### Month 12 — PIL and HIL

#### Topics
- PIL (Processor-in-the-Loop): firmware on real MCU, test harness on PC
- OpenOCD + GDB automation: scripted test execution over JTAG
- HIL (Hardware-in-the-Loop): real ECU + real I/O + real-time plant simulation
- HIL bench architecture: real-time PC (dSPACE, NI, or custom), breakout board, fault injection relays
- IO override and forcing: how HIL systems inject signals and override ECU outputs
- HIL test case structure: setup → stimulus → assert → teardown
- Limitations of HIL: what it still cannot test (radiation, temperature aging, EMC)

#### Reading
- **Reference:** [test/pil/test-pil-overview.md](../test/pil/test-pil-overview.md)
- **Reference:** [docs/aspice/val/](aspice/val/)
- **Lesson:** [docs/reference/lessons-learned/safety/PROCESS-hil-gap-analysis.md](reference/lessons-learned/safety/PROCESS-hil-gap-analysis.md)
- **Lesson:** [docs/reference/lessons-learned/safety/PROCESS-sil-hil-divergence-preflight.md](reference/lessons-learned/safety/PROCESS-sil-hil-divergence-preflight.md)

#### Exercises

**EX-12.1 — PIL Test Harness**
Using OpenOCD and Python:
- Flash the Q3 mini-project to a real STM32
- Write a Python script that sends CAN frames and reads CAN responses via USB adapter
- Assert: within 50ms of sending a pedal position, PWM output is within ±2% of expected
- Run 100 iterations, report pass rate

**EX-12.2 — SIL vs PIL Divergence**
Run the same test on both SIL (EX-11.2) and PIL (EX-12.1). Find and document one difference in behaviour between SIL and PIL. Hypothesise the cause (timing, floating point, peripheral timing).

**EX-12.3 — HIL Gap Analysis**
Read [docs/reference/lessons-learned/safety/PROCESS-hil-gap-analysis.md](reference/lessons-learned/safety/PROCESS-hil-gap-analysis.md). Write your own gap analysis for the Q3 mini-project: what failure modes does your SIL environment NOT cover, and what physical setup would be needed to cover them?

---

### Q4 Mini-Project: Full Test Pyramid

**Description:** Apply all four test layers to the Q3 mini-project:
- **Unit:** 50+ tests, 100% branch coverage on business logic, 0 MISRA violations
- **SIL:** Docker Compose with plant simulator, 5 automated scenarios (nominal + 4 fault injections)
- **PIL:** Python test harness, 100 iterations on real hardware, pass rate ≥99%
- **CI:** GitHub Actions or Jenkins pipeline that runs unit + SIL on every push, PIL weekly

**Assessment criteria:**
- [ ] All unit tests pass in CI
- [ ] All SIL scenarios produce JUnit XML with correct verdicts
- [ ] PIL pass rate ≥99% over 100 runs
- [ ] Pipeline fails the build if unit coverage drops below 80%
- [ ] Gap analysis written for what the test suite cannot cover

---

---

# YEAR 2 — PROFESSIONAL PRACTICE

---

## Q5 — ISO 26262 Functional Safety

**Goal:** Read, understand, and apply ISO 26262 Parts 4, 5, 6 to real embedded software.

---

### Month 13 — HARA and Safety Goals

#### Topics
- ISO 26262 structure: Part 1 (vocab) through Part 12 (motorcycles)
- Item definition: system boundary, operating modes, hazardous events
- HARA: severity (S0–S3), exposure (E0–E4), controllability (C0–C3), ASIL assignment
- Safety goals: property of the item, not the system
- Safe states: what constitutes a safe state for each failure mode
- FTTI (Fault Tolerant Time Interval): how fast the system must respond
- ASIL decomposition: redundancy decomposition, independence requirements

#### Reading
- **Standard:** ISO 26262-3:2018 (Concept phase) — key clauses 5–8
- **Reference:** [docs/safety/concept/hara.md](safety/concept/hara.md)
- **Reference:** [docs/safety/concept/safety-goals.md](safety/concept/safety-goals.md)
- **Reference:** [docs/safety/concept/item-definition.md](safety/concept/item-definition.md)
- **Reference:** [docs/safety/analysis/asil-decomposition.md](safety/analysis/asil-decomposition.md)
- **Reference:** [docs/safety/analysis/heartbeat-ftti-budget.md](safety/analysis/heartbeat-ftti-budget.md)

#### Exercises

**EX-13.1 — HARA for a Braking System**
Take a simplified vehicle braking system (front brake, rear brake, ECU, pedal sensor). Identify 5 hazardous events. For each: assign S, E, C values (with justification). Derive ASIL. Write the safety goal.

**EX-13.2 — Safe State Analysis**
For each safety goal you wrote in EX-13.1: define the safe state. What must be true of the system output for the system to be "safe" in that failure mode? What transition sequence achieves that state?

**EX-13.3 — FTTI Budget**
Read [docs/safety/analysis/heartbeat-ftti-budget.md](safety/analysis/heartbeat-ftti-budget.md). For one of your EX-13.1 safety goals: decompose the FTTI into: detection time + reaction time + safe state transition time. Does the system in this project meet the budget? Show your calculation.

---

### Month 14 — Software Safety Requirements and Architecture

#### Topics
- Technical Safety Requirements (TSR): from safety goals to system requirements
- Software Safety Requirements (SSR): from TSR to software
- HSI (Hardware-Software Interface): what software must know about hardware for safety
- Software architectural design for safety: freedom from interference, no shared globals
- Safe coding guidelines beyond MISRA: defensive programming, error return checked, no recursion
- ASIL-D code review requirements: independence, what a second reviewer must verify
- Formal methods awareness: when are they used, why most projects avoid them

#### Reading
- **Standard:** ISO 26262-6:2018 (Software) — clauses 7–10
- **Reference:** [docs/safety/requirements/sw-safety-reqs.md](safety/requirements/sw-safety-reqs.md)
- **Reference:** [docs/safety/requirements/technical-safety-reqs.md](safety/requirements/technical-safety-reqs.md)
- **Reference:** [docs/safety/concept/hsi-specification.md](safety/concept/hsi-specification.md)
- **Lesson:** [docs/reference/lessons-learned/safety/PROCESS-safety-case-development.md](reference/lessons-learned/safety/PROCESS-safety-case-development.md)

#### Exercises

**EX-14.1 — SSR Derivation**
Take safety goal SG-01 from [docs/safety/concept/safety-goals.md](safety/concept/safety-goals.md). Derive 3 technical safety requirements. Then derive 2 software safety requirements from each TSR. Format as: `SSR-XXX | Derived from TSR-YYY | Rationale | ASIL`.

**EX-14.2 — Freedom from Interference Checklist**
Review the Q3 mini-project code. Write a checklist of 10 items that verify "freedom from interference" between the `PedalSensor_SWC` and `MotorControl_SWC`. Check each item. Report: does this code meet ASIL-B freedom from interference requirements?

**EX-14.3 — HSI Document**
Write a 1-page HSI section for the STM32G4 CAN peripheral used in Q2:
- What hardware resources does the CAN driver use?
- What are the hardware assumptions (clock frequency, pin assignment, voltage levels)?
- What happens in software if the CAN peripheral returns an error?
- What must hardware guarantee for the software to be safe?

---

### Month 15 — Safety Verification and the Safety Case

#### Topics
- Safety case: what it is, what it proves, who reads it
- Safety evidence: test reports, coverage reports, review records, tool qualification
- Verification methods per ASIL: testing, formal verification, simulation — which are required
- Tool classification (ISO 26262 Part 8): TD1, TD2, TD3 — when to qualify a tool
- Diagnostic coverage: how to claim a DC value, hardware metrics (SPFM, LFM, PMHF)
- Safety validation: the final step — does the item meet its safety goals?
- Residual risk: what is left after all safety measures, how to accept it

#### Reading
- **Reference:** [docs/safety/plan/safety-case.md](safety/plan/safety-case.md)
- **Reference:** [docs/safety/plan/safety-plan.md](safety/plan/safety-plan.md)
- **Reference:** [docs/safety/analysis/hardware-metrics.md](safety/analysis/hardware-metrics.md)
- **Reference:** [docs/safety/validation/safety-validation-report.md](safety/validation/safety-validation-report.md)

#### Exercises

**EX-15.1 — Safety Case Fragment**
Write a safety case fragment for the heartbeat monitoring feature in the Q3 mini-project. Use GSN (Goal Structuring Notation) or plain text structured argument: claim → evidence → assumption → sub-claim. Cover: what failure it detects, what it does not detect, and what residual risk remains.

**EX-15.2 — Tool Qualification Decision**
Decide (with justification) whether `gcov` (code coverage tool) needs to be qualified per ISO 26262-8 in your project. Arguments to consider: what TD class is it, what error could it introduce, what is the impact on safety evidence.

**EX-15.3 — Hardware Metrics**
Read [docs/safety/analysis/hardware-metrics.md](safety/analysis/hardware-metrics.md). For a single-channel ADC reading pedal position (ASIL-D target): calculate SPFM assuming no diagnostic. What diagnostic measure would you add to meet ASIL-D SPFM target? What DC does that diagnostic achieve?

---

### Q5 Mini-Project: Safety Case for the Q3 System

**Description:** Write a complete (abbreviated) safety case for the Q3 mini-project:
- Item definition (1 page)
- HARA (5 hazardous events, ASIL assigned)
- 2 safety goals with FTTI
- 3 SSRs derived from each SG
- Safety evidence table: which test covers which SSR
- 1-page residual risk argument

**Assessment criteria:**
- [ ] ASIL assignments justified with S/E/C values
- [ ] Safety goals are properties of the system, not implementation details
- [ ] Every SSR has at least one test case mapped to it
- [ ] Residual risk explicitly acknowledged (not "no risk remaining")

---

---

## Q6 — ASPICE and Engineering Process

**Goal:** Understand and operate within an ASPICE-compliant development process.

---

### Month 16 — ASPICE Fundamentals

#### Topics
- ASPICE (Automotive SPICE): what it is, why OEMs require it, levels 0–5
- Key process areas: SYS.1–SYS.5, SWE.1–SWE.6, SUP.1, SUP.8, SUP.10, MAN.3
- Requirements engineering (SWE.1): elicitation, specification, bidirectional traceability
- Software architecture (SWE.2): design decisions, interface documentation
- Software unit design and implementation (SWE.3)
- Software unit verification (SWE.4): unit tests, static analysis, coverage
- Software integration (SWE.5) and system testing (SWE.6)
- Work products: what documents are mandatory at each process area

#### Reading
- **Standard:** ASPICE PAM 4.0 — Automotive SPICE Process Assessment Model (free from automotivespice.com)
- **Reference:** [docs/aspice/](aspice/) — browse all subdirectories
- **Reference:** [docs/reference/process-playbook.md](reference/process-playbook.md)

#### Exercises

**EX-16.1 — Process Gap Analysis**
Review the Q4 mini-project. For each ASPICE process area SWE.1–SWE.6, assess: which work products exist, which are missing, and what is needed to claim ASPICE Level 2 for each.

**EX-16.2 — SWR Document**
Write a Software Requirements Specification for the Q3 mini-project in ASPICE style:
- 10 functional requirements (shall statements)
- 3 non-functional requirements (timing, memory, MISRA)
- Each requirement: unique ID, rationale, ASIL, derived from (traceability up)
- Each requirement: verified by (traceability down to test)

**EX-16.3 — Architecture Decision Record**
Read [docs/reference/lessons-learned/process/PROCESS-architecture-decisions.md](reference/lessons-learned/process/PROCESS-architecture-decisions.md). Write an ADR for one design decision in the Q3 mini-project: the choice of task period for the speed control runnable. Format: context → options considered → decision → consequences.

---

### Month 17 — Traceability and Configuration Management

#### Topics
- Bidirectional traceability: stakeholder req → system req → SW req → SW design → code → test
- Traceability matrices: how to build, how to maintain, what breaks them
- Configuration management: baselines, change requests, version numbering
- CI-enforced traceability: how to fail a build when a requirement has no test
- Change impact analysis: when requirement changes, what else must change
- Branching strategy: trunk-based vs feature branch, what automotive projects use

#### Reading
- **Reference:** [docs/aspice/traceability/traceability-matrix.md](aspice/traceability/traceability-matrix.md)
- **Reference:** [docs/guides/traceability-guide.md](guides/traceability-guide.md)
- **Lesson:** [docs/reference/lessons-learned/process/PROCESS-traceability-automation.md](reference/lessons-learned/process/PROCESS-traceability-automation.md)
- **Lesson:** [docs/reference/lessons-learned/process/PROCESS-cross-document-consistency.md](reference/lessons-learned/process/PROCESS-cross-document-consistency.md)

#### Exercises

**EX-17.1 — Traceability Matrix**
Build a traceability matrix for the Q4 mini-project:
- Columns: Stakeholder Req → SW Req → Code Module → Test Case → Test Result
- At least 5 full chains
- Use a spreadsheet or Markdown table
- Identify any orphaned tests (no requirement) or untested requirements

**EX-17.2 — Change Impact Analysis**
A new requirement arrives: the pedal position range changes from 0–100% to 0–200%. Perform a change impact analysis:
- Which requirements change?
- Which code modules change?
- Which test cases must be re-run?
- Which baselines are invalidated?

---

### Month 18 — Reviews, Audits, and Risk Management

#### Topics
- Design reviews: checklists, roles (moderator, author, reviewer), how defects are logged
- Code reviews: what ASIL-D requires (independent reviewer), what to look for
- Risk management: risk identification, likelihood × severity matrix, mitigation tracking
- Supplier management: what you must verify when using a third-party BSW stack
- Lessons learned process: how to capture, store, and reuse engineering knowledge
- Project metrics: defect density, test coverage trends, open issues, velocity

#### Reading
- **Reference:** [docs/aspice/pm/risk-register.md](aspice/pm/risk-register.md)
- **Reference:** [docs/aspice/sqa/qa-plan.md](aspice/sqa/qa-plan.md)
- **Lesson:** [docs/reference/lessons-learned/process/PROCESS-hitl-review-methodology.md](reference/lessons-learned/process/PROCESS-hitl-review-methodology.md)
- **Reference:** [docs/reference/lessons-learned/reference-lessons-learned-overview.md](reference/lessons-learned/reference-lessons-learned-overview.md)

#### Exercises

**EX-18.1 — Code Review Checklist**
Write a 20-item code review checklist for ASIL-B embedded C code. Categories: safety, MISRA, memory, timing, error handling, testability. For each item: what to look for, what a violation looks like.

**EX-18.2 — Risk Register**
Create a risk register for the Q4 mini-project with 8 risks:
- 3 technical risks (e.g., SIL timing mismatch)
- 3 process risks (e.g., requirements changing late)
- 2 safety risks (e.g., undetected hardware fault)
For each: likelihood (1–5), impact (1–5), mitigation action, owner, status.

---

### Q6 Mini-Project: Process Package

**Description:** Produce an ASPICE-compliant process package for the Q4 mini-project:
- SRS (Software Requirements Specification) — 15+ requirements
- Software Architecture document — 2 diagrams + rationale
- Traceability matrix — full bidirectional
- Test report (SWE.4 work product)
- Change request record (simulate one requirement change from a "customer")
- Lessons learned entry — one real thing you learned during this quarter

---

---

## Q7 — ARXML, Code Generation, and Advanced Architecture

**Goal:** Understand model-based development and the ARXML → code pipeline.

---

### Month 19 — ARXML and Model-Based Development

#### Topics
- ARXML schema: what it describes (software components, ports, interfaces, ECU configuration)
- AUTOSAR data models: `SwComponentType`, `PortInterface`, `InternalBehavior`, `Runnable`
- System extract vs ECU extract: what each contains, why both are needed
- BSW module configuration in ARXML: ComConfig, CanIfConfig, NvMConfig
- Code generators: what they produce, template engines, Jinja2/Mustache for ARXML
- E2E data IDs: what they are, why they must be unique across ECUs, what breaks if they collide
- ARXML validation: schema validation vs semantic validation

#### Reading
- **Reference:** [docs/arxmlgen/architecture.md](arxmlgen/architecture.md)
- **Reference:** [docs/arxmlgen/user-guide.md](arxmlgen/user-guide.md)
- **Reference:** [docs/plans/plan-arxml-to-sil-feasibility-and-failure-modes.md](plans/plan-arxml-to-sil-feasibility-and-failure-modes.md)
- **Reference:** [docs/plans/plan-arxml-to-sil-pro-workflow.md](plans/plan-arxml-to-sil-pro-workflow.md)
- **Lesson:** [docs/lessons-learned/arxmlgen.md](../lessons-learned/arxmlgen.md)

#### Exercises

**EX-19.1 — ARXML Reader**
Write a Python script using `lxml` that parses [arxml/TaktflowSystem.arxml](../../arxml/TaktflowSystem.arxml) and outputs:
- All `SwComponentType` names and their ports
- All `PortInterface` types
- All `Runnable` names and their periods
Format as a Markdown table.

**EX-19.2 — Code Generator**
Write a Python code generator (using Jinja2 or string templates) that reads a simple ARXML component description and generates:
- A `.h` file with the port interface typedefs
- A `.c` file with stub runnable implementations
Test with one component from the system ARXML.

**EX-19.3 — E2E Data ID Audit**
Read [docs/lessons-learned/arxmlgen.md](../lessons-learned/arxmlgen.md). Extract all E2E data IDs from the ARXML file. Check for duplicates across ECUs. Report: are there any conflicts? What would the symptom be at runtime?

---

### Month 20 — Advanced Embedded Architecture

#### Topics
- State machine patterns: flat FSM, hierarchical FSM, statecharts
- Memory-mapped I/O abstraction: why direct register access in application code is wrong
- Layered architecture enforcement: how to catch layer violations at compile time (`-include` guards)
- Observer pattern in embedded: callbacks, function pointers, event dispatch
- Safety patterns: redundant computation, diverse redundancy, watchdog patterns
- Power management architecture: sleep modes coordinated across BSW modules
- Bootloader architecture: dual-bank OTA, signature verification, rollback

#### Reading
- **Book:** *Making Embedded Systems* — Elecia White (Ch. 4–7)
- **Book:** *Design Patterns for Embedded Systems in C* — Bruce Powel Douglass
- **Reference:** [docs/reference/lessons-learned/safety/PROCESS-sc-safety-critical-dataflow-state-transition-investigation.md](reference/lessons-learned/safety/PROCESS-sc-safety-critical-dataflow-state-transition-investigation.md)
- **Lesson:** [docs/reference/lessons-learned/process/PROCESS-commercial-sil-vs-custom-sil-fidelity.md](reference/lessons-learned/process/PROCESS-commercial-sil-vs-custom-sil-fidelity.md)

#### Exercises

**EX-20.1 — Hierarchical State Machine**
Implement a vehicle gear selector state machine in C:
- States: Park, Reverse, Neutral, Drive, Sport
- Sub-states in Drive: D1, D2, D3, D4
- Events: gear_up, gear_down, brake_press, speed_zero
- Guard conditions: cannot shift to Reverse above 5 kph
- Generate state transition table and implement as a dispatch table (no switch chains)
Write 30 unit tests.

**EX-20.2 — Bootloader Design**
Design (document, no implementation required) a dual-bank bootloader for STM32:
- Bank A: current firmware, Bank B: new firmware
- OTA download: firmware received over CAN, written to Bank B
- Signature verification before swap
- Rollback: if Bank B fails to start 3 times, revert to Bank A
Draw the state machine. Identify all failure modes and what happens in each.

---

### Month 21 — CI/CD for Embedded and DevOps

#### Topics
- GitHub Actions / Jenkins for embedded: matrix builds, hardware-in-the-loop jobs
- Artifact management: storing firmware binaries, test reports, coverage HTML
- Release management: semantic versioning, release notes, firmware manifest
- Nightly builds: what to run every night vs every commit
- Docker registries for embedded CI: reproducible build environments
- Security in CI: secrets management, signing keys in pipelines
- Monitoring: how to know if a nightly build failed at 3am

#### Reading
- **Lesson:** [docs/reference/lessons-learned/testing/PROCESS-sil-nightly-ci.md](reference/lessons-learned/testing/PROCESS-sil-nightly-ci.md)
- **Lesson:** [docs/reference/lessons-learned/testing/PROCESS-ci-test-hardening.md](reference/lessons-learned/testing/PROCESS-ci-test-hardening.md)
- **Reference:** [docs/aspice/cm/cm-strategy.md](aspice/cm/cm-strategy.md)

#### Exercises

**EX-21.1 — Full CI Pipeline**
Write a complete GitHub Actions workflow file that:
- On every push: build (cross-compile + host), lint (Cppcheck MISRA), unit tests, coverage check (fail if <80%)
- On every PR merge to main: SIL Docker tests, generate test report artifact
- Nightly: PIL test (if runner available), traceability check
Bonus: add a badge to README showing last build status.

**EX-21.2 — Release Automation**
Write a GitHub Actions workflow that, when a tag `vX.Y.Z` is pushed:
- Builds release firmware
- Signs the binary (GPG or openssl)
- Creates a GitHub Release with the firmware, signature, and test report attached
- Generates a changelog from commit messages since last tag

---

### Q7 Mini-Project: ARXML-Driven Code Generation

**Description:** Extend the arxmlgen pipeline:
- Add a new ECU definition to the ARXML (the motor controller from Q3)
- Generate its port interface header and stub runnable from ARXML
- Integrate generated code into the Q3 firmware (replace hand-written RTE stubs)
- Verify the generated code passes all existing Q4 unit tests
- Add a CI step that regenerates code from ARXML on every push and fails if generated output differs from committed output ("ARXML drift detection")

**Assessment criteria:**
- [ ] Generated `.h` and `.c` files match hand-written equivalents
- [ ] All Q4 unit tests still pass with generated code
- [ ] ARXML drift detection CI step works (manually introduce drift, verify CI fails)
- [ ] E2E data IDs verified unique across all ECUs in ARXML

---

---

## Q8 — Capstone Project

**Goal:** Build a complete, production-quality embedded system from scratch using everything learned.

---

### Months 22–24 — Capstone: Mini Vehicle Control System

**Description:**

Build a 3-ECU vehicle subsystem — a simplified electric motor drive unit:

```
┌──────────────┐    CAN     ┌──────────────┐    CAN     ┌──────────────┐
│  BCM         │ ─────────► │  MC (Motor   │ ─────────► │  IC          │
│  Body Control│            │  Controller) │            │  Instrument  │
│              │ ◄───────── │              │ ◄───────── │  Cluster     │
└──────────────┘            └──────────────┘            └──────────────┘
   STM32G4                     TMS570 or STM32G4            STM32G4
   - Pedal sensor (ADC)         - ASIL-D target             - CAN display
   - Mode switch                - Motor drive (PWM)         - Speed/fault display
   - Heartbeat                  - Heartbeat monitoring      - UDS logger
   - UDS diagnostic             - Safe state logic
```

---

### Capstone Requirements

**Functional:**
- [ ] Pedal input → motor speed command (0–100%) via CAN
- [ ] Motor controller: closed-loop speed or open-loop PWM
- [ ] Instrument cluster: display speed, mode, fault status
- [ ] E-stop: any ECU can assert e-stop → all motors stop within 100ms
- [ ] UDS: diagnostic session on all 3 ECUs (DID read, DTC read, session control)

**Safety (ASIL-B minimum):**
- [ ] HARA completed for the system
- [ ] 3 safety goals with FTTI defined
- [ ] Heartbeat monitoring on all CAN nodes, safe state on timeout
- [ ] E-stop path independent of normal control path
- [ ] Dual-channel pedal reading or substitution value on signal loss

**Process:**
- [ ] SRS with 20+ requirements, full traceability to tests
- [ ] Architecture document with at least 3 diagrams
- [ ] ASPICE-style review records for SRS and architecture
- [ ] Risk register with 10 risks, mitigations and status

**Testing:**
- [ ] Unit tests: 100+ test cases, 80%+ branch coverage
- [ ] SIL: 8 automated scenarios (nominal + 7 fault injections)
- [ ] PIL: 100 iterations, ≥99% pass rate
- [ ] CI pipeline: all layers run automatically

**Code quality:**
- [ ] Zero MISRA-C violations (or documented deviations)
- [ ] Zero compiler warnings at `-Wall -Wextra`
- [ ] All public API functions documented

---

### Capstone Deliverables

| Deliverable | Format |
|-------------|--------|
| Firmware (3 ECUs) | Git repo, tagged release |
| SRS | Markdown document |
| Architecture Document | Markdown + diagrams (draw.io or ASCII) |
| Safety Case (abbreviated) | Markdown |
| Traceability Matrix | Spreadsheet or Markdown |
| Test Report | JUnit XML + HTML summary |
| Risk Register | Spreadsheet or Markdown |
| Lessons Learned | Min. 5 entries in the format of this project |
| CI Pipeline | GitHub Actions workflow file |
| Demo Video | 2–5 min: show system running + e-stop + UDS session |

---

### Capstone Assessment

| Area | Weight | Criteria |
|------|--------|----------|
| Functionality | 25% | Does the system work as specified? |
| Safety | 25% | Is the safety case coherent? Does e-stop actually work? |
| Testing | 20% | Does the test pyramid exist and run in CI? |
| Process | 15% | Are the documents complete and traceable? |
| Code Quality | 15% | MISRA clean, warnings clean, readable |

---

---

## Reference — Book List

| Book | Stage | Why |
|------|-------|-----|
| *Programming Embedded Systems in C and C++* — Barr | Q1 | Essential foundations |
| *The Definitive Guide to ARM Cortex-M* — Yiu | Q1 | ARM architecture reference |
| *Make: AVR Programming* — Williams | Q1 | Peripheral concepts (apply to any MCU) |
| *Automotive Embedded Systems Handbook* — Navet | Q2–Q3 | Comprehensive automotive protocols + AUTOSAR |
| *A Comprehensible Guide to Controller Area Network* — Voss | Q2 | CAN deep dive |
| *AUTOSAR Compendium Part 1* — Rech | Q3 | AUTOSAR RTE and BSW explained |
| *Real-Time Concepts for Embedded Systems* — Li | Q3 | RTOS and scheduling |
| *Test-Driven Development for Embedded C* — Grenning | Q4 | TDD methodology for C |
| *Making Embedded Systems* — White | Q7 | Architecture patterns |
| *Design Patterns for Embedded Systems in C* — Douglass | Q7 | Advanced patterns |
| *Hardware Hacker* — Huang | Q2 | PCB bring-up mindset |

---

## Reference — Online Resources

| Resource | Topic |
|----------|-------|
| [interrupt.memfault.com](https://interrupt.memfault.com) | Practical embedded engineering blog |
| [embeddedartistry.com](https://embeddedartistry.com) | Architecture, patterns, professional practice |
| [autosar.org](https://autosar.org) | Free AUTOSAR documents |
| [automotivespice.com](https://automotivespice.com) | ASPICE PAM download |
| [kvaser.com/can-protocol-tutorial](https://kvaser.com/can-protocol-tutorial/) | CAN protocol reference |
| [csselectronics.com](https://csselectronics.com) | CAN, LIN, OBD-II tutorials |
| [vector.com/know-how](https://vector.com/int/en/know-how/) | UDS, XCP, automotive protocols |
| SocketCAN documentation (`man 7 can`) | CAN on Linux |
| [docs/LEARNING-JOURNEY.md](LEARNING-JOURNEY.md) | This project's guided reading path |

---

## Timeline Summary

| Quarter | Focus | Mini-Project | Key Skill Gained |
|---------|-------|-------------|-----------------|
| Q1 | C mastery, toolchain, bare-metal | Sensor logger | Write production C, build and debug |
| Q2 | CAN bus, protocols, hardware bring-up | 2-node CAN system | CAN fluency, hardware debugging |
| Q3 | AUTOSAR architecture, BSW, OS | Mini AUTOSAR stack | AUTOSAR fundamentals |
| Q4 | Full test pyramid (unit/SIL/MIL/PIL) | Full test pyramid | Test all layers, CI integration |
| Q5 | ISO 26262 functional safety | Safety case | Read and apply ISO 26262 |
| Q6 | ASPICE, process, traceability | Process package | Work in a formal process |
| Q7 | ARXML, code generation, advanced architecture | ARXML-driven codegen | Model-based development |
| Q8 | Capstone: full system | 3-ECU vehicle subsystem | Everything applied end-to-end |
