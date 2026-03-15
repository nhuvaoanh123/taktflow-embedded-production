// Taktflow Systems - Transport message router implementation

#include "systems/TransportSystem.h"

namespace taktflow
{
namespace systems
{

TransportSystem::TransportSystem(::async::ContextType transitionContext)
{
    setTransitionContext(transitionContext);
}

void TransportSystem::init()
{
    _transportRouter.init();
    transitionDone();
}

void TransportSystem::run() { transitionDone(); }

void TransportSystem::shutdown() { transitionDone(); }

void TransportSystem::addTransportLayer(::transport::AbstractTransportLayer& layer)
{
    _transportRouter.addTransportLayer(layer);
}

void TransportSystem::removeTransportLayer(::transport::AbstractTransportLayer& layer)
{
    _transportRouter.removeTransportLayer(layer);
}

::transport::ITransportMessageProvider& TransportSystem::getTransportMessageProvider()
{
    return _transportRouter;
}

} // namespace systems
} // namespace taktflow
