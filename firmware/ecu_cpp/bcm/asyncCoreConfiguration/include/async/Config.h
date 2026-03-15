// Taktflow Systems - BCM async task configuration

#pragma once

#define ASYNC_CONFIG_TICK_IN_US (1000U)

#ifndef __ASSEMBLER__

enum
{
    // TASK_IDLE = 0
    TASK_BACKGROUND = 1,
    TASK_CAN,
    TASK_BCM,
    TASK_UDS,
    ASYNC_CONFIG_TASK_COUNT,
};

enum
{
    ISR_GROUP_CAN = 0,
    ISR_GROUP_COUNT,
};

#endif // __ASSEMBLER__
