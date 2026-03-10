/**
 * @file    Rte_Cfg_Icu.c
 * @brief   RTE configuration for ICU -- signal table and runnable table
 * @date    2026-03-10
 *
 * GENERATED -- DO NOT EDIT
 * Source: ARXML architecture migration (tools/arxml/codegen.py)
 *
 * @standard AUTOSAR, ISO 26262 Part 6
 * @copyright Taktflow Systems 2026
 */

#include "Rte.h"
#include "Icu_Cfg.h"

/* ==================================================================== */
/* Forward declarations for SWC runnables
 * ================================================================== */

extern void Can_MainFunction_Read(void);
extern void Com_MainFunction_Rx(void);
extern void Swc_Dashboard_50ms(void);
extern void Swc_DtcDisplay_50ms(void);
extern void Icu_Heartbeat_500ms(void);
extern void Com_MainFunction_Tx(void);
extern void Can_MainFunction_BusOff(void);

/* ==================================================================== */
/* Signal Configuration Table
 * ================================================================== */

static const Rte_SignalConfigType icu_signal_config[ICU_SIG_COUNT] = {
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
    { ICU_SIG_MOTOR_RPM, 0u },   /* 16 */
    { ICU_SIG_TORQUE_PCT, 0u },   /* 17 */
    { ICU_SIG_MOTOR_TEMP, 0u },   /* 18 */
    { ICU_SIG_BATTERY_VOLTAGE, 0u },   /* 19 */
    { ICU_SIG_VEHICLE_STATE, 0u },   /* 20 */
    { ICU_SIG_ESTOP_ACTIVE, 0u },   /* 21 */
    { ICU_SIG_HEARTBEAT_CVC, 0u },   /* 22 */
    { ICU_SIG_HEARTBEAT_FZC, 0u },   /* 23 */
    { ICU_SIG_HEARTBEAT_RZC, 0u },   /* 24 */
    { ICU_SIG_OVERCURRENT_FLAG, 0u },   /* 25 */
    { ICU_SIG_LIGHT_STATUS, 0u },   /* 26 */
    { ICU_SIG_INDICATOR_STATE, 0u },   /* 27 */
    { ICU_SIG_DTC_BROADCAST, 0u },   /* 28 */
};

/* ==================================================================== */
/* Runnable Configuration Table
 * ================================================================== */

static const Rte_RunnableConfigType icu_runnable_config[] = {
    /* func, periodMs, priority, seId */
    { Can_MainFunction_Read,                    10u,  7u,  0xFFu },
    { Com_MainFunction_Rx,                      10u,  6u,  0xFFu },
    { Swc_Dashboard_50ms,                       50u,  5u,     0u },
    { Swc_DtcDisplay_50ms,                      50u,  4u,     1u },
    { Icu_Heartbeat_500ms,                      10u,  3u,  0xFFu },
    { Com_MainFunction_Tx,                      10u,  2u,  0xFFu },
    { Can_MainFunction_BusOff,                  50u,  1u,  0xFFu },
};

#define ICU_RUNNABLE_COUNT  (sizeof(icu_runnable_config) / sizeof(icu_runnable_config[0]))

/* ==================================================================== */
/* Aggregate RTE Configuration
 * ================================================================== */

const Rte_ConfigType icu_rte_config = {
    .signalConfig   = icu_signal_config,
    .signalCount    = ICU_SIG_COUNT,
    .runnableConfig = icu_runnable_config,
    .runnableCount  = (uint8)ICU_RUNNABLE_COUNT,
};
