/**
 * @file    Cvc_Cfg.h
 * @brief   CVC configuration -- all CVC-specific ID definitions
 * @date    2026-03-10
 *
 * GENERATED -- DO NOT EDIT
 * Source: ARXML architecture migration (tools/arxml/codegen.py)
 *
 * @standard AUTOSAR, ISO 26262 Part 6
 * @copyright Taktflow Systems 2026
 */

#ifndef CVC_CFG_H
#define CVC_CFG_H

/* ==================================================================== */
/* RTE Signal IDs (extends BSW well-known IDs at offset 16)
 * ================================================================== */

#define CVC_SIG_PEDAL_RAW_1                 16u
#define CVC_SIG_PEDAL_RAW_2                 17u
#define CVC_SIG_PEDAL_POSITION              18u
#define CVC_SIG_PEDAL_FAULT                 19u
#define CVC_SIG_VEHICLE_STATE               20u
#define CVC_SIG_TORQUE_REQUEST              21u
#define CVC_SIG_ESTOP_ACTIVE                22u
#define CVC_SIG_FZC_COMM_STATUS             23u
#define CVC_SIG_RZC_COMM_STATUS             24u
#define CVC_SIG_MOTOR_SPEED                 25u
#define CVC_SIG_FAULT_MASK                  26u
#define CVC_SIG_MOTOR_CURRENT               27u
#define CVC_SIG_MOTOR_CUTOFF                28u
#define CVC_SIG_STEERING_FAULT              29u
#define CVC_SIG_BRAKE_FAULT                 30u
#define CVC_SIG_SC_RELAY_KILL               31u
#define CVC_SIG_BATTERY_STATUS              32u
#define CVC_SIG_MOTOR_FAULT_RZC             33u
#define CVC_SIG_COUNT                       34u

/* ==================================================================== */
/* Com TX PDU IDs
 * ================================================================== */

#define CVC_COM_TX_ESTOP                    0u   /* CAN 0x001 */
#define CVC_COM_TX_HEARTBEAT                1u   /* CAN 0x010 */
#define CVC_COM_TX_VEHICLE_STATE            2u   /* CAN 0x100 */
#define CVC_COM_TX_TORQUE_REQ               3u   /* CAN 0x101 */
#define CVC_COM_TX_STEER_CMD                4u   /* CAN 0x102 */
#define CVC_COM_TX_BRAKE_CMD                5u   /* CAN 0x103 */
#define CVC_COM_TX_BODY_CMD                 6u   /* CAN 0x350 */
#define CVC_COM_TX_UDS_RSP                  7u   /* CAN 0x7E8 */
#define CVC_COM_TX_DTC                      8u   /* CAN 0x500 */

/* ==================================================================== */
/* Com RX PDU IDs
 * ================================================================== */

#define CVC_COM_RX_FZC_HB                   0u   /* CAN 0x011 */
#define CVC_COM_RX_RZC_HB                   1u   /* CAN 0x012 */
#define CVC_COM_RX_BRAKE_FAULT              2u   /* CAN 0x210 */
#define CVC_COM_RX_MOTOR_CUTOFF             3u   /* CAN 0x211 */
#define CVC_COM_RX_LIDAR                    4u   /* CAN 0x220 */
#define CVC_COM_RX_MOTOR_CURRENT            5u   /* CAN 0x301 */
#define CVC_COM_RX_SC_RELAY                 6u   /* CAN 0x013 */
#define CVC_COM_RX_BATTERY_STATUS           7u   /* CAN 0x303 */
#define CVC_COM_RX_ESTOP_INJECT             8u   /* CAN 0x001 */
#define CVC_COM_RX_STEER_STATUS             9u   /* CAN 0x200 */
#define CVC_COM_RX_MOTOR_STATUS             10u   /* CAN 0x300 */

/* ==================================================================== */
/* DTC Event IDs (Dem_EventIdType)
 * ================================================================== */

#define CVC_DTC_PEDAL_PLAUSIBILITY          0u
#define CVC_DTC_PEDAL_SENSOR1_FAIL          1u
#define CVC_DTC_PEDAL_SENSOR2_FAIL          2u
#define CVC_DTC_PEDAL_STUCK                 3u
#define CVC_DTC_CAN_FZC_TIMEOUT             4u
#define CVC_DTC_CAN_RZC_TIMEOUT             5u
#define CVC_DTC_CAN_BUS_OFF                 6u
#define CVC_DTC_MOTOR_OVERCURRENT           7u
#define CVC_DTC_MOTOR_OVERTEMP              8u
#define CVC_DTC_MOTOR_CUTOFF_RX             9u
#define CVC_DTC_BRAKE_FAULT_RX              10u
#define CVC_DTC_STEERING_FAULT_RX           11u
#define CVC_DTC_ESTOP_ACTIVATED             12u
#define CVC_DTC_BATT_UNDERVOLT              13u
#define CVC_DTC_BATT_OVERVOLT               14u
#define CVC_DTC_NVM_CRC_FAIL                15u
#define CVC_DTC_SELF_TEST_FAIL              16u
#define CVC_DTC_DISPLAY_COMM                17u
#define CVC_DTC_CREEP_FAULT                 18u

/* ==================================================================== */
/* E2E Data IDs
 * ================================================================== */

#define CVC_E2E_ESTOP_DATA_ID               0x01u
#define CVC_E2E_HEARTBEAT_DATA_ID           0x02u
#define CVC_E2E_VEHSTATE_DATA_ID            0x05u
#define CVC_E2E_TORQUE_DATA_ID              0x06u

/* ==================================================================== */
/* State and Fault Enums
 * ================================================================== */

#define CVC_COMM_OK                         0u
#define CVC_COMM_TIMEOUT                    1u
#define CVC_PEDAL_LATCH_CLEAR_CYCLES        50u
#define CVC_PEDAL_MAX_DEGRADED              750u
#define CVC_PEDAL_MAX_LIMP                  300u
#define CVC_PEDAL_MAX_RUN                   1000u
#define CVC_PEDAL_NO_FAULT                  0u
#define CVC_PEDAL_PLAUSIBILITY              1u
#define CVC_PEDAL_PLAUS_DEBOUNCE            2u
#define CVC_PEDAL_PLAUS_THRESHOLD           819u
#define CVC_PEDAL_RAMP_LIMIT                5u
#define CVC_PEDAL_SENSOR1_FAIL              3u
#define CVC_PEDAL_SENSOR2_FAIL              4u
#define CVC_PEDAL_STUCK                     2u
#define CVC_PEDAL_STUCK_CYCLES              100u
#define CVC_PEDAL_STUCK_THRESHOLD           10u
#define CVC_STATE_COUNT                     6u
#define CVC_STATE_DEGRADED                  2u
#define CVC_STATE_INIT                      0u
#define CVC_STATE_INVALID                   255u
#define CVC_STATE_LIMP                      3u
#define CVC_STATE_RUN                       1u
#define CVC_STATE_SAFE_STOP                 4u
#define CVC_STATE_SHUTDOWN                  5u

/* ==================================================================== */
/* Thresholds and Constants
 * ================================================================== */

#define CVC_CREEP_DEBOUNCE_TICKS            20u
#define CVC_E2E_SM_FZC_MAX_ERR_VALID        1u
#define CVC_E2E_SM_FZC_MIN_OK_INIT          2u
#define CVC_E2E_SM_FZC_MIN_OK_INV           3u
#define CVC_E2E_SM_RZC_MAX_ERR_VALID        2u
#define CVC_E2E_SM_RZC_MIN_OK_INIT          3u
#define CVC_E2E_SM_RZC_MIN_OK_INV           3u
#define CVC_EVT_CAN_TIMEOUT_DUAL            5u
#define CVC_EVT_CAN_TIMEOUT_SINGLE          4u
#define CVC_FAULT_UNLATCH_CYCLES            300u
#define CVC_HB_ALIVE_MAX                    15u
#define CVC_HB_TX_PERIOD_MS                 50u
#define CVC_INIT_HOLD_CYCLES                500u
#define CVC_POST_INIT_GRACE_CYCLES          0u
#define CVC_RTE_PERIOD_MS                   10u
#define CVC_SAFE_STOP_RECOVERY_CYCLES       200u

#endif /* CVC_CFG_H */
