/**
 * @file    Rte.h
 * @brief   Runtime Environment — signal exchange and runnable scheduling API
 * @date    2026-03-10
 *
 * @details Provides typed signal read/write and cyclic runnable dispatch.
 *          All SWCs communicate exclusively through the RTE.
 *
 * @standard AUTOSAR R22-11 (simplified)
 * @copyright Taktflow Systems 2026
 */
#ifndef RTE_H
#define RTE_H

#include <stdint.h>

/* Signal value type (all signals stored as uint32_t internally) */
typedef uint32_t Rte_SignalValueType;

/* Status return */
typedef enum {
    RTE_E_OK = 0u,
    RTE_E_INVALID,
    RTE_E_LIMIT,
    RTE_E_NOT_CONNECTED
} Rte_StatusType;

/**
 * @brief   Write a signal value to the RTE buffer.
 * @param   signal_id  Signal identifier (from *_Cfg.h)
 * @param   value      Signal value to write
 * @return  RTE_E_OK on success
 */
Rte_StatusType Rte_Write(uint16_t signal_id, Rte_SignalValueType value);

/**
 * @brief   Read a signal value from the RTE buffer.
 * @param   signal_id  Signal identifier (from *_Cfg.h)
 * @param   value      Pointer to receive signal value
 * @return  RTE_E_OK on success, RTE_E_INVALID if never written
 */
Rte_StatusType Rte_Read(uint16_t signal_id, Rte_SignalValueType *value);

/**
 * @brief   Initialize the RTE (clear all signal buffers, register runnables).
 */
void Rte_Init(void);

/**
 * @brief   Dispatch all due runnables based on elapsed ticks.
 * @param   tick_ms  Current tick count in milliseconds
 */
void Rte_MainFunction(uint32_t tick_ms);

#endif /* RTE_H */
