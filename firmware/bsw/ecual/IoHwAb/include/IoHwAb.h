/**
 * @file    IoHwAb.h
 * @brief   IO Hardware Abstraction — sensor/actuator access API
 * @date    2026-03-10
 *
 * @details Abstracts ADC, PWM, DIO access so SWCs don't touch MCAL directly.
 *
 * @standard AUTOSAR IoHwAb (simplified)
 * @copyright Taktflow Systems 2026
 */
#ifndef IOHWAB_H
#define IOHWAB_H

#include <stdint.h>

/**
 * @brief   Read an ADC channel value.
 * @param   group    ADC group identifier
 * @param   channel  Channel within group
 * @param   value    Pointer to receive raw ADC value
 * @return  0 on success
 */
uint8_t IoHwAb_AdcRead(uint8_t group, uint8_t channel, uint16_t *value);

/**
 * @brief   Set PWM duty cycle.
 * @param   channel  PWM channel identifier
 * @param   duty_us  Duty cycle in microseconds
 */
void IoHwAb_PwmSetDuty(uint8_t channel, uint16_t duty_us);

/**
 * @brief   Read digital input.
 * @param   channel  DIO channel identifier
 * @return  Pin level (0 or 1)
 */
uint8_t IoHwAb_DioRead(uint8_t channel);

/**
 * @brief   Write digital output.
 * @param   channel  DIO channel identifier
 * @param   level    Pin level (0 or 1)
 */
void IoHwAb_DioWrite(uint8_t channel, uint8_t level);

/**
 * @brief   Initialize IO Hardware Abstraction.
 */
void IoHwAb_Init(void);

#endif /* IOHWAB_H */
