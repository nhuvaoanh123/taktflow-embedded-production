// Taktflow Systems - Indicators SWC implementation
// Ported from firmware/ecu/bcm/src/Swc_Indicators.c

#include "swc/IndicatorsComponent.h"

namespace taktflow
{
namespace bcm
{
namespace swc
{

IndicatorsComponent::IndicatorsComponent(signals::BcmSignals& signals)
: _signals(signals)
{
}

void IndicatorsComponent::init()
{
    _flashCounter = 0u;
    _flashOn = true;
}

void IndicatorsComponent::execute()
{
    // Hazard requested via hazard command or E-stop
    bool const hazardRequested = (_signals.hazardCmd != 0u) || (_signals.estopActive != 0u);

    // Any indicator activity?
    bool const anyActive = hazardRequested || (_signals.turnSignalCmd != 0u);

    if (!anyActive)
    {
        _signals.leftIndicator = 0u;
        _signals.rightIndicator = 0u;
        _signals.hazardActive = 0u;
        _flashCounter = 0u;
        _flashOn = true;
        return;
    }

    // Flash timing: 33 ticks ON, 33 ticks OFF (~1.5 Hz at 10ms tick)
    ++_flashCounter;
    if (_flashOn)
    {
        if (_flashCounter >= signals::INDICATOR_FLASH_ON)
        {
            _flashOn = false;
            _flashCounter = 0u;
        }
    }
    else
    {
        if (_flashCounter >= signals::INDICATOR_FLASH_OFF)
        {
            _flashOn = true;
            _flashCounter = 0u;
        }
    }

    uint8_t const flashState = _flashOn ? 1u : 0u;

    if (hazardRequested)
    {
        // Hazard: both sides flash
        _signals.leftIndicator = flashState;
        _signals.rightIndicator = flashState;
        _signals.hazardActive = 1u;
    }
    else
    {
        // Turn signal: bit 0 = left, bit 1 = right
        _signals.leftIndicator = (_signals.turnSignalCmd & 0x01u) != 0u ? flashState : 0u;
        _signals.rightIndicator = (_signals.turnSignalCmd & 0x02u) != 0u ? flashState : 0u;
        _signals.hazardActive = 0u;
    }
}

} // namespace swc
} // namespace bcm
} // namespace taktflow
