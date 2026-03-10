/**
 * @file    PduR.h
 * @brief   PDU Router — routes PDUs between CanIf, Com, Dcm, CanTp
 * @date    2026-03-10
 *
 * @standard AUTOSAR PduR (simplified)
 * @copyright Taktflow Systems 2026
 */
#ifndef PDUR_H
#define PDUR_H

#include <stdint.h>

typedef uint8_t PduR_PduIdType;

typedef enum {
    PDUR_DEST_COM = 0u,
    PDUR_DEST_DCM,
    PDUR_DEST_CANTP,
    PDUR_DEST_NONE
} PduR_DestType;

/**
 * @brief   Route a received PDU to its destination module.
 * @param   pdu_id  RX PDU identifier
 * @param   data    Pointer to PDU data
 * @param   length  PDU data length
 */
void PduR_RxIndication(PduR_PduIdType pdu_id, const uint8_t *data, uint8_t length);

/**
 * @brief   Request transmission of a PDU.
 * @param   pdu_id  TX PDU identifier
 * @param   data    Pointer to PDU data
 * @param   length  PDU data length
 * @return  0 on success
 */
uint8_t PduR_Transmit(PduR_PduIdType pdu_id, const uint8_t *data, uint8_t length);

/**
 * @brief   Initialize PDU Router with ECU-specific routing config.
 */
void PduR_Init(void);

#endif /* PDUR_H */
