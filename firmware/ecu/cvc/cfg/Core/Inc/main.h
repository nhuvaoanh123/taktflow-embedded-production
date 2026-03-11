/**
 * @file    main.h
 * @brief   CubeMX compatibility header — declares Error_Handler
 * @note    The real main() lives in firmware/ecu/cvc/src/main.c.
 *          This header exists only because CubeMX HAL MSP code includes it.
 */
#ifndef MAIN_H
#define MAIN_H

#include "stm32g4xx_hal.h"

void Error_Handler(void);

#endif /* MAIN_H */
