# Lessons Learned — TMS570 Hardware Bring-up

## 2026-03-14 — ISR must use interrupt attribute for VIM dispatch

**Context:** TMS570 VIM dispatches ISRs by loading the handler address into IRQVECREG. The CPU jumps directly to the ISR via `ldr pc, [pc, #-0x1b0]` in IRQ mode. The ISR is responsible for its own prologue/epilogue.

**Mistake:** Declared `bringup_rti_compare0_isr()` as a regular `static void` function. Compiler generated `bx lr` return. In IRQ mode, this doesn't restore CPSR from SPSR and uses wrong return address (should be `lr - 4` due to ARM pipeline). CPU crashes on first IRQ return.

**Fix:** Use `void __attribute__((interrupt("IRQ")))` (tiarmclang) or `#pragma INTERRUPT(fn, IRQ)` (TI CGT). This makes the compiler generate `subs pc, lr, #4` which atomically restores CPSR from SPSR_irq and adjusts the return address.

**Principle:** On ARM Cortex-R, any function used as a VIM ISR entry point MUST have the interrupt attribute. Regular `bx lr` is never correct for IRQ/FIQ return. HALCoGen uses `#pragma INTERRUPT` — follow the same pattern.

## 2026-03-14 — systemInit() resets SCI peripheral, killing UART

**Context:** `_c_int00()` (HALCoGen startup) calls `systemInit()` before main. `sc_main.c` called `sc_sci_init()` first (to print boot messages), then `systemInit()` again (redundantly).

**Mistake:** The second `systemInit()` call resets peripheral registers including SCI, destroying the baud rate configuration. All UART output after that point was silently lost. Appeared as "UART stops after boot dump" — no garbled data, just silence.

**Fix:** Re-initialize SCI after `systemInit()` with a second `sc_sci_init()` call. Both calls are needed: first for early boot diagnostics, second to restore after peripheral reset.

**Principle:** If `systemInit()` is called after peripheral init, assume all peripherals need re-initialization. Check for silent failures — UART TX with wrong config produces no output (not garbled), making the failure invisible.
