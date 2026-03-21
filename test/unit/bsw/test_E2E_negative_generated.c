/**
 * @file    test_E2E_negative_generated.c
 * @brief   Negative and boundary tests for the E2E module
 * @date    2026-03-21
 *
 * @details Exercises every error path in E2E.c:
 *          - NULL pointer rejection for Protect/Check
 *          - Length mismatch rejection
 *          - CRC verification (correct and corrupted)
 *          - Alive counter: repeated, gap, wraparound
 *          - DataId mismatch detection
 *          - Supervision state machine transitions
 *
 * @standard AUTOSAR_SWS_E2ELibrary (Profile P01), ISO 26262 Part 6
 * @copyright Taktflow Systems 2026
 */

/* ---- Test build defines (must precede all includes) ---- */

#include "unity.h"

#include "E2E.h"
#include "Det.h"

#include <string.h>

/* ================================================================
 * Stubs
 * ================================================================ */

static uint16 stub_det_last_module;
static uint8  stub_det_last_api;
static uint8  stub_det_last_error;
static uint16 stub_det_error_count;

void Det_ReportError(uint16 ModuleId, uint8 InstanceId,
                     uint8 ApiId, uint8 ErrorId)
{
    (void)InstanceId;
    stub_det_last_module = ModuleId;
    stub_det_last_api    = ApiId;
    stub_det_last_error  = ErrorId;
    stub_det_error_count++;
}

void Det_ReportRuntimeError(uint16 ModuleId, uint8 InstanceId,
                            uint8 ApiId, uint8 ErrorId)
{
    (void)ModuleId; (void)InstanceId; (void)ApiId; (void)ErrorId;
}

/* ================================================================
 * Test fixtures
 * ================================================================ */

static E2E_ConfigType test_cfg;
static E2E_StateType  test_state;
static uint8          test_buf[8];

static void reset_stubs(void)
{
    stub_det_last_module = 0u;
    stub_det_last_api    = 0u;
    stub_det_last_error  = 0u;
    stub_det_error_count = 0u;
}

void setUp(void)
{
    reset_stubs();
    memset(test_buf, 0, sizeof(test_buf));
    test_cfg.DataId          = 0x05u;
    test_cfg.MaxDeltaCounter = 3u;
    test_cfg.DataLength      = 8u;
    test_state.Counter       = 0u;
}

void tearDown(void)
{
    /* nothing */
}

/* ================================================================
 * Helper: produce a valid E2E-protected PDU using E2E_Protect
 * ================================================================ */

static void make_valid_pdu(uint8* buf, E2E_ConfigType* cfg,
                           E2E_StateType* state)
{
    /* Fill payload (bytes 2..7) with test pattern */
    buf[2] = 0xAAu;
    buf[3] = 0xBBu;
    buf[4] = 0xCCu;
    buf[5] = 0xDDu;
    buf[6] = 0xEEu;
    buf[7] = 0x11u;
    (void)E2E_Protect(cfg, state, buf, cfg->DataLength);
}

/* ================================================================
 * TEST GROUP: E2E_Protect — NULL and invalid parameter checks
 * ================================================================ */

void test_Protect_NullConfig_ReturnsNotOk(void)
{
    Std_ReturnType ret = E2E_Protect(NULL_PTR, &test_state, test_buf, 8u);
    TEST_ASSERT_EQUAL_UINT8(E_NOT_OK, ret);
    TEST_ASSERT_EQUAL_UINT8(DET_E_PARAM_POINTER, stub_det_last_error);
    TEST_ASSERT_EQUAL_UINT16(DET_MODULE_E2E, stub_det_last_module);
    TEST_ASSERT_EQUAL_UINT8(E2E_API_PROTECT, stub_det_last_api);
}

void test_Protect_NullState_ReturnsNotOk(void)
{
    Std_ReturnType ret = E2E_Protect(&test_cfg, NULL_PTR, test_buf, 8u);
    TEST_ASSERT_EQUAL_UINT8(E_NOT_OK, ret);
    TEST_ASSERT_EQUAL_UINT8(DET_E_PARAM_POINTER, stub_det_last_error);
}

void test_Protect_NullData_ReturnsNotOk(void)
{
    Std_ReturnType ret = E2E_Protect(&test_cfg, &test_state, NULL_PTR, 8u);
    TEST_ASSERT_EQUAL_UINT8(E_NOT_OK, ret);
    TEST_ASSERT_EQUAL_UINT8(DET_E_PARAM_POINTER, stub_det_last_error);
}

void test_Protect_AllNull_ReturnsNotOk(void)
{
    Std_ReturnType ret = E2E_Protect(NULL_PTR, NULL_PTR, NULL_PTR, 8u);
    TEST_ASSERT_EQUAL_UINT8(E_NOT_OK, ret);
    /* First NULL check is Config */
    TEST_ASSERT_EQUAL_UINT8(DET_E_PARAM_POINTER, stub_det_last_error);
}

void test_Protect_LengthTooShort_ReturnsNotOk(void)
{
    /* Length < E2E_PAYLOAD_OFFSET (2) */
    Std_ReturnType ret = E2E_Protect(&test_cfg, &test_state, test_buf, 1u);
    TEST_ASSERT_EQUAL_UINT8(E_NOT_OK, ret);
    TEST_ASSERT_EQUAL_UINT8(DET_E_PARAM_VALUE, stub_det_last_error);
}

void test_Protect_LengthZero_ReturnsNotOk(void)
{
    Std_ReturnType ret = E2E_Protect(&test_cfg, &test_state, test_buf, 0u);
    TEST_ASSERT_EQUAL_UINT8(E_NOT_OK, ret);
    TEST_ASSERT_EQUAL_UINT8(DET_E_PARAM_VALUE, stub_det_last_error);
}

void test_Protect_LengthMismatch_ReturnsNotOk(void)
{
    /* Config says 8, but we pass 6 */
    Std_ReturnType ret = E2E_Protect(&test_cfg, &test_state, test_buf, 6u);
    TEST_ASSERT_EQUAL_UINT8(E_NOT_OK, ret);
    TEST_ASSERT_EQUAL_UINT8(DET_E_PARAM_VALUE, stub_det_last_error);
}

void test_Protect_LengthExactlyPayloadOffset_Mismatch(void)
{
    /* Length = 2 (E2E_PAYLOAD_OFFSET) but config says 8 */
    Std_ReturnType ret = E2E_Protect(&test_cfg, &test_state, test_buf, 2u);
    TEST_ASSERT_EQUAL_UINT8(E_NOT_OK, ret);
}

void test_Protect_ValidParams_ReturnsOk(void)
{
    test_buf[2] = 0xAAu;
    Std_ReturnType ret = E2E_Protect(&test_cfg, &test_state, test_buf, 8u);
    TEST_ASSERT_EQUAL_UINT8(E_OK, ret);
    TEST_ASSERT_EQUAL_UINT16(0u, stub_det_error_count);
}

void test_Protect_CounterIncrements(void)
{
    test_state.Counter = 0u;
    (void)E2E_Protect(&test_cfg, &test_state, test_buf, 8u);
    TEST_ASSERT_EQUAL_UINT8(1u, test_state.Counter);
    (void)E2E_Protect(&test_cfg, &test_state, test_buf, 8u);
    TEST_ASSERT_EQUAL_UINT8(2u, test_state.Counter);
}

void test_Protect_CounterWrapsAt15(void)
{
    test_state.Counter = 14u;  /* Next will be 15, then 0 */
    (void)E2E_Protect(&test_cfg, &test_state, test_buf, 8u);
    TEST_ASSERT_EQUAL_UINT8(15u, test_state.Counter);
    (void)E2E_Protect(&test_cfg, &test_state, test_buf, 8u);
    TEST_ASSERT_EQUAL_UINT8(0u, test_state.Counter);
}

void test_Protect_DataIdInByte0(void)
{
    test_cfg.DataId = 0x0Au;
    test_state.Counter = 0u;
    (void)E2E_Protect(&test_cfg, &test_state, test_buf, 8u);
    /* Byte 0 = (counter << 4) | (DataId & 0x0F) = (1 << 4) | 0x0A = 0x1A */
    TEST_ASSERT_EQUAL_HEX8(0x1Au, test_buf[0]);
}

void test_Protect_CrcWrittenToByte1(void)
{
    (void)E2E_Protect(&test_cfg, &test_state, test_buf, 8u);
    /* CRC is in byte 1 — just verify it's non-zero (valid CRC written) */
    /* We can't predict exact value without recomputing, but 0x00 is unlikely */
    /* Actually, let's verify the round-trip instead */
    E2E_StateType rx_state = { .Counter = 0u };
    E2E_CheckStatusType status = E2E_Check(&test_cfg, &rx_state, test_buf, 8u);
    TEST_ASSERT_EQUAL(E2E_STATUS_OK, status);
}

/* ================================================================
 * TEST GROUP: E2E_Check — NULL and invalid parameter checks
 * ================================================================ */

void test_Check_NullConfig_ReturnsError(void)
{
    E2E_CheckStatusType status = E2E_Check(NULL_PTR, &test_state, test_buf, 8u);
    TEST_ASSERT_EQUAL(E2E_STATUS_ERROR, status);
    TEST_ASSERT_EQUAL_UINT8(DET_E_PARAM_POINTER, stub_det_last_error);
}

void test_Check_NullState_ReturnsError(void)
{
    E2E_CheckStatusType status = E2E_Check(&test_cfg, NULL_PTR, test_buf, 8u);
    TEST_ASSERT_EQUAL(E2E_STATUS_ERROR, status);
    TEST_ASSERT_EQUAL_UINT8(DET_E_PARAM_POINTER, stub_det_last_error);
}

void test_Check_NullData_ReturnsError(void)
{
    E2E_CheckStatusType status = E2E_Check(&test_cfg, &test_state, NULL_PTR, 8u);
    TEST_ASSERT_EQUAL(E2E_STATUS_ERROR, status);
    TEST_ASSERT_EQUAL_UINT8(DET_E_PARAM_POINTER, stub_det_last_error);
}

void test_Check_LengthTooShort_ReturnsError(void)
{
    E2E_CheckStatusType status = E2E_Check(&test_cfg, &test_state, test_buf, 1u);
    TEST_ASSERT_EQUAL(E2E_STATUS_ERROR, status);
    TEST_ASSERT_EQUAL_UINT8(DET_E_PARAM_VALUE, stub_det_last_error);
}

void test_Check_LengthZero_ReturnsError(void)
{
    E2E_CheckStatusType status = E2E_Check(&test_cfg, &test_state, test_buf, 0u);
    TEST_ASSERT_EQUAL(E2E_STATUS_ERROR, status);
}

void test_Check_LengthMismatch_ReturnsError(void)
{
    E2E_CheckStatusType status = E2E_Check(&test_cfg, &test_state, test_buf, 6u);
    TEST_ASSERT_EQUAL(E2E_STATUS_ERROR, status);
}

/* ================================================================
 * TEST GROUP: E2E_Check — CRC verification
 * ================================================================ */

void test_Check_ValidPdu_ReturnsOk(void)
{
    E2E_StateType tx_state = { .Counter = 0u };
    E2E_StateType rx_state = { .Counter = 0u };
    make_valid_pdu(test_buf, &test_cfg, &tx_state);
    E2E_CheckStatusType status = E2E_Check(&test_cfg, &rx_state, test_buf, 8u);
    TEST_ASSERT_EQUAL(E2E_STATUS_OK, status);
}

void test_Check_CorruptedCrc_ReturnsError(void)
{
    E2E_StateType tx_state = { .Counter = 0u };
    E2E_StateType rx_state = { .Counter = 0u };
    make_valid_pdu(test_buf, &test_cfg, &tx_state);
    /* Corrupt CRC byte */
    test_buf[E2E_BYTE_CRC] ^= 0xFFu;
    E2E_CheckStatusType status = E2E_Check(&test_cfg, &rx_state, test_buf, 8u);
    TEST_ASSERT_EQUAL(E2E_STATUS_ERROR, status);
}

void test_Check_CorruptedPayload_ReturnsError(void)
{
    E2E_StateType tx_state = { .Counter = 0u };
    E2E_StateType rx_state = { .Counter = 0u };
    make_valid_pdu(test_buf, &test_cfg, &tx_state);
    /* Corrupt a payload byte — CRC will mismatch */
    test_buf[4] ^= 0x01u;
    E2E_CheckStatusType status = E2E_Check(&test_cfg, &rx_state, test_buf, 8u);
    TEST_ASSERT_EQUAL(E2E_STATUS_ERROR, status);
}

void test_Check_WrongDataId_ReturnsError(void)
{
    E2E_StateType tx_state = { .Counter = 0u };
    E2E_StateType rx_state = { .Counter = 0u };
    make_valid_pdu(test_buf, &test_cfg, &tx_state);
    /* Check with different DataId */
    E2E_ConfigType wrong_cfg = test_cfg;
    wrong_cfg.DataId = 0x0Au;  /* Different from 0x05 */
    E2E_CheckStatusType status = E2E_Check(&wrong_cfg, &rx_state, test_buf, 8u);
    TEST_ASSERT_EQUAL(E2E_STATUS_ERROR, status);
}

/* ================================================================
 * TEST GROUP: E2E_Check — alive counter verification
 * ================================================================ */

void test_Check_RepeatedCounter_ReturnsRepeated(void)
{
    E2E_StateType tx_state = { .Counter = 0u };
    E2E_StateType rx_state = { .Counter = 0u };
    make_valid_pdu(test_buf, &test_cfg, &tx_state);

    /* First check: OK (counter goes from 0 to 1) */
    E2E_CheckStatusType status = E2E_Check(&test_cfg, &rx_state, test_buf, 8u);
    TEST_ASSERT_EQUAL(E2E_STATUS_OK, status);

    /* Same PDU again without re-protecting (same counter) — REPEATED */
    status = E2E_Check(&test_cfg, &rx_state, test_buf, 8u);
    TEST_ASSERT_EQUAL(E2E_STATUS_REPEATED, status);
}

void test_Check_ConsecutiveCounters_ReturnsOk(void)
{
    E2E_StateType tx_state = { .Counter = 0u };
    E2E_StateType rx_state = { .Counter = 0u };

    /* First frame */
    make_valid_pdu(test_buf, &test_cfg, &tx_state);
    E2E_CheckStatusType status = E2E_Check(&test_cfg, &rx_state, test_buf, 8u);
    TEST_ASSERT_EQUAL(E2E_STATUS_OK, status);

    /* Second frame (counter increments by 1) */
    make_valid_pdu(test_buf, &test_cfg, &tx_state);
    status = E2E_Check(&test_cfg, &rx_state, test_buf, 8u);
    TEST_ASSERT_EQUAL(E2E_STATUS_OK, status);
}

void test_Check_CounterGapWithinMax_ReturnsOk(void)
{
    E2E_StateType tx_state = { .Counter = 0u };
    E2E_StateType rx_state = { .Counter = 0u };

    /* First frame: counter=1 */
    make_valid_pdu(test_buf, &test_cfg, &tx_state);
    E2E_CheckStatusType status = E2E_Check(&test_cfg, &rx_state, test_buf, 8u);
    TEST_ASSERT_EQUAL(E2E_STATUS_OK, status);

    /* Skip 2 frames (counter goes from 1 to 4 = gap of 3, MaxDelta=3) */
    make_valid_pdu(test_buf, &test_cfg, &tx_state);  /* counter=2 */
    make_valid_pdu(test_buf, &test_cfg, &tx_state);  /* counter=3 */
    make_valid_pdu(test_buf, &test_cfg, &tx_state);  /* counter=4 */
    status = E2E_Check(&test_cfg, &rx_state, test_buf, 8u);
    TEST_ASSERT_EQUAL(E2E_STATUS_OK, status);
}

void test_Check_CounterGapExceedsMax_ReturnsWrongSeq(void)
{
    E2E_StateType tx_state = { .Counter = 0u };
    E2E_StateType rx_state = { .Counter = 0u };

    /* First frame: counter=1 */
    make_valid_pdu(test_buf, &test_cfg, &tx_state);
    E2E_CheckStatusType status = E2E_Check(&test_cfg, &rx_state, test_buf, 8u);
    TEST_ASSERT_EQUAL(E2E_STATUS_OK, status);
    /* rx_state.Counter = 1 now */

    /* Skip enough frames to exceed MaxDelta (3). Need delta > 3. */
    /* Produce frames with counter 2,3,4,5. Only check counter=5 (delta=4 > 3) */
    make_valid_pdu(test_buf, &test_cfg, &tx_state);  /* counter=2 */
    make_valid_pdu(test_buf, &test_cfg, &tx_state);  /* counter=3 */
    make_valid_pdu(test_buf, &test_cfg, &tx_state);  /* counter=4 */
    make_valid_pdu(test_buf, &test_cfg, &tx_state);  /* counter=5 */
    status = E2E_Check(&test_cfg, &rx_state, test_buf, 8u);
    TEST_ASSERT_EQUAL(E2E_STATUS_WRONG_SEQ, status);
}

void test_Check_CounterWraparound15to0_ReturnsOk(void)
{
    E2E_StateType tx_state = { .Counter = 14u };  /* Next will be 15 */
    E2E_StateType rx_state = { .Counter = 0u };

    /* First frame: counter=15 */
    make_valid_pdu(test_buf, &test_cfg, &tx_state);
    E2E_CheckStatusType status = E2E_Check(&test_cfg, &rx_state, test_buf, 8u);
    /* rx was 0, received 15, delta = (15-0) & 0xF = 15 > MaxDelta(3) */
    /* This is actually WRONG_SEQ since 15 > 3 */
    /* Let's set up properly: rx_state matches previous counter */
    rx_state.Counter = 14u;
    status = E2E_Check(&test_cfg, &rx_state, test_buf, 8u);
    TEST_ASSERT_EQUAL(E2E_STATUS_OK, status);
    /* rx_state.Counter = 15 now */

    /* Next frame: counter wraps to 0 */
    make_valid_pdu(test_buf, &test_cfg, &tx_state);
    TEST_ASSERT_EQUAL_UINT8(0u, tx_state.Counter);  /* Verify wraparound happened */
    status = E2E_Check(&test_cfg, &rx_state, test_buf, 8u);
    /* delta = (0 - 15) & 0xF = 1 → OK */
    TEST_ASSERT_EQUAL(E2E_STATUS_OK, status);
}

void test_Check_CounterWraparound_Gap2_ReturnsOk(void)
{
    /* Set up: rx at 14, tx produces 15 then skips to produce counter=1 */
    E2E_StateType tx_state = { .Counter = 14u };
    E2E_StateType rx_state = { .Counter = 14u };

    /* Frame: counter=15 */
    make_valid_pdu(test_buf, &test_cfg, &tx_state);
    (void)E2E_Check(&test_cfg, &rx_state, test_buf, 8u);
    /* rx=15 now */

    /* Skip one: produce counter=0, then counter=1 */
    make_valid_pdu(test_buf, &test_cfg, &tx_state);  /* counter=0 */
    make_valid_pdu(test_buf, &test_cfg, &tx_state);  /* counter=1 */
    /* Delta = (1-15) & 0xF = 2 → within MaxDelta(3) → OK */
    E2E_CheckStatusType status = E2E_Check(&test_cfg, &rx_state, test_buf, 8u);
    TEST_ASSERT_EQUAL(E2E_STATUS_OK, status);
}

/* ================================================================
 * TEST GROUP: E2E_CalcCRC8 — basic validation
 * ================================================================ */

void test_CalcCRC8_EmptyData_ReturnsXorOfInit(void)
{
    /* CRC of zero-length data with init 0xFF, XOR-out 0xFF:
     * Table[0xFF ^ nothing] → just 0xFF ^ 0xFF = 0x00
     * Actually: no iterations, crc stays 0xFF, XOR-out → 0x00 */
    uint8 crc = E2E_CalcCRC8(NULL_PTR, 0u, E2E_CRC8_INIT);
    TEST_ASSERT_EQUAL_HEX8(0x00u, crc);
}

void test_CalcCRC8_SingleByte_Deterministic(void)
{
    uint8 data = 0x00u;
    uint8 crc1 = E2E_CalcCRC8(&data, 1u, E2E_CRC8_INIT);
    uint8 crc2 = E2E_CalcCRC8(&data, 1u, E2E_CRC8_INIT);
    TEST_ASSERT_EQUAL_HEX8(crc1, crc2);
}

void test_CalcCRC8_DifferentData_DifferentCrc(void)
{
    uint8 data1[4] = {0x01, 0x02, 0x03, 0x04};
    uint8 data2[4] = {0x01, 0x02, 0x03, 0x05};
    uint8 crc1 = E2E_CalcCRC8(data1, 4u, E2E_CRC8_INIT);
    uint8 crc2 = E2E_CalcCRC8(data2, 4u, E2E_CRC8_INIT);
    TEST_ASSERT_FALSE(crc1 == crc2);
}

/* ================================================================
 * TEST GROUP: E2E_SMInit — null check
 * ================================================================ */

void test_SMInit_NullPtr_NoCrash(void)
{
    E2E_SMInit(NULL_PTR);
    /* Just verify no crash — function returns silently */
    TEST_ASSERT_EQUAL_UINT16(0u, stub_det_error_count);
}

void test_SMInit_SetsNodata(void)
{
    E2E_SMType sm;
    sm.State = E2E_SM_VALID;
    sm.OkCount = 99u;
    sm.ErrCount = 99u;
    E2E_SMInit(&sm);
    TEST_ASSERT_EQUAL(E2E_SM_NODATA, sm.State);
    TEST_ASSERT_EQUAL_UINT8(0u, sm.OkCount);
    TEST_ASSERT_EQUAL_UINT8(0u, sm.ErrCount);
}

/* ================================================================
 * TEST GROUP: E2E_SMCheck — state transitions
 * ================================================================ */

void test_SMCheck_NullConfig_ReturnsInvalid(void)
{
    E2E_SMType sm;
    E2E_SMInit(&sm);
    E2E_SMStateType state = E2E_SMCheck(NULL_PTR, &sm, E2E_STATUS_OK);
    TEST_ASSERT_EQUAL(E2E_SM_INVALID, state);
}

void test_SMCheck_NullSM_ReturnsInvalid(void)
{
    E2E_SMConfigType cfg = { 3u, 2u, 1u };
    E2E_SMStateType state = E2E_SMCheck(&cfg, NULL_PTR, E2E_STATUS_OK);
    TEST_ASSERT_EQUAL(E2E_SM_INVALID, state);
}

void test_SMCheck_Nodata_ToInit_OnFirstCheck(void)
{
    E2E_SMConfigType cfg = { .WindowSizeValid = 3u, .WindowSizeInvalid = 2u, .WindowSizeInit = 1u };
    E2E_SMType sm;
    E2E_SMInit(&sm);
    E2E_SMStateType state = E2E_SMCheck(&cfg, &sm, E2E_STATUS_OK);
    TEST_ASSERT_EQUAL(E2E_SM_INIT, state);
}

void test_SMCheck_Nodata_ToInit_EvenOnError(void)
{
    E2E_SMConfigType cfg = { 3u, 2u, 1u };
    E2E_SMType sm;
    E2E_SMInit(&sm);
    E2E_SMStateType state = E2E_SMCheck(&cfg, &sm, E2E_STATUS_ERROR);
    TEST_ASSERT_EQUAL(E2E_SM_INIT, state);
}

void test_SMCheck_Init_ToValid_AfterWindowSizeInit(void)
{
    E2E_SMConfigType cfg = { .WindowSizeValid = 3u, .WindowSizeInvalid = 2u, .WindowSizeInit = 1u };
    E2E_SMType sm;
    E2E_SMInit(&sm);
    /* First check: NODATA → INIT (OkCount=1, WindowSizeInit=1) */
    E2E_SMStateType state = E2E_SMCheck(&cfg, &sm, E2E_STATUS_OK);
    TEST_ASSERT_EQUAL(E2E_SM_INIT, state);
    /* Second OK: OkCount=2 >= WindowSizeInit(1) → VALID */
    state = E2E_SMCheck(&cfg, &sm, E2E_STATUS_OK);
    TEST_ASSERT_EQUAL(E2E_SM_VALID, state);
}

void test_SMCheck_Init_ToInvalid_AfterWindowSizeInvalid(void)
{
    E2E_SMConfigType cfg = { 3u, 2u, 1u };
    E2E_SMType sm;
    E2E_SMInit(&sm);
    /* First: NODATA → INIT with error */
    E2E_SMCheck(&cfg, &sm, E2E_STATUS_ERROR);
    /* Second error: ErrCount=2 >= WindowSizeInvalid(2) → INVALID */
    E2E_SMStateType state = E2E_SMCheck(&cfg, &sm, E2E_STATUS_ERROR);
    TEST_ASSERT_EQUAL(E2E_SM_INVALID, state);
}

void test_SMCheck_Valid_ToInvalid_AfterConsecutiveErrors(void)
{
    E2E_SMConfigType cfg = { 3u, 2u, 1u };
    E2E_SMType sm;
    E2E_SMInit(&sm);
    /* Reach VALID state */
    E2E_SMCheck(&cfg, &sm, E2E_STATUS_OK);  /* NODATA → INIT */
    E2E_SMCheck(&cfg, &sm, E2E_STATUS_OK);  /* INIT → VALID */
    TEST_ASSERT_EQUAL(E2E_SM_VALID, sm.State);
    /* Now send errors to trigger INVALID */
    E2E_SMCheck(&cfg, &sm, E2E_STATUS_ERROR);
    TEST_ASSERT_EQUAL(E2E_SM_VALID, sm.State);  /* 1 error, need 2 */
    E2E_SMStateType state = E2E_SMCheck(&cfg, &sm, E2E_STATUS_ERROR);
    TEST_ASSERT_EQUAL(E2E_SM_INVALID, state);
}

void test_SMCheck_Invalid_ToValid_AfterRecovery(void)
{
    E2E_SMConfigType cfg = { .WindowSizeValid = 3u, .WindowSizeInvalid = 2u, .WindowSizeInit = 1u };
    E2E_SMType sm;
    E2E_SMInit(&sm);
    /* Reach INVALID */
    E2E_SMCheck(&cfg, &sm, E2E_STATUS_ERROR);  /* NODATA → INIT */
    E2E_SMCheck(&cfg, &sm, E2E_STATUS_ERROR);  /* INIT → INVALID */
    TEST_ASSERT_EQUAL(E2E_SM_INVALID, sm.State);
    /* Recovery: need WindowSizeValid(3) consecutive OKs */
    E2E_SMCheck(&cfg, &sm, E2E_STATUS_OK);     /* OkCount=1 */
    TEST_ASSERT_EQUAL(E2E_SM_INVALID, sm.State);
    E2E_SMCheck(&cfg, &sm, E2E_STATUS_OK);     /* OkCount=2 */
    TEST_ASSERT_EQUAL(E2E_SM_INVALID, sm.State);
    E2E_SMStateType state = E2E_SMCheck(&cfg, &sm, E2E_STATUS_OK);  /* OkCount=3 → VALID */
    TEST_ASSERT_EQUAL(E2E_SM_VALID, state);
}

void test_SMCheck_Invalid_Recovery_ResetByError(void)
{
    E2E_SMConfigType cfg = { 3u, 2u, 1u };
    E2E_SMType sm;
    E2E_SMInit(&sm);
    /* Reach INVALID */
    E2E_SMCheck(&cfg, &sm, E2E_STATUS_ERROR);
    E2E_SMCheck(&cfg, &sm, E2E_STATUS_ERROR);
    TEST_ASSERT_EQUAL(E2E_SM_INVALID, sm.State);
    /* Start recovery */
    E2E_SMCheck(&cfg, &sm, E2E_STATUS_OK);     /* OkCount=1 */
    E2E_SMCheck(&cfg, &sm, E2E_STATUS_OK);     /* OkCount=2 */
    /* Error resets OkCount */
    E2E_SMCheck(&cfg, &sm, E2E_STATUS_ERROR);
    TEST_ASSERT_EQUAL(E2E_SM_INVALID, sm.State);
    TEST_ASSERT_EQUAL_UINT8(0u, sm.OkCount);
}

void test_SMCheck_RepeatedStatus_CountsAsError(void)
{
    E2E_SMConfigType cfg = { 3u, 2u, 1u };
    E2E_SMType sm;
    E2E_SMInit(&sm);
    /* NODATA → INIT with REPEATED (counts as error) */
    E2E_SMCheck(&cfg, &sm, E2E_STATUS_REPEATED);
    TEST_ASSERT_EQUAL(E2E_SM_INIT, sm.State);
    /* Second REPEATED → INVALID (ErrCount=2 >= WindowSizeInvalid=2) */
    E2E_SMStateType state = E2E_SMCheck(&cfg, &sm, E2E_STATUS_REPEATED);
    TEST_ASSERT_EQUAL(E2E_SM_INVALID, state);
}

void test_SMCheck_WrongSeqStatus_CountsAsError(void)
{
    E2E_SMConfigType cfg = { 3u, 2u, 1u };
    E2E_SMType sm;
    E2E_SMInit(&sm);
    E2E_SMCheck(&cfg, &sm, E2E_STATUS_WRONG_SEQ);
    E2E_SMStateType state = E2E_SMCheck(&cfg, &sm, E2E_STATUS_WRONG_SEQ);
    TEST_ASSERT_EQUAL(E2E_SM_INVALID, state);
}

/* ================================================================
 * Unity main
 * ================================================================ */

int main(void)
{
    UNITY_BEGIN();

    /* E2E_Protect NULL/invalid */
    RUN_TEST(test_Protect_NullConfig_ReturnsNotOk);
    RUN_TEST(test_Protect_NullState_ReturnsNotOk);
    RUN_TEST(test_Protect_NullData_ReturnsNotOk);
    RUN_TEST(test_Protect_AllNull_ReturnsNotOk);
    RUN_TEST(test_Protect_LengthTooShort_ReturnsNotOk);
    RUN_TEST(test_Protect_LengthZero_ReturnsNotOk);
    RUN_TEST(test_Protect_LengthMismatch_ReturnsNotOk);
    RUN_TEST(test_Protect_LengthExactlyPayloadOffset_Mismatch);
    RUN_TEST(test_Protect_ValidParams_ReturnsOk);
    RUN_TEST(test_Protect_CounterIncrements);
    RUN_TEST(test_Protect_CounterWrapsAt15);
    RUN_TEST(test_Protect_DataIdInByte0);
    RUN_TEST(test_Protect_CrcWrittenToByte1);

    /* E2E_Check NULL/invalid */
    RUN_TEST(test_Check_NullConfig_ReturnsError);
    RUN_TEST(test_Check_NullState_ReturnsError);
    RUN_TEST(test_Check_NullData_ReturnsError);
    RUN_TEST(test_Check_LengthTooShort_ReturnsError);
    RUN_TEST(test_Check_LengthZero_ReturnsError);
    RUN_TEST(test_Check_LengthMismatch_ReturnsError);

    /* E2E_Check CRC */
    RUN_TEST(test_Check_ValidPdu_ReturnsOk);
    RUN_TEST(test_Check_CorruptedCrc_ReturnsError);
    RUN_TEST(test_Check_CorruptedPayload_ReturnsError);
    RUN_TEST(test_Check_WrongDataId_ReturnsError);

    /* E2E_Check alive counter */
    RUN_TEST(test_Check_RepeatedCounter_ReturnsRepeated);
    RUN_TEST(test_Check_ConsecutiveCounters_ReturnsOk);
    RUN_TEST(test_Check_CounterGapWithinMax_ReturnsOk);
    RUN_TEST(test_Check_CounterGapExceedsMax_ReturnsWrongSeq);
    RUN_TEST(test_Check_CounterWraparound15to0_ReturnsOk);
    RUN_TEST(test_Check_CounterWraparound_Gap2_ReturnsOk);

    /* CRC utility */
    RUN_TEST(test_CalcCRC8_EmptyData_ReturnsXorOfInit);
    RUN_TEST(test_CalcCRC8_SingleByte_Deterministic);
    RUN_TEST(test_CalcCRC8_DifferentData_DifferentCrc);

    /* SM Init */
    RUN_TEST(test_SMInit_NullPtr_NoCrash);
    RUN_TEST(test_SMInit_SetsNodata);

    /* SM Check transitions */
    RUN_TEST(test_SMCheck_NullConfig_ReturnsInvalid);
    RUN_TEST(test_SMCheck_NullSM_ReturnsInvalid);
    RUN_TEST(test_SMCheck_Nodata_ToInit_OnFirstCheck);
    RUN_TEST(test_SMCheck_Nodata_ToInit_EvenOnError);
    RUN_TEST(test_SMCheck_Init_ToValid_AfterWindowSizeInit);
    RUN_TEST(test_SMCheck_Init_ToInvalid_AfterWindowSizeInvalid);
    RUN_TEST(test_SMCheck_Valid_ToInvalid_AfterConsecutiveErrors);
    RUN_TEST(test_SMCheck_Invalid_ToValid_AfterRecovery);
    RUN_TEST(test_SMCheck_Invalid_Recovery_ResetByError);
    RUN_TEST(test_SMCheck_RepeatedStatus_CountsAsError);
    RUN_TEST(test_SMCheck_WrongSeqStatus_CountsAsError);

    return UNITY_END();
}
