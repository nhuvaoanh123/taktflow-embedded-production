/**
 * @file    Com_Cfg_Fzc.c
 * @brief   Com module configuration for FZC -- TX/RX PDUs and signal mappings
 * @date    2026-03-10
 *
 * GENERATED -- DO NOT EDIT
 * Source: ARXML architecture migration (tools/arxml/codegen.py)
 *
 * @standard AUTOSAR, ISO 26262 Part 6
 * @copyright Taktflow Systems 2026
 */

#include "Com.h"
#include "Fzc_Cfg.h"

/* ==================================================================== */
/* Shadow Buffers
 * ================================================================== */

static uint8    sig_tx_hb_alive;
static uint8    sig_tx_hb_ecu_id;
static uint8    sig_tx_hb_fault_mask;
static sint16   sig_tx_steer_angle;
static uint8    sig_tx_steer_fault;
static uint8    sig_tx_brake_pos;
static uint8    sig_tx_brake_fault;
static uint8    sig_tx_motor_cutoff;
static uint16   sig_tx_lidar_dist;
static uint8    sig_tx_lidar_zone;
static uint8    sig_rx_estop_active;
static uint8    sig_rx_vehicle_state;
static sint16   sig_rx_steer_cmd;
static uint8    sig_rx_brake_cmd;
static uint16   sig_rx_virt_steer_angle;
static uint16   sig_rx_virt_brake_pos;
static uint16   sig_rx_virt_brake_current;

/* ==================================================================== */
/* Signal Configuration Table
 * ================================================================== */

static const Com_SignalConfigType fzc_signal_config[] = {
    /* id, bitPos, bitSize, type, pduId, shadowBuf */
    {  0u,  16u,   8u, COM_UINT8,   FZC_COM_TX_HEARTBEAT,               &sig_tx_hb_alive                },
    {  1u,  24u,   8u, COM_UINT8,   FZC_COM_TX_HEARTBEAT,               &sig_tx_hb_ecu_id               },
    {  2u,  32u,   8u, COM_UINT8,   FZC_COM_TX_HEARTBEAT,               &sig_tx_hb_fault_mask           },
    {  3u,  16u,  16u, COM_SINT16,  FZC_COM_TX_STEER_STATUS,            &sig_tx_steer_angle             },
    {  4u,  48u,   4u, COM_UINT8,   FZC_COM_TX_STEER_STATUS,            &sig_tx_steer_fault             },
    {  5u,  16u,   8u, COM_UINT8,   FZC_COM_TX_BRAKE_STATUS,            &sig_tx_brake_pos               },
    {  6u,  16u,   8u, COM_UINT8,   FZC_COM_TX_BRAKE_FAULT,             &sig_tx_brake_fault             },
    {  7u,  16u,   8u, COM_UINT8,   FZC_COM_TX_MOTOR_CUTOFF,            &sig_tx_motor_cutoff            },
    {  8u,  16u,  16u, COM_UINT16,  FZC_COM_TX_LIDAR,                   &sig_tx_lidar_dist              },
    {  9u,  32u,   8u, COM_UINT8,   FZC_COM_TX_LIDAR,                   &sig_tx_lidar_zone              },
    { 10u,  16u,   8u, COM_UINT8,   FZC_COM_RX_ESTOP,                   &sig_rx_estop_active            },
    { 11u,  16u,   8u, COM_UINT8,   FZC_COM_RX_VEHICLE_STATE,           &sig_rx_vehicle_state           },
    { 12u,  16u,  16u, COM_SINT16,  FZC_COM_RX_STEER_CMD,               &sig_rx_steer_cmd               },
    { 13u,  16u,   8u, COM_UINT8,   FZC_COM_RX_BRAKE_CMD,               &sig_rx_brake_cmd               },
    { 14u,   0u,  16u, COM_UINT16,  FZC_COM_RX_VIRT_SENSORS,            &sig_rx_virt_steer_angle        },
    { 15u,  16u,  16u, COM_UINT16,  FZC_COM_RX_VIRT_SENSORS,            &sig_rx_virt_brake_pos          },
    { 16u,  32u,  16u, COM_UINT16,  FZC_COM_RX_VIRT_SENSORS,            &sig_rx_virt_brake_current      },
};

#define FZC_COM_SIGNAL_COUNT  (sizeof(fzc_signal_config) / sizeof(fzc_signal_config[0]))

/* ==================================================================== */
/* TX PDU Configuration Table
 * ================================================================== */

static const Com_TxPduConfigType fzc_tx_pdu_config[] = {
    /* pduId, dlc, cycleMs */
    { FZC_COM_TX_HEARTBEAT,               8u, 0u },
    { FZC_COM_TX_STEER_STATUS,            8u, 0u },
    { FZC_COM_TX_BRAKE_STATUS,            8u, 0u },
    { FZC_COM_TX_BRAKE_FAULT,             8u, 0u },
    { FZC_COM_TX_MOTOR_CUTOFF,            8u, 0u },
    { FZC_COM_TX_LIDAR,                   8u, 0u },
    { FZC_COM_TX_DTC_BROADCAST,           8u, 0u },
};

#define FZC_COM_TX_PDU_COUNT  (sizeof(fzc_tx_pdu_config) / sizeof(fzc_tx_pdu_config[0]))

/* ==================================================================== */
/* RX PDU Configuration Table
 * ================================================================== */

static const Com_RxPduConfigType fzc_rx_pdu_config[] = {
    /* pduId, dlc, timeoutMs */
    { FZC_COM_RX_ESTOP,                   8u, 0u },
    { FZC_COM_RX_VEHICLE_STATE,           8u, 0u },
    { FZC_COM_RX_STEER_CMD,               8u, 0u },
    { FZC_COM_RX_BRAKE_CMD,               8u, 0u },
    { FZC_COM_RX_VIRT_SENSORS,            8u, 0u },
};

#define FZC_COM_RX_PDU_COUNT  (sizeof(fzc_rx_pdu_config) / sizeof(fzc_rx_pdu_config[0]))

/* ==================================================================== */
/* Aggregate Com Configuration
 * ================================================================== */

const Com_ConfigType fzc_com_config = {
    .signalConfig = fzc_signal_config,
    .signalCount  = (uint8)FZC_COM_SIGNAL_COUNT,
    .txPduConfig  = fzc_tx_pdu_config,
    .txPduCount   = (uint8)FZC_COM_TX_PDU_COUNT,
    .rxPduConfig  = fzc_rx_pdu_config,
    .rxPduCount   = (uint8)FZC_COM_RX_PDU_COUNT,
};
