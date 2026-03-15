// Taktflow Systems - BCM CAN system implementation

#include "systems/BcmCanSystem.h"

namespace taktflow
{
namespace bcm
{
namespace systems
{

BcmCanSystem::BcmCanSystem(::async::ContextType context)
: _context(context)
, _timeout()
, _canConfig{"vcan0", 0}
, _canTransceiver(_canConfig)
{
    setTransitionContext(context);
}

void BcmCanSystem::init() { transitionDone(); }

void BcmCanSystem::run()
{
    _canTransceiver.init();
    _canTransceiver.open();
    ::async::scheduleAtFixedRate(
        _context, *this, _timeout, 1u, ::async::TimeUnit::MILLISECONDS);
    transitionDone();
}

void BcmCanSystem::shutdown()
{
    _timeout.cancel();
    _canTransceiver.close();
    _canTransceiver.shutdown();
    transitionDone();
}

void BcmCanSystem::execute()
{
    _canTransceiver.run(MAX_SENT_PER_RUN, MAX_RECEIVED_PER_RUN);
}

} // namespace systems
} // namespace bcm
} // namespace taktflow
