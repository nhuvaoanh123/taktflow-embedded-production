// Taktflow Systems - BCM async binding (FreeRTOS POSIX adapter)

#pragma once

#include <async/Config.h>
#include <async/StaticContextHook.h>
#include <runtime/RuntimeMonitor.h>
#include <runtime/RuntimeStatistics.h>

#include <platform/estdint.h>

#ifdef SUPPORT_FREERTOS
#include <async/FreeRtosAdapter.h>
#endif

namespace async
{
struct AsyncBinding : public Config
{
    static size_t const WAIT_EVENTS_TICK_COUNT = 100U;

#ifdef SUPPORT_FREERTOS
    using AdapterType = FreeRtosAdapter<AsyncBinding>;
#endif

    using RuntimeMonitorType = ::runtime::declare::RuntimeMonitor<
        ::runtime::RuntimeStatistics,
        ::runtime::RuntimeStatistics,
        AdapterType::OS_TASK_COUNT,
        ISR_GROUP_COUNT>;

    using ContextHookType = StaticContextHook<RuntimeMonitorType>;
};

using AsyncBindingType = AsyncBinding;
} // namespace async
