/**
 * @file    bsw_stubs.c
 * @brief   Stub implementations for BSW modules not used in the ThreadX experiment
 * @date    2026-03-20
 *
 * @details The ThreadX CAN experiment brings in Can, CanIf, PduR, Com, Rte,
 *          SchM, and Det. The remaining BSW modules referenced by the full
 *          FZC configs (Dcm, E2E, WdgM, Dem, BswM, CanTp, etc.) and SWC
 *          runnables are stubbed here so linking succeeds.
 */
#include "Std_Types.h"
#include "ComStack_Types.h"

/* ====================================================================
 * Dcm stubs
 * ==================================================================== */

void Dcm_RxIndication(PduIdType RxPduId, const PduInfoType* PduInfoPtr)
{
    (void)RxPduId;
    (void)PduInfoPtr;
}

/* ====================================================================
 * CanTp stubs
 * ==================================================================== */

void CanTp_RxIndication(PduIdType RxPduId, const PduInfoType* PduInfoPtr)
{
    (void)RxPduId;
    (void)PduInfoPtr;
}

/* ====================================================================
 * WdgM stubs
 * ==================================================================== */

uint8 WdgM_CheckpointReached(uint8 seId)
{
    (void)seId;
    return 0u; /* E_OK */
}

/* ====================================================================
 * E2E stubs
 * ==================================================================== */

typedef struct {
    uint8 DataId;
    uint8 MaxDeltaCounter;
    uint8 DataLength;
} E2E_ConfigType;

typedef struct {
    uint8 Counter;
} E2E_StateType;

uint8 E2E_Protect(const E2E_ConfigType* config, E2E_StateType* state,
                  uint8* data, uint8 length)
{
    (void)config;
    (void)length;
    /* Minimal: increment counter, write to byte 0 nibble */
    if ((data != (void*)0) && (state != (void*)0))
    {
        data[0] = (data[0] & 0xF0u) | (state->Counter & 0x0Fu);
        state->Counter++;
        if (state->Counter > 15u) { state->Counter = 0u; }
    }
    return 0u;
}

void E2E_Init(void)
{
    /* no-op */
}

/* ====================================================================
 * Dem stubs
 * ==================================================================== */

void Dem_ReportErrorStatus(uint8 eventId, uint8 status)
{
    (void)eventId;
    (void)status;
}

/* ====================================================================
 * SWC stubs — runnables referenced in Rte_Cfg_Fzc.c runnable table
 * ==================================================================== */

/* These are needed because the generated runnable config table has
 * function pointers to all FZC SWC runnables. We stub the ones we
 * don't implement in this experiment. */

void Swc_Steering_MainFunction(void) { }
void Swc_Brake_MainFunction(void) { }
void Swc_Lidar_MainFunction(void) { }
void Swc_FzcSafety_MainFunction(void) { }
void Swc_Buzzer_MainFunction(void) { }
void Swc_FzcCanMonitor_Check(void) { }
void Swc_FzcCom_Receive(void) { }
void Swc_FzcCom_TransmitSchedule(void) { }
void Swc_FzcSensorFeeder_MainFunction(void) { }

/* Init stubs for SWCs not used */
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
