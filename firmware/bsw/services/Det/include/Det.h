/**
 * @file    Det.h
 * @brief   Default Error Tracer — development-time error reporting
 * @date    2026-03-10
 *
 * @standard AUTOSAR DET (simplified)
 * @copyright Taktflow Systems 2026
 */
#ifndef DET_H
#define DET_H

#include <stdint.h>

/**
 * @brief   Report a development error.
 * @param   module_id   BSW module identifier
 * @param   instance_id Instance (usually 0)
 * @param   api_id      API service identifier
 * @param   error_id    Error code
 */
void Det_ReportError(uint16_t module_id, uint8_t instance_id,
                     uint8_t api_id, uint8_t error_id);

#endif /* DET_H */
