/**
 * @file    Com_Cfg_Rzc.c
 * @brief   Com module configuration for RZC -- TX/RX PDUs and signal mappings
 * @date    2026-03-10
 *
 * GENERATED -- DO NOT EDIT
 * Source: ARXML architecture migration (tools/arxml/codegen.py)
 *
 * @standard AUTOSAR, ISO 26262 Part 6
 * @copyright Taktflow Systems 2026
 */

#include "Com.h"
#include "Rzc_Cfg.h"

/* ==================================================================== */
/* Shadow Buffers
 * ================================================================== */

static uint8    sig_tx_hb_alive;
static uint8    sig_tx_hb_ecu_id;
static uint8    sig_tx_hb_fault_mask;
static uint8    sig_tx_torque_echo;
static uint16   sig_tx_motor_speed;
static uint8    sig_tx_motor_dir;
static uint8    sig_tx_motor_enable;
static uint8    sig_tx_motor_fault;
static uint16   sig_tx_current_mA;
static uint8    sig_tx_overcurrent;
static sint16   sig_tx_temp1;
static sint16   sig_tx_temp2;
static uint8    sig_tx_derating_pct;
static uint16   sig_tx_battery_mV;
static uint8    sig_tx_battery_status;
static uint8    sig_rx_estop_active;
static uint8    sig_rx_vehicle_state;
static sint16   sig_rx_torque_cmd;
static uint16   sig_rx_virt_motor_current;
static uint16   sig_rx_virt_motor_temp;
static uint16   sig_rx_virt_battery_voltage;
static uint16   sig_rx_virt_motor_rpm;

/* ==================================================================== */
/* Signal Configuration Table
 * ================================================================== */

static const Com_SignalConfigType rzc_signal_config[] = {
    /* id, bitPos, bitSize, type, pduId, shadowBuf */
    {  0u,  16u,   8u, COM_UINT8,   RZC_COM_TX_HEARTBEAT,               &sig_tx_hb_alive                },
    {  1u,  24u,   8u, COM_UINT8,   RZC_COM_TX_HEARTBEAT,               &sig_tx_hb_ecu_id               },
    {  2u,  32u,   8u, COM_UINT8,   RZC_COM_TX_HEARTBEAT,               &sig_tx_hb_fault_mask           },
    {  3u,  16u,   8u, COM_UINT8,   RZC_COM_TX_MOTOR_STATUS,            &sig_tx_torque_echo             },
    {  4u,  24u,  16u, COM_UINT16,  RZC_COM_TX_MOTOR_STATUS,            &sig_tx_motor_speed             },
    {  5u,  40u,   8u, COM_UINT8,   RZC_COM_TX_MOTOR_STATUS,            &sig_tx_motor_dir               },
    {  6u,  48u,   8u, COM_UINT8,   RZC_COM_TX_MOTOR_STATUS,            &sig_tx_motor_enable            },
    {  7u,  56u,   8u, COM_UINT8,   RZC_COM_TX_MOTOR_STATUS,            &sig_tx_motor_fault             },
    {  8u,  16u,  16u, COM_UINT16,  RZC_COM_TX_MOTOR_CURRENT,           &sig_tx_current_mA              },
    {  9u,  32u,   8u, COM_UINT8,   RZC_COM_TX_MOTOR_CURRENT,           &sig_tx_overcurrent             },
    { 10u,  16u,  16u, COM_SINT16,  RZC_COM_TX_MOTOR_TEMP,              &sig_tx_temp1                   },
    { 11u,  32u,  16u, COM_SINT16,  RZC_COM_TX_MOTOR_TEMP,              &sig_tx_temp2                   },
    { 12u,  48u,   8u, COM_UINT8,   RZC_COM_TX_MOTOR_TEMP,              &sig_tx_derating_pct            },
    { 13u,  16u,  16u, COM_UINT16,  RZC_COM_TX_BATTERY_STATUS,          &sig_tx_battery_mV              },
    { 14u,  32u,   8u, COM_UINT8,   RZC_COM_TX_BATTERY_STATUS,          &sig_tx_battery_status          },
    { 15u,  16u,   8u, COM_UINT8,   RZC_COM_RX_ESTOP,                   &sig_rx_estop_active            },
    { 16u,  16u,   8u, COM_UINT8,   RZC_COM_RX_VEHICLE_TORQUE,          &sig_rx_vehicle_state           },
    { 17u,  32u,  16u, COM_SINT16,  RZC_COM_RX_VEHICLE_TORQUE,          &sig_rx_torque_cmd              },
    { 18u,   0u,  16u, COM_UINT16,  RZC_COM_RX_VIRT_SENSORS,            &sig_rx_virt_motor_current      },
    { 19u,  16u,  16u, COM_UINT16,  RZC_COM_RX_VIRT_SENSORS,            &sig_rx_virt_motor_temp         },
    { 20u,  32u,  16u, COM_UINT16,  RZC_COM_RX_VIRT_SENSORS,            &sig_rx_virt_battery_voltage    },
    { 21u,  48u,  16u, COM_UINT16,  RZC_COM_RX_VIRT_SENSORS,            &sig_rx_virt_motor_rpm          },
};

#define RZC_COM_SIGNAL_COUNT  (sizeof(rzc_signal_config) / sizeof(rzc_signal_config[0]))

/* ==================================================================== */
/* TX PDU Configuration Table
 * ================================================================== */

static const Com_TxPduConfigType rzc_tx_pdu_config[] = {
    /* pduId, dlc, cycleMs */
    { RZC_COM_TX_HEARTBEAT,               8u, 0u },
    { RZC_COM_TX_MOTOR_STATUS,            8u, 0u },
    { RZC_COM_TX_MOTOR_CURRENT,           8u, 0u },
    { RZC_COM_TX_MOTOR_TEMP,              8u, 0u },
    { RZC_COM_TX_BATTERY_STATUS,          8u, 0u },
    { RZC_COM_TX_DTC_BROADCAST,           8u, 0u },
};

#define RZC_COM_TX_PDU_COUNT  (sizeof(rzc_tx_pdu_config) / sizeof(rzc_tx_pdu_config[0]))

/* ==================================================================== */
/* RX PDU Configuration Table
 * ================================================================== */

static const Com_RxPduConfigType rzc_rx_pdu_config[] = {
    /* pduId, dlc, timeoutMs */
    { RZC_COM_RX_ESTOP,                   8u, 0u },
    { RZC_COM_RX_VEHICLE_TORQUE,          8u, 0u },
    { RZC_COM_RX_VIRT_SENSORS,            8u, 0u },
};

#define RZC_COM_RX_PDU_COUNT  (sizeof(rzc_rx_pdu_config) / sizeof(rzc_rx_pdu_config[0]))

/* ==================================================================== */
/* Aggregate Com Configuration
 * ================================================================== */

const Com_ConfigType rzc_com_config = {
    .signalConfig = rzc_signal_config,
    .signalCount  = (uint8)RZC_COM_SIGNAL_COUNT,
    .txPduConfig  = rzc_tx_pdu_config,
    .txPduCount   = (uint8)RZC_COM_TX_PDU_COUNT,
    .rxPduConfig  = rzc_rx_pdu_config,
    .rxPduCount   = (uint8)RZC_COM_RX_PDU_COUNT,
};
