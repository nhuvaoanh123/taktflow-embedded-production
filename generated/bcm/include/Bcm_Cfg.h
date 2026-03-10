/**
 * @file    Bcm_Cfg.h
 * @brief   BCM configuration -- all BCM-specific ID definitions
 * @date    2026-03-10
 *
 * GENERATED -- DO NOT EDIT
 * Source: ARXML architecture migration (tools/arxml/codegen.py)
 *
 * @standard AUTOSAR, ISO 26262 Part 6
 * @copyright Taktflow Systems 2026
 */

#ifndef BCM_CFG_H
#define BCM_CFG_H

/* ==================================================================== */
/* RTE Signal IDs (extends BSW well-known IDs at offset 16)
 * ================================================================== */

#define BCM_SIG_VEHICLE_SPEED               16u
#define BCM_SIG_VEHICLE_STATE               17u
#define BCM_SIG_BODY_CONTROL_CMD            18u
#define BCM_SIG_LIGHT_HEADLAMP              19u
#define BCM_SIG_LIGHT_TAIL                  20u
#define BCM_SIG_INDICATOR_LEFT              21u
#define BCM_SIG_INDICATOR_RIGHT             22u
#define BCM_SIG_HAZARD_ACTIVE               23u
#define BCM_SIG_DOOR_LOCK_STATE             24u
#define BCM_SIG_ESTOP_ACTIVE                25u
#define BCM_SIG_COUNT                       26u

/* ==================================================================== */
/* Com TX PDU IDs
 * ================================================================== */

#define BCM_COM_TX_LIGHT_STATUS             0u   /* CAN 0x400 */
#define BCM_COM_TX_INDICATOR_STATE          1u   /* CAN 0x401 */
#define BCM_COM_TX_DOOR_LOCK                2u   /* CAN 0x402 */

/* ==================================================================== */
/* Com RX PDU IDs
 * ================================================================== */

#define BCM_COM_RX_VEHICLE_STATE            0u   /* CAN 0x100 */
#define BCM_COM_RX_MOTOR_CURRENT            1u   /* CAN 0x301 */
#define BCM_COM_RX_BODY_CMD                 2u   /* CAN 0x350 */

#endif /* BCM_CFG_H */
