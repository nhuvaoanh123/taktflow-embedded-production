/**
 * @file    test_E2E_SM_full_generated.c
 * @brief   Exhaustive sliding-window state machine tests for E2E_Sm
 * @date    2026-03-21
 *
 * @verifies SWR-BSW-026, TSR-022, TSR-023, SSR-BSW-026
 *
 * @details Tests the 3-state (INIT/VALID/INVALID) sliding window E2E SM
 *          defined in E2E_Sm.c / E2E_Sm.h.
 *
 *          State machine:
 *            INIT    + (OkCount >= MinOkStateInit)    -> VALID
 *            VALID   + (ErrorCount > MaxErrorStateValid) -> INVALID
 *            INVALID + (OkCount >= MinOkStateInvalid) -> VALID
 *
 *          Window: circular buffer of size WindowSize (max 16).
 *          Each E2E_Sm_Check call inserts one sample (OK or ERROR),
 *          evicts the oldest when the window is full, and re-evaluates.
 *
 *          Inputs: E2E_STATUS_OK = OK, everything else = ERROR
 *
 *          Total tests: ~48 covering:
 *            - Every state x every input (3x3 = 9 base)
 *            - Window filling (16 samples)
 *            - Exactly-at-threshold transitions
 *            - Off-by-one (threshold-1 stays, threshold transitions)
 *            - Window eviction (oldest sample replaced)
 *            - NULL pointer defense
 *            - Window size edge cases
 */
#include "unity.h"
#include "E2E_Sm.h"

/* ==================================================================
 * Test configurations
 * ================================================================== */

/** Standard config: WindowSize=8, transitions at moderate thresholds */
static const E2E_SmConfigType cfg_std = {
    .WindowSize         = 8u,
    .MinOkStateInit     = 4u,   /* 4 OKs to leave INIT */
    .MaxErrorStateValid = 2u,   /* >2 errors in VALID -> INVALID */
    .MinOkStateInvalid  = 5u,   /* 5 OKs to leave INVALID */
};

/** Tight config: WindowSize=4, low thresholds for quick transitions */
static const E2E_SmConfigType cfg_tight = {
    .WindowSize         = 4u,
    .MinOkStateInit     = 2u,
    .MaxErrorStateValid = 1u,
    .MinOkStateInvalid  = 3u,
};

/** Max window config: WindowSize=16 (E2E_SM_MAX_WINDOW) */
static const E2E_SmConfigType cfg_max = {
    .WindowSize         = 16u,
    .MinOkStateInit     = 8u,
    .MaxErrorStateValid = 4u,
    .MinOkStateInvalid  = 10u,
};

/** Minimal config: WindowSize=1 */
static const E2E_SmConfigType cfg_min = {
    .WindowSize         = 1u,
    .MinOkStateInit     = 1u,
    .MaxErrorStateValid = 0u,
    .MinOkStateInvalid  = 1u,
};

static E2E_SmStateType sm;

/* ==================================================================
 * Helpers
 * ================================================================== */

/** Feed N consecutive OK samples */
static void feed_ok(const E2E_SmConfigType *cfg, uint8 n)
{
    uint8 i;
    for (i = 0u; i < n; i++) {
        (void)E2E_Sm_Check(cfg, &sm, E2E_STATUS_OK);
    }
}

/** Feed N consecutive ERROR samples */
static void feed_error(const E2E_SmConfigType *cfg, uint8 n)
{
    uint8 i;
    for (i = 0u; i < n; i++) {
        (void)E2E_Sm_Check(cfg, &sm, E2E_STATUS_ERROR);
    }
}

/** Get SM to VALID state (from INIT) with standard config */
static void go_valid(const E2E_SmConfigType *cfg)
{
    E2E_Sm_Init(&sm);
    feed_ok(cfg, cfg->MinOkStateInit);
}

/** Get SM to INVALID state via VALID (from INIT).
 *  Fills the entire window with errors so OkCount = 0. */
static void go_invalid(const E2E_SmConfigType *cfg)
{
    go_valid(cfg);
    /* Feed enough errors to fill window, evicting all OKs */
    feed_error(cfg, cfg->WindowSize);
}

/* ==================================================================
 * setUp / tearDown
 * ================================================================== */

void setUp(void)
{
    E2E_Sm_Init(&sm);
}

void tearDown(void) {}

/* ==================================================================
 * Section 1: Initialization tests
 * ================================================================== */

void test_Init_sets_INIT_state(void)
{
    TEST_ASSERT_EQUAL(E2E_SM_INIT, sm.Status);
    TEST_ASSERT_EQUAL(0u, sm.WindowIdx);
    TEST_ASSERT_EQUAL(0u, sm.OkCount);
    TEST_ASSERT_EQUAL(0u, sm.ErrorCount);
    TEST_ASSERT_EQUAL(0u, sm.SampleCount);
}

void test_Init_fills_window_with_errors(void)
{
    uint8 i;
    for (i = 0u; i < E2E_SM_MAX_WINDOW; i++) {
        TEST_ASSERT_EQUAL(1u, sm.WindowBuf[i]);  /* SM_ENTRY_ERROR = 1 */
    }
}

void test_Init_null_pointer_safe(void)
{
    E2E_Sm_Init(NULL_PTR);  /* should not crash */
}

/* ==================================================================
 * Section 2: State x Input base matrix (3 states x 3 inputs = 9 tests)
 *
 * Inputs: E2E_STATUS_OK, E2E_STATUS_ERROR, E2E_STATUS_REPEATED
 * ================================================================== */

/* ---- INIT state ---- */

void test_INIT_OK_stays_INIT_if_below_threshold(void)
{
    /* cfg_std: need 4 OKs. After 1 OK, still INIT. */
    E2E_SmStatusType s = E2E_Sm_Check(&cfg_std, &sm, E2E_STATUS_OK);
    TEST_ASSERT_EQUAL(E2E_SM_INIT, s);
}

void test_INIT_ERROR_stays_INIT(void)
{
    E2E_SmStatusType s = E2E_Sm_Check(&cfg_std, &sm, E2E_STATUS_ERROR);
    TEST_ASSERT_EQUAL(E2E_SM_INIT, s);
}

void test_INIT_REPEATED_stays_INIT(void)
{
    /* E2E_STATUS_REPEATED counts as ERROR */
    E2E_SmStatusType s = E2E_Sm_Check(&cfg_std, &sm, E2E_STATUS_REPEATED);
    TEST_ASSERT_EQUAL(E2E_SM_INIT, s);
}

/* ---- VALID state ---- */

void test_VALID_OK_stays_VALID(void)
{
    go_valid(&cfg_std);
    E2E_SmStatusType s = E2E_Sm_Check(&cfg_std, &sm, E2E_STATUS_OK);
    TEST_ASSERT_EQUAL(E2E_SM_VALID, s);
}

void test_VALID_ERROR_stays_VALID_if_below_threshold(void)
{
    go_valid(&cfg_std);
    /* MaxErrorStateValid=2: 1 error is tolerated */
    E2E_SmStatusType s = E2E_Sm_Check(&cfg_std, &sm, E2E_STATUS_ERROR);
    TEST_ASSERT_EQUAL(E2E_SM_VALID, s);
}

void test_VALID_REPEATED_counts_as_error(void)
{
    go_valid(&cfg_std);
    /* REPEATED is an error input */
    E2E_SmStatusType s = E2E_Sm_Check(&cfg_std, &sm, E2E_STATUS_REPEATED);
    TEST_ASSERT_EQUAL(E2E_SM_VALID, s);  /* 1 error < threshold */
}

/* ---- INVALID state ---- */

void test_INVALID_OK_stays_INVALID_if_below_threshold(void)
{
    go_invalid(&cfg_std);
    E2E_SmStatusType s = E2E_Sm_Check(&cfg_std, &sm, E2E_STATUS_OK);
    TEST_ASSERT_EQUAL(E2E_SM_INVALID, s);
}

void test_INVALID_ERROR_stays_INVALID(void)
{
    go_invalid(&cfg_std);
    E2E_SmStatusType s = E2E_Sm_Check(&cfg_std, &sm, E2E_STATUS_ERROR);
    TEST_ASSERT_EQUAL(E2E_SM_INVALID, s);
}

void test_INVALID_REPEATED_stays_INVALID(void)
{
    go_invalid(&cfg_std);
    E2E_SmStatusType s = E2E_Sm_Check(&cfg_std, &sm, E2E_STATUS_REPEATED);
    TEST_ASSERT_EQUAL(E2E_SM_INVALID, s);
}

/* ==================================================================
 * Section 3: Window filling (16 samples with max window)
 * ================================================================== */

void test_window_filling_16_OKs_reaches_VALID(void)
{
    /* cfg_max: WindowSize=16, MinOkStateInit=8 */
    E2E_Sm_Init(&sm);
    uint8 i;
    E2E_SmStatusType s = E2E_SM_INIT;

    for (i = 0u; i < 16u; i++) {
        s = E2E_Sm_Check(&cfg_max, &sm, E2E_STATUS_OK);
        if (i < 7u) {
            TEST_ASSERT_EQUAL(E2E_SM_INIT, s);
        } else {
            TEST_ASSERT_EQUAL(E2E_SM_VALID, s);
        }
    }

    TEST_ASSERT_EQUAL(16u, sm.OkCount);
    TEST_ASSERT_EQUAL(0u, sm.ErrorCount);
    TEST_ASSERT_EQUAL(16u, sm.SampleCount);
}

void test_window_filling_sample_count_saturates(void)
{
    E2E_Sm_Init(&sm);
    uint8 i;
    for (i = 0u; i < 20u; i++) {
        (void)E2E_Sm_Check(&cfg_std, &sm, E2E_STATUS_OK);
    }
    /* SampleCount should saturate at WindowSize (8), not exceed */
    TEST_ASSERT_EQUAL(8u, sm.SampleCount);
}

void test_window_filling_mixed_ok_error(void)
{
    /* cfg_std: WindowSize=8. Feed alternating OK/ERROR */
    E2E_Sm_Init(&sm);
    uint8 i;
    for (i = 0u; i < 8u; i++) {
        if ((i % 2u) == 0u) {
            (void)E2E_Sm_Check(&cfg_std, &sm, E2E_STATUS_OK);
        } else {
            (void)E2E_Sm_Check(&cfg_std, &sm, E2E_STATUS_ERROR);
        }
    }
    /* 4 OK, 4 ERROR in window.
     * SM went INIT->VALID at 4th OK (sample 7), then VALID->INVALID at
     * 4th ERROR (sample 8) because ErrorCount(4) > MaxErrorStateValid(1). */
    TEST_ASSERT_EQUAL(E2E_SM_INVALID, sm.Status);
    TEST_ASSERT_EQUAL(4u, sm.OkCount);
    TEST_ASSERT_EQUAL(4u, sm.ErrorCount);
}

/* ==================================================================
 * Section 4: Exactly-at-threshold transitions
 * ================================================================== */

void test_INIT_to_VALID_at_exact_MinOkStateInit(void)
{
    /* cfg_std: MinOkStateInit=4. Feed exactly 4 OKs. */
    E2E_Sm_Init(&sm);
    E2E_SmStatusType s;

    feed_ok(&cfg_std, 3u);
    TEST_ASSERT_EQUAL(E2E_SM_INIT, sm.Status);

    s = E2E_Sm_Check(&cfg_std, &sm, E2E_STATUS_OK);  /* 4th OK */
    TEST_ASSERT_EQUAL(E2E_SM_VALID, s);
}

void test_VALID_to_INVALID_at_exact_MaxErrorStateValid(void)
{
    /* cfg_std: MaxErrorStateValid=2. Need >2 errors (i.e., 3). */
    go_valid(&cfg_std);

    feed_error(&cfg_std, 2u);
    TEST_ASSERT_EQUAL(E2E_SM_VALID, sm.Status);  /* 2 errors: at threshold */

    E2E_SmStatusType s = E2E_Sm_Check(&cfg_std, &sm, E2E_STATUS_ERROR);
    TEST_ASSERT_EQUAL(E2E_SM_INVALID, s);  /* 3 errors: exceeds threshold */
}

void test_INVALID_to_VALID_at_exact_MinOkStateInvalid(void)
{
    /* cfg_std: MinOkStateInvalid=5 */
    go_invalid(&cfg_std);

    feed_ok(&cfg_std, 4u);
    TEST_ASSERT_EQUAL(E2E_SM_INVALID, sm.Status);

    E2E_SmStatusType s = E2E_Sm_Check(&cfg_std, &sm, E2E_STATUS_OK);
    TEST_ASSERT_EQUAL(E2E_SM_VALID, s);  /* 5th OK: meets threshold */
}

/* ==================================================================
 * Section 5: Off-by-one tests (threshold-1 stays, threshold transitions)
 * ================================================================== */

void test_INIT_threshold_minus_one_stays(void)
{
    /* cfg_std: MinOkStateInit=4. 3 OKs = not enough. */
    E2E_Sm_Init(&sm);
    feed_ok(&cfg_std, 3u);
    TEST_ASSERT_EQUAL(E2E_SM_INIT, sm.Status);
}

void test_VALID_threshold_minus_one_stays(void)
{
    /* cfg_std: MaxErrorStateValid=2. 2 errors: at threshold, stays VALID */
    go_valid(&cfg_std);
    feed_error(&cfg_std, 2u);
    TEST_ASSERT_EQUAL(E2E_SM_VALID, sm.Status);
}

void test_INVALID_threshold_minus_one_stays(void)
{
    /* cfg_std: MinOkStateInvalid=5. 4 OKs: not enough */
    go_invalid(&cfg_std);
    feed_ok(&cfg_std, 4u);
    TEST_ASSERT_EQUAL(E2E_SM_INVALID, sm.Status);
}

void test_INIT_exactly_at_threshold_transitions(void)
{
    E2E_Sm_Init(&sm);
    feed_ok(&cfg_std, 4u);  /* exactly MinOkStateInit */
    TEST_ASSERT_EQUAL(E2E_SM_VALID, sm.Status);
}

void test_VALID_exactly_over_threshold_transitions(void)
{
    go_valid(&cfg_std);
    feed_error(&cfg_std, 3u);  /* MaxErrorStateValid=2, need >2 */
    TEST_ASSERT_EQUAL(E2E_SM_INVALID, sm.Status);
}

void test_INVALID_exactly_at_threshold_transitions(void)
{
    go_invalid(&cfg_std);
    feed_ok(&cfg_std, 5u);  /* exactly MinOkStateInvalid */
    TEST_ASSERT_EQUAL(E2E_SM_VALID, sm.Status);
}

/* ==================================================================
 * Section 6: Window eviction (oldest sample replaced)
 * ================================================================== */

void test_eviction_ok_replaced_by_error(void)
{
    /* cfg_tight: WindowSize=4, MinOkStateInit=2 */
    E2E_Sm_Init(&sm);

    /* Fill window: [OK, OK, OK, OK] -> VALID */
    feed_ok(&cfg_tight, 4u);
    TEST_ASSERT_EQUAL(E2E_SM_VALID, sm.Status);
    TEST_ASSERT_EQUAL(4u, sm.OkCount);
    TEST_ASSERT_EQUAL(0u, sm.ErrorCount);

    /* Next ERROR evicts oldest OK: [ERR, OK, OK, OK] */
    (void)E2E_Sm_Check(&cfg_tight, &sm, E2E_STATUS_ERROR);
    TEST_ASSERT_EQUAL(3u, sm.OkCount);
    TEST_ASSERT_EQUAL(1u, sm.ErrorCount);
    TEST_ASSERT_EQUAL(E2E_SM_VALID, sm.Status);
}

void test_eviction_error_replaced_by_ok(void)
{
    /* cfg_tight: WindowSize=4, MaxErrorStateValid=1 */
    go_valid(&cfg_tight);

    /* Push 2 errors -> INVALID (>1 error) */
    feed_error(&cfg_tight, 2u);
    TEST_ASSERT_EQUAL(E2E_SM_INVALID, sm.Status);

    /* Fill rest of window and wrap around with OKs.
     * Keep feeding OKs until the errors are evicted. */
    feed_ok(&cfg_tight, 3u);  /* MinOkStateInvalid=3 */
    TEST_ASSERT_EQUAL(E2E_SM_VALID, sm.Status);
}

void test_eviction_maintains_correct_counts(void)
{
    E2E_Sm_Init(&sm);

    /* Fill 4 slots: [OK, ERR, OK, ERR] */
    (void)E2E_Sm_Check(&cfg_tight, &sm, E2E_STATUS_OK);
    (void)E2E_Sm_Check(&cfg_tight, &sm, E2E_STATUS_ERROR);
    (void)E2E_Sm_Check(&cfg_tight, &sm, E2E_STATUS_OK);
    (void)E2E_Sm_Check(&cfg_tight, &sm, E2E_STATUS_ERROR);

    TEST_ASSERT_EQUAL(2u, sm.OkCount);
    TEST_ASSERT_EQUAL(2u, sm.ErrorCount);
    TEST_ASSERT_EQUAL(4u, sm.SampleCount);

    /* Now window wraps: evict slot 0 (OK), insert OK */
    (void)E2E_Sm_Check(&cfg_tight, &sm, E2E_STATUS_OK);
    /* [OK(new), ERR, OK, ERR] -> OkCount still 2 (evicted OK, added OK) */
    TEST_ASSERT_EQUAL(2u, sm.OkCount);
    TEST_ASSERT_EQUAL(2u, sm.ErrorCount);

    /* Evict slot 1 (ERR), insert OK */
    (void)E2E_Sm_Check(&cfg_tight, &sm, E2E_STATUS_OK);
    /* OkCount=3, ErrorCount=1 */
    TEST_ASSERT_EQUAL(3u, sm.OkCount);
    TEST_ASSERT_EQUAL(1u, sm.ErrorCount);
}

void test_eviction_transitions_VALID_to_INVALID(void)
{
    /* cfg_tight: WindowSize=4, MaxErrorStateValid=1 */
    go_valid(&cfg_tight);

    /* Window is [OK, OK, OK, OK] (from go_valid: fed 2 OKs, but window wraps)
     * Actually go_valid feeds MinOkStateInit=2 OKs. Window=[OK, OK, ?, ?]
     * with SampleCount=2. Let's fill it completely first. */
    E2E_Sm_Init(&sm);
    feed_ok(&cfg_tight, 4u);  /* [OK, OK, OK, OK], VALID */

    /* Feed 2 errors: evict 2 OKs, add 2 ERRs: [ERR, ERR, OK, OK] */
    (void)E2E_Sm_Check(&cfg_tight, &sm, E2E_STATUS_ERROR);
    TEST_ASSERT_EQUAL(E2E_SM_VALID, sm.Status);  /* 1 error <= threshold */

    (void)E2E_Sm_Check(&cfg_tight, &sm, E2E_STATUS_ERROR);
    TEST_ASSERT_EQUAL(E2E_SM_INVALID, sm.Status);  /* 2 errors > 1 threshold */
}

void test_eviction_transitions_INVALID_to_VALID(void)
{
    /* Start in INVALID, then fill window with OKs */
    E2E_Sm_Init(&sm);
    feed_ok(&cfg_tight, 2u);  /* -> VALID */
    feed_error(&cfg_tight, 2u);  /* -> INVALID */

    /* Now feed enough OKs to reach MinOkStateInvalid=3 */
    feed_ok(&cfg_tight, 3u);
    TEST_ASSERT_EQUAL(E2E_SM_VALID, sm.Status);
}

/* ==================================================================
 * Section 7: NULL pointer defense
 * ================================================================== */

void test_Check_null_config_returns_INVALID(void)
{
    E2E_SmStatusType s = E2E_Sm_Check(NULL_PTR, &sm, E2E_STATUS_OK);
    TEST_ASSERT_EQUAL(E2E_SM_INVALID, s);
}

void test_Check_null_state_returns_INVALID(void)
{
    E2E_SmStatusType s = E2E_Sm_Check(&cfg_std, NULL_PTR, E2E_STATUS_OK);
    TEST_ASSERT_EQUAL(E2E_SM_INVALID, s);
}

void test_Check_both_null_returns_INVALID(void)
{
    E2E_SmStatusType s = E2E_Sm_Check(NULL_PTR, NULL_PTR, E2E_STATUS_OK);
    TEST_ASSERT_EQUAL(E2E_SM_INVALID, s);
}

/* ==================================================================
 * Section 8: Window size edge cases
 * ================================================================== */

void test_window_size_1_immediate_transition(void)
{
    /* cfg_min: WindowSize=1, MinOkStateInit=1 */
    E2E_Sm_Init(&sm);

    E2E_SmStatusType s = E2E_Sm_Check(&cfg_min, &sm, E2E_STATUS_OK);
    TEST_ASSERT_EQUAL(E2E_SM_VALID, s);

    /* One error: MaxErrorStateValid=0, so >0 errors -> INVALID */
    s = E2E_Sm_Check(&cfg_min, &sm, E2E_STATUS_ERROR);
    TEST_ASSERT_EQUAL(E2E_SM_INVALID, s);

    /* One OK: MinOkStateInvalid=1 -> VALID */
    s = E2E_Sm_Check(&cfg_min, &sm, E2E_STATUS_OK);
    TEST_ASSERT_EQUAL(E2E_SM_VALID, s);
}

void test_window_size_0_returns_INVALID(void)
{
    static const E2E_SmConfigType cfg_zero = {
        .WindowSize = 0u,
        .MinOkStateInit = 1u,
        .MaxErrorStateValid = 0u,
        .MinOkStateInvalid = 1u,
    };
    E2E_SmStatusType s = E2E_Sm_Check(&cfg_zero, &sm, E2E_STATUS_OK);
    TEST_ASSERT_EQUAL(E2E_SM_INVALID, s);
}

void test_window_size_exceeds_max_clamped(void)
{
    /* WindowSize > E2E_SM_MAX_WINDOW (16) should be clamped */
    static const E2E_SmConfigType cfg_huge = {
        .WindowSize = 255u,
        .MinOkStateInit = 8u,
        .MaxErrorStateValid = 4u,
        .MinOkStateInvalid = 10u,
    };
    E2E_Sm_Init(&sm);
    feed_ok(&cfg_huge, 8u);
    /* If clamped to 16, MinOkStateInit=8 reached -> VALID */
    TEST_ASSERT_EQUAL(E2E_SM_VALID, sm.Status);
    /* SampleCount should not exceed 16 */
    TEST_ASSERT_TRUE(sm.SampleCount <= 16u);
}

void test_window_size_max_16_fills_correctly(void)
{
    E2E_Sm_Init(&sm);
    feed_ok(&cfg_max, 16u);
    TEST_ASSERT_EQUAL(16u, sm.SampleCount);
    TEST_ASSERT_EQUAL(16u, sm.OkCount);
    TEST_ASSERT_EQUAL(0u, sm.ErrorCount);
    TEST_ASSERT_EQUAL(0u, sm.WindowIdx);  /* wrapped around */
}

/* ==================================================================
 * Section 9: All E2E_CheckStatusType inputs mapped correctly
 * ================================================================== */

void test_STATUS_WRONG_SEQ_counts_as_error(void)
{
    E2E_Sm_Init(&sm);
    (void)E2E_Sm_Check(&cfg_std, &sm, E2E_STATUS_WRONG_SEQ);
    TEST_ASSERT_EQUAL(1u, sm.ErrorCount);
    TEST_ASSERT_EQUAL(0u, sm.OkCount);
}

void test_STATUS_NO_NEW_DATA_counts_as_error(void)
{
    E2E_Sm_Init(&sm);
    (void)E2E_Sm_Check(&cfg_std, &sm, E2E_STATUS_NO_NEW_DATA);
    TEST_ASSERT_EQUAL(1u, sm.ErrorCount);
    TEST_ASSERT_EQUAL(0u, sm.OkCount);
}

void test_STATUS_OK_is_only_ok_input(void)
{
    E2E_Sm_Init(&sm);
    (void)E2E_Sm_Check(&cfg_std, &sm, E2E_STATUS_OK);
    TEST_ASSERT_EQUAL(0u, sm.ErrorCount);
    TEST_ASSERT_EQUAL(1u, sm.OkCount);
}

/* ==================================================================
 * Section 10: Full lifecycle test
 * ================================================================== */

void test_full_lifecycle_INIT_VALID_INVALID_VALID(void)
{
    /* cfg_tight: WindowSize=4, MinOkStateInit=2, MaxErrorStateValid=1,
     *            MinOkStateInvalid=3 */
    E2E_Sm_Init(&sm);

    /* INIT -> feed 2 OKs -> VALID */
    feed_ok(&cfg_tight, 2u);
    TEST_ASSERT_EQUAL(E2E_SM_VALID, sm.Status);

    /* VALID -> feed 2 ERRORs -> INVALID (>1 error) */
    feed_error(&cfg_tight, 2u);
    TEST_ASSERT_EQUAL(E2E_SM_INVALID, sm.Status);

    /* INVALID -> feed 3 OKs -> VALID */
    feed_ok(&cfg_tight, 3u);
    TEST_ASSERT_EQUAL(E2E_SM_VALID, sm.Status);

    /* VALID -> single error tolerated */
    (void)E2E_Sm_Check(&cfg_tight, &sm, E2E_STATUS_ERROR);
    TEST_ASSERT_EQUAL(E2E_SM_VALID, sm.Status);

    /* VALID -> second error -> INVALID again */
    (void)E2E_Sm_Check(&cfg_tight, &sm, E2E_STATUS_ERROR);
    TEST_ASSERT_EQUAL(E2E_SM_INVALID, sm.Status);
}

void test_reinit_after_INVALID_resets_completely(void)
{
    go_invalid(&cfg_std);
    TEST_ASSERT_EQUAL(E2E_SM_INVALID, sm.Status);

    E2E_Sm_Init(&sm);
    TEST_ASSERT_EQUAL(E2E_SM_INIT, sm.Status);
    TEST_ASSERT_EQUAL(0u, sm.OkCount);
    TEST_ASSERT_EQUAL(0u, sm.ErrorCount);
    TEST_ASSERT_EQUAL(0u, sm.SampleCount);
}

/* ==================================================================
 * Unity test runner
 * ================================================================== */

int main(void)
{
    UNITY_BEGIN();

    /* Section 1: Initialization (3 tests) */
    RUN_TEST(test_Init_sets_INIT_state);
    RUN_TEST(test_Init_fills_window_with_errors);
    RUN_TEST(test_Init_null_pointer_safe);

    /* Section 2: State x Input (9 tests) */
    RUN_TEST(test_INIT_OK_stays_INIT_if_below_threshold);
    RUN_TEST(test_INIT_ERROR_stays_INIT);
    RUN_TEST(test_INIT_REPEATED_stays_INIT);
    RUN_TEST(test_VALID_OK_stays_VALID);
    RUN_TEST(test_VALID_ERROR_stays_VALID_if_below_threshold);
    RUN_TEST(test_VALID_REPEATED_counts_as_error);
    RUN_TEST(test_INVALID_OK_stays_INVALID_if_below_threshold);
    RUN_TEST(test_INVALID_ERROR_stays_INVALID);
    RUN_TEST(test_INVALID_REPEATED_stays_INVALID);

    /* Section 3: Window filling (3 tests) */
    RUN_TEST(test_window_filling_16_OKs_reaches_VALID);
    RUN_TEST(test_window_filling_sample_count_saturates);
    RUN_TEST(test_window_filling_mixed_ok_error);

    /* Section 4: Exactly-at-threshold (3 tests) */
    RUN_TEST(test_INIT_to_VALID_at_exact_MinOkStateInit);
    RUN_TEST(test_VALID_to_INVALID_at_exact_MaxErrorStateValid);
    RUN_TEST(test_INVALID_to_VALID_at_exact_MinOkStateInvalid);

    /* Section 5: Off-by-one (6 tests) */
    RUN_TEST(test_INIT_threshold_minus_one_stays);
    RUN_TEST(test_VALID_threshold_minus_one_stays);
    RUN_TEST(test_INVALID_threshold_minus_one_stays);
    RUN_TEST(test_INIT_exactly_at_threshold_transitions);
    RUN_TEST(test_VALID_exactly_over_threshold_transitions);
    RUN_TEST(test_INVALID_exactly_at_threshold_transitions);

    /* Section 6: Window eviction (5 tests) */
    RUN_TEST(test_eviction_ok_replaced_by_error);
    RUN_TEST(test_eviction_error_replaced_by_ok);
    RUN_TEST(test_eviction_maintains_correct_counts);
    RUN_TEST(test_eviction_transitions_VALID_to_INVALID);
    RUN_TEST(test_eviction_transitions_INVALID_to_VALID);

    /* Section 7: NULL pointer (3 tests) */
    RUN_TEST(test_Check_null_config_returns_INVALID);
    RUN_TEST(test_Check_null_state_returns_INVALID);
    RUN_TEST(test_Check_both_null_returns_INVALID);

    /* Section 8: Window size edge cases (4 tests) */
    RUN_TEST(test_window_size_1_immediate_transition);
    RUN_TEST(test_window_size_0_returns_INVALID);
    RUN_TEST(test_window_size_exceeds_max_clamped);
    RUN_TEST(test_window_size_max_16_fills_correctly);

    /* Section 9: Input mapping (3 tests) */
    RUN_TEST(test_STATUS_WRONG_SEQ_counts_as_error);
    RUN_TEST(test_STATUS_NO_NEW_DATA_counts_as_error);
    RUN_TEST(test_STATUS_OK_is_only_ok_input);

    /* Section 10: Full lifecycle (2 tests) */
    RUN_TEST(test_full_lifecycle_INIT_VALID_INVALID_VALID);
    RUN_TEST(test_reinit_after_INVALID_resets_completely);

    return UNITY_END();
}
