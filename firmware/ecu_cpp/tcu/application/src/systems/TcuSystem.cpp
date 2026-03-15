// Taktflow Systems - TCU main system implementation

#include "systems/TcuSystem.h"

namespace taktflow
{
namespace tcu
{
namespace systems
{

// -- TcuCanListener --

TcuSystem::TcuCanListener::TcuCanListener(signals::CanFrameCodec& codec)
: _codec(codec)
, _filter()
{
    _filter.add(0x001u); // EStop_Broadcast
    _filter.add(0x010u); // CVC_Heartbeat
    _filter.add(0x100u); // Vehicle_State
    _filter.add(0x300u); // Motor_Status
    _filter.add(0x301u); // Motor_Current
    _filter.add(0x302u); // Motor_Temperature
    _filter.add(0x303u); // Battery_Status
    _filter.add(0x500u); // DTC_Broadcast
}

void TcuSystem::TcuCanListener::frameReceived(::can::CANFrame const& frame)
{
    _codec.decodeRx(frame);
}

::can::IFilter& TcuSystem::TcuCanListener::getFilter() { return _filter; }

// -- TcuSystem --

TcuSystem::TcuSystem(::async::ContextType context, ::can::ICanTransceiver& transceiver)
: _context(context)
, _timeout()
, _transceiver(transceiver)
, _signals()
, _codec(_signals, transceiver)
, _dtcStore(_signals)
, _obd2Pids(_signals, _dtcStore)
, _canListener(_codec)
{
    setTransitionContext(context);
}

void TcuSystem::init()
{
    _dtcStore.init();
    _obd2Pids.init();
    transitionDone();
}

void TcuSystem::run()
{
    _transceiver.addCANFrameListener(_canListener);
    ::async::scheduleAtFixedRate(
        _context, *this, _timeout, 10u, ::async::TimeUnit::MILLISECONDS);
    transitionDone();
}

void TcuSystem::shutdown()
{
    _timeout.cancel();
    _transceiver.removeCANFrameListener(_canListener);
    transitionDone();
}

void TcuSystem::execute()
{
    // Run SWCs (10ms tick)
    _dtcStore.execute();

    // Heartbeat at 500ms (every 50th tick)
    ++_heartbeatTickCounter;
    if (_heartbeatTickCounter >= 50u)
    {
        _heartbeatTickCounter = 0u;
        _codec.encodeTxHeartbeat();
    }
}

} // namespace systems
} // namespace tcu
} // namespace taktflow
