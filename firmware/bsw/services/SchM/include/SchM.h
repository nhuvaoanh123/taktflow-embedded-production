/**
 * @file    SchM.h
 * @brief   Schedule Manager — BSW module main function scheduling
 * @date    2026-03-10
 *
 * @standard AUTOSAR SchM (simplified)
 * @copyright Taktflow Systems 2026
 */
#ifndef SCHM_H
#define SCHM_H

#include <stdint.h>

/**
 * @brief   Initialize scheduler with ECU-specific timing config.
 */
void SchM_Init(void);

/**
 * @brief   Run one scheduler tick — dispatches BSW and RTE main functions.
 * @param   tick_ms  Current system tick in milliseconds
 */
void SchM_MainFunction(uint32_t tick_ms);

#endif /* SCHM_H */
