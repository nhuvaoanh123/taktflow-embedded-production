// Taktflow Systems - ICU main system (SWC scheduler + CAN listener)

#pragma once

#include "signals/CanFrameCodec.h"
#include "signals/IcuSignals.h"
#include "swc/DashboardComponent.h"
#include "swc/DtcDisplayComponent.h"

#include <async/Async.h>
#include <async/IRunnable.h>
#include <can/filter/BitFieldFilter.h>
#include <can/framemgmt/ICANFrameListener.h>
#include <can/transceiver/ICanTransceiver.h>
#include <lifecycle/AsyncLifecycleComponent.h>

namespace taktflow
{
namespace icu
{
namespace systems
{

class IcuSystem
: public ::lifecycle::AsyncLifecycleComponent
, private ::async::IRunnable
{
public:
    IcuSystem(::async::ContextType context, ::can::ICanTransceiver& transceiver);

    IcuSystem(IcuSystem const&) = delete;
    IcuSystem& operator=(IcuSystem const&) = delete;

    void init() final;
    void run() final;
    void shutdown() final;
    signals::IcuSignals& getSignals() { return _signals; }

private:
    void execute() final;

    /// Inner CAN frame listener
    class IcuCanListener : public ::can::ICANFrameListener
    {
    public:
        IcuCanListener(signals::CanFrameCodec& codec);

        void frameReceived(::can::CANFrame const& frame) final;
        ::can::IFilter& getFilter() final;

    private:
        signals::CanFrameCodec& _codec;
        ::can::BitFieldFilter _filter;
    };

    ::async::ContextType const _context;
    ::async::TimeoutType _timeout;
    ::can::ICanTransceiver& _transceiver;

    signals::IcuSignals _signals;
    signals::CanFrameCodec _codec;

    swc::DashboardComponent _dashboard;
    swc::DtcDisplayComponent _dtcDisplay;

    IcuCanListener _canListener;

    uint8_t _heartbeatTickCounter{0};
};

} // namespace systems
} // namespace icu
} // namespace taktflow
