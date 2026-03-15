// Taktflow Systems - FreeRTOS platform config for BCM (POSIX)

#pragma once

#ifdef __cplusplus
extern "C"
{
#endif

#define INCLUDE_uxTaskPriorityGet (1)
#undef configCHECK_FOR_STACK_OVERFLOW
#define configCHECK_FOR_STACK_OVERFLOW 2

#ifndef MINIMUM_STACK_SIZE
#define EXTRA_THREAD_DATA_STACK_SIZE 40U
#if (defined(_DYNAMIC_STACK_SIZE_SOURCE)) || (defined(_GNU_SOURCE))
#define MINIMUM_STACK_SIZE (16384U + EXTRA_THREAD_DATA_STACK_SIZE)
#else
#include <pthread.h>
#define MINIMUM_STACK_SIZE ((PTHREAD_STACK_MIN) + EXTRA_THREAD_DATA_STACK_SIZE)
#endif
#endif // MINIMUM_STACK_SIZE

#ifdef __cplusplus
} /* extern "C" */
#endif
