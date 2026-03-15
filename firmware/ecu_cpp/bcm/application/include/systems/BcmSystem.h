// Taktflow Systems - BCM main system (SWC scheduler + CAN listener)

#pragma once

#include "signals/BcmSignals.h"
#include "signals/CanFrameCodec.h"
#include "swc/DoorLockComponent.h"
#include "swc/IndicatorsComponent.h"
#include "swc/LightsComponent.h"

#include <async/Async.h>
#include <async/IRunnable.h>
#include <can/filter/BitFieldFilter.h>
#include <can/framemgmt/ICANFrameListener.h>
#include <can/transceiver/ICanTransceiver.h>
#include <lifecycle/AsyncLifecycleComponent.h>

namespace taktflow
{
namespace bcm
{
namespace systems
{

class BcmSystem
: public ::lifecycle::AsyncLifecycleComponent
, private ::async::IRunnable
{
public:
    BcmSystem(::async::ContextType context, ::can::ICanTransceiver& transceiver);

    BcmSystem(BcmSystem const&) = delete;
    BcmSystem& operator=(BcmSystem const&) = delete;

    void init() final;
    void run() final;
    void shutdown() final;
    signals::BcmSignals& getSignals() { return _signals; }

private:
    void execute() final;

    /// Inner CAN frame listener
    class BcmCanListener : public ::can::ICANFrameListener
    {
    public:
        BcmCanListener(signals::CanFrameCodec& codec);

        void frameReceived(::can::CANFrame const& frame) final;
        ::can::IFilter& getFilter() final;

    private:
        signals::CanFrameCodec& _codec;
        ::can::BitFieldFilter _filter;
    };

    ::async::ContextType const _context;
    ::async::TimeoutType _timeout;
    ::can::ICanTransceiver& _transceiver;

    signals::BcmSignals _signals;
    signals::CanFrameCodec _codec;

    swc::LightsComponent _lights;
    swc::IndicatorsComponent _indicators;
    swc::DoorLockComponent _doorLock;

    BcmCanListener _canListener;

    uint8_t _doorLockTickCounter{0};
    uint8_t _heartbeatTickCounter{0};
};

} // namespace systems
} // namespace bcm
} // namespace taktflow
