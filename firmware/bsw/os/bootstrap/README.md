# OSEK OS Bootstrap

This folder holds the first OSEK-first OS starter as a reusable kernel lab.

Why it lives here:

- close to the future `firmware/bsw/os` integration point
- clearly separate from the active BSW build
- easy to revisit when we start the real STM32 and TMS570 ports

Contents:

- `include/Os.h` - starter OSEK-facing API
- `src/Os_Core.c` - OS lifecycle, bootstrap helpers, test-only ISR wrapper
- `src/Os_Task.c` - task services and activation logic
- `src/Os_Scheduler.c` - ready selection, nested dispatch, preemption behavior
- `src/Os_Internal.h` - shared bootstrap kernel state
- `test/test_Os_asild.c` - focused Unity tests for the starter

This folder is a pre-Phase-1 bootstrap only.

Current scope:

- static tasks
- application modes
- priority-based ready selection
- activation limits
- nested dispatch for host verification
- immediate higher-priority preemption in bootstrap form
- Cat2 ISR exit dispatch simulation in bootstrap form

Not integrated yet:

- BSW makefiles
- ECU startup
- STM32 PendSV port
- TMS570 IRQ/VIM port

Reference plan:

- `docs/plans/OSEK_OS_SPEC.md`
- `docs/plans/plan-os-threadx-bootstrap.md`
