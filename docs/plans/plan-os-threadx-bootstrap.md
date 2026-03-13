# Plan: OSEK Bootstrap with ThreadX Learning Map

**Status:** IN PROGRESS
**Created:** 2026-03-13
**Author:** Codex + andao

## Goal

Support the primary OSEK roadmap in `docs/plans/OSEK_OS_SPEC.md` by keeping a
small, static, host-testable kernel bootstrap in `firmware/bsw/os/bootstrap/`,
while using ThreadX as a reference for kernel structure and target-port
bring-up on STM32 and TMS570.

This is not a plan to clone ThreadX APIs. It is a plan to:

1. Learn kernel structure from ThreadX internals.
2. Keep the OSEK/AUTOSAR service model on our side.
3. Separate kernel policy from target-port mechanics early.

## Relationship to the Main Spec

`docs/plans/OSEK_OS_SPEC.md` is the authoritative roadmap.

This document is subordinate to that spec and exists only to answer:

- which ThreadX internals are worth studying
- which target-port mechanics can be learned from ThreadX
- how to use the bootstrap folder without confusing it with the real kernel

## Why ThreadX Helps

ThreadX is a good acceleration tool for this effort because it shows three things very clearly:

1. A compact core kernel with clear ownership boundaries.
2. A disciplined separation between portable scheduler policy and CPU-specific context-switch code.
3. Mature Cortex-M4 and Cortex-R5 ports that map well to our two targets.

For this work we will study ThreadX internals, not reuse its public API shape.

## Target Mapping

| Our target | MCU family | CPU | ThreadX reference area |
|---|---|---|---|
| Zone ECUs | STM32 | Cortex-M4 | `ports/cortex_m4/*` |
| Safety controller | TMS570 | Cortex-R5 | `ports/cortex_r5/*` |
| Host learning build | POSIX | x86/x64 | Portable core only |

## ThreadX Files Worth Studying First

### Portable Kernel Core

| Topic | ThreadX file | Why it matters |
|---|---|---|
| Kernel bootstrap | `common/src/tx_initialize_kernel_enter.c` | Shows the handoff from reset/application entry into kernel initialization and scheduler start. |
| Thread create path | `common/src/tx_thread_create.c` | Good reference for control block initialization, stack preparation, and activation logic. |
| Thread internals | `common/inc/tx_thread.h` | Shows what a mature TCB needs beyond the public API. |
| Global API surface | `common/inc/tx_api.h` | Useful as a completeness checklist, not as an AUTOSAR API template. |

### STM32 / Cortex-M4 Port

| Topic | ThreadX file | Lesson for our OS |
|---|---|---|
| Port constants and type model | `ports/cortex_m4/gnu/inc/tx_port.h` | What belongs in the CPU port boundary versus the portable kernel. |
| First schedule + PendSV | `ports/cortex_m4/gnu/src/tx_thread_schedule.S` | PendSV-driven context switch path, interrupt enable timing, PSP/MSP split. |
| Initial low-level bring-up | `ports/cortex_m4/gnu/example_build/tx_initialize_low_level.S` | Startup timer hook-in, vector/table setup, first free RAM handoff. |
| Initial stack frame | `ports/cortex_m4/gnu/src/tx_thread_stack_build.S` | How a task starts for the first time on ARMv7-M. |

### TMS570 / Cortex-R5 Port

| Topic | ThreadX file | Lesson for our OS |
|---|---|---|
| Port constants and interrupt model | `ports/cortex_r5/gnu/inc/tx_port.h` | IRQ/FIQ mask model, timer thread defaults, architectural assumptions. |
| Scheduler loop | `ports/cortex_r5/gnu/src/tx_thread_schedule.S` | How the first runnable thread is selected and transferred on ARM-R. |
| Context save / restore | `ports/cortex_r5/gnu/src/tx_thread_context_save.S` and `tx_thread_context_restore.S` | Banked registers, mode switching, and return path details we must respect on TMS570. |
| Low-level init | `ports/cortex_r5/gnu/example_build/tx_initialize_low_level.S` | Reset-to-kernel transition and timer/vector setup ideas. |

## Architectural Direction for Our OS

### Keep OSEK/AUTOSAR Semantics

Portable kernel services should follow OSEK first, then AUTOSAR OS extensions:

- `StartOS()`
- `ActivateTask()`
- `TerminateTask()`
- `ChainTask()`
- `Schedule()`
- `GetTaskID()`
- `GetTaskState()`
- later: resources, events, alarms, schedule tables, ISR2

### Learn Internal Structure from ThreadX

Portable kernel structure should learn from ThreadX patterns:

- explicit task control blocks
- explicit ready-state bookkeeping
- strict split between portable core and port layer
- first-class scheduler entry instead of ad-hoc main-loop logic
- architecture-owned context switch code in assembly, not hidden in generic C

## Incremental Delivery Plan

| Phase | Outcome | Status |
|---|---|---|
| 0 | Host-testable pre-Phase-1 bootstrap in `firmware/bsw/os/bootstrap` | IN PROGRESS |
| 1 | OSEK BCC1 core per `OSEK_OS_SPEC.md` | PENDING |
| 2 | STM32 Cortex-M4 port boundary and PendSV design | PENDING |
| 3 | TMS570 Cortex-R5 port boundary and IRQ/VIM design | PENDING |
| 4 | OSEK full plus AUTOSAR resources/events/extended services | PENDING |
| 5 | Counters, alarms, schedule tables, ISR2, protection work | PENDING |

## Bootstrap Scope

This bootstrap is intentionally small:

- static task table
- application modes
- priority-based ready selection
- activation limits
- basic task state tracking
- host-runnable unit tests

This bootstrap will not attempt:

- real context switching
- stack switching
- ISR2 nesting
- resource ceiling protocol
- alarms or schedule tables

The bootstrap uses cooperative task completion so we can verify scheduler
policy before starting the real OSEK BCC1 target-port work.

## CPU Port Strategy

### STM32 / Cortex-M4

Study ThreadX Cortex-M4 for:

- PendSV as the only task switch point
- SysTick or GPT as the tick source
- PSP for threads, MSP for exceptions
- exception-frame-based first task launch
- minimal interrupt disabled window around ready-list updates

Planned OS port files:

- `firmware/bsw/os/include/Os_Port.h`
- `firmware/platform/stm32/src/Os_Port_Stm32.c`
- `firmware/platform/stm32/src/Os_Port_Stm32_Asm.S`

### TMS570 / Cortex-R5

Study ThreadX Cortex-R5 for:

- mode-aware context save/restore
- IRQ vs FIQ handling choices
- vector/VIM integration
- RTI-based system tick
- banked register preservation rules

Planned OS port files:

- `firmware/bsw/os/include/Os_Port.h`
- `firmware/platform/tms570/src/Os_Port_Tms570.c`
- `firmware/platform/tms570/src/Os_Port_Tms570_Asm.S`

## Kernel/Data Model to Converge Toward

| Area | Starter design | Future design |
|---|---|---|
| Task config | Static array | Generated AUTOSAR-like config |
| Ready selection | Portable bitmap + stamps | Bitmap + per-priority queue |
| Dispatch | Cooperative host execution | Real port-driven context switch |
| Task type | Basic tasks first | Basic + extended tasks |
| Timing | None | counter/alarm/schedule table integration |

## Guardrails

1. Do not copy ThreadX service names into our AUTOSAR-facing API.
2. Do not let portable kernel code depend on STM32 or TMS570 headers.
3. Do not implement context switching in generic C if the final target needs assembly.
4. Do not block Phase 1 on perfect AUTOSAR completeness.

## Done for This Bootstrap Turn

1. Add a first `Os` starter in `firmware/bsw/os/bootstrap`.
2. Keep it host-testable with Unity.
3. Keep the work clearly below the main OSEK spec, not mistaken for a live kernel.
4. Capture the learning path from ThreadX for later STM32 and TMS570 port work.
