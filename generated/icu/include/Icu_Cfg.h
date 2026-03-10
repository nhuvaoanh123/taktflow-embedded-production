/**
 * @file    Icu_Cfg.h
 * @brief   ICU configuration -- all ICU-specific ID definitions
 * @date    2026-03-10
 *
 * GENERATED -- DO NOT EDIT
 * Source: ARXML architecture migration (tools/arxml/codegen.py)
 *
 * @standard AUTOSAR, ISO 26262 Part 6
 * @copyright Taktflow Systems 2026
 */

#ifndef ICU_CFG_H
#define ICU_CFG_H

/* ==================================================================== */
/* RTE Signal IDs (extends BSW well-known IDs at offset 16)
 * ================================================================== */

#define ICU_SIG_MOTOR_RPM                   16u
#define ICU_SIG_TORQUE_PCT                  17u
#define ICU_SIG_MOTOR_TEMP                  18u
#define ICU_SIG_BATTERY_VOLTAGE             19u
#define ICU_SIG_VEHICLE_STATE               20u
#define ICU_SIG_ESTOP_ACTIVE                21u
#define ICU_SIG_HEARTBEAT_CVC               22u
#define ICU_SIG_HEARTBEAT_FZC               23u
#define ICU_SIG_HEARTBEAT_RZC               24u
#define ICU_SIG_OVERCURRENT_FLAG            25u
#define ICU_SIG_LIGHT_STATUS                26u
#define ICU_SIG_INDICATOR_STATE             27u
#define ICU_SIG_DTC_BROADCAST               28u
#define ICU_SIG_COUNT                       29u

/* ==================================================================== */
/* Com TX PDU IDs
 * ================================================================== */

#define ICU_COM_TX_HEARTBEAT                0u   /* CAN 0x014 */

/* ==================================================================== */
/* Com RX PDU IDs
 * ================================================================== */

#define ICU_COM_RX_ESTOP                    0u   /* CAN 0x001 */
#define ICU_COM_RX_HB_CVC                   1u   /* CAN 0x010 */
#define ICU_COM_RX_HB_FZC                   2u   /* CAN 0x011 */
#define ICU_COM_RX_HB_RZC                   3u   /* CAN 0x012 */
#define ICU_COM_RX_VEHICLE_STATE            4u   /* CAN 0x100 */
#define ICU_COM_RX_TORQUE_REQ               5u   /* CAN 0x101 */
#define ICU_COM_RX_MOTOR_CURRENT            6u   /* CAN 0x301 */
#define ICU_COM_RX_MOTOR_TEMP               7u   /* CAN 0x302 */
#define ICU_COM_RX_BATTERY                  8u   /* CAN 0x303 */
#define ICU_COM_RX_LIGHT_STATUS             9u   /* CAN 0x400 */
#define ICU_COM_RX_INDICATOR                10u   /* CAN 0x401 */
#define ICU_COM_RX_DOOR_LOCK                11u   /* CAN 0x402 */
#define ICU_COM_RX_DTC_BCAST                12u   /* CAN 0x500 */
#define ICU_COM_RX_PDU_COUNT                13u

/* ==================================================================== */
/* Thresholds and Constants
 * ================================================================== */

#define ICU_BATT_GREEN_MIN                  11000u
#define ICU_BATT_YELLOW_MIN                 10000u
#define ICU_HB_TIMEOUT_TICKS                4u
#define ICU_TEMP_GREEN_MAX                  59u
#define ICU_TEMP_ORANGE_MAX                 99u
#define ICU_TEMP_YELLOW_MAX                 79u

#endif /* ICU_CFG_H */
