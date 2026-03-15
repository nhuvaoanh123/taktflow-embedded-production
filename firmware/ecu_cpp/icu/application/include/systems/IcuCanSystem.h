// Taktflow Systems - ICU SocketCAN lifecycle component

#pragma once

#include <async/Async.h>
#include <async/IRunnable.h>
#include <can/transceiver/ICanTransceiver.h>
#include <lifecycle/AsyncLifecycleComponent.h>

namespace taktflow
{
namespace icu
{
namespace systems
{

class IcuCanSystem
: public ::lifecycle::AsyncLifecycleComponent
, private ::async::IRunnable
{
public:
    explicit IcuCanSystem(::async::ContextType context);

    IcuCanSystem(IcuCanSystem const&) = delete;
    IcuCanSystem& operator=(IcuCanSystem const&) = delete;

    ::can::ICanTransceiver& getTransceiver();

    void init() final;
    void run() final;
    void shutdown() final;

private:
    void execute() final;

    ::async::ContextType const _context;
    ::async::TimeoutType _timeout;
};

} // namespace systems
} // namespace icu
} // namespace taktflow
