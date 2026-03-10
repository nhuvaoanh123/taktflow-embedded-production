/**
 * @file    Com_Cfg_Icu.c
 * @brief   Com module configuration for ICU -- TX/RX PDUs and signal mappings
 * @date    2026-03-10
 *
 * GENERATED -- DO NOT EDIT
 * Source: ARXML architecture migration (tools/arxml/codegen.py)
 *
 * @standard AUTOSAR, ISO 26262 Part 6
 * @copyright Taktflow Systems 2026
 */

#include "Com.h"
#include "Icu_Cfg.h"

/* ==================================================================== */
/* Shadow Buffers
 * ================================================================== */

static uint8    sig_rx_estop_active;
static uint8    sig_rx_hb_cvc_alive;
static uint8    sig_rx_hb_cvc_ecu_id;
static uint8    sig_rx_hb_cvc_state;
static uint8    sig_rx_hb_fzc_alive;
static uint8    sig_rx_hb_fzc_ecu_id;
static uint8    sig_rx_hb_rzc_alive;
static uint8    sig_rx_hb_rzc_ecu_id;
static uint8    sig_rx_vehicle_state;
static uint16   sig_rx_torque_request;
static uint16   sig_rx_motor_current;
static uint16   sig_rx_motor_rpm;
static uint8    sig_rx_overcurrent_flag;
static uint8    sig_rx_motor_temp;
static uint16   sig_rx_battery_voltage;
static uint8    sig_rx_light_status;
static uint8    sig_rx_indicator_state;
static uint8    sig_rx_door_lock;
static uint16   sig_rx_dtc_broadcast;
static uint8    sig_tx_hb_alive;
static uint8    sig_tx_hb_ecu_id;

/* ==================================================================== */
/* Signal Configuration Table
 * ================================================================== */

static const Com_SignalConfigType icu_signal_config[] = {
    /* id, bitPos, bitSize, type, pduId, shadowBuf */
    {  0u,  16u,   8u, COM_UINT8,   ICU_COM_RX_ESTOP,                   &sig_rx_estop_active            },
    {  1u,  16u,   8u, COM_UINT8,   ICU_COM_RX_HB_CVC,                  &sig_rx_hb_cvc_alive            },
    {  2u,  24u,   8u, COM_UINT8,   ICU_COM_RX_HB_CVC,                  &sig_rx_hb_cvc_ecu_id           },
    {  3u,  32u,   8u, COM_UINT8,   ICU_COM_RX_HB_CVC,                  &sig_rx_hb_cvc_state            },
    {  4u,  16u,   8u, COM_UINT8,   ICU_COM_RX_HB_FZC,                  &sig_rx_hb_fzc_alive            },
    {  5u,  24u,   8u, COM_UINT8,   ICU_COM_RX_HB_FZC,                  &sig_rx_hb_fzc_ecu_id           },
    {  6u,  16u,   8u, COM_UINT8,   ICU_COM_RX_HB_RZC,                  &sig_rx_hb_rzc_alive            },
    {  7u,  24u,   8u, COM_UINT8,   ICU_COM_RX_HB_RZC,                  &sig_rx_hb_rzc_ecu_id           },
    {  8u,  16u,   8u, COM_UINT8,   ICU_COM_RX_VEHICLE_STATE,           &sig_rx_vehicle_state           },
    {  9u,  16u,  16u, COM_UINT16,  ICU_COM_RX_TORQUE_REQ,              &sig_rx_torque_request          },
    { 10u,  16u,  16u, COM_UINT16,  ICU_COM_RX_MOTOR_CURRENT,           &sig_rx_motor_current           },
    { 11u,  32u,  16u, COM_UINT16,  ICU_COM_RX_MOTOR_CURRENT,           &sig_rx_motor_rpm               },
    { 12u,  48u,   8u, COM_UINT8,   ICU_COM_RX_MOTOR_CURRENT,           &sig_rx_overcurrent_flag        },
    { 13u,  16u,   8u, COM_UINT8,   ICU_COM_RX_MOTOR_TEMP,              &sig_rx_motor_temp              },
    { 14u,  16u,  16u, COM_UINT16,  ICU_COM_RX_BATTERY,                 &sig_rx_battery_voltage         },
    { 15u,  16u,   8u, COM_UINT8,   ICU_COM_RX_LIGHT_STATUS,            &sig_rx_light_status            },
    { 16u,  16u,   8u, COM_UINT8,   ICU_COM_RX_INDICATOR,               &sig_rx_indicator_state         },
    { 17u,  16u,   8u, COM_UINT8,   ICU_COM_RX_DOOR_LOCK,               &sig_rx_door_lock               },
    { 18u,  16u,  32u, COM_UINT16,  ICU_COM_RX_DTC_BCAST,               &sig_rx_dtc_broadcast           },
    { 19u,  16u,   8u, COM_UINT8,   ICU_COM_TX_HEARTBEAT,               &sig_tx_hb_alive                },
    { 20u,  24u,   8u, COM_UINT8,   ICU_COM_TX_HEARTBEAT,               &sig_tx_hb_ecu_id               },
};

#define ICU_COM_SIGNAL_COUNT  (sizeof(icu_signal_config) / sizeof(icu_signal_config[0]))

/* ==================================================================== */
/* TX PDU Configuration Table
 * ================================================================== */

static const Com_TxPduConfigType icu_tx_pdu_config[] = {
    /* pduId, dlc, cycleMs */
    { ICU_COM_TX_HEARTBEAT,               8u, 0u },
};

#define ICU_COM_TX_PDU_COUNT  (sizeof(icu_tx_pdu_config) / sizeof(icu_tx_pdu_config[0]))

/* ==================================================================== */
/* RX PDU Configuration Table
 * ================================================================== */

static const Com_RxPduConfigType icu_rx_pdu_config[] = {
    /* pduId, dlc, timeoutMs */
    { ICU_COM_RX_ESTOP,                   8u, 0u },
    { ICU_COM_RX_HB_CVC,                  8u, 0u },
    { ICU_COM_RX_HB_FZC,                  8u, 0u },
    { ICU_COM_RX_HB_RZC,                  8u, 0u },
    { ICU_COM_RX_VEHICLE_STATE,           8u, 0u },
    { ICU_COM_RX_TORQUE_REQ,              8u, 0u },
    { ICU_COM_RX_MOTOR_CURRENT,           8u, 0u },
    { ICU_COM_RX_MOTOR_TEMP,              8u, 0u },
    { ICU_COM_RX_BATTERY,                 8u, 0u },
    { ICU_COM_RX_LIGHT_STATUS,            8u, 0u },
    { ICU_COM_RX_INDICATOR,               8u, 0u },
    { ICU_COM_RX_DOOR_LOCK,               8u, 0u },
    { ICU_COM_RX_DTC_BCAST,               8u, 0u },
    { ICU_COM_RX_PDU_COUNT,               8u, 0u },
};

#define ICU_COM_RX_PDU_COUNT  (sizeof(icu_rx_pdu_config) / sizeof(icu_rx_pdu_config[0]))

/* ==================================================================== */
/* Aggregate Com Configuration
 * ================================================================== */

const Com_ConfigType icu_com_config = {
    .signalConfig = icu_signal_config,
    .signalCount  = (uint8)ICU_COM_SIGNAL_COUNT,
    .txPduConfig  = icu_tx_pdu_config,
    .txPduCount   = (uint8)ICU_COM_TX_PDU_COUNT,
    .rxPduConfig  = icu_rx_pdu_config,
    .rxPduCount   = (uint8)ICU_COM_RX_PDU_COUNT,
};
