// Taktflow Systems - Lights SWC (headlamp and tail light control)

#pragma once

#include "signals/BcmSignals.h"

namespace taktflow
{
namespace bcm
{
namespace swc
{

class LightsComponent
{
public:
    explicit LightsComponent(signals::BcmSignals& signals);

    void init();
    void execute();

private:
    signals::BcmSignals& _signals;
};

} // namespace swc
} // namespace bcm
} // namespace taktflow
