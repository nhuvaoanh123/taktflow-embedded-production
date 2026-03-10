/**
 * @file    Rte_Cfg_Rzc.c
 * @brief   RTE configuration for RZC -- signal table and runnable table
 * @date    2026-03-10
 *
 * GENERATED -- DO NOT EDIT
 * Source: ARXML architecture migration (tools/arxml/codegen.py)
 *
 * @standard AUTOSAR, ISO 26262 Part 6
 * @copyright Taktflow Systems 2026
 */

#include "Rte.h"
#include "Rzc_Cfg.h"

/* ==================================================================== */
/* Forward declarations for SWC runnables
 * ================================================================== */

extern void Swc_CurrentMonitor_MainFunction(void);
extern void Swc_Motor_MainFunction(void);
extern void Swc_Encoder_MainFunction(void);
extern void Can_MainFunction_Read(void);
extern void Com_MainFunction_Rx(void);
extern void Swc_RzcCom_Receive(void);
extern void Swc_RzcSensorFeeder_MainFunction(void);
extern void Swc_RzcCom_TransmitSchedule(void);
extern void Com_MainFunction_Tx(void);
extern void Swc_TempMonitor_MainFunction(void);
extern void Swc_Battery_MainFunction(void);
extern void Swc_Heartbeat_MainFunction(void);
extern void Can_MainFunction_BusOff(void);
extern void Swc_RzcSafety_MainFunction(void);

/* ==================================================================== */
/* Signal Configuration Table
 * ================================================================== */

static const Rte_SignalConfigType rzc_signal_config[RZC_SIG_COUNT] = {
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
    { RZC_SIG_TORQUE_CMD, 0u },   /* 16 */
    { RZC_SIG_TORQUE_ECHO, 0u },   /* 17 */
    { RZC_SIG_MOTOR_SPEED, 0u },   /* 18 */
    { RZC_SIG_MOTOR_DIR, 0u },   /* 19 */
    { RZC_SIG_MOTOR_ENABLE, 0u },   /* 20 */
    { RZC_SIG_MOTOR_FAULT, 0u },   /* 21 */
    { RZC_SIG_CURRENT_MA, 0u },   /* 22 */
    { RZC_SIG_OVERCURRENT, 0u },   /* 23 */
    { RZC_SIG_TEMP1_DC, 0u },   /* 24 */
    { RZC_SIG_TEMP2_DC, 0u },   /* 25 */
    { RZC_SIG_DERATING_PCT, 0u },   /* 26 */
    { RZC_SIG_TEMP_FAULT, 0u },   /* 27 */
    { RZC_SIG_BATTERY_MV, 0u },   /* 28 */
    { RZC_SIG_BATTERY_STATUS, 0u },   /* 29 */
    { RZC_SIG_ENCODER_SPEED, 0u },   /* 30 */
    { RZC_SIG_ENCODER_DIR, 0u },   /* 31 */
    { RZC_SIG_ENCODER_STALL, 0u },   /* 32 */
    { RZC_SIG_VEHICLE_STATE, 0u },   /* 33 */
    { RZC_SIG_ESTOP_ACTIVE, 0u },   /* 34 */
    { RZC_SIG_FAULT_MASK, 0u },   /* 35 */
    { RZC_SIG_SELF_TEST_RESULT, 0u },   /* 36 */
    { RZC_SIG_HEARTBEAT_ALIVE, 0u },   /* 37 */
    { RZC_SIG_SAFETY_STATUS, 0u },   /* 38 */
    { RZC_SIG_CMD_TIMEOUT, 0u },   /* 39 */
    { RZC_SIG_BATTERY_SOC, 0u },   /* 40 */
};

/* ==================================================================== */
/* Runnable Configuration Table
 * ================================================================== */

static const Rte_RunnableConfigType rzc_runnable_config[] = {
    /* func, periodMs, priority, seId */
    { Swc_CurrentMonitor_MainFunction,           1u, 11u,     0u },
    { Swc_Motor_MainFunction,                   10u, 10u,     1u },
    { Swc_Encoder_MainFunction,                 10u, 10u,     2u },
    { Can_MainFunction_Read,                     1u,  9u,  0xFFu },
    { Com_MainFunction_Rx,                      10u,  8u,  0xFFu },
    { Swc_RzcCom_Receive,                       10u,  7u,  0xFFu },
    { Swc_RzcSensorFeeder_MainFunction,         10u,  6u,  0xFFu },
    { Swc_RzcCom_TransmitSchedule,              10u,  5u,  0xFFu },
    { Com_MainFunction_Tx,                      10u,  4u,  0xFFu },
    { Swc_TempMonitor_MainFunction,            100u,  4u,     3u },
    { Swc_Battery_MainFunction,                100u,  4u,     4u },
    { Swc_Heartbeat_MainFunction,               50u,  3u,     5u },
    { Can_MainFunction_BusOff,                  10u,  2u,  0xFFu },
    { Swc_RzcSafety_MainFunction,              100u,  2u,     6u },
};

#define RZC_RUNNABLE_COUNT  (sizeof(rzc_runnable_config) / sizeof(rzc_runnable_config[0]))

/* ==================================================================== */
/* Aggregate RTE Configuration
 * ================================================================== */

const Rte_ConfigType rzc_rte_config = {
    .signalConfig   = rzc_signal_config,
    .signalCount    = RZC_SIG_COUNT,
    .runnableConfig = rzc_runnable_config,
    .runnableCount  = (uint8)RZC_RUNNABLE_COUNT,
};
