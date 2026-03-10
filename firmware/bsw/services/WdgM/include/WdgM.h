/**
 * @file    WdgM.h
 * @brief   Watchdog Manager — supervised entity alive monitoring
 * @date    2026-03-10
 *
 * @standard AUTOSAR WdgM (simplified)
 * @copyright Taktflow Systems 2026
 */
#ifndef WDGM_H
#define WDGM_H

#include <stdint.h>

typedef uint8_t WdgM_SupervisedEntityIdType;

/**
 * @brief   Report alive checkpoint for a supervised entity.
 * @param   se_id  Supervised entity identifier (from Rte_Cfg)
 */
void WdgM_CheckpointReached(WdgM_SupervisedEntityIdType se_id);

/**
 * @brief   Initialize WdgM module.
 */
void WdgM_Init(void);

/**
 * @brief   WdgM main function — check alive counters and deadlines.
 */
void WdgM_MainFunction(void);

#endif /* WDGM_H */
