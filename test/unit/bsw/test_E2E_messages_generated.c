/**
 * @file    test_E2E_messages_generated.c
 * @brief   Per-message E2E protect/check tests — generated from DBC
 * @date    2026-03-21
 *
 * @verifies SWR-BSW-023, SWR-BSW-024, SWR-BSW-025
 *
 * @details  Tests every E2E-protected CAN message from taktflow_vehicle.dbc.
 *           For each message: roundtrip OK, corrupt CRC, replay counter,
 *           skip counter beyond MaxDeltaCounter, correct increment.
 *           DataIds extracted from BA_ "E2E_DataID" attributes.
 *
 * Generated: 20 messages x 5 scenarios = 100 tests.
 */
#include "unity.h"
#include "E2E.h"

/* ---- Helpers ---- */

/**
 * @brief  Run protect+check roundtrip and return check status
 */
static E2E_CheckStatusType helper_protect_check(
    const E2E_ConfigType* cfg,
    E2E_StateType* tx, E2E_StateType* rx,
    uint8* buf, uint16 len)
{
    (void)E2E_Protect(cfg, tx, buf, len);
    return E2E_Check(cfg, rx, buf, len);
}

/**
 * @brief  Fill payload bytes (offset 2..len-1) with deterministic data
 */
static void helper_fill_payload(uint8* buf, uint16 len)
{
    uint16 i;
    for (i = 0u; i < len; i++) {
        buf[i] = 0u;
    }
    for (i = E2E_PAYLOAD_OFFSET; i < len; i++) {
        buf[i] = (uint8)(0x10u + i);
    }
}

/* ---- Fixtures ---- */

void setUp(void)
{
    E2E_Init();
}

void tearDown(void) { }

/* ==================================================================
 * Macros for per-message test generation
 *
 * MSG_NAME:  C identifier-safe message name
 * CAN_ID:    CAN message ID (decimal, for documentation)
 * DATA_ID:   E2E DataId from DBC BA_ attribute
 * DLC:       PDU length in bytes
 * MAX_DELTA: MaxDeltaCounter (default 2 from DBC BA_DEF_DEF_)
 * ================================================================== */

#define E2E_MSG_TESTS(MSG_NAME, CAN_ID, DATA_ID, DLC, MAX_DELTA)             \
                                                                               \
/* 1. Protect + Check roundtrip → OK */                                        \
void test_E2E_##MSG_NAME##_roundtrip_ok(void)                                 \
{                                                                              \
    E2E_ConfigType cfg = { (DATA_ID), (MAX_DELTA), (DLC) };                   \
    E2E_StateType tx = { 0u };                                                \
    E2E_StateType rx = { 0u };                                                \
    uint8 buf[DLC];                                                           \
    helper_fill_payload(buf, (DLC));                                          \
    E2E_CheckStatusType s = helper_protect_check(&cfg, &tx, &rx, buf, (DLC));\
    TEST_ASSERT_EQUAL(E2E_STATUS_OK, s);                                      \
}                                                                              \
                                                                               \
/* 2. Corrupt CRC byte → ERROR (WRONG_CRC) */                                 \
void test_E2E_##MSG_NAME##_corrupt_crc(void)                                  \
{                                                                              \
    E2E_ConfigType cfg = { (DATA_ID), (MAX_DELTA), (DLC) };                   \
    E2E_StateType tx = { 0u };                                                \
    E2E_StateType rx = { 0u };                                                \
    uint8 buf[DLC];                                                           \
    helper_fill_payload(buf, (DLC));                                          \
    (void)E2E_Protect(&cfg, &tx, buf, (DLC));                                \
    buf[E2E_BYTE_CRC] ^= 0xFFu;  /* Flip all CRC bits */                     \
    E2E_CheckStatusType s = E2E_Check(&cfg, &rx, buf, (DLC));                \
    TEST_ASSERT_EQUAL(E2E_STATUS_ERROR, s);                                   \
}                                                                              \
                                                                               \
/* 3. Replay same counter → REPEATED */                                        \
void test_E2E_##MSG_NAME##_replay_counter(void)                               \
{                                                                              \
    E2E_ConfigType cfg = { (DATA_ID), (MAX_DELTA), (DLC) };                   \
    E2E_StateType tx = { 0u };                                                \
    E2E_StateType rx = { 0u };                                                \
    uint8 buf[DLC];                                                           \
    helper_fill_payload(buf, (DLC));                                          \
    (void)E2E_Protect(&cfg, &tx, buf, (DLC));                                \
    (void)E2E_Check(&cfg, &rx, buf, (DLC));                                  \
    /* Same PDU again — counter unchanged */                                   \
    E2E_CheckStatusType s = E2E_Check(&cfg, &rx, buf, (DLC));                \
    TEST_ASSERT_EQUAL(E2E_STATUS_REPEATED, s);                                \
}                                                                              \
                                                                               \
/* 4. Skip counter by MaxDeltaCounter+1 → WRONG_SEQUENCE */                   \
void test_E2E_##MSG_NAME##_wrong_sequence(void)                               \
{                                                                              \
    E2E_ConfigType cfg = { (DATA_ID), (MAX_DELTA), (DLC) };                   \
    E2E_StateType tx = { 0u };                                                \
    E2E_StateType rx = { 0u };                                                \
    uint8 buf[DLC];                                                           \
    helper_fill_payload(buf, (DLC));                                          \
    /* First valid message */                                                  \
    (void)E2E_Protect(&cfg, &tx, buf, (DLC));                                \
    (void)E2E_Check(&cfg, &rx, buf, (DLC));                                  \
    /* Skip MaxDelta+1 protects (only last one arrives) */                     \
    {                                                                          \
        uint8 skip;                                                            \
        for (skip = 0u; skip <= (MAX_DELTA); skip++) {                        \
            (void)E2E_Protect(&cfg, &tx, buf, (DLC));                         \
        }                                                                      \
    }                                                                          \
    E2E_CheckStatusType s = E2E_Check(&cfg, &rx, buf, (DLC));                \
    TEST_ASSERT_EQUAL(E2E_STATUS_WRONG_SEQ, s);                              \
}                                                                              \
                                                                               \
/* 5. Correct counter increment → OK */                                        \
void test_E2E_##MSG_NAME##_correct_increment(void)                            \
{                                                                              \
    E2E_ConfigType cfg = { (DATA_ID), (MAX_DELTA), (DLC) };                   \
    E2E_StateType tx = { 0u };                                                \
    E2E_StateType rx = { 0u };                                                \
    uint8 buf[DLC];                                                           \
    helper_fill_payload(buf, (DLC));                                          \
    /* First message establishes baseline */                                   \
    (void)E2E_Protect(&cfg, &tx, buf, (DLC));                                \
    (void)E2E_Check(&cfg, &rx, buf, (DLC));                                  \
    /* Second message: counter increments by 1 → OK */                         \
    (void)E2E_Protect(&cfg, &tx, buf, (DLC));                                \
    E2E_CheckStatusType s = E2E_Check(&cfg, &rx, buf, (DLC));                \
    TEST_ASSERT_EQUAL(E2E_STATUS_OK, s);                                      \
}

/* ==================================================================
 * E2E Message Definitions from taktflow_vehicle.dbc
 *
 * Format: E2E_MSG_TESTS(Name, CAN_ID, DataId, DLC, MaxDeltaCounter)
 *
 * DataIds from BA_ "E2E_DataID" BO_ <id> <value>;
 * MaxDeltaCounter default = 2 (BA_DEF_DEF_ "E2E_MaxDeltaCounter" 2)
 * DLC from BO_ message definition
 * ================================================================== */

/*  1 */ E2E_MSG_TESTS(EStop_Broadcast,    1,    1u,  4u, 2u)
/*  2 */ E2E_MSG_TESTS(CVC_Heartbeat,     16,    2u,  4u, 2u)
/*  3 */ E2E_MSG_TESTS(FZC_Heartbeat,     17,    3u,  4u, 2u)
/*  4 */ E2E_MSG_TESTS(RZC_Heartbeat,     18,    4u,  4u, 2u)
/*  5 */ E2E_MSG_TESTS(SC_Status,         19,    0u,  4u, 2u)
/*  6 */ E2E_MSG_TESTS(ICU_Heartbeat,     20,    5u,  4u, 2u)
/*  7 */ E2E_MSG_TESTS(TCU_Heartbeat,     21,    6u,  4u, 2u)
/*  8 */ E2E_MSG_TESTS(BCM_Heartbeat,     22,    7u,  4u, 2u)
/*  9 */ E2E_MSG_TESTS(Vehicle_State,    256,    5u,  6u, 2u)
/* 10 */ E2E_MSG_TESTS(Torque_Request,   257,    6u,  8u, 2u)
/* 11 */ E2E_MSG_TESTS(Steer_Command,    258,    7u,  8u, 2u)
/* 12 */ E2E_MSG_TESTS(Brake_Command,    259,    8u,  8u, 2u)
/* 13 */ E2E_MSG_TESTS(Steering_Status,  512,    9u,  8u, 2u)
/* 14 */ E2E_MSG_TESTS(Brake_Status,     513,   10u,  8u, 2u)
/* 15 */ E2E_MSG_TESTS(Brake_Fault,      528,   11u,  4u, 2u)
/* 16 */ E2E_MSG_TESTS(Motor_Cutoff_Req, 529,   12u,  4u, 2u)
/* 17 */ E2E_MSG_TESTS(Lidar_Distance,   544,   13u,  8u, 2u)
/* 18 */ E2E_MSG_TESTS(Motor_Status,     768,   14u,  8u, 2u)
/* 19 */ E2E_MSG_TESTS(Motor_Current,    769,   15u,  8u, 2u)
/* 20 */ E2E_MSG_TESTS(Motor_Temperature,770,    0u,  8u, 2u)

/* ==================================================================
 * Unity Test Runner
 * ================================================================== */

int main(void)
{
    UNITY_BEGIN();

    /* 1. EStop_Broadcast (CAN 0x001, DataId=1, DLC=4) */
    RUN_TEST(test_E2E_EStop_Broadcast_roundtrip_ok);
    RUN_TEST(test_E2E_EStop_Broadcast_corrupt_crc);
    RUN_TEST(test_E2E_EStop_Broadcast_replay_counter);
    RUN_TEST(test_E2E_EStop_Broadcast_wrong_sequence);
    RUN_TEST(test_E2E_EStop_Broadcast_correct_increment);

    /* 2. CVC_Heartbeat (CAN 0x010, DataId=2, DLC=4) */
    RUN_TEST(test_E2E_CVC_Heartbeat_roundtrip_ok);
    RUN_TEST(test_E2E_CVC_Heartbeat_corrupt_crc);
    RUN_TEST(test_E2E_CVC_Heartbeat_replay_counter);
    RUN_TEST(test_E2E_CVC_Heartbeat_wrong_sequence);
    RUN_TEST(test_E2E_CVC_Heartbeat_correct_increment);

    /* 3. FZC_Heartbeat (CAN 0x011, DataId=3, DLC=4) */
    RUN_TEST(test_E2E_FZC_Heartbeat_roundtrip_ok);
    RUN_TEST(test_E2E_FZC_Heartbeat_corrupt_crc);
    RUN_TEST(test_E2E_FZC_Heartbeat_replay_counter);
    RUN_TEST(test_E2E_FZC_Heartbeat_wrong_sequence);
    RUN_TEST(test_E2E_FZC_Heartbeat_correct_increment);

    /* 4. RZC_Heartbeat (CAN 0x012, DataId=4, DLC=4) */
    RUN_TEST(test_E2E_RZC_Heartbeat_roundtrip_ok);
    RUN_TEST(test_E2E_RZC_Heartbeat_corrupt_crc);
    RUN_TEST(test_E2E_RZC_Heartbeat_replay_counter);
    RUN_TEST(test_E2E_RZC_Heartbeat_wrong_sequence);
    RUN_TEST(test_E2E_RZC_Heartbeat_correct_increment);

    /* 5. SC_Status (CAN 0x013, DataId=0, DLC=4) */
    RUN_TEST(test_E2E_SC_Status_roundtrip_ok);
    RUN_TEST(test_E2E_SC_Status_corrupt_crc);
    RUN_TEST(test_E2E_SC_Status_replay_counter);
    RUN_TEST(test_E2E_SC_Status_wrong_sequence);
    RUN_TEST(test_E2E_SC_Status_correct_increment);

    /* 6. ICU_Heartbeat (CAN 0x014, DataId=5, DLC=4) */
    RUN_TEST(test_E2E_ICU_Heartbeat_roundtrip_ok);
    RUN_TEST(test_E2E_ICU_Heartbeat_corrupt_crc);
    RUN_TEST(test_E2E_ICU_Heartbeat_replay_counter);
    RUN_TEST(test_E2E_ICU_Heartbeat_wrong_sequence);
    RUN_TEST(test_E2E_ICU_Heartbeat_correct_increment);

    /* 7. TCU_Heartbeat (CAN 0x015, DataId=6, DLC=4) */
    RUN_TEST(test_E2E_TCU_Heartbeat_roundtrip_ok);
    RUN_TEST(test_E2E_TCU_Heartbeat_corrupt_crc);
    RUN_TEST(test_E2E_TCU_Heartbeat_replay_counter);
    RUN_TEST(test_E2E_TCU_Heartbeat_wrong_sequence);
    RUN_TEST(test_E2E_TCU_Heartbeat_correct_increment);

    /* 8. BCM_Heartbeat (CAN 0x016, DataId=7, DLC=4) */
    RUN_TEST(test_E2E_BCM_Heartbeat_roundtrip_ok);
    RUN_TEST(test_E2E_BCM_Heartbeat_corrupt_crc);
    RUN_TEST(test_E2E_BCM_Heartbeat_replay_counter);
    RUN_TEST(test_E2E_BCM_Heartbeat_wrong_sequence);
    RUN_TEST(test_E2E_BCM_Heartbeat_correct_increment);

    /* 9. Vehicle_State (CAN 0x100, DataId=5, DLC=6) */
    RUN_TEST(test_E2E_Vehicle_State_roundtrip_ok);
    RUN_TEST(test_E2E_Vehicle_State_corrupt_crc);
    RUN_TEST(test_E2E_Vehicle_State_replay_counter);
    RUN_TEST(test_E2E_Vehicle_State_wrong_sequence);
    RUN_TEST(test_E2E_Vehicle_State_correct_increment);

    /* 10. Torque_Request (CAN 0x101, DataId=6, DLC=8) */
    RUN_TEST(test_E2E_Torque_Request_roundtrip_ok);
    RUN_TEST(test_E2E_Torque_Request_corrupt_crc);
    RUN_TEST(test_E2E_Torque_Request_replay_counter);
    RUN_TEST(test_E2E_Torque_Request_wrong_sequence);
    RUN_TEST(test_E2E_Torque_Request_correct_increment);

    /* 11. Steer_Command (CAN 0x102, DataId=7, DLC=8) */
    RUN_TEST(test_E2E_Steer_Command_roundtrip_ok);
    RUN_TEST(test_E2E_Steer_Command_corrupt_crc);
    RUN_TEST(test_E2E_Steer_Command_replay_counter);
    RUN_TEST(test_E2E_Steer_Command_wrong_sequence);
    RUN_TEST(test_E2E_Steer_Command_correct_increment);

    /* 12. Brake_Command (CAN 0x103, DataId=8, DLC=8) */
    RUN_TEST(test_E2E_Brake_Command_roundtrip_ok);
    RUN_TEST(test_E2E_Brake_Command_corrupt_crc);
    RUN_TEST(test_E2E_Brake_Command_replay_counter);
    RUN_TEST(test_E2E_Brake_Command_wrong_sequence);
    RUN_TEST(test_E2E_Brake_Command_correct_increment);

    /* 13. Steering_Status (CAN 0x200, DataId=9, DLC=8) */
    RUN_TEST(test_E2E_Steering_Status_roundtrip_ok);
    RUN_TEST(test_E2E_Steering_Status_corrupt_crc);
    RUN_TEST(test_E2E_Steering_Status_replay_counter);
    RUN_TEST(test_E2E_Steering_Status_wrong_sequence);
    RUN_TEST(test_E2E_Steering_Status_correct_increment);

    /* 14. Brake_Status (CAN 0x201, DataId=10, DLC=8) */
    RUN_TEST(test_E2E_Brake_Status_roundtrip_ok);
    RUN_TEST(test_E2E_Brake_Status_corrupt_crc);
    RUN_TEST(test_E2E_Brake_Status_replay_counter);
    RUN_TEST(test_E2E_Brake_Status_wrong_sequence);
    RUN_TEST(test_E2E_Brake_Status_correct_increment);

    /* 15. Brake_Fault (CAN 0x210, DataId=11, DLC=4) */
    RUN_TEST(test_E2E_Brake_Fault_roundtrip_ok);
    RUN_TEST(test_E2E_Brake_Fault_corrupt_crc);
    RUN_TEST(test_E2E_Brake_Fault_replay_counter);
    RUN_TEST(test_E2E_Brake_Fault_wrong_sequence);
    RUN_TEST(test_E2E_Brake_Fault_correct_increment);

    /* 16. Motor_Cutoff_Req (CAN 0x211, DataId=12, DLC=4) */
    RUN_TEST(test_E2E_Motor_Cutoff_Req_roundtrip_ok);
    RUN_TEST(test_E2E_Motor_Cutoff_Req_corrupt_crc);
    RUN_TEST(test_E2E_Motor_Cutoff_Req_replay_counter);
    RUN_TEST(test_E2E_Motor_Cutoff_Req_wrong_sequence);
    RUN_TEST(test_E2E_Motor_Cutoff_Req_correct_increment);

    /* 17. Lidar_Distance (CAN 0x220, DataId=13, DLC=8) */
    RUN_TEST(test_E2E_Lidar_Distance_roundtrip_ok);
    RUN_TEST(test_E2E_Lidar_Distance_corrupt_crc);
    RUN_TEST(test_E2E_Lidar_Distance_replay_counter);
    RUN_TEST(test_E2E_Lidar_Distance_wrong_sequence);
    RUN_TEST(test_E2E_Lidar_Distance_correct_increment);

    /* 18. Motor_Status (CAN 0x300, DataId=14, DLC=8) */
    RUN_TEST(test_E2E_Motor_Status_roundtrip_ok);
    RUN_TEST(test_E2E_Motor_Status_corrupt_crc);
    RUN_TEST(test_E2E_Motor_Status_replay_counter);
    RUN_TEST(test_E2E_Motor_Status_wrong_sequence);
    RUN_TEST(test_E2E_Motor_Status_correct_increment);

    /* 19. Motor_Current (CAN 0x301, DataId=15, DLC=8) */
    RUN_TEST(test_E2E_Motor_Current_roundtrip_ok);
    RUN_TEST(test_E2E_Motor_Current_corrupt_crc);
    RUN_TEST(test_E2E_Motor_Current_replay_counter);
    RUN_TEST(test_E2E_Motor_Current_wrong_sequence);
    RUN_TEST(test_E2E_Motor_Current_correct_increment);

    /* 20. Motor_Temperature (CAN 0x302, DataId=0, DLC=8) */
    RUN_TEST(test_E2E_Motor_Temperature_roundtrip_ok);
    RUN_TEST(test_E2E_Motor_Temperature_corrupt_crc);
    RUN_TEST(test_E2E_Motor_Temperature_replay_counter);
    RUN_TEST(test_E2E_Motor_Temperature_wrong_sequence);
    RUN_TEST(test_E2E_Motor_Temperature_correct_increment);

    return UNITY_END();
}
