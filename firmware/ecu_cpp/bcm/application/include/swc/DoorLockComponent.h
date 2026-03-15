// Taktflow Systems - Door Lock SWC (auto-lock, auto-unlock, manual lock)

#pragma once

#include "signals/BcmSignals.h"

namespace taktflow
{
namespace bcm
{
namespace swc
{

class DoorLockComponent
{
public:
    explicit DoorLockComponent(signals::BcmSignals& signals);

    void init();
    void execute();

private:
    signals::BcmSignals& _signals;
    uint8_t _lockState{0};
    uint8_t _prevVehicleState{0};
};

} // namespace swc
} // namespace bcm
} // namespace taktflow
