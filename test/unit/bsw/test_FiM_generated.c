/**
 * @file    test_FiM_generated.c
 * @brief   Comprehensive unit tests for Function Inhibition Manager
 * @date    2026-03-21
 *
 * @verifies SWR-BSW-027
 *
 * @details Tests FiM initialization, permission queries per FID,
 *          inhibition when DEM event is active, permission when clear,
 *          invalid FID handling, and multi-inhibition configurations.
 *
 * Total: ~15 tests
 */
#include "unity.h"
#include "FiM.h"
#include "Dem.h"
#include "Det.h"

/* ==================================================================
 * Mock: Dem_GetEventStatus (controlled per-test)
 * ================================================================== */

#define MOCK_DEM_MAX_EVENTS 32u

static uint8 mock_dem_statuses[MOCK_DEM_MAX_EVENTS];

Std_ReturnType Dem_GetEventStatus(uint8 EventId, uint8* StatusPtr)
{
    if (EventId >= MOCK_DEM_MAX_EVENTS) {
        return E_NOT_OK;
    }
    if (StatusPtr == NULL_PTR) {
        return E_NOT_OK;
    }
    *StatusPtr = mock_dem_statuses[EventId];
    return E_OK;
}

/* ==================================================================
 * Test inhibition configurations
 *
 * FID 0: Torque control   — inhibited by event 7 (motor overcurrent, TestFailed)
 * FID 1: Steering control  — inhibited by event 11 (steering fault, ConfirmedDTC)
 * FID 2: Brake control     — inhibited by event 10 (brake fault, TestFailed)
 * FID 3: Body control      — no inhibition (always permitted)
 * ================================================================== */

#define DEM_STATUS_TEST_FAILED   0x01u
#define DEM_STATUS_CONFIRMED_DTC 0x08u

static const FiM_InhibitionConfigType test_inhibitions[] = {
    { 0u, 7u,  DEM_STATUS_TEST_FAILED },    /* Torque ← motor overcurrent */
    { 1u, 11u, DEM_STATUS_CONFIRMED_DTC },   /* Steering ← steering fault */
    { 2u, 10u, DEM_STATUS_TEST_FAILED },     /* Brake ← brake fault */
};

static FiM_ConfigType test_config;

/* ==================================================================
 * Fixtures
 * ================================================================== */

void setUp(void)
{
    uint8 i;
    for (i = 0u; i < MOCK_DEM_MAX_EVENTS; i++) {
        mock_dem_statuses[i] = 0u;
    }

    test_config.inhibitions     = test_inhibitions;
    test_config.inhibitionCount = 3u;
    test_config.functionCount   = 4u;

    Det_Init();
    FiM_Init(&test_config);
}

void tearDown(void) { }

/* ==================================================================
 * 1. Initialization
 * ================================================================== */

/** @verifies SWR-BSW-027 — NULL config: init fails gracefully */
void test_FiM_Init_null_no_crash(void)
{
    FiM_Init(NULL_PTR);
    boolean perm = TRUE;
    Std_ReturnType ret = FiM_GetFunctionPermission(0u, &perm);
    TEST_ASSERT_EQUAL(E_NOT_OK, ret);
}

/** @verifies SWR-BSW-027 — Valid init: all functions permitted by default */
void test_FiM_Init_all_permitted(void)
{
    boolean perm = FALSE;
    uint8 i;
    for (i = 0u; i < 4u; i++) {
        Std_ReturnType ret = FiM_GetFunctionPermission(i, &perm);
        TEST_ASSERT_EQUAL(E_OK, ret);
        TEST_ASSERT_TRUE(perm);
    }
}

/* ==================================================================
 * 2. GetFunctionPermission for each FID
 * ================================================================== */

/** @verifies SWR-BSW-027 — FID 0 (Torque) permitted when no faults */
void test_FiM_permission_fid0_no_fault(void)
{
    FiM_MainFunction();
    boolean perm = FALSE;
    (void)FiM_GetFunctionPermission(0u, &perm);
    TEST_ASSERT_TRUE(perm);
}

/** @verifies SWR-BSW-027 — FID 1 (Steering) permitted when no faults */
void test_FiM_permission_fid1_no_fault(void)
{
    FiM_MainFunction();
    boolean perm = FALSE;
    (void)FiM_GetFunctionPermission(1u, &perm);
    TEST_ASSERT_TRUE(perm);
}

/** @verifies SWR-BSW-027 — FID 2 (Brake) permitted when no faults */
void test_FiM_permission_fid2_no_fault(void)
{
    FiM_MainFunction();
    boolean perm = FALSE;
    (void)FiM_GetFunctionPermission(2u, &perm);
    TEST_ASSERT_TRUE(perm);
}

/** @verifies SWR-BSW-027 — FID 3 (Body) always permitted (no inhibition) */
void test_FiM_permission_fid3_always_permitted(void)
{
    /* Even with faults on other events, FID 3 has no inhibitions */
    mock_dem_statuses[7]  = DEM_STATUS_TEST_FAILED;
    mock_dem_statuses[10] = DEM_STATUS_TEST_FAILED;
    mock_dem_statuses[11] = DEM_STATUS_CONFIRMED_DTC;
    FiM_MainFunction();

    boolean perm = FALSE;
    (void)FiM_GetFunctionPermission(3u, &perm);
    TEST_ASSERT_TRUE(perm);
}

/* ==================================================================
 * 3. Inhibition when DEM event is active
 * ================================================================== */

/** @verifies SWR-BSW-027 — Torque inhibited by motor overcurrent */
void test_FiM_inhibit_torque_on_overcurrent(void)
{
    mock_dem_statuses[7] = DEM_STATUS_TEST_FAILED;
    FiM_MainFunction();

    boolean perm = TRUE;
    (void)FiM_GetFunctionPermission(0u, &perm);
    TEST_ASSERT_FALSE(perm);
}

/** @verifies SWR-BSW-027 — Steering inhibited by confirmed steering fault */
void test_FiM_inhibit_steering_on_fault(void)
{
    mock_dem_statuses[11] = DEM_STATUS_CONFIRMED_DTC;
    FiM_MainFunction();

    boolean perm = TRUE;
    (void)FiM_GetFunctionPermission(1u, &perm);
    TEST_ASSERT_FALSE(perm);
}

/** @verifies SWR-BSW-027 — Brake inhibited by brake fault */
void test_FiM_inhibit_brake_on_fault(void)
{
    mock_dem_statuses[10] = DEM_STATUS_TEST_FAILED;
    FiM_MainFunction();

    boolean perm = TRUE;
    (void)FiM_GetFunctionPermission(2u, &perm);
    TEST_ASSERT_FALSE(perm);
}

/* ==================================================================
 * 4. Permission restored when DEM event clears
 * ================================================================== */

/** @verifies SWR-BSW-027 — Torque re-permitted after overcurrent clears */
void test_FiM_permission_restored_on_clear(void)
{
    /* Inhibit */
    mock_dem_statuses[7] = DEM_STATUS_TEST_FAILED;
    FiM_MainFunction();
    boolean perm = TRUE;
    (void)FiM_GetFunctionPermission(0u, &perm);
    TEST_ASSERT_FALSE(perm);

    /* Clear the fault */
    mock_dem_statuses[7] = 0u;
    FiM_MainFunction();
    (void)FiM_GetFunctionPermission(0u, &perm);
    TEST_ASSERT_TRUE(perm);
}

/** @verifies SWR-BSW-027 — Wrong mask bit does NOT inhibit */
void test_FiM_wrong_mask_no_inhibit(void)
{
    /* Steering checks ConfirmedDTC (0x08), set only TestFailed (0x01) */
    mock_dem_statuses[11] = DEM_STATUS_TEST_FAILED;
    FiM_MainFunction();

    boolean perm = FALSE;
    (void)FiM_GetFunctionPermission(1u, &perm);
    TEST_ASSERT_TRUE(perm);  /* Not inhibited: mask doesn't match */
}

/* ==================================================================
 * 5. Invalid FID
 * ================================================================== */

/** @verifies SWR-BSW-027 — Invalid FID returns E_NOT_OK */
void test_FiM_invalid_fid(void)
{
    boolean perm = TRUE;
    Std_ReturnType ret = FiM_GetFunctionPermission(32u, &perm);
    TEST_ASSERT_EQUAL(E_NOT_OK, ret);
}

/** @verifies SWR-BSW-027 — NULL permission pointer returns E_NOT_OK */
void test_FiM_null_permission_ptr(void)
{
    Std_ReturnType ret = FiM_GetFunctionPermission(0u, NULL_PTR);
    TEST_ASSERT_EQUAL(E_NOT_OK, ret);
}

/* ==================================================================
 * Unity Test Runner
 * ================================================================== */

int main(void)
{
    UNITY_BEGIN();

    /* Initialization */
    RUN_TEST(test_FiM_Init_null_no_crash);
    RUN_TEST(test_FiM_Init_all_permitted);

    /* Permission per FID */
    RUN_TEST(test_FiM_permission_fid0_no_fault);
    RUN_TEST(test_FiM_permission_fid1_no_fault);
    RUN_TEST(test_FiM_permission_fid2_no_fault);
    RUN_TEST(test_FiM_permission_fid3_always_permitted);

    /* Inhibition active */
    RUN_TEST(test_FiM_inhibit_torque_on_overcurrent);
    RUN_TEST(test_FiM_inhibit_steering_on_fault);
    RUN_TEST(test_FiM_inhibit_brake_on_fault);

    /* Permission restored */
    RUN_TEST(test_FiM_permission_restored_on_clear);
    RUN_TEST(test_FiM_wrong_mask_no_inhibit);

    /* Invalid FID */
    RUN_TEST(test_FiM_invalid_fid);
    RUN_TEST(test_FiM_null_permission_ptr);

    return UNITY_END();
}
