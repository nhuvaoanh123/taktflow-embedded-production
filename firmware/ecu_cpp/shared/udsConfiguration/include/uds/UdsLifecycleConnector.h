// Taktflow Systems - UDS lifecycle connector (stub)

#pragma once

#include "uds/lifecycle/IUdsLifecycleConnector.h"

namespace lifecycle
{
class LifecycleManager;
}

namespace uds
{
class UdsLifecycleConnector : public IUdsLifecycleConnector
{
public:
    explicit UdsLifecycleConnector(lifecycle::LifecycleManager&) {}

    bool isModeChangePossible() const { return true; }
    bool requestPowerdown(bool, uint8_t&) { return true; }
    bool requestShutdown(ShutdownType, uint32_t) { return true; }
};
} // namespace uds
