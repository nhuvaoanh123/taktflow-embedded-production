/**
 * @file    Fzc_Cfg.h
 * @brief   FZC configuration -- all FZC-specific ID definitions
 * @date    2026-03-10
 *
 * GENERATED -- DO NOT EDIT
 * Source: ARXML architecture migration (tools/arxml/codegen.py)
 *
 * @standard AUTOSAR, ISO 26262 Part 6
 * @copyright Taktflow Systems 2026
 */

#ifndef FZC_CFG_H
#define FZC_CFG_H

/* ==================================================================== */
/* RTE Signal IDs (extends BSW well-known IDs at offset 16)
 * ================================================================== */

#define FZC_SIG_STEER_CMD                   16u
#define FZC_SIG_STEER_ANGLE                 17u
#define FZC_SIG_STEER_FAULT                 18u
#define FZC_SIG_BRAKE_CMD                   19u
#define FZC_SIG_BRAKE_POS                   20u
#define FZC_SIG_BRAKE_FAULT                 21u
#define FZC_SIG_LIDAR_DIST                  22u
#define FZC_SIG_LIDAR_SIGNAL                23u
#define FZC_SIG_LIDAR_ZONE                  24u
#define FZC_SIG_LIDAR_FAULT                 25u
#define FZC_SIG_VEHICLE_STATE               26u
#define FZC_SIG_ESTOP_ACTIVE                27u
#define FZC_SIG_BUZZER_PATTERN              28u
#define FZC_SIG_MOTOR_CUTOFF                29u
#define FZC_SIG_FAULT_MASK                  30u
#define FZC_SIG_STEER_PWM_DISABLE           31u
#define FZC_SIG_BRAKE_PWM_DISABLE           32u
#define FZC_SIG_SELF_TEST_RESULT            33u
#define FZC_SIG_HEARTBEAT_ALIVE             34u
#define FZC_SIG_SAFETY_STATUS               35u
#define FZC_SIG_COUNT                       36u

/* ==================================================================== */
/* Com TX PDU IDs
 * ================================================================== */

#define FZC_COM_TX_HEARTBEAT                0u   /* CAN 0x011 */
#define FZC_COM_TX_STEER_STATUS             1u   /* CAN 0x200 */
#define FZC_COM_TX_BRAKE_STATUS             2u   /* CAN 0x201 */
#define FZC_COM_TX_BRAKE_FAULT              3u   /* CAN 0x210 */
#define FZC_COM_TX_MOTOR_CUTOFF             4u   /* CAN 0x211 */
#define FZC_COM_TX_LIDAR                    5u   /* CAN 0x220 */
#define FZC_COM_TX_DTC_BROADCAST            6u   /* CAN 0x500 */

/* ==================================================================== */
/* Com RX PDU IDs
 * ================================================================== */

#define FZC_COM_RX_ESTOP                    0u   /* CAN 0x001 */
#define FZC_COM_RX_VEHICLE_STATE            1u   /* CAN 0x100 */
#define FZC_COM_RX_STEER_CMD                2u   /* CAN 0x102 */
#define FZC_COM_RX_BRAKE_CMD                3u   /* CAN 0x103 */
#define FZC_COM_RX_VIRT_SENSORS             4u   /* CAN 0x600 */

/* ==================================================================== */
/* DTC Event IDs (Dem_EventIdType)
 * ================================================================== */

#define FZC_DTC_STEER_PLAUSIBILITY          0u
#define FZC_DTC_STEER_RANGE                 1u
#define FZC_DTC_STEER_RATE                  2u
#define FZC_DTC_STEER_TIMEOUT               3u
#define FZC_DTC_STEER_SPI_FAIL              4u
#define FZC_DTC_BRAKE_FAULT                 5u
#define FZC_DTC_BRAKE_TIMEOUT               6u
#define FZC_DTC_BRAKE_PWM_FAIL              7u
#define FZC_DTC_LIDAR_TIMEOUT               8u
#define FZC_DTC_LIDAR_CHECKSUM              9u
#define FZC_DTC_LIDAR_STUCK                 10u
#define FZC_DTC_LIDAR_SIGNAL_LOW            11u
#define FZC_DTC_CAN_BUS_OFF                 12u
#define FZC_DTC_SELF_TEST_FAIL              13u
#define FZC_DTC_WATCHDOG_FAIL               14u
#define FZC_DTC_BRAKE_OSCILLATION           15u

/* ==================================================================== */
/* E2E Data IDs
 * ================================================================== */

#define FZC_E2E_ESTOP_DATA_ID               0x01u
#define FZC_E2E_HEARTBEAT_DATA_ID           0x03u
#define FZC_E2E_VEHSTATE_DATA_ID            0x10u
#define FZC_E2E_STEER_CMD_DATA_ID           0x12u
#define FZC_E2E_BRAKE_CMD_DATA_ID           0x13u
#define FZC_E2E_STEER_STATUS_DATA_ID        0x20u
#define FZC_E2E_BRAKE_STATUS_DATA_ID        0x21u
#define FZC_E2E_LIDAR_DATA_ID               0x22u

/* ==================================================================== */
/* State and Fault Enums
 * ================================================================== */

#define FZC_BRAKE_FAULT_DEBOUNCE            3u
#define FZC_BRAKE_NO_FAULT                  0u
#define FZC_BRAKE_PWM_FAULT_THRESH          2u
#define FZC_STATE_COUNT                     6u
#define FZC_STATE_DEGRADED                  2u
#define FZC_STATE_INIT                      0u
#define FZC_STATE_LIMP                      3u
#define FZC_STATE_RUN                       1u
#define FZC_STATE_SAFE_STOP                 4u
#define FZC_STATE_SHUTDOWN                  5u
#define FZC_STEER_NO_FAULT                  0u

/* ==================================================================== */
/* Thresholds and Constants
 * ================================================================== */

#define FZC_BRAKE_AUTO_TIMEOUT_MS           100u
#define FZC_BRAKE_CMD_TIMEOUT               2u
#define FZC_BRAKE_LATCH_CLEAR_CYCLES        50u
#define FZC_BRAKE_OSCILLATION_DEBOUNCE      4u
#define FZC_BRAKE_PWM_MAX                   100u
#define FZC_BRAKE_PWM_MIN                   0u
#define FZC_HB_ALIVE_MAX                    15u
#define FZC_HB_MAX_MISS                     3u
#define FZC_HB_TIMEOUT_MS                   150u
#define FZC_HB_TX_PERIOD_MS                 50u
#define FZC_LIDAR_DEGRADE_CYCLES            200u
#define FZC_LIDAR_RANGE_MAX_CM              1200u
#define FZC_LIDAR_RANGE_MIN_CM              2u
#define FZC_LIDAR_SIGNAL_MIN                100u
#define FZC_LIDAR_STUCK_CYCLES              50u
#define FZC_LIDAR_TIMEOUT_MS                100u
#define FZC_POST_INIT_GRACE_CYCLES          0u
#define FZC_RTE_PERIOD_MS                   10u
#define FZC_STEER_ANGLE_MAX                 45u
#define FZC_STEER_CMD_TIMEOUT               4u
#define FZC_STEER_CMD_TIMEOUT_MS            100u
#define FZC_STEER_LATCH_CLEAR_CYCLES        50u
#define FZC_STEER_PLAUS_DEBOUNCE            5u
#define FZC_STEER_PLAUS_THRESHOLD_DEG       5u
#define FZC_STEER_PWM_MAX_US                2000u
#define FZC_STEER_PWM_MIN_US                1000u
#define FZC_STEER_RATE_LIMIT_DEG_10MS       3u

#endif /* FZC_CFG_H */
