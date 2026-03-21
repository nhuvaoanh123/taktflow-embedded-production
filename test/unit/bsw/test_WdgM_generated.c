/**
 * @file    test_WdgM_generated.c
 * @brief   Comprehensive unit tests for Watchdog Manager
 * @date    2026-03-21
 *
 * @verifies SWR-BSW-019, SWR-BSW-020, SWR-BSW-021, SWR-BSW-022
 *
 * @details Tests WdgM initialization, checkpoint alive counting,
 *          supervision cycle pass/fail, expired transition,
 *          global status, and DIO watchdog gating.
 *
 * Total: ~20 tests
 */
#include "unity.h"
#include "WdgM.h"
#include "Det.h"

/* ==================================================================
 * Mock: Dio (watchdog pin toggle)
 * ================================================================== */

static uint8 mock_dio_flip_count;
static uint8 mock_dio_last_channel;

uint8 Dio_FlipChannel(uint8 ChannelId)
{
    mock_dio_last_channel = ChannelId;
    mock_dio_flip_count++;
    return STD_HIGH;
}

/* ==================================================================
 * Mock: Dem (error reporting from WdgM)
 * ================================================================== */

static uint8 mock_dem_event_id;
static uint8 mock_dem_status;
static uint8 mock_dem_report_count;

void Dem_ReportErrorStatus(Dem_EventIdType EventId, Dem_EventStatusType EventStatus)
{
    mock_dem_event_id = EventId;
    mock_dem_status = (uint8)EventStatus;
    mock_dem_report_count++;
}

/* ==================================================================
 * Test SE configurations
 * ================================================================== */

/* SE 0: App task — 4-6 checkpoints/cycle, 2 failed cycle tolerance */
/* SE 1: Com task — exactly 1 checkpoint/cycle, 1 cycle tolerance */
/* SE 2: Diag task — 2-3 checkpoints/cycle, 3 cycle tolerance */
/* SE 3: Safety task — 10-10 checkpoints/cycle, 0 tolerance (immediate expire) */

static const WdgM_SupervisedEntityConfigType test_se_configs[] = {
    { 0u, 4u, 6u, 2u },   /* SE 0: App */
    { 1u, 1u, 1u, 1u },   /* SE 1: Com */
    { 2u, 2u, 3u, 3u },   /* SE 2: Diag */
    { 3u, 10u, 10u, 0u }, /* SE 3: Safety (zero tolerance) */
};

static WdgM_ConfigType test_config;

/* ==================================================================
 * Fixtures
 * ================================================================== */

void setUp(void)
{
    mock_dio_flip_count   = 0u;
    mock_dio_last_channel = 0xFFu;
    mock_dem_event_id     = 0xFFu;
    mock_dem_status       = 0xFFu;
    mock_dem_report_count = 0u;

    test_config.seConfig      = test_se_configs;
    test_config.seCount       = 4u;
    test_config.wdtDioChannel = 3u;

    Det_Init();
    WdgM_Init(&test_config);
}

void tearDown(void) { }

/* ==================================================================
 * 1. Initialization
 * ================================================================== */

/** @verifies SWR-BSW-019 — NULL config reports error, stays uninit */
void test_WdgM_Init_null_config_no_crash(void)
{
    WdgM_Init(NULL_PTR);
    /* Global status should be FAILED */
    TEST_ASSERT_EQUAL(WDGM_GLOBAL_STATUS_FAILED, WdgM_GetGlobalStatus());
}

/** @verifies SWR-BSW-019 — Valid init sets global status OK */
void test_WdgM_Init_valid_config_ok(void)
{
    TEST_ASSERT_EQUAL(WDGM_GLOBAL_STATUS_OK, WdgM_GetGlobalStatus());
}

/** @verifies SWR-BSW-019 — All SEs start at LOCAL_STATUS_OK */
void test_WdgM_Init_all_se_ok(void)
{
    WdgM_LocalStatusType status;
    uint8 i;
    for (i = 0u; i < 4u; i++) {
        Std_ReturnType ret = WdgM_GetLocalStatus(i, &status);
        TEST_ASSERT_EQUAL(E_OK, ret);
        TEST_ASSERT_EQUAL(WDGM_LOCAL_STATUS_OK, status);
    }
}

/* ==================================================================
 * 2. CheckpointReached for each supervised entity
 * ================================================================== */

/** @verifies SWR-BSW-019 — SE 0 checkpoint returns OK */
void test_WdgM_CheckpointReached_SE0(void)
{
    Std_ReturnType ret = WdgM_CheckpointReached(0u);
    TEST_ASSERT_EQUAL(E_OK, ret);
}

/** @verifies SWR-BSW-019 — SE 1 checkpoint returns OK */
void test_WdgM_CheckpointReached_SE1(void)
{
    TEST_ASSERT_EQUAL(E_OK, WdgM_CheckpointReached(1u));
}

/** @verifies SWR-BSW-019 — SE 2 checkpoint returns OK */
void test_WdgM_CheckpointReached_SE2(void)
{
    TEST_ASSERT_EQUAL(E_OK, WdgM_CheckpointReached(2u));
}

/** @verifies SWR-BSW-019 — SE 3 checkpoint returns OK */
void test_WdgM_CheckpointReached_SE3(void)
{
    TEST_ASSERT_EQUAL(E_OK, WdgM_CheckpointReached(3u));
}

/** @verifies SWR-BSW-019 — Invalid SE returns E_NOT_OK */
void test_WdgM_CheckpointReached_invalid_se(void)
{
    Std_ReturnType ret = WdgM_CheckpointReached(4u);
    TEST_ASSERT_EQUAL(E_NOT_OK, ret);
}

/** @verifies SWR-BSW-019 — Checkpoint before init returns E_NOT_OK */
void test_WdgM_CheckpointReached_before_init(void)
{
    WdgM_Init(NULL_PTR);  /* Force uninit */
    Std_ReturnType ret = WdgM_CheckpointReached(0u);
    TEST_ASSERT_EQUAL(E_NOT_OK, ret);
}

/* ==================================================================
 * 3. Missed checkpoint -> FAILED -> EXPIRED
 * ================================================================== */

/** @verifies SWR-BSW-020 — No checkpoints -> FAILED after MainFunction */
void test_WdgM_missed_checkpoint_fails(void)
{
    /* Do not send any checkpoints for SE 0 (expects 4-6) */
    WdgM_MainFunction();

    WdgM_LocalStatusType status;
    (void)WdgM_GetLocalStatus(0u, &status);
    TEST_ASSERT_EQUAL(WDGM_LOCAL_STATUS_FAILED, status);
}

/** @verifies SWR-BSW-020 — Exceeding FailedRefCycleTol -> EXPIRED */
void test_WdgM_expired_after_tolerance(void)
{
    /* SE 0: tolerance = 2, need 3 consecutive fails to expire */
    WdgM_MainFunction();  /* Fail 1 */
    WdgM_MainFunction();  /* Fail 2 */
    WdgM_MainFunction();  /* Fail 3 -> EXPIRED */

    WdgM_LocalStatusType status;
    (void)WdgM_GetLocalStatus(0u, &status);
    TEST_ASSERT_EQUAL(WDGM_LOCAL_STATUS_EXPIRED, status);
}

/** @verifies SWR-BSW-020 — Zero tolerance SE expires immediately */
void test_WdgM_zero_tolerance_immediate_expire(void)
{
    /* SE 3: tolerance = 0, expects 10 checkpoints */
    /* Only send 1 checkpoint (not enough) */
    WdgM_CheckpointReached(3u);
    WdgM_MainFunction();  /* Fail 1 → exceeds 0 tolerance → EXPIRED */

    WdgM_LocalStatusType status;
    (void)WdgM_GetLocalStatus(3u, &status);
    TEST_ASSERT_EQUAL(WDGM_LOCAL_STATUS_EXPIRED, status);
}

/** @verifies SWR-BSW-020, SWR-BSW-022 — Expired SE reports to DEM */
void test_WdgM_expired_reports_dem(void)
{
    /* SE 1: tolerance=1, need 2 fails */
    WdgM_MainFunction();  /* Fail 1 */
    WdgM_MainFunction();  /* Fail 2 → EXPIRED, DEM report */

    TEST_ASSERT_TRUE(mock_dem_report_count > 0u);
}

/* ==================================================================
 * 4. MainFunction cycle — normal operation
 * ================================================================== */

/** @verifies SWR-BSW-019, SWR-BSW-021 — All SEs pass: global OK, WDI toggled */
void test_WdgM_MainFunction_all_pass(void)
{
    /* SE 0: 5 checkpoints (within 4-6) */
    uint8 i;
    for (i = 0u; i < 5u; i++) { WdgM_CheckpointReached(0u); }
    /* SE 1: 1 checkpoint (exactly 1) */
    WdgM_CheckpointReached(1u);
    /* SE 2: 2 checkpoints (within 2-3) */
    WdgM_CheckpointReached(2u);
    WdgM_CheckpointReached(2u);
    /* SE 3: 10 checkpoints (exactly 10) */
    for (i = 0u; i < 10u; i++) { WdgM_CheckpointReached(3u); }

    WdgM_MainFunction();

    TEST_ASSERT_EQUAL(WDGM_GLOBAL_STATUS_OK, WdgM_GetGlobalStatus());
    TEST_ASSERT_EQUAL_UINT8(1u, mock_dio_flip_count);
    TEST_ASSERT_EQUAL_UINT8(3u, mock_dio_last_channel);
}

/** @verifies SWR-BSW-021 — One SE fails: global FAILED, no WDI toggle */
void test_WdgM_MainFunction_one_fail_no_wdi(void)
{
    /* SE 0: pass */
    uint8 i;
    for (i = 0u; i < 5u; i++) { WdgM_CheckpointReached(0u); }
    /* SE 1: pass */
    WdgM_CheckpointReached(1u);
    /* SE 2: pass */
    WdgM_CheckpointReached(2u);
    WdgM_CheckpointReached(2u);
    /* SE 3: FAIL (0 of 10 expected) */

    WdgM_MainFunction();

    TEST_ASSERT_EQUAL(WDGM_GLOBAL_STATUS_FAILED, WdgM_GetGlobalStatus());
    TEST_ASSERT_EQUAL_UINT8(0u, mock_dio_flip_count);
}

/** @verifies SWR-BSW-019 — Recovery: failed SE passes next cycle */
void test_WdgM_recovery_after_fail(void)
{
    /* Cycle 1: SE 0 fails */
    WdgM_MainFunction();
    WdgM_LocalStatusType status;
    (void)WdgM_GetLocalStatus(0u, &status);
    TEST_ASSERT_EQUAL(WDGM_LOCAL_STATUS_FAILED, status);

    /* Cycle 2: SE 0 passes — send enough checkpoints */
    uint8 i;
    for (i = 0u; i < 5u; i++) { WdgM_CheckpointReached(0u); }
    /* Also feed other SEs to get global OK */
    WdgM_CheckpointReached(1u);
    WdgM_CheckpointReached(2u);
    WdgM_CheckpointReached(2u);
    for (i = 0u; i < 10u; i++) { WdgM_CheckpointReached(3u); }

    WdgM_MainFunction();

    (void)WdgM_GetLocalStatus(0u, &status);
    TEST_ASSERT_EQUAL(WDGM_LOCAL_STATUS_OK, status);
}

/* ==================================================================
 * 5. GetLocalStatus
 * ================================================================== */

/** @verifies SWR-BSW-019 — GetLocalStatus NULL pointer */
void test_WdgM_GetLocalStatus_null_ptr(void)
{
    Std_ReturnType ret = WdgM_GetLocalStatus(0u, NULL_PTR);
    TEST_ASSERT_EQUAL(E_NOT_OK, ret);
}

/** @verifies SWR-BSW-019 — GetLocalStatus invalid SE */
void test_WdgM_GetLocalStatus_invalid_se(void)
{
    WdgM_LocalStatusType status;
    Std_ReturnType ret = WdgM_GetLocalStatus(4u, &status);
    TEST_ASSERT_EQUAL(E_NOT_OK, ret);
}

/** @verifies SWR-BSW-019 — GetLocalStatus before init */
void test_WdgM_GetLocalStatus_before_init(void)
{
    WdgM_Init(NULL_PTR);  /* Force uninit */
    WdgM_LocalStatusType status;
    Std_ReturnType ret = WdgM_GetLocalStatus(0u, &status);
    TEST_ASSERT_EQUAL(E_NOT_OK, ret);
}

/* ==================================================================
 * Unity Test Runner
 * ================================================================== */

int main(void)
{
    UNITY_BEGIN();

    /* Initialization */
    RUN_TEST(test_WdgM_Init_null_config_no_crash);
    RUN_TEST(test_WdgM_Init_valid_config_ok);
    RUN_TEST(test_WdgM_Init_all_se_ok);

    /* CheckpointReached */
    RUN_TEST(test_WdgM_CheckpointReached_SE0);
    RUN_TEST(test_WdgM_CheckpointReached_SE1);
    RUN_TEST(test_WdgM_CheckpointReached_SE2);
    RUN_TEST(test_WdgM_CheckpointReached_SE3);
    RUN_TEST(test_WdgM_CheckpointReached_invalid_se);
    RUN_TEST(test_WdgM_CheckpointReached_before_init);

    /* Missed checkpoint / expiry */
    RUN_TEST(test_WdgM_missed_checkpoint_fails);
    RUN_TEST(test_WdgM_expired_after_tolerance);
    RUN_TEST(test_WdgM_zero_tolerance_immediate_expire);
    RUN_TEST(test_WdgM_expired_reports_dem);

    /* MainFunction normal operation */
    RUN_TEST(test_WdgM_MainFunction_all_pass);
    RUN_TEST(test_WdgM_MainFunction_one_fail_no_wdi);
    RUN_TEST(test_WdgM_recovery_after_fail);

    /* GetLocalStatus */
    RUN_TEST(test_WdgM_GetLocalStatus_null_ptr);
    RUN_TEST(test_WdgM_GetLocalStatus_invalid_se);
    RUN_TEST(test_WdgM_GetLocalStatus_before_init);

    return UNITY_END();
}
