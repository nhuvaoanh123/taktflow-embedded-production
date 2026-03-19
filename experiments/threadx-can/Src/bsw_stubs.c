/**
 * @file    bsw_stubs.c
 * @brief   Stub implementations for BSW modules not used in ThreadX experiment
 * @date    2026-03-20
 *
 * @details Step 8: Most BSW modules are now real. Only SWC runnables
 *          (except Swc_Heartbeat) and a few init functions remain stubbed.
 */
#include "Std_Types.h"

/* ====================================================================
 * SWC runnables — referenced in Rte_Cfg_Fzc.c runnable table
 * ==================================================================== */

void Swc_Steering_MainFunction(void) { }
void Swc_Brake_MainFunction(void) { }
void Swc_Lidar_MainFunction(void) { }
void Swc_FzcSafety_MainFunction(void) { }
void Swc_Buzzer_MainFunction(void) { }
void Swc_FzcCanMonitor_Check(void) { }
void Swc_FzcCom_Receive(void) { }
void Swc_FzcCom_TransmitSchedule(void) { }
void Swc_FzcSensorFeeder_MainFunction(void) { }

/* SWC init stubs */
void Swc_Brake_Init(void) { }
void Swc_Buzzer_Init(void) { }
void Swc_FzcCanMonitor_Init(void) { }
void Swc_FzcCom_Init(void) { }
void Swc_FzcDcm_Init(void) { }
void Swc_FzcNvm_Init(void) { }
void Swc_FzcSafety_Init(void) { }
void Swc_FzcScheduler_Init(void) { }
void Swc_FzcSensorFeeder_Init(void) { }
void Swc_Lidar_Init(void) { }
void Swc_Steering_Init(void) { }

/* ====================================================================
 * IoHwAb stubs — referenced by SWCs
 * ==================================================================== */
void IoHwAb_Init(const void* cfg) { (void)cfg; }

/* ====================================================================
 * NvM stubs — referenced by Dem
 * ==================================================================== */
Std_ReturnType NvM_ReadBlock(uint8 BlockId, void* DstPtr)
{
    (void)BlockId; (void)DstPtr;
    return E_OK;
}

Std_ReturnType NvM_WriteBlock(uint8 BlockId, const void* SrcPtr)
{
    (void)BlockId; (void)SrcPtr;
    return E_OK;
}
