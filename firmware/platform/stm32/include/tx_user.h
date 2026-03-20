/**
 * @file    tx_user.h
 * @brief   ThreadX user configuration for Taktflow STM32 ECUs
 *
 * Configured for ST CMSIS-RTOS2 wrapper compatibility.
 * Must match x-cube-azrtos-g4 requirements.
 */

#ifndef TX_USER_H
#define TX_USER_H

/* Required by ST CMSIS-RTOS2 wrapper */
#define TX_THREAD_USER_EXTENSION    ULONG tx_thread_detached_joinable;
#define TX_MAX_PRIORITIES           64
#define USE_DYNAMIC_MEMORY_ALLOCATION

/* Stack checking for debug builds */
#ifndef NDEBUG
#define TX_ENABLE_STACK_CHECKING
#endif

#endif /* TX_USER_H */
