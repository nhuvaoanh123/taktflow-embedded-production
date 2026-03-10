/**
 * @file    Com.h
 * @brief   Communication Service — signal-level CAN send/receive API
 * @date    2026-03-10
 *
 * @standard AUTOSAR COM (simplified)
 * @copyright Taktflow Systems 2026
 */
#ifndef COM_H
#define COM_H

#include <stdint.h>

typedef uint16_t Com_SignalIdType;
typedef uint8_t  Com_PduIdType;

typedef enum {
    COM_E_OK = 0u,
    COM_E_PARAM,
    COM_E_NOT_OK
} Com_StatusType;

/**
 * @brief   Send a signal value via COM (queued for next TX cycle).
 * @param   signal_id  Signal identifier
 * @param   data       Pointer to signal data
 * @return  COM_E_OK on success
 */
Com_StatusType Com_SendSignal(Com_SignalIdType signal_id, const void *data);

/**
 * @brief   Receive a signal value from COM shadow buffer.
 * @param   signal_id  Signal identifier
 * @param   data       Pointer to receive signal data
 * @return  COM_E_OK on success
 */
Com_StatusType Com_ReceiveSignal(Com_SignalIdType signal_id, void *data);

/**
 * @brief   Initialize COM module with ECU-specific config.
 */
void Com_Init(void);

/**
 * @brief   COM main function — process TX and RX PDUs.
 */
void Com_MainFunction(void);

#endif /* COM_H */
