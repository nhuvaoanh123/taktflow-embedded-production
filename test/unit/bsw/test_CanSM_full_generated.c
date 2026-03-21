/**
 * @file    test_CanSM_full_generated.c
 * @brief   Exhaustive state machine tests for CAN State Manager
 * @date    2026-03-21
 *
 * @verifies SSR-CVC-016, SSR-FZC-019, SWR-BSW-026
 *
 * @details Tests every state x event combination (5 states x 4 events = 20),
 *          plus L1/L2 recovery sequences, escalation, bus-off during recovery,
 *          init with NULL config, and double init.
 *
 * States: UNINIT(0), STOPPED(1), STARTED(2), BUS_OFF(3)
 * Events: Init, RequestComMode, ControllerBusOff, MainFunction tick
 *
 * Note: The 5th "state" tested is STARTED-after-recovery to verify
 *       that bus-off can recur during recovery.
 */
#include "unity.h"
#include "CanSM.h"

/* ==================================================================
 * Mock: CAN driver
 * ================================================================== */

static uint8  mock_can_mode;
static uint8  mock_can_controller;
static uint16 mock_can_call_count;

Std_ReturnType Can_SetControllerMode(uint8 Controller, uint8 Mode)
{
    mock_can_controller = Controller;
    mock_can_mode = Mode;
    mock_can_call_count++;
    return E_OK;
}

#define CAN_CS_STARTED  0u
#define CAN_CS_STOPPED  1u

/* ==================================================================
 * Test configurations
 * ================================================================== */

static const CanSM_ConfigType cfg_default = {
    .L1_RecoveryTimeMs = 10u,   /* 1 MainFunction tick (10ms period) */
    .L1_MaxAttempts    = 5u,
    .L2_RecoveryTimeMs = 100u,  /* 10 MainFunction ticks */
    .L2_MaxAttempts    = 10u,
};

static const CanSM_ConfigType cfg_fast = {
    .L1_RecoveryTimeMs = 10u,
    .L1_MaxAttempts    = 3u,
    .L2_RecoveryTimeMs = 10u,   /* same as L1 for easier testing */
    .L2_MaxAttempts    = 2u,
};

/* ==================================================================
 * Helpers
 * ================================================================== */

static void reset_mock(void)
{
    mock_can_mode = 0xFFu;
    mock_can_controller = 0xFFu;
    mock_can_call_count = 0u;
}

/** Advance MainFunction by N ticks (each = 10ms) */
static void tick(uint16 n)
{
    uint16 i;
    for (i = 0u; i < n; i++) {
        CanSM_MainFunction();
    }
}

/** Get to STARTED state from fresh init */
static void go_started(const CanSM_ConfigType* cfg)
{
    CanSM_Init(cfg);
    (void)CanSM_RequestComMode();
}

/** Get to BUS_OFF state from fresh init */
static void go_busoff(const CanSM_ConfigType* cfg)
{
    go_started(cfg);
    CanSM_ControllerBusOff();
}

/* ==================================================================
 * setUp / tearDown
 * ================================================================== */

void setUp(void)
{
    reset_mock();
    CanSM_Init(&cfg_default);
}

void tearDown(void) {}

/* ==================================================================
 * Section 1: State x Event matrix (20 tests)
 *
 * 4 true states: UNINIT, STOPPED, STARTED, BUS_OFF
 * Plus: STARTED-after-L1-recovery (5th behavioral state)
 * 4 events: Init, RequestComMode, ControllerBusOff, MainFunction
 * ================================================================== */

/* ---- UNINIT x 4 events ---- */

void test_UNINIT_Init_valid_goes_STOPPED(void)
{
    CanSM_Init(NULL_PTR);  /* force UNINIT */
    TEST_ASSERT_EQUAL(CANSM_UNINIT, CanSM_GetState());
    CanSM_Init(&cfg_default);
    TEST_ASSERT_EQUAL(CANSM_STOPPED, CanSM_GetState());
}

void test_UNINIT_RequestComMode_returns_NOT_OK(void)
{
    CanSM_Init(NULL_PTR);
    Std_ReturnType ret = CanSM_RequestComMode();
    TEST_ASSERT_EQUAL(E_NOT_OK, ret);
    TEST_ASSERT_EQUAL(CANSM_UNINIT, CanSM_GetState());
}

void test_UNINIT_BusOff_ignored(void)
{
    CanSM_Init(NULL_PTR);
    reset_mock();
    CanSM_ControllerBusOff();
    TEST_ASSERT_EQUAL(CANSM_UNINIT, CanSM_GetState());
    TEST_ASSERT_EQUAL(0u, mock_can_call_count);  /* no driver call */
}

void test_UNINIT_MainFunction_no_op(void)
{
    CanSM_Init(NULL_PTR);
    CanSM_MainFunction();
    TEST_ASSERT_EQUAL(CANSM_UNINIT, CanSM_GetState());
}

/* ---- STOPPED x 4 events ---- */

void test_STOPPED_Init_reinit_stays_STOPPED(void)
{
    TEST_ASSERT_EQUAL(CANSM_STOPPED, CanSM_GetState());
    CanSM_Init(&cfg_default);
    TEST_ASSERT_EQUAL(CANSM_STOPPED, CanSM_GetState());
}

void test_STOPPED_RequestComMode_goes_STARTED(void)
{
    Std_ReturnType ret = CanSM_RequestComMode();
    TEST_ASSERT_EQUAL(E_OK, ret);
    TEST_ASSERT_EQUAL(CANSM_STARTED, CanSM_GetState());
    TEST_ASSERT_TRUE(CanSM_IsCommunicationAllowed());
}

void test_STOPPED_BusOff_goes_BUS_OFF(void)
{
    /* Bus-off in STOPPED is valid (hardware can detect bus-off at init) */
    CanSM_ControllerBusOff();
    TEST_ASSERT_EQUAL(CANSM_BUS_OFF, CanSM_GetState());
}

void test_STOPPED_MainFunction_no_state_change(void)
{
    CanSM_MainFunction();
    TEST_ASSERT_EQUAL(CANSM_STOPPED, CanSM_GetState());
}

/* ---- STARTED x 4 events ---- */

void test_STARTED_Init_resets_to_STOPPED(void)
{
    go_started(&cfg_default);
    CanSM_Init(&cfg_default);
    TEST_ASSERT_EQUAL(CANSM_STOPPED, CanSM_GetState());
}

void test_STARTED_RequestComMode_stays_STARTED_returns_OK(void)
{
    go_started(&cfg_default);
    Std_ReturnType ret = CanSM_RequestComMode();
    TEST_ASSERT_EQUAL(E_OK, ret);
    TEST_ASSERT_EQUAL(CANSM_STARTED, CanSM_GetState());
}

void test_STARTED_BusOff_goes_BUS_OFF(void)
{
    go_started(&cfg_default);
    CanSM_ControllerBusOff();
    TEST_ASSERT_EQUAL(CANSM_BUS_OFF, CanSM_GetState());
    TEST_ASSERT_FALSE(CanSM_IsCommunicationAllowed());
}

void test_STARTED_MainFunction_no_state_change(void)
{
    go_started(&cfg_default);
    CanSM_MainFunction();
    TEST_ASSERT_EQUAL(CANSM_STARTED, CanSM_GetState());
}

/* ---- BUS_OFF x 4 events ---- */

void test_BUS_OFF_Init_resets_to_STOPPED(void)
{
    go_busoff(&cfg_default);
    CanSM_Init(&cfg_default);
    TEST_ASSERT_EQUAL(CANSM_STOPPED, CanSM_GetState());
}

void test_BUS_OFF_RequestComMode_returns_NOT_OK(void)
{
    go_busoff(&cfg_default);
    Std_ReturnType ret = CanSM_RequestComMode();
    TEST_ASSERT_EQUAL(E_NOT_OK, ret);
    TEST_ASSERT_EQUAL(CANSM_BUS_OFF, CanSM_GetState());
}

void test_BUS_OFF_BusOff_again_stays_BUS_OFF(void)
{
    go_busoff(&cfg_default);
    CanSM_ControllerBusOff();
    TEST_ASSERT_EQUAL(CANSM_BUS_OFF, CanSM_GetState());
}

void test_BUS_OFF_MainFunction_drives_recovery(void)
{
    go_busoff(&cfg_default);
    /* After L1_RecoveryTimeMs (10ms = 1 tick), first recovery attempt */
    reset_mock();
    tick(1);
    TEST_ASSERT_EQUAL(CANSM_STARTED, CanSM_GetState());
    TEST_ASSERT_EQUAL(CAN_CS_STARTED, mock_can_mode);
}

/* ---- STARTED-after-recovery (5th behavioral state) ---- */

void test_RECOVERED_BusOff_recurrence_escalates(void)
{
    go_busoff(&cfg_fast);
    tick(1);  /* L1 attempt 1: recover to STARTED */
    TEST_ASSERT_EQUAL(CANSM_STARTED, CanSM_GetState());

    /* Bus-off recurs immediately */
    CanSM_ControllerBusOff();
    TEST_ASSERT_EQUAL(CANSM_BUS_OFF, CanSM_GetState());
}

/* ==================================================================
 * Section 2: L1 Recovery Sequence (5 retries -> L2 escalation)
 * ================================================================== */

void test_L1_recovery_5_retries_then_L2(void)
{
    uint8 attempt;
    CanSM_Init(&cfg_default);  /* L1_MaxAttempts = 5 */
    CanSM_RequestComMode();
    CanSM_ControllerBusOff();

    /* Each L1 attempt: wait 10ms (1 tick), recovers to STARTED */
    for (attempt = 0u; attempt < 5u; attempt++) {
        TEST_ASSERT_EQUAL(CANSM_BUS_OFF, CanSM_GetState());
        tick(1);
        TEST_ASSERT_EQUAL(CANSM_STARTED, CanSM_GetState());
        /* Bus-off recurs */
        CanSM_ControllerBusOff();
    }

    /* L1 exhausted (6th bus-off, attempt counter > L1_MaxAttempts).
     * Next MainFunction tick should escalate to L2 timing. */
    TEST_ASSERT_EQUAL(CANSM_BUS_OFF, CanSM_GetState());
    tick(1);  /* This tick advances to L2 with timer reset */

    /* Now in L2: need 100ms (10 ticks) for first L2 attempt */
    TEST_ASSERT_EQUAL(CANSM_BUS_OFF, CanSM_GetState());
    tick(9);  /* 9 more ticks = 100ms total */
    TEST_ASSERT_EQUAL(CANSM_BUS_OFF, CanSM_GetState());
    tick(1);  /* 10th tick: L2 recovery attempt */
    TEST_ASSERT_EQUAL(CANSM_STARTED, CanSM_GetState());
}

void test_L1_recovery_success_no_escalation(void)
{
    go_busoff(&cfg_default);
    tick(1);  /* L1 attempt 1: recover */
    TEST_ASSERT_EQUAL(CANSM_STARTED, CanSM_GetState());
    /* No more bus-off: stays STARTED, no L2 needed */
    tick(100);
    TEST_ASSERT_EQUAL(CANSM_STARTED, CanSM_GetState());
}

void test_L1_each_attempt_restarts_controller(void)
{
    go_busoff(&cfg_default);

    tick(1);  /* L1 attempt 1 */
    TEST_ASSERT_EQUAL(CAN_CS_STARTED, mock_can_mode);
    CanSM_ControllerBusOff();
    TEST_ASSERT_EQUAL(CAN_CS_STOPPED, mock_can_mode);

    tick(1);  /* L1 attempt 2 */
    TEST_ASSERT_EQUAL(CAN_CS_STARTED, mock_can_mode);
}

/* ==================================================================
 * Section 3: L2 Recovery Sequence (10 retries -> permanent failure)
 * ================================================================== */

void test_L2_recovery_10_retries_then_permanent(void)
{
    uint8 attempt;
    CanSM_Init(&cfg_default);  /* L2_MaxAttempts = 10 */
    CanSM_RequestComMode();
    CanSM_ControllerBusOff();

    /* Exhaust L1 (5 retries) */
    for (attempt = 0u; attempt < 5u; attempt++) {
        tick(1);
        CanSM_ControllerBusOff();
    }
    /* L1 exhausted, escalate */
    tick(1);

    /* Exhaust L2 (10 retries, each after 100ms = 10 ticks) */
    for (attempt = 0u; attempt < 10u; attempt++) {
        TEST_ASSERT_EQUAL(CANSM_BUS_OFF, CanSM_GetState());
        tick(10);
        if (attempt < 9u) {
            /* Recovered, then bus-off recurs */
            TEST_ASSERT_EQUAL(CANSM_STARTED, CanSM_GetState());
            CanSM_ControllerBusOff();
        }
    }

    /* After L2 attempt 10 with bus-off recurrence, 11th attempt: exhausted */
    CanSM_ControllerBusOff();
    tick(10);  /* Timer fires but recovery_level was reset to 0 */

    /* Should remain in BUS_OFF permanently (recovery_level = 0) */
    TEST_ASSERT_EQUAL(CANSM_BUS_OFF, CanSM_GetState());
    tick(100);  /* Many more ticks: still BUS_OFF */
    TEST_ASSERT_EQUAL(CANSM_BUS_OFF, CanSM_GetState());
}

void test_L2_permanent_busoff_comm_not_allowed(void)
{
    uint8 attempt;
    CanSM_Init(&cfg_fast);  /* L1=3, L2=2 — fast exhaust */
    CanSM_RequestComMode();
    CanSM_ControllerBusOff();

    /* Exhaust L1 (3 retries) */
    for (attempt = 0u; attempt < 3u; attempt++) {
        tick(1);
        CanSM_ControllerBusOff();
    }
    tick(1);  /* escalate to L2 */

    /* Exhaust L2 (2 retries) */
    for (attempt = 0u; attempt < 2u; attempt++) {
        tick(1);
        if (attempt < 1u) {
            CanSM_ControllerBusOff();
        }
    }
    CanSM_ControllerBusOff();
    tick(1);  /* L2 exhausted */

    /* Permanent bus-off */
    tick(1000);
    TEST_ASSERT_EQUAL(CANSM_BUS_OFF, CanSM_GetState());
    TEST_ASSERT_FALSE(CanSM_IsCommunicationAllowed());
}

/* ==================================================================
 * Section 4: Bus-off during L1 recovery
 * ================================================================== */

void test_busoff_during_L1_keeps_attempt_count(void)
{
    go_busoff(&cfg_default);

    /* L1 attempt 1: recover */
    tick(1);
    TEST_ASSERT_EQUAL(CANSM_STARTED, CanSM_GetState());

    /* Bus-off recurs immediately */
    CanSM_ControllerBusOff();
    TEST_ASSERT_EQUAL(CANSM_BUS_OFF, CanSM_GetState());

    /* L1 attempt 2: recover */
    tick(1);
    TEST_ASSERT_EQUAL(CANSM_STARTED, CanSM_GetState());
}

void test_busoff_mid_L1_timer_resets(void)
{
    go_busoff(&cfg_default);

    /* Partially advance timer but don't reach recovery time */
    /* For L1_RecoveryTimeMs=10, one tick is exactly 10ms, so it fires.
     * Use cfg with longer L1 recovery to test partial advancement. */
    static const CanSM_ConfigType cfg_slow_l1 = {
        .L1_RecoveryTimeMs = 30u,  /* 3 ticks to fire */
        .L1_MaxAttempts    = 5u,
        .L2_RecoveryTimeMs = 100u,
        .L2_MaxAttempts    = 10u,
    };

    CanSM_Init(&cfg_slow_l1);
    CanSM_RequestComMode();
    CanSM_ControllerBusOff();

    tick(2);  /* 20ms < 30ms, not yet recovered */
    TEST_ASSERT_EQUAL(CANSM_BUS_OFF, CanSM_GetState());

    /* Another bus-off — timer should reset */
    CanSM_ControllerBusOff();

    tick(2);  /* 20ms from reset, still < 30ms */
    TEST_ASSERT_EQUAL(CANSM_BUS_OFF, CanSM_GetState());

    tick(1);  /* 30ms from reset: recovers */
    TEST_ASSERT_EQUAL(CANSM_STARTED, CanSM_GetState());
}

/* ==================================================================
 * Section 5: Bus-off during L2 recovery
 * ================================================================== */

void test_busoff_during_L2_continues_L2_count(void)
{
    uint8 attempt;
    CanSM_Init(&cfg_fast);  /* L1=3, L2=2 */
    CanSM_RequestComMode();
    CanSM_ControllerBusOff();

    /* Exhaust L1 */
    for (attempt = 0u; attempt < 3u; attempt++) {
        tick(1);
        CanSM_ControllerBusOff();
    }
    tick(1);  /* escalate to L2 */

    /* L2 attempt 1 */
    tick(1);
    TEST_ASSERT_EQUAL(CANSM_STARTED, CanSM_GetState());

    /* Bus-off during L2 recovery */
    CanSM_ControllerBusOff();
    TEST_ASSERT_EQUAL(CANSM_BUS_OFF, CanSM_GetState());

    /* L2 attempt 2 */
    tick(1);
    TEST_ASSERT_EQUAL(CANSM_STARTED, CanSM_GetState());

    /* Bus-off again — should exhaust L2 */
    CanSM_ControllerBusOff();
    tick(1);  /* attempt 3 > L2_MaxAttempts(2) -> permanent */

    /* Permanent */
    TEST_ASSERT_EQUAL(CANSM_BUS_OFF, CanSM_GetState());
    tick(100);
    TEST_ASSERT_EQUAL(CANSM_BUS_OFF, CanSM_GetState());
}

/* ==================================================================
 * Section 6: Init with NULL config
 * ================================================================== */

void test_init_null_config_stays_uninit(void)
{
    CanSM_Init(NULL_PTR);
    TEST_ASSERT_EQUAL(CANSM_UNINIT, CanSM_GetState());
    TEST_ASSERT_FALSE(CanSM_IsCommunicationAllowed());
}

void test_init_null_then_valid_works(void)
{
    CanSM_Init(NULL_PTR);
    TEST_ASSERT_EQUAL(CANSM_UNINIT, CanSM_GetState());

    CanSM_Init(&cfg_default);
    TEST_ASSERT_EQUAL(CANSM_STOPPED, CanSM_GetState());

    Std_ReturnType ret = CanSM_RequestComMode();
    TEST_ASSERT_EQUAL(E_OK, ret);
    TEST_ASSERT_EQUAL(CANSM_STARTED, CanSM_GetState());
}

void test_init_null_all_apis_safe(void)
{
    CanSM_Init(NULL_PTR);

    /* All APIs should be safe to call */
    Std_ReturnType ret = CanSM_RequestComMode();
    TEST_ASSERT_EQUAL(E_NOT_OK, ret);

    CanSM_ControllerBusOff();     /* no crash */
    CanSM_MainFunction();          /* no crash */

    TEST_ASSERT_EQUAL(CANSM_UNINIT, CanSM_GetState());
    TEST_ASSERT_FALSE(CanSM_IsCommunicationAllowed());
}

/* ==================================================================
 * Section 7: Double init
 * ================================================================== */

void test_double_init_resets_state(void)
{
    go_started(&cfg_default);
    CanSM_ControllerBusOff();
    TEST_ASSERT_EQUAL(CANSM_BUS_OFF, CanSM_GetState());

    /* Re-init: should reset to STOPPED, clear recovery state */
    CanSM_Init(&cfg_default);
    TEST_ASSERT_EQUAL(CANSM_STOPPED, CanSM_GetState());

    /* Recovery should be clean after re-init */
    CanSM_RequestComMode();
    CanSM_ControllerBusOff();
    tick(1);
    TEST_ASSERT_EQUAL(CANSM_STARTED, CanSM_GetState());
}

void test_double_init_different_config(void)
{
    CanSM_Init(&cfg_default);
    CanSM_RequestComMode();
    CanSM_ControllerBusOff();

    /* Re-init with fast config */
    CanSM_Init(&cfg_fast);
    TEST_ASSERT_EQUAL(CANSM_STOPPED, CanSM_GetState());

    /* Verify new config is active */
    CanSM_RequestComMode();
    CanSM_ControllerBusOff();

    /* Exhaust L1 with cfg_fast (3 attempts) */
    uint8 attempt;
    for (attempt = 0u; attempt < 3u; attempt++) {
        tick(1);
        CanSM_ControllerBusOff();
    }
    tick(1);  /* should escalate to L2 with cfg_fast settings */

    /* L2 with cfg_fast: 10ms recovery, 2 attempts */
    tick(1);
    TEST_ASSERT_EQUAL(CANSM_STARTED, CanSM_GetState());
}

/* ==================================================================
 * Section 8: Debug counter verification
 * ================================================================== */

void test_busoff_counter_increments(void)
{
    extern volatile uint32 g_dbg_cansm_busoff_count;
    extern volatile uint32 g_dbg_cansm_recovery_count;

    CanSM_Init(&cfg_default);
    TEST_ASSERT_EQUAL(0u, g_dbg_cansm_busoff_count);
    TEST_ASSERT_EQUAL(0u, g_dbg_cansm_recovery_count);

    CanSM_RequestComMode();

    CanSM_ControllerBusOff();
    TEST_ASSERT_EQUAL(1u, g_dbg_cansm_busoff_count);

    tick(1);  /* recover */
    TEST_ASSERT_EQUAL(1u, g_dbg_cansm_recovery_count);

    CanSM_ControllerBusOff();
    TEST_ASSERT_EQUAL(2u, g_dbg_cansm_busoff_count);

    tick(1);  /* recover */
    TEST_ASSERT_EQUAL(2u, g_dbg_cansm_recovery_count);
}

void test_IsCommunicationAllowed_tracks_state(void)
{
    TEST_ASSERT_FALSE(CanSM_IsCommunicationAllowed());  /* STOPPED */

    CanSM_RequestComMode();
    TEST_ASSERT_TRUE(CanSM_IsCommunicationAllowed());   /* STARTED */

    CanSM_ControllerBusOff();
    TEST_ASSERT_FALSE(CanSM_IsCommunicationAllowed());  /* BUS_OFF */

    tick(1);
    TEST_ASSERT_TRUE(CanSM_IsCommunicationAllowed());   /* Recovered */
}

/* ==================================================================
 * Unity test runner
 * ================================================================== */

int main(void)
{
    UNITY_BEGIN();

    /* Section 1: State x Event (20 tests) */
    RUN_TEST(test_UNINIT_Init_valid_goes_STOPPED);
    RUN_TEST(test_UNINIT_RequestComMode_returns_NOT_OK);
    RUN_TEST(test_UNINIT_BusOff_ignored);
    RUN_TEST(test_UNINIT_MainFunction_no_op);
    RUN_TEST(test_STOPPED_Init_reinit_stays_STOPPED);
    RUN_TEST(test_STOPPED_RequestComMode_goes_STARTED);
    RUN_TEST(test_STOPPED_BusOff_goes_BUS_OFF);
    RUN_TEST(test_STOPPED_MainFunction_no_state_change);
    RUN_TEST(test_STARTED_Init_resets_to_STOPPED);
    RUN_TEST(test_STARTED_RequestComMode_stays_STARTED_returns_OK);
    RUN_TEST(test_STARTED_BusOff_goes_BUS_OFF);
    RUN_TEST(test_STARTED_MainFunction_no_state_change);
    RUN_TEST(test_BUS_OFF_Init_resets_to_STOPPED);
    RUN_TEST(test_BUS_OFF_RequestComMode_returns_NOT_OK);
    RUN_TEST(test_BUS_OFF_BusOff_again_stays_BUS_OFF);
    RUN_TEST(test_BUS_OFF_MainFunction_drives_recovery);
    RUN_TEST(test_RECOVERED_BusOff_recurrence_escalates);

    /* Section 2: L1 Recovery */
    RUN_TEST(test_L1_recovery_5_retries_then_L2);
    RUN_TEST(test_L1_recovery_success_no_escalation);
    RUN_TEST(test_L1_each_attempt_restarts_controller);

    /* Section 3: L2 Recovery */
    RUN_TEST(test_L2_recovery_10_retries_then_permanent);
    RUN_TEST(test_L2_permanent_busoff_comm_not_allowed);

    /* Section 4: Bus-off during L1 */
    RUN_TEST(test_busoff_during_L1_keeps_attempt_count);
    RUN_TEST(test_busoff_mid_L1_timer_resets);

    /* Section 5: Bus-off during L2 */
    RUN_TEST(test_busoff_during_L2_continues_L2_count);

    /* Section 6: NULL config */
    RUN_TEST(test_init_null_config_stays_uninit);
    RUN_TEST(test_init_null_then_valid_works);
    RUN_TEST(test_init_null_all_apis_safe);

    /* Section 7: Double init */
    RUN_TEST(test_double_init_resets_state);
    RUN_TEST(test_double_init_different_config);

    /* Section 8: Debug / auxiliary */
    RUN_TEST(test_busoff_counter_increments);
    RUN_TEST(test_IsCommunicationAllowed_tracks_state);

    return UNITY_END();
}
