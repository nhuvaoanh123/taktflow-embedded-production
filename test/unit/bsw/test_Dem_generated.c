/**
 * @file    test_Dem_generated.c
 * @brief   Comprehensive unit tests for Diagnostic Event Manager
 * @date    2026-03-21
 *
 * @verifies SWR-BSW-017, SWR-BSW-018
 *
 * @details Tests DEM initialization, error reporting with debounce,
 *          DTC confirmation, occurrence counters, ECU ID broadcast,
 *          event status queries, and DTC clearing.
 *
 * Total: ~20 tests
 */
#include "unity.h"
#include "Dem.h"
#include "Det.h"

#include <string.h>

/* ==================================================================
 * Mocks: NvM (persistence stubs)
 * ================================================================== */

#define NVM_BLOCK_SIZE 1024u

static uint8 mock_nvm_data[NVM_BLOCK_SIZE];

Std_ReturnType NvM_ReadBlock(uint16 BlockId, void* DataPtr)
{
    (void)BlockId;
    if (DataPtr != NULL_PTR) {
        (void)memcpy(DataPtr, mock_nvm_data, NVM_BLOCK_SIZE);
    }
    return E_OK;
}

Std_ReturnType NvM_WriteBlock(uint16 BlockId, const void* DataPtr)
{
    (void)BlockId;
    if (DataPtr != NULL_PTR) {
        (void)memcpy(mock_nvm_data, DataPtr, NVM_BLOCK_SIZE);
    }
    return E_OK;
}

/* ==================================================================
 * Mocks: SchM (exclusive area stubs)
 * ================================================================== */


/* ==================================================================
 * Mocks: PduR (DTC broadcast TX)
 * ================================================================== */

static uint8  mock_pdur_tx_count;
static uint8  mock_pdur_last_data[8];
static uint16 mock_pdur_last_pdu_id;

Std_ReturnType PduR_Transmit(uint16 TxPduId, const void* PduInfoPtr)
{
    mock_pdur_last_pdu_id = TxPduId;
    mock_pdur_tx_count++;
    /* PduInfoType: first member is SduDataPtr, second is SduLength */
    typedef struct { const uint8* SduDataPtr; uint16 SduLength; } PduInfo_t;
    const PduInfo_t* info = (const PduInfo_t*)PduInfoPtr;
    if ((info != NULL_PTR) && (info->SduDataPtr != NULL_PTR)) {
        (void)memcpy(mock_pdur_last_data, info->SduDataPtr, 8u);
    }
    return E_OK;
}

/* ==================================================================
 * Fixtures
 * ================================================================== */

void setUp(void)
{
    (void)memset(mock_nvm_data, 0u, sizeof(mock_nvm_data));
    mock_pdur_tx_count    = 0u;
    mock_pdur_last_pdu_id = 0xFFFFu;
    (void)memset(mock_pdur_last_data, 0u, sizeof(mock_pdur_last_data));

    Det_Init();
    Dem_Init(NULL_PTR);
}

void tearDown(void) { }

/* ==================================================================
 * 1. Initialization
 * ================================================================== */

/** @verifies SWR-BSW-017 */
void test_Dem_Init_null_config_no_crash(void)
{
    /* Dem_Init(NULL) should not crash — config is cast-away */
    Dem_Init(NULL_PTR);
    /* Verify module is operational: get status for event 0 */
    uint8 status = 0xFFu;
    Std_ReturnType ret = Dem_GetEventStatus(0u, &status);
    TEST_ASSERT_EQUAL(E_OK, ret);
    TEST_ASSERT_EQUAL_HEX8(0x00u, status);
}

/** @verifies SWR-BSW-017 */
void test_Dem_Init_clears_all_events(void)
{
    /* Report an error, then re-init */
    Dem_ReportErrorStatus(0u, DEM_EVENT_STATUS_FAILED);
    Dem_Init(NULL_PTR);

    uint8 status = 0xFFu;
    (void)Dem_GetEventStatus(0u, &status);
    TEST_ASSERT_EQUAL_HEX8(0x00u, status);
}

/* ==================================================================
 * 2. Error Reporting — per event type
 * ================================================================== */

/** @verifies SWR-BSW-017 — Pedal plausibility (event 0) */
void test_Dem_ReportError_pedal_plausibility(void)
{
    Dem_ReportErrorStatus(0u, DEM_EVENT_STATUS_FAILED);
    uint8 status = 0u;
    (void)Dem_GetEventStatus(0u, &status);
    TEST_ASSERT_BITS(DEM_STATUS_TEST_FAILED, DEM_STATUS_TEST_FAILED, status);
}

/** @verifies SWR-BSW-017 — CAN FZC timeout (event 4) */
void test_Dem_ReportError_can_fzc_timeout(void)
{
    Dem_ReportErrorStatus(4u, DEM_EVENT_STATUS_FAILED);
    uint8 status = 0u;
    (void)Dem_GetEventStatus(4u, &status);
    TEST_ASSERT_BITS(DEM_STATUS_TEST_FAILED, DEM_STATUS_TEST_FAILED, status);
}

/** @verifies SWR-BSW-017 — Motor overcurrent (event 7) */
void test_Dem_ReportError_motor_overcurrent(void)
{
    Dem_ReportErrorStatus(7u, DEM_EVENT_STATUS_FAILED);
    uint8 status = 0u;
    (void)Dem_GetEventStatus(7u, &status);
    TEST_ASSERT_BITS(DEM_STATUS_TEST_FAILED, DEM_STATUS_TEST_FAILED, status);
}

/** @verifies SWR-BSW-017 — Battery undervolt (event 13) */
void test_Dem_ReportError_battery_undervolt(void)
{
    Dem_ReportErrorStatus(13u, DEM_EVENT_STATUS_FAILED);
    uint8 status = 0u;
    (void)Dem_GetEventStatus(13u, &status);
    TEST_ASSERT_BITS(DEM_STATUS_TEST_FAILED, DEM_STATUS_TEST_FAILED, status);
}

/** @verifies SWR-BSW-017 — Invalid event ID */
void test_Dem_ReportError_invalid_event_id(void)
{
    uint16 det_before = Det_GetErrorCount();
    Dem_ReportErrorStatus(DEM_MAX_EVENTS, DEM_EVENT_STATUS_FAILED);
    TEST_ASSERT_EQUAL_UINT16(det_before + 1u, Det_GetErrorCount());
}

/** @verifies SWR-BSW-017 — Passed status clears testFailed */
void test_Dem_ReportError_passed_clears_test_failed(void)
{
    Dem_ReportErrorStatus(0u, DEM_EVENT_STATUS_FAILED);
    /* Report passed enough to decrement debounce below zero */
    Dem_ReportErrorStatus(0u, DEM_EVENT_STATUS_PASSED);
    Dem_ReportErrorStatus(0u, DEM_EVENT_STATUS_PASSED);
    Dem_ReportErrorStatus(0u, DEM_EVENT_STATUS_PASSED);
    Dem_ReportErrorStatus(0u, DEM_EVENT_STATUS_PASSED);

    uint8 status = 0xFFu;
    (void)Dem_GetEventStatus(0u, &status);
    TEST_ASSERT_BITS_LOW(DEM_STATUS_TEST_FAILED, status);
}

/* ==================================================================
 * 3. DTC Confirmation via Debounce
 * ================================================================== */

/** @verifies SWR-BSW-017, SWR-BSW-018 — DTC confirmed after threshold */
void test_Dem_dtc_confirmed_after_debounce_threshold(void)
{
    /* DEM_DEBOUNCE_FAIL_THRESHOLD = 3 */
    Dem_ReportErrorStatus(0u, DEM_EVENT_STATUS_FAILED);
    Dem_ReportErrorStatus(0u, DEM_EVENT_STATUS_FAILED);
    Dem_ReportErrorStatus(0u, DEM_EVENT_STATUS_FAILED);

    uint8 status = 0u;
    (void)Dem_GetEventStatus(0u, &status);
    TEST_ASSERT_BITS(DEM_STATUS_CONFIRMED_DTC, DEM_STATUS_CONFIRMED_DTC, status);
}

/** @verifies SWR-BSW-017 — DTC NOT confirmed below threshold */
void test_Dem_dtc_not_confirmed_below_threshold(void)
{
    Dem_ReportErrorStatus(0u, DEM_EVENT_STATUS_FAILED);
    Dem_ReportErrorStatus(0u, DEM_EVENT_STATUS_FAILED);

    uint8 status = 0u;
    (void)Dem_GetEventStatus(0u, &status);
    TEST_ASSERT_BITS_LOW(DEM_STATUS_CONFIRMED_DTC, status);
}

/* ==================================================================
 * 4. Occurrence Counter
 * ================================================================== */

/** @verifies SWR-BSW-018 — Occurrence counter increments on confirmation */
void test_Dem_occurrence_counter_increments(void)
{
    /* Confirm DTC (3 fails) */
    Dem_ReportErrorStatus(0u, DEM_EVENT_STATUS_FAILED);
    Dem_ReportErrorStatus(0u, DEM_EVENT_STATUS_FAILED);
    Dem_ReportErrorStatus(0u, DEM_EVENT_STATUS_FAILED);

    uint32 count = 0u;
    Std_ReturnType ret = Dem_GetOccurrenceCounter(0u, &count);
    TEST_ASSERT_EQUAL(E_OK, ret);
    TEST_ASSERT_EQUAL_UINT32(1u, count);
}

/** @verifies SWR-BSW-018 — Occurrence counter for invalid event */
void test_Dem_occurrence_counter_invalid_event(void)
{
    uint32 count = 0u;
    Std_ReturnType ret = Dem_GetOccurrenceCounter(DEM_MAX_EVENTS, &count);
    TEST_ASSERT_EQUAL(E_NOT_OK, ret);
}

/** @verifies SWR-BSW-018 — Occurrence counter with NULL pointer */
void test_Dem_occurrence_counter_null_ptr(void)
{
    Std_ReturnType ret = Dem_GetOccurrenceCounter(0u, NULL_PTR);
    TEST_ASSERT_EQUAL(E_NOT_OK, ret);
}

/* ==================================================================
 * 5. ECU ID and Broadcast
 * ================================================================== */

/** @verifies SWR-BSW-018 — SetEcuId + verify in broadcast */
void test_Dem_SetEcuId_appears_in_broadcast(void)
{
    Dem_SetEcuId(0x10u);  /* CVC */
    Dem_SetBroadcastPduId(5u);

    /* Confirm a DTC so broadcast has something to send */
    Dem_ReportErrorStatus(0u, DEM_EVENT_STATUS_FAILED);
    Dem_ReportErrorStatus(0u, DEM_EVENT_STATUS_FAILED);
    Dem_ReportErrorStatus(0u, DEM_EVENT_STATUS_FAILED);

    Dem_MainFunction();

    TEST_ASSERT_EQUAL_UINT8(1u, mock_pdur_tx_count);
    /* Byte 4 = ECU source ID */
    TEST_ASSERT_EQUAL_HEX8(0x10u, mock_pdur_last_data[4]);
}

/** @verifies SWR-BSW-018 — MainFunction broadcasts confirmed DTC */
void test_Dem_MainFunction_broadcasts_dtc(void)
{
    Dem_SetBroadcastPduId(5u);
    Dem_SetDtcCode(0u, 0xC00100u);

    Dem_ReportErrorStatus(0u, DEM_EVENT_STATUS_FAILED);
    Dem_ReportErrorStatus(0u, DEM_EVENT_STATUS_FAILED);
    Dem_ReportErrorStatus(0u, DEM_EVENT_STATUS_FAILED);

    Dem_MainFunction();

    TEST_ASSERT_EQUAL_UINT8(1u, mock_pdur_tx_count);
    /* Bytes 0-2: DTC code 0xC00100 */
    TEST_ASSERT_EQUAL_HEX8(0xC0u, mock_pdur_last_data[0]);
    TEST_ASSERT_EQUAL_HEX8(0x01u, mock_pdur_last_data[1]);
    TEST_ASSERT_EQUAL_HEX8(0x00u, mock_pdur_last_data[2]);
}

/** @verifies SWR-BSW-018 — MainFunction skips if broadcast PDU not configured */
void test_Dem_MainFunction_skips_unconfigured_pdu(void)
{
    /* Do NOT call Dem_SetBroadcastPduId */
    Dem_ReportErrorStatus(0u, DEM_EVENT_STATUS_FAILED);
    Dem_ReportErrorStatus(0u, DEM_EVENT_STATUS_FAILED);
    Dem_ReportErrorStatus(0u, DEM_EVENT_STATUS_FAILED);

    Dem_MainFunction();

    TEST_ASSERT_EQUAL_UINT8(0u, mock_pdur_tx_count);
}

/** @verifies SWR-BSW-018 — MainFunction does not re-broadcast same DTC */
void test_Dem_MainFunction_no_rebroadcast(void)
{
    Dem_SetBroadcastPduId(5u);
    Dem_ReportErrorStatus(0u, DEM_EVENT_STATUS_FAILED);
    Dem_ReportErrorStatus(0u, DEM_EVENT_STATUS_FAILED);
    Dem_ReportErrorStatus(0u, DEM_EVENT_STATUS_FAILED);

    Dem_MainFunction();
    Dem_MainFunction();

    TEST_ASSERT_EQUAL_UINT8(1u, mock_pdur_tx_count);
}

/* ==================================================================
 * 6. Event Status Queries
 * ================================================================== */

/** @verifies SWR-BSW-017 — GetEventStatus valid event */
void test_Dem_GetEventStatus_valid(void)
{
    uint8 status = 0xFFu;
    Std_ReturnType ret = Dem_GetEventStatus(0u, &status);
    TEST_ASSERT_EQUAL(E_OK, ret);
    TEST_ASSERT_EQUAL_HEX8(0x00u, status);
}

/** @verifies SWR-BSW-017 — GetEventStatus invalid event */
void test_Dem_GetEventStatus_invalid_event(void)
{
    uint8 status = 0u;
    Std_ReturnType ret = Dem_GetEventStatus(DEM_MAX_EVENTS, &status);
    TEST_ASSERT_EQUAL(E_NOT_OK, ret);
}

/** @verifies SWR-BSW-017 — GetEventStatus NULL pointer */
void test_Dem_GetEventStatus_null_ptr(void)
{
    Std_ReturnType ret = Dem_GetEventStatus(0u, NULL_PTR);
    TEST_ASSERT_EQUAL(E_NOT_OK, ret);
}

/* ==================================================================
 * 7. ClearAllDTCs
 * ================================================================== */

/** @verifies SWR-BSW-017, SWR-BSW-018 */
void test_Dem_ClearAllDTCs_resets_status_and_counters(void)
{
    /* Confirm DTC */
    Dem_ReportErrorStatus(0u, DEM_EVENT_STATUS_FAILED);
    Dem_ReportErrorStatus(0u, DEM_EVENT_STATUS_FAILED);
    Dem_ReportErrorStatus(0u, DEM_EVENT_STATUS_FAILED);

    Std_ReturnType ret = Dem_ClearAllDTCs();
    TEST_ASSERT_EQUAL(E_OK, ret);

    uint8 status = 0xFFu;
    (void)Dem_GetEventStatus(0u, &status);
    TEST_ASSERT_EQUAL_HEX8(0x00u, status);

    uint32 count = 99u;
    (void)Dem_GetOccurrenceCounter(0u, &count);
    TEST_ASSERT_EQUAL_UINT32(0u, count);
}

/* ==================================================================
 * 8. SetDtcCode
 * ================================================================== */

/** @verifies SWR-BSW-018 — SetDtcCode invalid event */
void test_Dem_SetDtcCode_invalid_event(void)
{
    uint16 det_before = Det_GetErrorCount();
    Dem_SetDtcCode(DEM_MAX_EVENTS, 0xC00100u);
    TEST_ASSERT_EQUAL_UINT16(det_before + 1u, Det_GetErrorCount());
}

/* ==================================================================
 * Unity Test Runner
 * ================================================================== */

int main(void)
{
    UNITY_BEGIN();

    /* Initialization */
    RUN_TEST(test_Dem_Init_null_config_no_crash);
    RUN_TEST(test_Dem_Init_clears_all_events);

    /* Error Reporting per event type */
    RUN_TEST(test_Dem_ReportError_pedal_plausibility);
    RUN_TEST(test_Dem_ReportError_can_fzc_timeout);
    RUN_TEST(test_Dem_ReportError_motor_overcurrent);
    RUN_TEST(test_Dem_ReportError_battery_undervolt);
    RUN_TEST(test_Dem_ReportError_invalid_event_id);
    RUN_TEST(test_Dem_ReportError_passed_clears_test_failed);

    /* DTC Confirmation */
    RUN_TEST(test_Dem_dtc_confirmed_after_debounce_threshold);
    RUN_TEST(test_Dem_dtc_not_confirmed_below_threshold);

    /* Occurrence Counter */
    RUN_TEST(test_Dem_occurrence_counter_increments);
    RUN_TEST(test_Dem_occurrence_counter_invalid_event);
    RUN_TEST(test_Dem_occurrence_counter_null_ptr);

    /* ECU ID and Broadcast */
    RUN_TEST(test_Dem_SetEcuId_appears_in_broadcast);
    RUN_TEST(test_Dem_MainFunction_broadcasts_dtc);
    RUN_TEST(test_Dem_MainFunction_skips_unconfigured_pdu);
    RUN_TEST(test_Dem_MainFunction_no_rebroadcast);

    /* Event Status Queries */
    RUN_TEST(test_Dem_GetEventStatus_valid);
    RUN_TEST(test_Dem_GetEventStatus_invalid_event);
    RUN_TEST(test_Dem_GetEventStatus_null_ptr);

    /* ClearAllDTCs */
    RUN_TEST(test_Dem_ClearAllDTCs_resets_status_and_counters);

    /* SetDtcCode */
    RUN_TEST(test_Dem_SetDtcCode_invalid_event);

    return UNITY_END();
}
