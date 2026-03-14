/**
 * @file    Os_Port_Tms570_Bringup.c
 * @brief   Minimal hardware bring-up tests for the TMS570 bootstrap OS port
 * @date    2026-03-14
 *
 * @details Each test proves one hardware primitive in isolation, reports
 *          pass/fail over SCI UART, then restores the previous state so
 *          the main SC polled loop can continue normally.
 *
 *          Build with -DOS_BOOTSTRAP_BRINGUP to include these tests.
 *          Call Os_Port_Tms570_BringupAll() from sc_main.c after rtiInit()
 *          and rtiStartCounter() but before the main loop.
 *
 * @note    Safety level: ASIL D bring-up only — not for production
 * @standard ISO 26262 Part 6
 * @copyright Taktflow Systems 2026
 */

#ifdef OS_BOOTSTRAP_BRINGUP
#ifdef PLATFORM_TMS570

/* HALCoGen headers first — they define uint32, boolean, etc.
 * Do NOT include sc_types.h here — its boolean typedef (uint8)
 * conflicts with HALCoGen's boolean (bool). */
#include "HL_sys_vim.h"
#include "HL_reg_rti.h"

/* SCI debug output — declared here to avoid sc_hw.h/sc_types.h conflict */
extern void sc_sci_puts(const char* str);
extern void sc_sci_put_uint(uint32 val);

/* ==================================================================
 * Shared helpers
 * ================================================================== */

extern void _enable_IRQ_interrupt_(void);
extern void _disable_IRQ_interrupt_(void);

#ifndef TRUE
#define TRUE  1U
#endif
#ifndef FALSE
#define FALSE 0U
#endif

/** Busy-wait delay — approximately `cycles` loop iterations at 300 MHz.
 *  30 000 000 iterations ≈ 100ms. */
static void bringup_delay(volatile uint32 cycles)
{
    while (cycles > 0u) {
        cycles--;
    }
}

/** @brief  Print a uint32 as 8-digit hex via SCI UART */
static void bringup_put_hex(uint32 val)
{
    char buf[9];
    uint32 i;
    uint32 nibble;
    for (i = 0u; i < 8u; i++) {
        nibble = (val >> (28u - (i * 4u))) & 0xFu;
        buf[i] = (nibble < 10u) ? (char)('0' + nibble) : (char)('A' + nibble - 10u);
    }
    buf[8] = '\0';
    sc_sci_puts(buf);
}

/* ==================================================================
 * Test 1: Prove RTI compare0 fires as IRQ via VIM channel 2
 * ================================================================== */

/** @brief  Counter incremented by our test ISR */
static volatile uint32 bringup_rti_irq_count = 0u;

/**
 * @brief  Minimal RTI compare0 ISR — increment counter, acknowledge
 *
 * @note   This is NOT the bootstrap OS tick handler. It is a standalone
 *         proof-of-concept ISR used only during bring-up.
 */
void __attribute__((interrupt("IRQ"))) bringup_rti_compare0_isr(void)
{
    bringup_rti_irq_count++;
    rtiREG1->INTFLAG = (uint32)1u;  /* W1C: acknowledge compare0 */
}

/**
 * @brief  Prove RTI compare0 fires as IRQ through VIM channel 2
 *
 * Preconditions (met by sc_main.c before calling):
 *   - vimInit() called by _c_int00
 *   - rtiInit() called — RTI configured for 10ms tick, interrupts disabled
 *   - rtiStartCounter() called — counter block 0 running
 *
 * Test sequence:
 *   1. Map RTI compare0 (request 2) to VIM channel 2 with our test ISR
 *   2. Enable VIM channel 2 as IRQ
 *   3. Enable RTI compare0 interrupt generation (SETINTENA bit 0)
 *   4. Enable CPU IRQs (CPSR I-bit clear)
 *   5. Wait ~200ms (expect ~20 ticks at 10ms period)
 *   6. Report count over SCI UART
 *   7. Restore: disable compare0 interrupt, disable VIM channel 2,
 *      clear pending flag, disable CPU IRQs
 *
 * @return TRUE if at least one IRQ was received, FALSE otherwise
 */
static boolean bringup_test_rti_compare0_irq(void)
{
    uint32 startCount;
    uint32 endCount;
    boolean pass;

    sc_sci_puts("[BRINGUP-1] RTI compare0 IRQ via VIM ch2...\r\n");

    /* Reset counter */
    bringup_rti_irq_count = 0u;

    /* 1. Map RTI compare0 (request 2) → VIM channel 2 with our ISR */
    vimChannelMap(2u, 2u, (t_isrFuncPTR)&bringup_rti_compare0_isr);

    /* 2. Enable VIM channel 2 as IRQ */
    vimEnableInterrupt(2u, SYS_IRQ);

    /* 3. Enable RTI compare0 interrupt generation */
    rtiREG1->SETINTENA = (uint32)1u;

    /* 4. Enable CPU IRQs */
    startCount = bringup_rti_irq_count;
    _enable_IRQ_interrupt_();

    /* 5. Wait ~200ms — expect ~20 interrupts at 10ms period */
    bringup_delay(60000000u);

    /* 6. Disable CPU IRQs before touching VIM/RTI state */
    _disable_IRQ_interrupt_();

    endCount = bringup_rti_irq_count;

    /* 7. Restore polled mode */
    rtiREG1->CLEARINTENA = (uint32)1u;    /* Disable compare0 interrupt */
    vimDisableInterrupt(2u);               /* Disable VIM channel 2 */
    rtiREG1->INTFLAG = (uint32)1u;         /* Clear any pending flag */

    /* Report */
    sc_sci_puts("[BRINGUP-1] IRQ count: ");
    sc_sci_put_uint(endCount - startCount);
    sc_sci_puts(" (expect ~20)\r\n");

    pass = ((endCount - startCount) > 0u) ? TRUE : FALSE;
    sc_sci_puts(pass ? "[BRINGUP-1] PASS\r\n" : "[BRINGUP-1] FAIL\r\n");

    return pass;
}

/* ==================================================================
 * Test 3: Prove same-task IRQ return preserves callee-saved registers
 *
 * Called from inside the launched task (test 2). Loads known sentinel
 * values into R4-R11, enables RTI compare0 IRQ, busy-waits while IRQs
 * fire, then verifies all registers are unchanged after ISR returns.
 *
 * Proves:
 *   - IRQ fires while a launched task is running
 *   - __attribute__((interrupt("IRQ"))) ISR correctly saves/restores
 *     CPSR (via SPSR_irq) and scratch registers (R0-R3, R12, LR)
 *   - Callee-saved registers (R4-R11) survive the ISR round-trip
 *   - Task SP is unchanged after IRQ return
 * ================================================================== */

/** @brief  Sentinel values for R4-R11 register preservation check */
static const uint32 bringup_reg_sentinels[8] = {
    0xDEAD0004u, 0xDEAD0005u, 0xDEAD0006u, 0xDEAD0007u,
    0xDEAD0008u, 0xDEAD0009u, 0xDEAD000Au, 0xDEAD000Bu
};

/**
 * @brief  Prove IRQ return preserves callee-saved registers R4-R11
 *
 * @return TRUE if all registers preserved and at least one IRQ fired
 *
 * @note   Must be called from the launched task (System mode, IRQs disabled).
 *         Reuses bringup_rti_compare0_isr from test 1.
 */
static boolean bringup_test_same_task_irq_return(void)
{
    uint32 after[8];
    uint32 spBefore;
    uint32 spAfter;
    uint32 irqsBefore;
    uint32 irqsAfter;
    boolean pass;
    uint32 i;

    sc_sci_puts("[BRINGUP-3] Same-task IRQ return + register preservation...\r\n");

    /* Setup VIM/RTI — same ISR as test 1 */
    bringup_rti_irq_count = 0u;
    vimChannelMap(2u, 2u, (t_isrFuncPTR)&bringup_rti_compare0_isr);
    vimEnableInterrupt(2u, SYS_IRQ);
    rtiREG1->SETINTENA = (uint32)1u;

    irqsBefore = bringup_rti_irq_count;

    /* Load sentinels into R4-R11, capture SP, enable IRQ, busy-wait
     * while IRQs fire, disable IRQ, read R4-R11 and SP back.
     *
     * All in one asm block so the compiler cannot use R4-R11 for
     * other purposes during the test window. */
    __asm__ volatile(
        /* Capture SP before */
        "MOV    %[spb], sp          \n\t"
        /* Load sentinel values into R4-R11 */
        "LDR    r4, [%[sen], #0]    \n\t"
        "LDR    r5, [%[sen], #4]    \n\t"
        "LDR    r6, [%[sen], #8]    \n\t"
        "LDR    r7, [%[sen], #12]   \n\t"
        "LDR    r8, [%[sen], #16]   \n\t"
        "LDR    r9, [%[sen], #20]   \n\t"
        "LDR    r10, [%[sen], #24]  \n\t"
        "LDR    r11, [%[sen], #28]  \n\t"
        /* Enable IRQ (clear I bit) */
        "CPSIE  i                   \n\t"
        /* Busy-wait ~620ms at 300 MHz — expect ~62 IRQs at 10ms */
        "MOV    r0, %[cnt]          \n\t"
        "1:                         \n\t"
        "SUBS   r0, r0, #1          \n\t"
        "BNE    1b                  \n\t"
        /* Disable IRQ */
        "CPSID  i                   \n\t"
        /* Store R4-R11 to after[] */
        "STR    r4, [%[aft], #0]    \n\t"
        "STR    r5, [%[aft], #4]    \n\t"
        "STR    r6, [%[aft], #8]    \n\t"
        "STR    r7, [%[aft], #12]   \n\t"
        "STR    r8, [%[aft], #16]   \n\t"
        "STR    r9, [%[aft], #20]   \n\t"
        "STR    r10, [%[aft], #24]  \n\t"
        "STR    r11, [%[aft], #28]  \n\t"
        /* Capture SP after */
        "MOV    %[spa], sp          \n\t"
        : [spb] "=&r" (spBefore), [spa] "=&r" (spAfter)
        : [sen] "r" (bringup_reg_sentinels), [aft] "r" (after),
          [cnt] "r" ((uint32)0x00800000u)
        : "r0", "r4", "r5", "r6", "r7", "r8", "r9", "r10", "r11",
          "memory"
    );

    irqsAfter = bringup_rti_irq_count;

    /* Restore polled mode */
    rtiREG1->CLEARINTENA = (uint32)1u;
    vimDisableInterrupt(2u);
    rtiREG1->INTFLAG = (uint32)1u;

    /* Verify register preservation */
    pass = TRUE;
    for (i = 0u; i < 8u; i++) {
        if (bringup_reg_sentinels[i] != after[i]) {
            pass = FALSE;
            sc_sci_puts("[BRINGUP-3] R");
            sc_sci_put_uint(i + 4u);
            sc_sci_puts(" MISMATCH: 0x");
            bringup_put_hex(bringup_reg_sentinels[i]);
            sc_sci_puts(" -> 0x");
            bringup_put_hex(after[i]);
            sc_sci_puts("\r\n");
        }
    }

    /* Verify SP preservation */
    if (spBefore != spAfter) {
        pass = FALSE;
        sc_sci_puts("[BRINGUP-3] SP MISMATCH: 0x");
        bringup_put_hex(spBefore);
        sc_sci_puts(" -> 0x");
        bringup_put_hex(spAfter);
        sc_sci_puts("\r\n");
    }

    /* Report */
    sc_sci_puts("[BRINGUP-3] IRQs during test: ");
    sc_sci_put_uint(irqsAfter - irqsBefore);
    sc_sci_puts("\r\n");

    if ((irqsAfter - irqsBefore) == 0u) {
        sc_sci_puts("[BRINGUP-3] FAIL - no IRQs fired\r\n");
        return FALSE;
    }

    sc_sci_puts(pass ? "[BRINGUP-3] PASS\r\n" : "[BRINGUP-3] FAIL\r\n");
    return pass;
}

/* ==================================================================
 * Test 2: Prove first-task context launch via direct MSR+BX
 *
 * Builds a synthetic initial frame on a dedicated stack, then launches
 * the task using MSR CPSR_cxsf + BX (not exception return). Proves:
 *   - Initial frame layout is correct
 *   - Direct mode switch via MSR works from System mode
 *   - CPU lands at the task entry function in System mode (0x1F)
 *   - Task can execute C code and call SCI UART functions
 *
 * This test is a ONE-WAY TRIP — it never returns to BringupAll.
 * The task entry also runs test 3 (IRQ return) before entering
 * the LED blink loop.
 * ================================================================== */

/** @brief  512-byte stack for the bring-up test task */
static uint8 bringup_task_stack[512u] __attribute__((aligned(8)));

/* LED control — declared here to avoid sc_hw.h/sc_types.h conflict */
extern void sc_het_led_on(void);
extern void sc_het_led_off(void);

/** @brief  Initial frame: 2 header words + 15 register words = 17 */
#define BRINGUP_FRAME_WORDS  17u
#define BRINGUP_FRAME_BYTES  (BRINGUP_FRAME_WORDS * 4u)

/** @brief  Target CPSR mode bits for the task (System mode = 0x1F).
 *
 * HALCoGen's _c_int00 switches to System mode before calling main(),
 * so tasks should also run in System mode. The full CPSR value is
 * computed at runtime by reading the current CPSR and replacing the
 * low 8 bits (mode + I/F/T flags) with the target mode. This
 * preserves the E bit (big-endian) and condition flags. */
#define BRINGUP_TARGET_MODE  0x1Fu

/**
 * @brief  Entry point for the first-task bring-up test
 *
 * Verifies CPU mode via MRS CPSR, then runs test 3 (same-task IRQ
 * return with register preservation). Reports combined pass/fail,
 * then enters a polled RTI LED blink loop to prove the task is alive.
 * This function never returns.
 *
 * @note   Entered via direct MSR+BX from bringup_launch_task().
 *         R0-R12 and LR are zeroed, SP points past the consumed frame.
 */
__attribute__((used))
static void bringup_first_task_entry(void)
{
    uint32 cpsrVal;
    boolean allPass;
    boolean test3Pass;
    uint32 blink = 0u;

    __asm__ volatile("MRS %0, CPSR" : "=r"(cpsrVal));

    sc_sci_puts("[BRINGUP-2] First task entry reached!\r\n");
    sc_sci_puts("[BRINGUP-2] CPSR = 0x");
    bringup_put_hex(cpsrVal);
    sc_sci_puts(" (mode = 0x");
    bringup_put_hex(cpsrVal & 0x1Fu);
    sc_sci_puts(")\r\n");

    /* Expect System mode (0x1F) — HALCoGen runs main() in System mode */
    allPass = ((cpsrVal & 0x1Fu) == BRINGUP_TARGET_MODE) ? TRUE : FALSE;
    sc_sci_puts(allPass ? "[BRINGUP-2] PASS\r\n" : "[BRINGUP-2] FAIL\r\n");

    /* Test 3: same-task IRQ return with register preservation */
    test3Pass = bringup_test_same_task_irq_return();
    if (test3Pass == FALSE) {
        allPass = FALSE;
    }

    sc_sci_puts("=== Bring-up ");
    sc_sci_puts(allPass ? "ALL PASS" : "SOME FAILED");
    sc_sci_puts(" ===\r\n\r\n");

    /* Stay alive: polled RTI LED blink (500ms on / 500ms off) */
    for (;;) {
        if ((rtiREG1->INTFLAG & 1u) != 0u) {
            rtiREG1->INTFLAG = 1u;
            blink++;
            if (blink < 50u) {
                sc_het_led_on();
            } else if (blink < 100u) {
                sc_het_led_off();
            } else {
                blink = 0u;
            }
        }
    }
}

/**
 * @brief  Launch a task by setting CPSR, SP, zeroing regs, and branching
 *
 * @param  frameSp  Base address of the initial frame (StackType at offset 0)
 * @param  cpsr     Target CPSR value (must preserve I/F bits to avoid
 *                  spurious interrupts during launch)
 *
 * @note   AAPCS: frameSp in R0, cpsr in R1.
 * @note   This function never returns.
 *
 * Uses direct MSR CPSR_cxsf + BX instead of exception return (LDMIA ^).
 * Exception return is not usable here because HALCoGen runs main() in
 * System mode, which has no SPSR — LDMIA ^ from System mode is
 * UNPREDICTABLE per ARM ARM.
 *
 * Critical: the target CPSR must keep I=1, F=1 (interrupts disabled)
 * during the launch sequence. Mask only mode bits (0x1F), not the full
 * low byte (0xFF), when computing the target from current CPSR.
 */
__attribute__((naked, noreturn))
static void bringup_launch_task(uint32 frameSp, uint32 cpsr)
{
    __asm__ volatile(
        /* R0 = frameSp, R1 = target CPSR (AAPCS) */

        /* 1. Compute post-frame SP and load entry address */
        "ADD    r2, r0, #68         \n\t"   /* R2 = post-frame task SP */
        "LDR    r3, [r0, #64]       \n\t"   /* R3 = PC from frame[16] */

        /* 2. Switch to target mode directly via MSR CPSR */
        "MSR    CPSR_cxsf, r1      \n\t"   /* CPSR = target (System mode) */

        /* 3. Now in System mode — set SP */
        "MOV    sp, r2              \n\t"   /* SP_sys = post-frame SP */

        /* 4. Zero all registers to match frame (R0-R12, LR = 0) */
        "MOV    r0, #0              \n\t"
        "MOV    r1, #0              \n\t"
        "MOV    r2, #0              \n\t"
        "MOV    r4, #0              \n\t"
        "MOV    r5, #0              \n\t"
        "MOV    r6, #0              \n\t"
        "MOV    r7, #0              \n\t"
        "MOV    r8, #0              \n\t"
        "MOV    r9, #0              \n\t"
        "MOV    r10, #0             \n\t"
        "MOV    r11, #0             \n\t"
        "MOV    r12, #0             \n\t"
        "MOV    lr, #0              \n\t"

        /* 5. Jump to task entry */
        "BX     r3                  \n\t"
    );
}

/**
 * @brief  Build initial frame and launch first task via direct MSR+BX
 *
 * Frame layout (17 x uint32 = 68 bytes):
 *   [0]  StackType = 1 (interrupt frame)
 *   [1]  CPSR = 0x13 (SVC mode, IRQ+FIQ enabled)
 *   [2]  R0 = 0
 *   ...
 *   [14] R12 = 0
 *   [15] LR = 0
 *   [16] PC = bringup_first_task_entry
 *
 * After LDMIA: SP = frameSp + 68, pointing past the consumed frame.
 * The task uses the remaining stack space below SP (growing downward).
 *
 * @note   This function never returns — one-way trip to the task.
 */
static void bringup_test_first_task_launch(void)
{
    uintptr_t stackTop;
    uintptr_t frameSp;
    uint32* frame;
    uint32 i;
    uint32 currentCpsr;
    uint32 targetCpsr;

    sc_sci_puts("[BRINGUP-2] First-task launch via direct MSR+BX...\r\n");

    /* Read current CPSR to preserve E (big-endian), I/F, and condition flags.
     * Replace only mode bits [4:0] with target mode. Do NOT mask the full
     * low byte — that would clear I/F bits, enabling interrupts and causing
     * a spurious FIQ during the launch sequence. */
    __asm__ volatile("MRS %0, CPSR" : "=r"(currentCpsr));
    targetCpsr = (currentCpsr & ~(uint32)0x1Fu) | BRINGUP_TARGET_MODE;

    sc_sci_puts("[BRINGUP-2] Current CPSR = 0x");
    bringup_put_hex(currentCpsr);
    sc_sci_puts(", target = 0x");
    bringup_put_hex(targetCpsr);
    sc_sci_puts("\r\n");

    /* Allocate frame at top of stack, 8-byte aligned */
    stackTop = (uintptr_t)&bringup_task_stack[sizeof(bringup_task_stack)];
    frameSp = (stackTop - BRINGUP_FRAME_BYTES) & ~(uintptr_t)7u;
    frame = (uint32*)frameSp;

    /* Zero entire frame */
    for (i = 0u; i < BRINGUP_FRAME_WORDS; i++) {
        frame[i] = 0u;
    }

    /* Fill header and entry point */
    frame[0] = 1u;                          /* StackType = interrupt frame */
    frame[1] = targetCpsr;                  /* CPSR for exception return */
    frame[16] = (uint32)(uintptr_t)&bringup_first_task_entry;  /* PC */

    sc_sci_puts("[BRINGUP-2] Frame at 0x");
    bringup_put_hex((uint32)frameSp);
    sc_sci_puts(", entry at 0x");
    bringup_put_hex(frame[16]);
    sc_sci_puts("\r\n");

    /* One-way trip — launch the task */
    bringup_launch_task((uint32)frameSp, targetCpsr);
}

/* ==================================================================
 * Public entry point
 * ================================================================== */

/**
 * @brief  Run all bring-up tests and report summary
 *
 * Call from sc_main.c after rtiStartCounter() and before the main loop.
 *
 * @note   Test 2 (first-task launch) is a one-way trip — this function
 *         never returns. The task entry function takes over as the
 *         new main loop with LED blink.
 */
void Os_Port_Tms570_BringupAll(void)
{
    sc_sci_puts("\r\n=== OS Bootstrap Bring-up Tests ===\r\n");

    if (bringup_test_rti_compare0_irq() == FALSE) {
        sc_sci_puts("=== Bring-up SOME FAILED ===\r\n\r\n");
        return;  /* Don't attempt further tests if IRQ is broken */
    }

    /* Test 2 is a one-way trip — never returns.
     * The task entry prints the final summary. */
    bringup_test_first_task_launch();

    /* Future bring-up tests (added inside the task or as separate steps):
     *   bringup_test_same_task_irq_return()
     *   bringup_test_two_task_switch()
     *   bringup_test_irq_preemption()
     *   bringup_test_fiq_ownership()
     */
}

#endif /* PLATFORM_TMS570 */
#endif /* OS_BOOTSTRAP_BRINGUP */
