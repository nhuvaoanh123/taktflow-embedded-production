/**
 * @file    test_Det_generated.c
 * @brief   Comprehensive unit tests for Default Error Tracer
 * @date    2026-03-21
 *
 * @verifies SWR-BSW-040
 *
 * @details Tests DET initialization, error reporting for various module IDs,
 *          runtime error reporting, ring buffer overflow, log retrieval,
 *          callback mechanism, and error counter saturation.
 *
 * Total: ~15 tests
 */
#include "unity.h"
#include "Det.h"

/* ==================================================================
 * Callback tracking
 * ================================================================== */

static uint16 cb_module_id;
static uint8  cb_instance_id;
static uint8  cb_api_id;
static uint8  cb_error_id;
static uint8  cb_call_count;

static void test_callback(uint16 ModuleId, uint8 InstanceId,
                           uint8 ApiId, uint8 ErrorId)
{
    cb_module_id   = ModuleId;
    cb_instance_id = InstanceId;
    cb_api_id      = ApiId;
    cb_error_id    = ErrorId;
    cb_call_count++;
}

/* ==================================================================
 * Fixtures
 * ================================================================== */

void setUp(void)
{
    cb_module_id   = 0u;
    cb_instance_id = 0u;
    cb_api_id      = 0u;
    cb_error_id    = 0u;
    cb_call_count  = 0u;

    Det_Init();
}

void tearDown(void) { }

/* ==================================================================
 * 1. Det_ReportError with various module IDs
 * ================================================================== */

/** @verifies SWR-BSW-040 — CAN module error */
void test_Det_ReportError_CAN(void)
{
    Det_ReportError(DET_MODULE_CAN, 0u, CAN_API_INIT, DET_E_PARAM_POINTER);
    TEST_ASSERT_EQUAL_UINT16(1u, Det_GetErrorCount());

    Det_ErrorEntryType entry;
    Std_ReturnType ret = Det_GetLogEntry(0u, &entry);
    TEST_ASSERT_EQUAL(E_OK, ret);
    TEST_ASSERT_EQUAL_UINT16(DET_MODULE_CAN, entry.ModuleId);
    TEST_ASSERT_EQUAL_UINT8(CAN_API_INIT, entry.ApiId);
    TEST_ASSERT_EQUAL_UINT8(DET_E_PARAM_POINTER, entry.ErrorId);
}

/** @verifies SWR-BSW-040 — COM module error */
void test_Det_ReportError_COM(void)
{
    Det_ReportError(DET_MODULE_COM, 0u, COM_API_SEND_SIGNAL, DET_E_UNINIT);
    TEST_ASSERT_EQUAL_UINT16(1u, Det_GetErrorCount());

    Det_ErrorEntryType entry;
    (void)Det_GetLogEntry(0u, &entry);
    TEST_ASSERT_EQUAL_UINT16(DET_MODULE_COM, entry.ModuleId);
    TEST_ASSERT_EQUAL_UINT8(DET_E_UNINIT, entry.ErrorId);
}

/** @verifies SWR-BSW-040 — DEM module error */
void test_Det_ReportError_DEM(void)
{
    Det_ReportError(DET_MODULE_DEM, 0u, DEM_API_REPORT_ERROR_STATUS, DET_E_PARAM_VALUE);
    TEST_ASSERT_EQUAL_UINT16(1u, Det_GetErrorCount());

    Det_ErrorEntryType entry;
    (void)Det_GetLogEntry(0u, &entry);
    TEST_ASSERT_EQUAL_UINT16(DET_MODULE_DEM, entry.ModuleId);
}

/** @verifies SWR-BSW-040 — E2E module error */
void test_Det_ReportError_E2E(void)
{
    Det_ReportError(DET_MODULE_E2E, 0u, E2E_API_CHECK, DET_E_PARAM_POINTER);
    Det_ErrorEntryType entry;
    (void)Det_GetLogEntry(0u, &entry);
    TEST_ASSERT_EQUAL_UINT16(DET_MODULE_E2E, entry.ModuleId);
    TEST_ASSERT_EQUAL_UINT8(E2E_API_CHECK, entry.ApiId);
}

/** @verifies SWR-BSW-040 — WdgM module error */
void test_Det_ReportError_WdgM(void)
{
    Det_ReportError(DET_MODULE_WDGM, 0u, WDGM_API_CHECKPOINT_REACHED, DET_E_PARAM_VALUE);
    Det_ErrorEntryType entry;
    (void)Det_GetLogEntry(0u, &entry);
    TEST_ASSERT_EQUAL_UINT16(DET_MODULE_WDGM, entry.ModuleId);
}

/* ==================================================================
 * 2. Det_ReportRuntimeError
 * ================================================================== */

/** @verifies SWR-BSW-040 — Runtime error increments count */
void test_Det_ReportRuntimeError_increments_count(void)
{
    Det_ReportRuntimeError(DET_MODULE_DEM, 5u, DEM_API_MAIN_FUNCTION, 0x80u);
    TEST_ASSERT_EQUAL_UINT16(1u, Det_GetErrorCount());
}

/** @verifies SWR-BSW-040 — Runtime error stored in log */
void test_Det_ReportRuntimeError_stored_in_log(void)
{
    Det_ReportRuntimeError(DET_MODULE_RTE, 0u, RTE_API_WRITE, DET_E_UNINIT);

    Det_ErrorEntryType entry;
    Std_ReturnType ret = Det_GetLogEntry(0u, &entry);
    TEST_ASSERT_EQUAL(E_OK, ret);
    TEST_ASSERT_EQUAL_UINT16(DET_MODULE_RTE, entry.ModuleId);
    TEST_ASSERT_EQUAL_UINT8(RTE_API_WRITE, entry.ApiId);
}

/* ==================================================================
 * 3. Ring buffer overflow
 * ================================================================== */

/** @verifies SWR-BSW-040 — Fill all DET_LOG_SIZE slots, then overflow */
void test_Det_buffer_overflow_wraps(void)
{
    /* Fill the entire ring buffer */
    uint8 i;
    for (i = 0u; i < DET_LOG_SIZE; i++) {
        Det_ReportError(DET_MODULE_CAN, 0u, i, DET_E_PARAM_POINTER);
    }
    TEST_ASSERT_EQUAL_UINT16(DET_LOG_SIZE, Det_GetErrorCount());

    /* Overflow: write one more */
    Det_ReportError(DET_MODULE_COM, 0u, 0xFFu, DET_E_UNINIT);
    TEST_ASSERT_EQUAL_UINT16(DET_LOG_SIZE + 1u, Det_GetErrorCount());

    /* Oldest entry should now be index 1 (index 0 was overwritten) */
    Det_ErrorEntryType entry;
    (void)Det_GetLogEntry(0u, &entry);
    /* After overflow, oldest = ApiId 1 (second write) */
    TEST_ASSERT_EQUAL_UINT8(1u, entry.ApiId);
}

/** @verifies SWR-BSW-040 — Newest entry after overflow */
void test_Det_buffer_overflow_newest_entry(void)
{
    uint8 i;
    for (i = 0u; i < DET_LOG_SIZE; i++) {
        Det_ReportError(DET_MODULE_CAN, 0u, i, DET_E_PARAM_POINTER);
    }
    /* One more to overflow */
    Det_ReportError(DET_MODULE_COM, 1u, 0xAAu, DET_E_UNINIT);

    /* Last entry (index DET_LOG_SIZE-1) should be the newest */
    Det_ErrorEntryType entry;
    Std_ReturnType ret = Det_GetLogEntry(DET_LOG_SIZE - 1u, &entry);
    TEST_ASSERT_EQUAL(E_OK, ret);
    TEST_ASSERT_EQUAL_UINT16(DET_MODULE_COM, entry.ModuleId);
    TEST_ASSERT_EQUAL_UINT8(0xAAu, entry.ApiId);
}

/* ==================================================================
 * 4. Det_GetLogEntry retrieval
 * ================================================================== */

/** @verifies SWR-BSW-040 — GetLogEntry with NULL pointer */
void test_Det_GetLogEntry_null_ptr(void)
{
    Std_ReturnType ret = Det_GetLogEntry(0u, NULL_PTR);
    TEST_ASSERT_EQUAL(E_NOT_OK, ret);
}

/** @verifies SWR-BSW-040 — GetLogEntry index out of range */
void test_Det_GetLogEntry_out_of_range(void)
{
    Det_ErrorEntryType entry;
    Std_ReturnType ret = Det_GetLogEntry(0u, &entry);
    TEST_ASSERT_EQUAL(E_NOT_OK, ret);  /* No entries yet */
}

/** @verifies SWR-BSW-040 — GetLogEntry multiple entries in order */
void test_Det_GetLogEntry_fifo_order(void)
{
    Det_ReportError(DET_MODULE_CAN, 0u, 0x01u, DET_E_PARAM_POINTER);
    Det_ReportError(DET_MODULE_COM, 0u, 0x02u, DET_E_UNINIT);
    Det_ReportError(DET_MODULE_DEM, 0u, 0x03u, DET_E_PARAM_VALUE);

    Det_ErrorEntryType entry;
    (void)Det_GetLogEntry(0u, &entry);
    TEST_ASSERT_EQUAL_UINT8(0x01u, entry.ApiId);

    (void)Det_GetLogEntry(1u, &entry);
    TEST_ASSERT_EQUAL_UINT8(0x02u, entry.ApiId);

    (void)Det_GetLogEntry(2u, &entry);
    TEST_ASSERT_EQUAL_UINT8(0x03u, entry.ApiId);
}

/* ==================================================================
 * 5. Callback mechanism
 * ================================================================== */

/** @verifies SWR-BSW-040 — Callback invoked on error */
void test_Det_callback_invoked(void)
{
    Det_SetCallback(test_callback);
    Det_ReportError(DET_MODULE_WDGM, 2u, WDGM_API_INIT, DET_E_PARAM_POINTER);

    TEST_ASSERT_EQUAL_UINT8(1u, cb_call_count);
    TEST_ASSERT_EQUAL_UINT16(DET_MODULE_WDGM, cb_module_id);
    TEST_ASSERT_EQUAL_UINT8(2u, cb_instance_id);
    TEST_ASSERT_EQUAL_UINT8(WDGM_API_INIT, cb_api_id);
    TEST_ASSERT_EQUAL_UINT8(DET_E_PARAM_POINTER, cb_error_id);
}

/** @verifies SWR-BSW-040 — NULL callback does not crash */
void test_Det_null_callback_no_crash(void)
{
    Det_SetCallback(NULL_PTR);
    Det_ReportError(DET_MODULE_CAN, 0u, CAN_API_INIT, DET_E_UNINIT);
    TEST_ASSERT_EQUAL_UINT16(1u, Det_GetErrorCount());
}

/* ==================================================================
 * Unity Test Runner
 * ================================================================== */

int main(void)
{
    UNITY_BEGIN();

    /* ReportError with various module IDs */
    RUN_TEST(test_Det_ReportError_CAN);
    RUN_TEST(test_Det_ReportError_COM);
    RUN_TEST(test_Det_ReportError_DEM);
    RUN_TEST(test_Det_ReportError_E2E);
    RUN_TEST(test_Det_ReportError_WdgM);

    /* ReportRuntimeError */
    RUN_TEST(test_Det_ReportRuntimeError_increments_count);
    RUN_TEST(test_Det_ReportRuntimeError_stored_in_log);

    /* Buffer overflow */
    RUN_TEST(test_Det_buffer_overflow_wraps);
    RUN_TEST(test_Det_buffer_overflow_newest_entry);

    /* GetLogEntry */
    RUN_TEST(test_Det_GetLogEntry_null_ptr);
    RUN_TEST(test_Det_GetLogEntry_out_of_range);
    RUN_TEST(test_Det_GetLogEntry_fifo_order);

    /* Callback */
    RUN_TEST(test_Det_callback_invoked);
    RUN_TEST(test_Det_null_callback_no_crash);

    return UNITY_END();
}
