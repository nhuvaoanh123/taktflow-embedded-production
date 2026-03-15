// Taktflow Systems - TCU SocketCAN lifecycle component

#pragma once

#include <async/Async.h>
#include <async/IRunnable.h>
#include <can/transceiver/ICanTransceiver.h>
#include <lifecycle/AsyncLifecycleComponent.h>

namespace taktflow
{
namespace tcu
{
namespace systems
{

class TcuCanSystem
: public ::lifecycle::AsyncLifecycleComponent
, private ::async::IRunnable
{
public:
    explicit TcuCanSystem(::async::ContextType context);

    TcuCanSystem(TcuCanSystem const&) = delete;
    TcuCanSystem& operator=(TcuCanSystem const&) = delete;

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
} // namespace tcu
} // namespace taktflow
