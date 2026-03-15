// Taktflow Systems - BCM CAN system (SocketCAN transceiver lifecycle)

#pragma once

#include <async/Async.h>
#include <async/IRunnable.h>
#include <can/SocketCanTransceiver.h>
#include <lifecycle/AsyncLifecycleComponent.h>

namespace taktflow
{
namespace bcm
{
namespace systems
{

class BcmCanSystem
: public ::lifecycle::AsyncLifecycleComponent
, private ::async::IRunnable
{
public:
    explicit BcmCanSystem(::async::ContextType context);

    BcmCanSystem(BcmCanSystem const&) = delete;
    BcmCanSystem& operator=(BcmCanSystem const&) = delete;

    ::can::ICanTransceiver& getTransceiver() { return _canTransceiver; }

    void init() final;
    void run() final;
    void shutdown() final;

private:
    void execute() final;

    static constexpr int MAX_SENT_PER_RUN     = 16;
    static constexpr int MAX_RECEIVED_PER_RUN = 16;

    ::async::ContextType const _context;
    ::async::TimeoutType _timeout;
    ::can::SocketCanTransceiver::DeviceConfig _canConfig;
    ::can::SocketCanTransceiver _canTransceiver;
};

} // namespace systems
} // namespace bcm
} // namespace taktflow
