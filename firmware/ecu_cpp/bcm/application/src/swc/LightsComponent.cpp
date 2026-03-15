// Taktflow Systems - Lights SWC implementation
// Ported from firmware/ecu/bcm/src/Swc_Lights.c

#include "swc/LightsComponent.h"

namespace taktflow
{
namespace bcm
{
namespace swc
{

LightsComponent::LightsComponent(signals::BcmSignals& signals)
: _signals(signals)
{
}

void LightsComponent::init()
{
    // All outputs zero-initialized via BcmSignals default values
}

void LightsComponent::execute()
{
    // Auto headlamp: ON when DRIVING and speed > 0
    bool const autoOn = (_signals.vehicleState == signals::VSTATE_DRIVING)
                        && (_signals.motorSpeedRpm > 0u);

    // Manual override from body control command
    bool const manualOverride = (_signals.headlightCmd != 0u);

    // Headlamp ON if auto condition met OR manual override
    uint8_t const headlamp = (autoOn || manualOverride) ? 1u : 0u;

    _signals.headlightOn = headlamp;
    _signals.tailLightOn = headlamp; // tail follows headlamp
}

} // namespace swc
} // namespace bcm
} // namespace taktflow
