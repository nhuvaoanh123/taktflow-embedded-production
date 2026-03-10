/**
 * @file    Tcu_Cfg.h
 * @brief   TCU configuration -- all TCU-specific ID definitions
 * @date    2026-03-10
 *
 * GENERATED -- DO NOT EDIT
 * Source: ARXML architecture migration (tools/arxml/codegen.py)
 *
 * @standard AUTOSAR, ISO 26262 Part 6
 * @copyright Taktflow Systems 2026
 */

#ifndef TCU_CFG_H
#define TCU_CFG_H

/* ==================================================================== */
/* RTE Signal IDs (extends BSW well-known IDs at offset 16)
 * ================================================================== */

#define TCU_SIG_VEHICLE_SPEED               16u
#define TCU_SIG_MOTOR_TEMP                  17u
#define TCU_SIG_BATTERY_VOLTAGE             18u
#define TCU_SIG_MOTOR_CURRENT               19u
#define TCU_SIG_TORQUE_PCT                  20u
#define TCU_SIG_MOTOR_RPM                   21u
#define TCU_SIG_DTC_BROADCAST               22u
#define TCU_SIG_COUNT                       23u

/* ==================================================================== */
/* Com TX PDU IDs
 * ================================================================== */

#define TCU_COM_TX_UDS_RSP                  0u
#define TCU_COM_TX_HEARTBEAT                1u
#define TCU_COM_TX_PDU_COUNT_ACTUAL         2u

/* ==================================================================== */
/* Com RX PDU IDs
 * ================================================================== */

#define TCU_COM_RX_UDS_FUNC                 0u
#define TCU_COM_RX_UDS_PHYS                 1u
#define TCU_COM_RX_VEHICLE_STATE            2u
#define TCU_COM_RX_MOTOR_CURRENT            3u
#define TCU_COM_RX_MOTOR_TEMP               4u
#define TCU_COM_RX_BATTERY                  5u
#define TCU_COM_RX_DTC_BCAST                6u
#define TCU_COM_RX_HB_CVC                   7u
#define TCU_COM_RX_HB_FZC                   8u
#define TCU_COM_RX_HB_RZC                   9u

/* ==================================================================== */
/* DTC Event IDs (Dem_EventIdType)
 * ================================================================== */

#define TCU_DTC_AGING_CLEAR_CYCLES          40u

/* ==================================================================== */
/* Thresholds and Constants
 * ================================================================== */

#define TCU_UDS_MAX_SECURITY_ATTEMPTS       3u
#define TCU_UDS_SESSION_TIMEOUT_TICKS       500u

#endif /* TCU_CFG_H */
