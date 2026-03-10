/**
 * @file    CanIf.h
 * @brief   CAN Interface — routes CAN frames between MCAL and upper layers
 * @date    2026-03-10
 *
 * @standard AUTOSAR CanIf (simplified)
 * @copyright Taktflow Systems 2026
 */
#ifndef CANIF_H
#define CANIF_H

#include <stdint.h>

typedef uint16_t CanIf_TxPduIdType;

typedef struct {
    uint32_t can_id;
    uint8_t  dlc;
    uint8_t  data[8];
} CanIf_PduInfoType;

/**
 * @brief   Transmit a PDU via CAN.
 * @param   tx_pdu_id  TX PDU identifier (from generated CanIf config)
 * @param   pdu_info   Pointer to PDU data
 * @return  0 on success
 */
uint8_t CanIf_Transmit(CanIf_TxPduIdType tx_pdu_id, const CanIf_PduInfoType *pdu_info);

/**
 * @brief   RX indication callback — called by MCAL CAN driver on frame reception.
 * @param   can_id  Received CAN ID
 * @param   data    Pointer to 8-byte frame data
 * @param   dlc     Data length code
 */
void CanIf_RxIndication(uint32_t can_id, const uint8_t *data, uint8_t dlc);

/**
 * @brief   Initialize CanIf with ECU-specific routing tables.
 */
void CanIf_Init(void);

#endif /* CANIF_H */
