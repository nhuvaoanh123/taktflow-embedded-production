/**
 * @file    E2E.h
 * @brief   End-to-End protection — CRC8 + alive counter + data ID
 * @date    2026-03-10
 *
 * @details Implements E2E Profile used across all Taktflow CAN messages:
 *          Byte 0: [DataID:4][AliveCounter:4]
 *          Byte 7: CRC8 over bytes 0-6
 *
 * @standard AUTOSAR E2E (simplified), ISO 26262 Part 6
 * @copyright Taktflow Systems 2026
 */
#ifndef E2E_H
#define E2E_H

#include <stdint.h>

typedef enum {
    E2E_E_OK = 0u,
    E2E_E_WRONGCRC,
    E2E_E_WRONGSEQUENCE,
    E2E_E_REPEATED,
    E2E_E_NOTAVAILABLE
} E2E_StatusType;

/**
 * @brief   Protect an 8-byte CAN message with E2E header + CRC.
 * @param   data      8-byte message buffer (modified in place)
 * @param   data_id   4-bit data ID for this message type
 * @param   counter   Pointer to alive counter (incremented after use)
 */
void E2E_Protect(uint8_t data[8], uint8_t data_id, uint8_t *counter);

/**
 * @brief   Check E2E protection on a received 8-byte CAN message.
 * @param   data        8-byte received message
 * @param   data_id     Expected 4-bit data ID
 * @param   last_counter  Pointer to last seen counter (updated on success)
 * @return  E2E_E_OK if valid
 */
E2E_StatusType E2E_Check(const uint8_t data[8], uint8_t data_id, uint8_t *last_counter);

#endif /* E2E_H */
