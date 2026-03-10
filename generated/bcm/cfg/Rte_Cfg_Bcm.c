/**
 * @file    Rte_Cfg_Bcm.c
 * @brief   RTE configuration for BCM -- signal table and runnable table
 * @date    2026-03-10
 *
 * GENERATED -- DO NOT EDIT
 * Source: ARXML architecture migration (tools/arxml/codegen.py)
 *
 * @standard AUTOSAR, ISO 26262 Part 6
 * @copyright Taktflow Systems 2026
 */

#include "Rte.h"
#include "Bcm_Cfg.h"

/* ==================================================================== */
/* Forward declarations for SWC runnables
 * ================================================================== */

extern void Can_MainFunction_Read(void);
extern void Com_MainFunction_Rx(void);
extern void Bcm_ComBridge_10ms(void);
extern void Swc_Lights_10ms(void);
extern void Swc_Indicators_10ms(void);
extern void Swc_DoorLock_100ms(void);
extern void Com_MainFunction_Tx(void);
extern void Can_MainFunction_BusOff(void);

/* ==================================================================== */
/* Signal Configuration Table
 * ================================================================== */

static const Rte_SignalConfigType bcm_signal_config[BCM_SIG_COUNT] = {
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
    { BCM_SIG_VEHICLE_SPEED, 0u },   /* 16 */
    { BCM_SIG_VEHICLE_STATE, 0u },   /* 17 */
    { BCM_SIG_BODY_CONTROL_CMD, 0u },   /* 18 */
    { BCM_SIG_LIGHT_HEADLAMP, 0u },   /* 19 */
    { BCM_SIG_LIGHT_TAIL, 0u },   /* 20 */
    { BCM_SIG_INDICATOR_LEFT, 0u },   /* 21 */
    { BCM_SIG_INDICATOR_RIGHT, 0u },   /* 22 */
    { BCM_SIG_HAZARD_ACTIVE, 0u },   /* 23 */
    { BCM_SIG_DOOR_LOCK_STATE, 0u },   /* 24 */
    { BCM_SIG_ESTOP_ACTIVE, 0u },   /* 25 */
};

/* ==================================================================== */
/* Runnable Configuration Table
 * ================================================================== */

static const Rte_RunnableConfigType bcm_runnable_config[] = {
    /* func, periodMs, priority, seId */
    { Can_MainFunction_Read,                    10u,  8u,  0xFFu },
    { Com_MainFunction_Rx,                      10u,  7u,  0xFFu },
    { Bcm_ComBridge_10ms,                       10u,  6u,  0xFFu },
    { Swc_Lights_10ms,                          10u,  5u,     0u },
    { Swc_Indicators_10ms,                      10u,  4u,     1u },
    { Swc_DoorLock_100ms,                      100u,  3u,     2u },
    { Com_MainFunction_Tx,                      10u,  2u,  0xFFu },
    { Can_MainFunction_BusOff,                  10u,  1u,  0xFFu },
};

#define BCM_RUNNABLE_COUNT  (sizeof(bcm_runnable_config) / sizeof(bcm_runnable_config[0]))

/* ==================================================================== */
/* Aggregate RTE Configuration
 * ================================================================== */

const Rte_ConfigType bcm_rte_config = {
    .signalConfig   = bcm_signal_config,
    .signalCount    = BCM_SIG_COUNT,
    .runnableConfig = bcm_runnable_config,
    .runnableCount  = (uint8)BCM_RUNNABLE_COUNT,
};
