// Taktflow Systems - TCU main system (SWC scheduler + CAN listener)

#pragma once

#include "signals/CanFrameCodec.h"
#include "signals/TcuSignals.h"
#include "swc/DtcStoreComponent.h"
#include "swc/Obd2PidsComponent.h"

#include <async/Async.h>
#include <async/IRunnable.h>
#include <can/filter/BitFieldFilter.h>
#include <can/framemgmt/ICANFrameListener.h>
#include <can/transceiver/ICanTransceiver.h>
#include <lifecycle/AsyncLifecycleComponent.h>

namespace taktflow
{
namespace tcu
{
namespace systems
{

class TcuSystem
: public ::lifecycle::AsyncLifecycleComponent
, private ::async::IRunnable
{
public:
    TcuSystem(::async::ContextType context, ::can::ICanTransceiver& transceiver);

    TcuSystem(TcuSystem const&) = delete;
    TcuSystem& operator=(TcuSystem const&) = delete;

    void init() final;
    void run() final;
    void shutdown() final;
    signals::TcuSignals& getSignals() { return _signals; }

private:
    void execute() final;

    class TcuCanListener : public ::can::ICANFrameListener
    {
    public:
        TcuCanListener(signals::CanFrameCodec& codec);

        void frameReceived(::can::CANFrame const& frame) final;
        ::can::IFilter& getFilter() final;

    private:
        signals::CanFrameCodec& _codec;
        ::can::BitFieldFilter _filter;
    };

    ::async::ContextType const _context;
    ::async::TimeoutType _timeout;
    ::can::ICanTransceiver& _transceiver;

    signals::TcuSignals _signals;
    signals::CanFrameCodec _codec;

    swc::DtcStoreComponent _dtcStore;
    swc::Obd2PidsComponent _obd2Pids;

    TcuCanListener _canListener;

    uint8_t _heartbeatTickCounter{0};
};

} // namespace systems
} // namespace tcu
} // namespace taktflow
