/**
 * @file    Dem.h
 * @brief   Diagnostic Event Manager — DTC storage and status tracking
 * @date    2026-03-10
 *
 * @standard AUTOSAR DEM (simplified)
 * @copyright Taktflow Systems 2026
 */
#ifndef DEM_H
#define DEM_H

#include <stdint.h>

typedef uint16_t Dem_EventIdType;

typedef enum {
    DEM_EVENT_STATUS_PASSED = 0u,
    DEM_EVENT_STATUS_FAILED,
    DEM_EVENT_STATUS_PREPASSED,
    DEM_EVENT_STATUS_PREFAILED
} Dem_EventStatusType;

/**
 * @brief   Report a diagnostic event status.
 * @param   event_id  DTC event identifier (from *_Cfg.h)
 * @param   status    Event status (passed/failed)
 */
void Dem_ReportErrorStatus(Dem_EventIdType event_id, Dem_EventStatusType status);

/**
 * @brief   Initialize DEM module.
 */
void Dem_Init(void);

/**
 * @brief   DEM main function — process debouncing and status updates.
 */
void Dem_MainFunction(void);

#endif /* DEM_H */
