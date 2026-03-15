// Taktflow Systems - Transport message router lifecycle component

#pragma once

#include <lifecycle/AsyncLifecycleComponent.h>
#include <transport/ITransportSystem.h>
#include <transport/routing/TransportRouterSimple.h>

namespace taktflow
{
namespace systems
{

class TransportSystem
: public ::transport::ITransportSystem
, public ::lifecycle::AsyncLifecycleComponent
{
public:
    explicit TransportSystem(::async::ContextType transitionContext);

    void init() override;
    void run() override;
    void shutdown() override;

    void addTransportLayer(::transport::AbstractTransportLayer& layer) override;
    void removeTransportLayer(::transport::AbstractTransportLayer& layer) override;
    ::transport::ITransportMessageProvider& getTransportMessageProvider() override;

private:
    ::transport::TransportRouterSimple _transportRouter;
};

} // namespace systems
} // namespace taktflow
