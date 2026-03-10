/**
 * @file    Rte_Cfg_Fzc.c
 * @brief   RTE configuration for FZC -- signal table and runnable table
 * @date    2026-03-10
 *
 * GENERATED -- DO NOT EDIT
 * Source: ARXML architecture migration (tools/arxml/codegen.py)
 *
 * @standard AUTOSAR, ISO 26262 Part 6
 * @copyright Taktflow Systems 2026
 */

#include "Rte.h"
#include "Fzc_Cfg.h"

/* ==================================================================== */
/* Forward declarations for SWC runnables
 * ================================================================== */

extern void Can_MainFunction_Read(void);
extern void Com_MainFunction_Rx(void);
extern void Swc_FzcCom_Receive(void);
extern void Swc_FzcSensorFeeder_MainFunction(void);
extern void Swc_Steering_MainFunction(void);
extern void Swc_Brake_MainFunction(void);
extern void Swc_Lidar_MainFunction(void);
extern void Swc_Heartbeat_MainFunction(void);
extern void Swc_FzcSafety_MainFunction(void);
extern void Swc_Buzzer_MainFunction(void);
extern void Swc_FzcCanMonitor_Check(void);
extern void Swc_FzcCom_TransmitSchedule(void);
extern void Com_MainFunction_Tx(void);
extern void Can_MainFunction_BusOff(void);

/* ==================================================================== */
/* Signal Configuration Table
 * ================================================================== */

static const Rte_SignalConfigType fzc_signal_config[FZC_SIG_COUNT] = {
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
    { FZC_SIG_STEER_CMD, 0u },   /* 16 */
    { FZC_SIG_STEER_ANGLE, 0u },   /* 17 */
    { FZC_SIG_STEER_FAULT, 0u },   /* 18 */
    { FZC_SIG_BRAKE_CMD, 0u },   /* 19 */
    { FZC_SIG_BRAKE_POS, 0u },   /* 20 */
    { FZC_SIG_BRAKE_FAULT, 0u },   /* 21 */
    { FZC_SIG_LIDAR_DIST, 0u },   /* 22 */
    { FZC_SIG_LIDAR_SIGNAL, 0u },   /* 23 */
    { FZC_SIG_LIDAR_ZONE, 0u },   /* 24 */
    { FZC_SIG_LIDAR_FAULT, 0u },   /* 25 */
    { FZC_SIG_VEHICLE_STATE, 0u },   /* 26 */
    { FZC_SIG_ESTOP_ACTIVE, 0u },   /* 27 */
    { FZC_SIG_BUZZER_PATTERN, 0u },   /* 28 */
    { FZC_SIG_MOTOR_CUTOFF, 0u },   /* 29 */
    { FZC_SIG_FAULT_MASK, 0u },   /* 30 */
    { FZC_SIG_STEER_PWM_DISABLE, 0u },   /* 31 */
    { FZC_SIG_BRAKE_PWM_DISABLE, 0u },   /* 32 */
    { FZC_SIG_SELF_TEST_RESULT, 0u },   /* 33 */
    { FZC_SIG_HEARTBEAT_ALIVE, 0u },   /* 34 */
    { FZC_SIG_SAFETY_STATUS, 0u },   /* 35 */
};

/* ==================================================================== */
/* Runnable Configuration Table
 * ================================================================== */

static const Rte_RunnableConfigType fzc_runnable_config[] = {
    /* func, periodMs, priority, seId */
    { Can_MainFunction_Read,                    10u, 14u,  0xFFu },
    { Com_MainFunction_Rx,                      10u, 13u,  0xFFu },
    { Swc_FzcCom_Receive,                       10u, 12u,  0xFFu },
    { Swc_FzcSensorFeeder_MainFunction,         10u, 11u,  0xFFu },
    { Swc_Steering_MainFunction,                10u, 10u,     0u },
    { Swc_Brake_MainFunction,                   10u,  9u,     1u },
    { Swc_Lidar_MainFunction,                   10u,  8u,     2u },
    { Swc_Heartbeat_MainFunction,               10u,  7u,     3u },
    { Swc_FzcSafety_MainFunction,               10u,  6u,     4u },
    { Swc_Buzzer_MainFunction,                  10u,  5u,     5u },
    { Swc_FzcCanMonitor_Check,                  10u,  4u,  0xFFu },
    { Swc_FzcCom_TransmitSchedule,              10u,  3u,  0xFFu },
    { Com_MainFunction_Tx,                      10u,  2u,  0xFFu },
    { Can_MainFunction_BusOff,                  10u,  1u,  0xFFu },
};

#define FZC_RUNNABLE_COUNT  (sizeof(fzc_runnable_config) / sizeof(fzc_runnable_config[0]))

/* ==================================================================== */
/* Aggregate RTE Configuration
 * ================================================================== */

const Rte_ConfigType fzc_rte_config = {
    .signalConfig   = fzc_signal_config,
    .signalCount    = FZC_SIG_COUNT,
    .runnableConfig = fzc_runnable_config,
    .runnableCount  = (uint8)FZC_RUNNABLE_COUNT,
};
