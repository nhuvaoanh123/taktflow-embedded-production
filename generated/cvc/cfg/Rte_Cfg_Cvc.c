/**
 * @file    Rte_Cfg_Cvc.c
 * @brief   RTE configuration for CVC -- signal table and runnable table
 * @date    2026-03-10
 *
 * GENERATED -- DO NOT EDIT
 * Source: ARXML architecture migration (tools/arxml/codegen.py)
 *
 * @standard AUTOSAR, ISO 26262 Part 6
 * @copyright Taktflow Systems 2026
 */

#include "Rte.h"
#include "Cvc_Cfg.h"

/* ==================================================================== */
/* Forward declarations for SWC runnables
 * ================================================================== */

extern void Can_MainFunction_Read(void);
extern void Com_MainFunction_Rx(void);
extern void Swc_EStop_MainFunction(void);
extern void Swc_Pedal_MainFunction(void);
extern void Swc_VehicleState_MainFunction(void);
extern void Swc_Heartbeat_MainFunction(void);
extern void Swc_Dashboard_MainFunction(void);
extern void Com_MainFunction_Tx(void);
extern void Can_MainFunction_BusOff(void);

/* ==================================================================== */
/* Signal Configuration Table
 * ================================================================== */

static const Rte_SignalConfigType cvc_signal_config[CVC_SIG_COUNT] = {
    {  0u, 0u },   /* 0: BSW reserved */
    {  1u, 0u },   /* 1: BSW reserved */
    {  2u, 0u },   /* 2: BSW reserved */
    {  3u, 0u },   /* 3: BSW reserved */
    {  4u, 0u },   /* 4: BSW reserved */
    {  5u, 0u },   /* 5: BSW reserved */
    {  6u, 0u },   /* 6: BSW reserved */
    {  7u, 0u },   /* 7: BSW reserved */
    {  8u, 0u },   /* 8: BSW reserved */
    {  9u, 0u },   /* 9: BSW reserved */
    { 10u, 0u },   /* 10: BSW reserved */
    { 11u, 0u },   /* 11: BSW reserved */
    { 12u, 0u },   /* 12: BSW reserved */
    { 13u, 0u },   /* 13: BSW reserved */
    { 14u, 0u },   /* 14: BSW reserved */
    { 15u, 0u },   /* 15: BSW reserved */
    { CVC_SIG_PEDAL_RAW_1, 0u },   /* 16 */
    { CVC_SIG_PEDAL_RAW_2, 0u },   /* 17 */
    { CVC_SIG_PEDAL_POSITION, 0u },   /* 18 */
    { CVC_SIG_PEDAL_FAULT, 0u },   /* 19 */
    { CVC_SIG_VEHICLE_STATE, 0u },   /* 20 */
    { CVC_SIG_TORQUE_REQUEST, 0u },   /* 21 */
    { CVC_SIG_ESTOP_ACTIVE, 0u },   /* 22 */
    { CVC_SIG_FZC_COMM_STATUS, 0u },   /* 23 */
    { CVC_SIG_RZC_COMM_STATUS, 0u },   /* 24 */
    { CVC_SIG_MOTOR_SPEED, 0u },   /* 25 */
    { CVC_SIG_FAULT_MASK, 0u },   /* 26 */
    { CVC_SIG_MOTOR_CURRENT, 0u },   /* 27 */
    { CVC_SIG_MOTOR_CUTOFF, 0u },   /* 28 */
    { CVC_SIG_STEERING_FAULT, 0u },   /* 29 */
    { CVC_SIG_BRAKE_FAULT, 0u },   /* 30 */
    { CVC_SIG_SC_RELAY_KILL, 0u },   /* 31 */
    { CVC_SIG_BATTERY_STATUS, 0u },   /* 32 */
    { CVC_SIG_MOTOR_FAULT_RZC, 0u },   /* 33 */
};

/* ==================================================================== */
/* Runnable Configuration Table
 * ================================================================== */

static const Rte_RunnableConfigType cvc_runnable_config[] = {
    /* func, periodMs, priority, seId */
    { Can_MainFunction_Read,                     1u,  9u,  0xFFu },
    { Com_MainFunction_Rx,                      10u,  8u,  0xFFu },
    { Swc_EStop_MainFunction,                   10u,  7u,     2u },
    { Swc_Pedal_MainFunction,                   10u,  6u,     0u },
    { Swc_VehicleState_MainFunction,            10u,  5u,     1u },
    { Swc_Heartbeat_MainFunction,               10u,  4u,     3u },
    { Swc_Dashboard_MainFunction,               10u,  3u,     4u },
    { Com_MainFunction_Tx,                      10u,  2u,  0xFFu },
    { Can_MainFunction_BusOff,                  10u,  1u,  0xFFu },
};

#define CVC_RUNNABLE_COUNT  (sizeof(cvc_runnable_config) / sizeof(cvc_runnable_config[0]))

/* ==================================================================== */
/* Aggregate RTE Configuration
 * ================================================================== */

const Rte_ConfigType cvc_rte_config = {
    .signalConfig   = cvc_signal_config,
    .signalCount    = CVC_SIG_COUNT,
    .runnableConfig = cvc_runnable_config,
    .runnableCount  = (uint8)CVC_RUNNABLE_COUNT,
};
