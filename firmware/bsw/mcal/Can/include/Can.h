/**
 * @file    Can.h
 * @brief   CAN Driver — MCAL CAN controller abstraction
 * @date    2026-03-10
 *
 * @details Platform-specific implementations in firmware/platform/*/src/Can_*.c
 *
 * @standard AUTOSAR MCAL CAN (simplified)
 * @copyright Taktflow Systems 2026
 */
#ifndef CAN_H
#define CAN_H

#include <stdint.h>

typedef struct {
    uint32_t id;
    uint8_t  dlc;
    uint8_t  data[8];
} Can_PduType;

/**
 * @brief   Initialize CAN controller.
 * @return  0 on success
 */
uint8_t Can_Init(void);

/**
 * @brief   Write a CAN frame to the TX mailbox.
 * @param   pdu  Pointer to CAN PDU
 * @return  0 on success
 */
uint8_t Can_Write(const Can_PduType *pdu);

/**
 * @brief   Read pending CAN frames from RX FIFO.
 * @note    Calls CanIf_RxIndication for each received frame.
 */
void Can_MainFunction_Read(void);

/**
 * @brief   Check for bus-off condition.
 */
void Can_MainFunction_BusOff(void);

#endif /* CAN_H */
