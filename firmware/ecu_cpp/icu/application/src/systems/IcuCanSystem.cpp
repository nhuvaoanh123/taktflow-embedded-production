// Taktflow Systems - ICU SocketCAN lifecycle component

#include "systems/IcuCanSystem.h"

#include <can/SocketCanTransceiver.h>

namespace taktflow
{
namespace icu
{
namespace systems
{

static ::can::SocketCanTransceiver::DeviceConfig const canConfig{"vcan0", 0};
static ::can::SocketCanTransceiver canTransceiver{canConfig};

IcuCanSystem::IcuCanSystem(::async::ContextType context)
: _context(context)
, _timeout()
{
    setTransitionContext(context);
}

::can::ICanTransceiver& IcuCanSystem::getTransceiver() { return canTransceiver; }

void IcuCanSystem::init()
{
    canTransceiver.init();
    transitionDone();
}

void IcuCanSystem::run()
{
    canTransceiver.open();
    ::async::scheduleAtFixedRate(
        _context, *this, _timeout, 1u, ::async::TimeUnit::MILLISECONDS);
    transitionDone();
}

void IcuCanSystem::shutdown()
{
    _timeout.cancel();
    canTransceiver.close();
    transitionDone();
}

void IcuCanSystem::execute()
{
    canTransceiver.run(3, 3);
}

} // namespace systems
} // namespace icu
} // namespace taktflow
