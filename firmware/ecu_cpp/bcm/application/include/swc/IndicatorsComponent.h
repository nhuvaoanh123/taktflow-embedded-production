// Taktflow Systems - Indicators SWC (turn signals, hazard, E-stop flash)

#pragma once

#include "signals/BcmSignals.h"

namespace taktflow
{
namespace bcm
{
namespace swc
{

class IndicatorsComponent
{
public:
    explicit IndicatorsComponent(signals::BcmSignals& signals);

    void init();
    void execute();

private:
    signals::BcmSignals& _signals;
    uint8_t _flashCounter{0};
    bool _flashOn{true};
};

} // namespace swc
} // namespace bcm
} // namespace taktflow
