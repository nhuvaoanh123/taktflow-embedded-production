/**
 * @file    Rzc_Cfg.h
 * @brief   RZC configuration -- all RZC-specific ID definitions
 * @date    2026-03-10
 *
 * GENERATED -- DO NOT EDIT
 * Source: ARXML architecture migration (tools/arxml/codegen.py)
 *
 * @standard AUTOSAR, ISO 26262 Part 6
 * @copyright Taktflow Systems 2026
 */

#ifndef RZC_CFG_H
#define RZC_CFG_H

/* ==================================================================== */
/* RTE Signal IDs (extends BSW well-known IDs at offset 16)
 * ================================================================== */

#define RZC_SIG_TORQUE_CMD                  16u
#define RZC_SIG_TORQUE_ECHO                 17u
#define RZC_SIG_MOTOR_SPEED                 18u
#define RZC_SIG_MOTOR_DIR                   19u
#define RZC_SIG_MOTOR_ENABLE                20u
#define RZC_SIG_MOTOR_FAULT                 21u
#define RZC_SIG_CURRENT_MA                  22u
#define RZC_SIG_OVERCURRENT                 23u
#define RZC_SIG_TEMP1_DC                    24u
#define RZC_SIG_TEMP2_DC                    25u
#define RZC_SIG_DERATING_PCT                26u
#define RZC_SIG_TEMP_FAULT                  27u
#define RZC_SIG_BATTERY_MV                  28u
#define RZC_SIG_BATTERY_STATUS              29u
#define RZC_SIG_ENCODER_SPEED               30u
#define RZC_SIG_ENCODER_DIR                 31u
#define RZC_SIG_ENCODER_STALL               32u
#define RZC_SIG_VEHICLE_STATE               33u
#define RZC_SIG_ESTOP_ACTIVE                34u
#define RZC_SIG_FAULT_MASK                  35u
#define RZC_SIG_SELF_TEST_RESULT            36u
#define RZC_SIG_HEARTBEAT_ALIVE             37u
#define RZC_SIG_SAFETY_STATUS               38u
#define RZC_SIG_CMD_TIMEOUT                 39u
#define RZC_SIG_BATTERY_SOC                 40u
#define RZC_SIG_COUNT                       41u

/* ==================================================================== */
/* Com TX PDU IDs
 * ================================================================== */

#define RZC_COM_TX_HEARTBEAT                0u   /* CAN 0x012 */
#define RZC_COM_TX_MOTOR_STATUS             1u   /* CAN 0x300 */
#define RZC_COM_TX_MOTOR_CURRENT            2u   /* CAN 0x301 */
#define RZC_COM_TX_MOTOR_TEMP               3u   /* CAN 0x302 */
#define RZC_COM_TX_BATTERY_STATUS           4u   /* CAN 0x303 */
#define RZC_COM_TX_DTC_BROADCAST            5u   /* CAN 0x500 */

/* ==================================================================== */
/* Com RX PDU IDs
 * ================================================================== */

#define RZC_COM_RX_ESTOP                    0u   /* CAN 0x001 */
#define RZC_COM_RX_VEHICLE_TORQUE           1u   /* CAN 0x100 */
#define RZC_COM_RX_VIRT_SENSORS             2u   /* CAN 0x601 */

/* ==================================================================== */
/* DTC Event IDs (Dem_EventIdType)
 * ================================================================== */

#define RZC_DTC_OVERCURRENT                 0u
#define RZC_DTC_OVERTEMP                    1u
#define RZC_DTC_STALL                       2u
#define RZC_DTC_DIRECTION                   3u
#define RZC_DTC_SHOOT_THROUGH               4u
#define RZC_DTC_CAN_BUS_OFF                 5u
#define RZC_DTC_CMD_TIMEOUT                 6u
#define RZC_DTC_SELF_TEST_FAIL              7u
#define RZC_DTC_WATCHDOG_FAIL               8u
#define RZC_DTC_BATTERY                     9u
#define RZC_DTC_ENCODER                     10u
#define RZC_DTC_ZERO_CAL                    11u

/* ==================================================================== */
/* E2E Data IDs
 * ================================================================== */

#define RZC_E2E_ESTOP_DATA_ID               0x01u
#define RZC_E2E_HEARTBEAT_DATA_ID           0x04u
#define RZC_E2E_VEHSTATE_DATA_ID            0x05u
#define RZC_E2E_MOTOR_STATUS_DATA_ID        0x0Eu
#define RZC_E2E_MOTOR_CURRENT_DATA_ID       0x0Fu
#define RZC_E2E_MOTOR_TEMP_DATA_ID          0x10u
#define RZC_E2E_BATTERY_DATA_ID             0x11u

/* ==================================================================== */
/* State and Fault Enums
 * ================================================================== */

#define RZC_STATE_COUNT                     6u
#define RZC_STATE_DEGRADED                  2u
#define RZC_STATE_INIT                      0u
#define RZC_STATE_LIMP                      3u
#define RZC_STATE_RUN                       1u
#define RZC_STATE_SAFE_STOP                 4u
#define RZC_STATE_SHUTDOWN                  5u

/* ==================================================================== */
/* Thresholds and Constants
 * ================================================================== */

#define RZC_BATT_NOMINAL_MV                 12600u
#define RZC_CAN_ERR_WARN_TIMEOUT_MS         500u
#define RZC_CAN_SILENCE_TIMEOUT_MS          200u
#define RZC_CURRENT_OC_DEBOUNCE             10u
#define RZC_ENCODER_STALL_MIN_PWM           10u
#define RZC_ENCODER_STALL_TIMEOUT_MS        500u
#define RZC_HB_ALIVE_MAX                    15u
#define RZC_HB_MAX_MISS                     3u
#define RZC_HB_TIMEOUT_MS                   150u
#define RZC_HB_TX_PERIOD_MS                 50u
#define RZC_MOTOR_CMD_TIMEOUT               2u
#define RZC_MOTOR_CMD_TIMEOUT_MS            100u
#define RZC_MOTOR_LIMIT_DEGRADED            75u
#define RZC_MOTOR_LIMIT_LIMP                30u
#define RZC_MOTOR_LIMIT_RUN                 100u
#define RZC_MOTOR_LIMIT_SAFE_STOP           0u
#define RZC_MOTOR_MAX_DUTY_PCT              95u
#define RZC_RTE_PERIOD_MS                   50u
#define RZC_TEMP_MAX_DDC                    1500u

#endif /* RZC_CFG_H */
