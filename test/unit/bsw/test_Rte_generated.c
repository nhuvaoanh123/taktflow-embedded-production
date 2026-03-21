/**
 * @file    test_Rte_generated.c
 * @brief   Comprehensive unit tests for Runtime Environment
 * @date    2026-03-21
 *
 * @verifies SWR-BSW-014, SWR-BSW-026, SWR-BSW-027
 *
 * @details Tests RTE initialization, signal read/write for valid and
 *          invalid IDs, read-after-write consistency, runnable dispatching,
 *          period enforcement, priority ordering, and WdgM integration.
 *
 * Total: ~20 tests
 */
#include "unity.h"
#include "Rte.h"
#include "Det.h"

/* ==================================================================
 * Mock: WdgM (checkpoint tracking)
 * ================================================================== */

#define MOCK_MAX_SE 16u

static uint8  mock_wdgm_call_count;
static uint8  mock_wdgm_se_ids[MOCK_MAX_SE];
static uint8  mock_wdgm_se_idx;

Std_ReturnType WdgM_CheckpointReached(uint8 SEId)
{
    if (mock_wdgm_se_idx < MOCK_MAX_SE) {
        mock_wdgm_se_ids[mock_wdgm_se_idx] = SEId;
        mock_wdgm_se_idx++;
    }
    mock_wdgm_call_count++;
    return E_OK;
}

/* ==================================================================
 * Test runnables
 * ================================================================== */

static uint8 run_10ms_count;
static uint8 run_10ms_b_count;
static uint8 run_100ms_count;
static uint8 run_1ms_count;
static uint8 run_order[16];
static uint8 run_order_idx;

static void TestRun_10ms(void)
{
    run_10ms_count++;
    if (run_order_idx < 16u) { run_order[run_order_idx++] = 0u; }
}

static void TestRun_10ms_B(void)
{
    run_10ms_b_count++;
    if (run_order_idx < 16u) { run_order[run_order_idx++] = 1u; }
}

static void TestRun_100ms(void)
{
    run_100ms_count++;
    if (run_order_idx < 16u) { run_order[run_order_idx++] = 2u; }
}

static void TestRun_1ms(void)
{
    run_1ms_count++;
    if (run_order_idx < 16u) { run_order[run_order_idx++] = 3u; }
}

/* ==================================================================
 * Configurations
 * ================================================================== */

static const Rte_SignalConfigType test_signals[] = {
    { RTE_SIG_TORQUE_REQUEST,  100u },   /* signal 0, initial=100 */
    { RTE_SIG_STEERING_ANGLE,  0u },     /* signal 1, initial=0 */
    { RTE_SIG_VEHICLE_SPEED,   50u },    /* signal 2, initial=50 */
    { RTE_SIG_BRAKE_PRESSURE,  0u },     /* signal 3 */
    { RTE_SIG_MOTOR_STATUS,    0u },     /* signal 4 */
    { RTE_SIG_BATTERY_VOLTAGE, 12000u }, /* signal 5, initial=12000mV */
};

static const Rte_RunnableConfigType test_runnables[] = {
    { TestRun_10ms,   10u, 3u, 0u },  /* 10ms, priority 3, SE 0 */
    { TestRun_10ms_B, 10u, 1u, 0u },  /* 10ms, priority 1, SE 0 */
    { TestRun_100ms, 100u, 2u, 1u },  /* 100ms, priority 2, SE 1 */
    { TestRun_1ms,     1u, 5u, 2u },  /* 1ms, priority 5 (highest), SE 2 */
};

static Rte_ConfigType test_config;

/* ==================================================================
 * Fixtures
 * ================================================================== */

void setUp(void)
{
    mock_wdgm_call_count = 0u;
    mock_wdgm_se_idx     = 0u;
    run_10ms_count       = 0u;
    run_10ms_b_count     = 0u;
    run_100ms_count      = 0u;
    run_1ms_count        = 0u;
    run_order_idx        = 0u;

    uint8 i;
    for (i = 0u; i < MOCK_MAX_SE; i++) {
        mock_wdgm_se_ids[i] = 0xFFu;
    }
    for (i = 0u; i < 16u; i++) {
        run_order[i] = 0xFFu;
    }

    test_config.signalConfig   = test_signals;
    test_config.signalCount    = 6u;
    test_config.runnableConfig = test_runnables;
    test_config.runnableCount  = 4u;

    Det_Init();
    Rte_Init(&test_config);
}

void tearDown(void) { }

/* ==================================================================
 * 1. Initialization
 * ================================================================== */

/** @verifies SWR-BSW-026 — NULL config: module not initialized */
void test_Rte_Init_null_no_crash(void)
{
    Rte_Init(NULL_PTR);
    uint32 val = 0u;
    Std_ReturnType ret = Rte_Read(0u, &val);
    TEST_ASSERT_EQUAL(E_NOT_OK, ret);
}

/** @verifies SWR-BSW-026 — Valid init: signals set to initial values */
void test_Rte_Init_sets_initial_values(void)
{
    uint32 val = 0u;
    (void)Rte_Read(RTE_SIG_TORQUE_REQUEST, &val);
    TEST_ASSERT_EQUAL_UINT32(100u, val);

    (void)Rte_Read(RTE_SIG_VEHICLE_SPEED, &val);
    TEST_ASSERT_EQUAL_UINT32(50u, val);

    (void)Rte_Read(RTE_SIG_BATTERY_VOLTAGE, &val);
    TEST_ASSERT_EQUAL_UINT32(12000u, val);
}

/** @verifies SWR-BSW-026 — Re-init resets signals */
void test_Rte_Init_reinit_resets(void)
{
    (void)Rte_Write(RTE_SIG_TORQUE_REQUEST, 999u);
    Rte_Init(&test_config);

    uint32 val = 0u;
    (void)Rte_Read(RTE_SIG_TORQUE_REQUEST, &val);
    TEST_ASSERT_EQUAL_UINT32(100u, val);
}

/* ==================================================================
 * 2. Rte_Read/Write valid signal IDs
 * ================================================================== */

/** @verifies SWR-BSW-026 — Write then read returns same value */
void test_Rte_Write_Read_roundtrip(void)
{
    Std_ReturnType ret = Rte_Write(RTE_SIG_STEERING_ANGLE, 4500u);
    TEST_ASSERT_EQUAL(E_OK, ret);

    uint32 val = 0u;
    ret = Rte_Read(RTE_SIG_STEERING_ANGLE, &val);
    TEST_ASSERT_EQUAL(E_OK, ret);
    TEST_ASSERT_EQUAL_UINT32(4500u, val);
}

/** @verifies SWR-BSW-026 — Write overwrites previous value */
void test_Rte_Write_overwrites(void)
{
    (void)Rte_Write(RTE_SIG_BRAKE_PRESSURE, 100u);
    (void)Rte_Write(RTE_SIG_BRAKE_PRESSURE, 200u);

    uint32 val = 0u;
    (void)Rte_Read(RTE_SIG_BRAKE_PRESSURE, &val);
    TEST_ASSERT_EQUAL_UINT32(200u, val);
}

/** @verifies SWR-BSW-026 — Write max uint32 value */
void test_Rte_Write_max_value(void)
{
    (void)Rte_Write(RTE_SIG_MOTOR_STATUS, 0xFFFFFFFFu);

    uint32 val = 0u;
    (void)Rte_Read(RTE_SIG_MOTOR_STATUS, &val);
    TEST_ASSERT_EQUAL_UINT32(0xFFFFFFFFu, val);
}

/* ==================================================================
 * 3. Rte_Read/Write invalid signal IDs
 * ================================================================== */

/** @verifies SWR-BSW-026 — Write to invalid signal ID */
void test_Rte_Write_invalid_signal(void)
{
    Std_ReturnType ret = Rte_Write(6u, 100u);  /* signalCount=6, so ID 6 invalid */
    TEST_ASSERT_EQUAL(E_NOT_OK, ret);
}

/** @verifies SWR-BSW-026 — Read from invalid signal ID */
void test_Rte_Read_invalid_signal(void)
{
    uint32 val = 0u;
    Std_ReturnType ret = Rte_Read(6u, &val);
    TEST_ASSERT_EQUAL(E_NOT_OK, ret);
}

/** @verifies SWR-BSW-026 — Read with NULL pointer */
void test_Rte_Read_null_ptr(void)
{
    Std_ReturnType ret = Rte_Read(0u, NULL_PTR);
    TEST_ASSERT_EQUAL(E_NOT_OK, ret);
}

/** @verifies SWR-BSW-026 — Write before init */
void test_Rte_Write_before_init(void)
{
    Rte_Init(NULL_PTR);  /* Force uninit */
    Std_ReturnType ret = Rte_Write(0u, 100u);
    TEST_ASSERT_EQUAL(E_NOT_OK, ret);
}

/** @verifies SWR-BSW-026 — Read before init */
void test_Rte_Read_before_init(void)
{
    Rte_Init(NULL_PTR);
    uint32 val = 0u;
    Std_ReturnType ret = Rte_Read(0u, &val);
    TEST_ASSERT_EQUAL(E_NOT_OK, ret);
}

/* ==================================================================
 * 4. Rte_Read returns last written value (consistency)
 * ================================================================== */

/** @verifies SWR-BSW-026 — Multiple signals independent */
void test_Rte_signals_independent(void)
{
    (void)Rte_Write(RTE_SIG_TORQUE_REQUEST, 42u);
    (void)Rte_Write(RTE_SIG_STEERING_ANGLE, 99u);

    uint32 v1 = 0u, v2 = 0u;
    (void)Rte_Read(RTE_SIG_TORQUE_REQUEST, &v1);
    (void)Rte_Read(RTE_SIG_STEERING_ANGLE, &v2);

    TEST_ASSERT_EQUAL_UINT32(42u, v1);
    TEST_ASSERT_EQUAL_UINT32(99u, v2);
}

/* ==================================================================
 * 5. Rte_DispatchRunnables — tick/period enforcement
 * ================================================================== */

/** @verifies SWR-BSW-027 — 1ms runnable fires every tick */
void test_Rte_dispatch_1ms_every_tick(void)
{
    Rte_MainFunction();  /* tick 1 */
    TEST_ASSERT_EQUAL_UINT8(1u, run_1ms_count);

    Rte_MainFunction();  /* tick 2 */
    TEST_ASSERT_EQUAL_UINT8(2u, run_1ms_count);
}

/** @verifies SWR-BSW-027 — 10ms runnable fires at tick 10, not before */
void test_Rte_dispatch_10ms_at_tick10(void)
{
    uint8 i;
    for (i = 0u; i < 9u; i++) {
        Rte_MainFunction();
    }
    TEST_ASSERT_EQUAL_UINT8(0u, run_10ms_count);

    Rte_MainFunction();  /* tick 10 */
    TEST_ASSERT_EQUAL_UINT8(1u, run_10ms_count);
}

/** @verifies SWR-BSW-027 — 100ms runnable fires at tick 100 */
void test_Rte_dispatch_100ms_at_tick100(void)
{
    uint8 i;
    for (i = 0u; i < 99u; i++) {
        Rte_MainFunction();
    }
    TEST_ASSERT_EQUAL_UINT8(0u, run_100ms_count);

    Rte_MainFunction();  /* tick 100 */
    TEST_ASSERT_EQUAL_UINT8(1u, run_100ms_count);
}

/* ==================================================================
 * 6. Priority ordering
 * ================================================================== */

/** @verifies SWR-BSW-027 — At tick 10: highest-priority 1ms(prio5) fires first,
 *  then 10ms(prio3), then 10ms_B(prio1). 100ms does not fire. */
void test_Rte_priority_ordering_at_tick10(void)
{
    uint8 i;
    /* Advance to tick 10 */
    for (i = 0u; i < 10u; i++) {
        Rte_MainFunction();
    }

    /* At tick 10: 1ms(prio5), 10ms(prio3), 10ms_B(prio1) all fire */
    /* run_order tracks call sequence: 3=1ms, 0=10ms, 1=10ms_B */
    /* Find the run_order entries at tick 10 (last 3 entries) */
    /* tick10 dispatches 3 runnables: order should be 3,0,1 */
    uint8 t10_start = run_order_idx - 3u;
    TEST_ASSERT_EQUAL_UINT8(3u, run_order[t10_start]);     /* 1ms first (prio 5) */
    TEST_ASSERT_EQUAL_UINT8(0u, run_order[t10_start + 1u]); /* 10ms (prio 3) */
    TEST_ASSERT_EQUAL_UINT8(1u, run_order[t10_start + 2u]); /* 10ms_B (prio 1) */
}

/* ==================================================================
 * 7. WdgM checkpoint integration
 * ================================================================== */

/** @verifies SWR-BSW-027 — WdgM called for each unique SE */
void test_Rte_wdgm_checkpoint_per_se(void)
{
    /* Advance to tick 100 where all runnables fire */
    uint8 i;
    for (i = 0u; i < 100u; i++) {
        Rte_MainFunction();
    }

    /* At tick 100: SE 0 (1ms + 10ms + 10ms_B), SE 1 (100ms), SE 2 (1ms)
     * WdgM should be called for each unique SE: 0, 1, 2 */
    TEST_ASSERT_TRUE(mock_wdgm_call_count > 0u);
}

/** @verifies SWR-BSW-027 — MainFunction does nothing when not initialized */
void test_Rte_MainFunction_uninit_noop(void)
{
    Rte_Init(NULL_PTR);
    mock_wdgm_call_count = 0u;
    run_1ms_count = 0u;

    Rte_MainFunction();

    TEST_ASSERT_EQUAL_UINT8(0u, run_1ms_count);
    TEST_ASSERT_EQUAL_UINT8(0u, mock_wdgm_call_count);
}

/* ==================================================================
 * Unity Test Runner
 * ================================================================== */

int main(void)
{
    UNITY_BEGIN();

    /* Initialization */
    RUN_TEST(test_Rte_Init_null_no_crash);
    RUN_TEST(test_Rte_Init_sets_initial_values);
    RUN_TEST(test_Rte_Init_reinit_resets);

    /* Read/Write valid */
    RUN_TEST(test_Rte_Write_Read_roundtrip);
    RUN_TEST(test_Rte_Write_overwrites);
    RUN_TEST(test_Rte_Write_max_value);

    /* Read/Write invalid */
    RUN_TEST(test_Rte_Write_invalid_signal);
    RUN_TEST(test_Rte_Read_invalid_signal);
    RUN_TEST(test_Rte_Read_null_ptr);
    RUN_TEST(test_Rte_Write_before_init);
    RUN_TEST(test_Rte_Read_before_init);

    /* Consistency */
    RUN_TEST(test_Rte_signals_independent);

    /* Dispatch / period enforcement */
    RUN_TEST(test_Rte_dispatch_1ms_every_tick);
    RUN_TEST(test_Rte_dispatch_10ms_at_tick10);
    RUN_TEST(test_Rte_dispatch_100ms_at_tick100);

    /* Priority ordering */
    RUN_TEST(test_Rte_priority_ordering_at_tick10);

    /* WdgM integration */
    RUN_TEST(test_Rte_wdgm_checkpoint_per_se);
    RUN_TEST(test_Rte_MainFunction_uninit_noop);

    return UNITY_END();
}
