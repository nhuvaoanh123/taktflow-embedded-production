// Taktflow Systems - TCU SocketCAN lifecycle component

#include "systems/TcuCanSystem.h"

#include <can/SocketCanTransceiver.h>

namespace taktflow
{
namespace tcu
{
namespace systems
{

static ::can::SocketCanTransceiver::DeviceConfig const canConfig{"vcan0", 0};
static ::can::SocketCanTransceiver canTransceiver{canConfig};

TcuCanSystem::TcuCanSystem(::async::ContextType context)
: _context(context)
, _timeout()
{
    setTransitionContext(context);
}

::can::ICanTransceiver& TcuCanSystem::getTransceiver() { return canTransceiver; }

void TcuCanSystem::init()
{
    canTransceiver.init();
    transitionDone();
}

void TcuCanSystem::run()
{
    canTransceiver.open();
    ::async::scheduleAtFixedRate(
        _context, *this, _timeout, 1u, ::async::TimeUnit::MILLISECONDS);
    transitionDone();
}

void TcuCanSystem::shutdown()
{
    _timeout.cancel();
    canTransceiver.close();
    transitionDone();
}

void TcuCanSystem::execute()
{
    canTransceiver.run(3, 3);
}

} // namespace systems
} // namespace tcu
} // namespace taktflow
