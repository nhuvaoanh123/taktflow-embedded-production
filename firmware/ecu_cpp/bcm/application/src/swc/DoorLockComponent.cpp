// Taktflow Systems - Door Lock SWC implementation
// Ported from firmware/ecu/bcm/src/Swc_DoorLock.c

#include "swc/DoorLockComponent.h"

namespace taktflow
{
namespace bcm
{
namespace swc
{

DoorLockComponent::DoorLockComponent(signals::BcmSignals& signals)
: _signals(signals)
{
}

void DoorLockComponent::init()
{
    _lockState = 0u;
    _prevVehicleState = signals::VSTATE_INIT;
}

void DoorLockComponent::execute()
{
    uint8_t const vehicleState = _signals.vehicleState;
    uint16_t const speed = _signals.motorSpeedRpm;

    // Manual lock command (doorLockCmd != 0 → locked)
    if (_signals.doorLockCmd != 0u)
    {
        _lockState = 1u;
    }

    // Auto-lock: speed > AUTO_LOCK_SPEED → locked
    if (speed > signals::AUTO_LOCK_SPEED)
    {
        _lockState = 1u;
    }

    // Auto-unlock: transition from non-parked to parked
    bool const wasNotParked = (_prevVehicleState != signals::VSTATE_INIT)
                              && (_prevVehicleState != signals::VSTATE_READY);
    bool const isParked = (vehicleState == signals::VSTATE_INIT)
                          || (vehicleState == signals::VSTATE_READY);

    if (wasNotParked && isParked)
    {
        _lockState = 0u;
    }

    _prevVehicleState = vehicleState;
    _signals.frontLeftLock = _lockState;
}

} // namespace swc
} // namespace bcm
} // namespace taktflow
