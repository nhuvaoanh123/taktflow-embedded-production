/**
 * @file    Com_Cfg_Bcm.c
 * @brief   Com module configuration for BCM -- TX/RX PDUs and signal mappings
 * @date    2026-03-10
 *
 * GENERATED -- DO NOT EDIT
 * Source: ARXML architecture migration (tools/arxml/codegen.py)
 *
 * @standard AUTOSAR, ISO 26262 Part 6
 * @copyright Taktflow Systems 2026
 */

#include "Com.h"
#include "Bcm_Cfg.h"

/* ==================================================================== */
/* Shadow Buffers
 * ================================================================== */

static uint8    sig_tx_light_headlamp;
static uint8    sig_tx_light_tail;
static uint8    sig_tx_indicator_left;
static uint8    sig_tx_indicator_right;
static uint8    sig_tx_hazard_active;
static uint8    sig_tx_door_lock_state;
static uint8    sig_rx_vehicle_state;
static uint16   sig_rx_vehicle_speed;
static uint8    sig_rx_body_cmd_byte0;
static uint8    sig_rx_body_cmd_byte1;
static uint8    sig_rx_estop_active;
static uint16   sig_rx_motor_current;

/* ==================================================================== */
/* Signal Configuration Table
 * ================================================================== */

static const Com_SignalConfigType bcm_signal_config[] = {
    /* id, bitPos, bitSize, type, pduId, shadowBuf */
    {  0u,  16u,   8u, COM_UINT8,   BCM_COM_TX_LIGHT_STATUS,            &sig_tx_light_headlamp          },
    {  1u,  24u,   8u, COM_UINT8,   BCM_COM_TX_LIGHT_STATUS,            &sig_tx_light_tail              },
    {  2u,  16u,   8u, COM_UINT8,   BCM_COM_TX_INDICATOR_STATE,         &sig_tx_indicator_left          },
    {  3u,  24u,   8u, COM_UINT8,   BCM_COM_TX_INDICATOR_STATE,         &sig_tx_indicator_right         },
    {  4u,  32u,   8u, COM_UINT8,   BCM_COM_TX_INDICATOR_STATE,         &sig_tx_hazard_active           },
    {  5u,  16u,   8u, COM_UINT8,   BCM_COM_TX_DOOR_LOCK,               &sig_tx_door_lock_state         },
    {  6u,  16u,   8u, COM_UINT8,   BCM_COM_RX_VEHICLE_STATE,           &sig_rx_vehicle_state           },
    {  7u,  24u,  16u, COM_UINT16,  BCM_COM_RX_VEHICLE_STATE,           &sig_rx_vehicle_speed           },
    {  8u,  16u,   8u, COM_UINT8,   BCM_COM_RX_BODY_CMD,                &sig_rx_body_cmd_byte0          },
    {  9u,  24u,   8u, COM_UINT8,   BCM_COM_RX_BODY_CMD,                &sig_rx_body_cmd_byte1          },
    { 10u,  32u,   8u, COM_UINT8,   BCM_COM_RX_BODY_CMD,                &sig_rx_estop_active            },
    { 11u,  16u,  16u, COM_UINT16,  BCM_COM_RX_MOTOR_CURRENT,           &sig_rx_motor_current           },
};

#define BCM_COM_SIGNAL_COUNT  (sizeof(bcm_signal_config) / sizeof(bcm_signal_config[0]))

/* ==================================================================== */
/* TX PDU Configuration Table
 * ================================================================== */

static const Com_TxPduConfigType bcm_tx_pdu_config[] = {
    /* pduId, dlc, cycleMs */
    { BCM_COM_TX_LIGHT_STATUS,            8u, 0u },
    { BCM_COM_TX_INDICATOR_STATE,         8u, 0u },
    { BCM_COM_TX_DOOR_LOCK,               8u, 0u },
};

#define BCM_COM_TX_PDU_COUNT  (sizeof(bcm_tx_pdu_config) / sizeof(bcm_tx_pdu_config[0]))

/* ==================================================================== */
/* RX PDU Configuration Table
 * ================================================================== */

static const Com_RxPduConfigType bcm_rx_pdu_config[] = {
    /* pduId, dlc, timeoutMs */
    { BCM_COM_RX_VEHICLE_STATE,           8u, 0u },
    { BCM_COM_RX_MOTOR_CURRENT,           8u, 0u },
    { BCM_COM_RX_BODY_CMD,                8u, 0u },
};

#define BCM_COM_RX_PDU_COUNT  (sizeof(bcm_rx_pdu_config) / sizeof(bcm_rx_pdu_config[0]))

/* ==================================================================== */
/* Aggregate Com Configuration
 * ================================================================== */

const Com_ConfigType bcm_com_config = {
    .signalConfig = bcm_signal_config,
    .signalCount  = (uint8)BCM_COM_SIGNAL_COUNT,
    .txPduConfig  = bcm_tx_pdu_config,
    .txPduCount   = (uint8)BCM_COM_TX_PDU_COUNT,
    .rxPduConfig  = bcm_rx_pdu_config,
    .rxPduCount   = (uint8)BCM_COM_RX_PDU_COUNT,
};
