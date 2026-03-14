/**
 * @file    sc_types.h
 * @brief   Platform type definitions for TMS570LC43x Safety Controller
 * @date    2026-03-14
 *
 * @details Delegates to AUTOSAR BSW Platform_Types.h and Std_Types.h
 *          so that SC application code and the OSEK bootstrap kernel
 *          share a single set of type definitions (no redefinition
 *          conflicts when Os.h is included alongside sc_types.h).
 *
 * @safety_req SWR-SC-001 to SWR-SC-026
 * @traces_to  SSR-SC-001 to SSR-SC-017
 * @note    Safety level: ASIL D
 * @standard ISO 26262 Part 6, MISRA C:2012
 * @copyright Taktflow Systems 2026
 */
#ifndef SC_TYPES_H
#define SC_TYPES_H

#include "Std_Types.h"      /* uint8, uint16, uint32, sint*, boolean,
                               TRUE, FALSE, E_OK, E_NOT_OK, NULL_PTR */

#endif /* SC_TYPES_H */
