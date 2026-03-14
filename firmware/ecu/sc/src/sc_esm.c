/**
 * @file    sc_esm.c
 * @brief   ESM (Error Signaling Module) lockstep handler for Safety Controller
 * @date    2026-02-23
 *
 * @safety_req SWR-SC-014, SWR-SC-015
 * @traces_to  SSR-SC-014, SSR-SC-015
 * @note    Safety level: ASIL D
 * @standard ISO 26262 Part 6
 * @copyright Taktflow Systems 2026
 */
#include "sc_esm.h"
#include "Sc_Hw_Cfg.h"
#include "sc_gio.h"

/* ==================================================================
 * External: ESM hardware register access (HALCoGen-style)
 * ================================================================== */

extern void    esm_enable_group1_channel(uint8 channel);
extern void    esm_clear_flag(uint8 group, uint8 channel);
extern boolean esm_is_flag_set(uint8 group, uint8 channel);
extern void    esm_set_runtime_mode(void);

/* ==================================================================
 * Constants
 * ================================================================== */

#define ESM_GROUP1              1u
#define ESM_CHANNEL_LOCKSTEP    2u     /* CCM-R5F lockstep compare error */
#define ESM_CHANNEL_DCC1       21u    /* DCC1 clock comparator error */

/* ==================================================================
 * Module State
 * ================================================================== */

/** ESM error active flag (latched) */
static boolean esm_error_active;

/* ==================================================================
 * Public API
 * ================================================================== */

void SC_ESM_Init(void)
{
    esm_error_active = FALSE;

    /* Clear residual ESM Group 1 flags left over from startup.
     * Channel 2 (lockstep) may be flagged after JTAG debug reset.
     * Channel 21 (DCC1) is a nuisance flag from HALCoGen clock check. */
    esm_clear_flag(ESM_GROUP1, ESM_CHANNEL_LOCKSTEP);
    esm_clear_flag(ESM_GROUP1, ESM_CHANNEL_DCC1);

    /* Verify lockstep channel is clean before enabling.
     * If the CCM-R5F error is persistent (not cleared by write-1-to-clear),
     * enabling EEPAPR1 would assert nERROR → Group 3 ISR → halt.
     * In that case, latch error but do NOT enable — let the watchdog
     * feed continue (startup BIST already passed). */
    if (esm_is_flag_set(ESM_GROUP1, ESM_CHANNEL_LOCKSTEP) != FALSE) {
        esm_error_active = TRUE;
        return;
    }

    /* Enable ESM Group 1 Channel 2 for nERROR pin assertion.
     * If lockstep error occurs at runtime:
     *   Channel 2 flag → EEPAPR1[2] → nERROR low → ESM Group 3
     *   → esmGroup3Notification() → SC_ESM_HighLevelInterrupt()
     *   → relay off + halt → watchdog reset */
    esm_enable_group1_channel(ESM_CHANNEL_LOCKSTEP);

    /* Signal to esmGroup3Notification that startup is complete.
     * From this point, Group 3 errors trigger safety response
     * instead of clear-and-continue. */
    esm_set_runtime_mode();
}

void SC_ESM_HighLevelInterrupt(void)
{
    /* CRITICAL: This ISR must execute in < 100 clock cycles.
     * Immediately de-energize relay, set system LED, latch error. */

    /* De-energize relay (direct GIO write — no function call overhead) */
    gioSetBit(SC_GIO_PORT_A, SC_PIN_RELAY, 0u);

    /* System fault LED ON */
    gioSetBit(SC_GIO_PORT_A, SC_PIN_LED_SYS, 1u);

    /* Latch error */
    esm_error_active = TRUE;

    /* Clear ESM flag to acknowledge */
    esm_clear_flag(ESM_GROUP1, ESM_CHANNEL_LOCKSTEP);

    /* Enter infinite loop — TPS3823 watchdog will reset the MCU
     * because we are no longer feeding WDI from the main loop */
    for (;;) {
        /* Intentional infinite loop — watchdog reset expected */
    }
}

boolean SC_ESM_IsErrorActive(void)
{
    return esm_error_active;
}
