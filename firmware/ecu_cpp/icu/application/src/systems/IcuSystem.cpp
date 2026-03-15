// Taktflow Systems - ICU main system implementation

#include "systems/IcuSystem.h"

namespace taktflow
{
namespace icu
{
namespace systems
{

// -- IcuCanListener --

IcuSystem::IcuCanListener::IcuCanListener(signals::CanFrameCodec& codec)
: _codec(codec)
, _filter()
{
    // Accept CAN IDs used by ICU
    _filter.add(0x001u); // EStop_Broadcast
    _filter.add(0x010u); // CVC_Heartbeat
    _filter.add(0x011u); // FZC_Heartbeat
    _filter.add(0x012u); // RZC_Heartbeat
    _filter.add(0x100u); // Vehicle_State
    _filter.add(0x300u); // Motor_Status
    _filter.add(0x301u); // Motor_Current
    _filter.add(0x302u); // Motor_Temperature
    _filter.add(0x303u); // Battery_Status
    _filter.add(0x500u); // DTC_Broadcast
}

void IcuSystem::IcuCanListener::frameReceived(::can::CANFrame const& frame)
{
    _codec.decodeRx(frame);
}

::can::IFilter& IcuSystem::IcuCanListener::getFilter() { return _filter; }

// -- IcuSystem --

IcuSystem::IcuSystem(::async::ContextType context, ::can::ICanTransceiver& transceiver)
: _context(context)
, _timeout()
, _transceiver(transceiver)
, _signals()
, _codec(_signals, transceiver)
, _dashboard(_signals)
, _dtcDisplay(_signals)
, _canListener(_codec)
{
    setTransitionContext(context);
}

void IcuSystem::init()
{
    _dashboard.init();
    _dtcDisplay.init();
    transitionDone();
}

void IcuSystem::run()
{
    _transceiver.addCANFrameListener(_canListener);
    ::async::scheduleAtFixedRate(
        _context, *this, _timeout, 50u, ::async::TimeUnit::MILLISECONDS);
    transitionDone();
}

void IcuSystem::shutdown()
{
    _timeout.cancel();
    _transceiver.removeCANFrameListener(_canListener);
    transitionDone();
}

void IcuSystem::execute()
{
    // Run SWCs (50ms tick)
    _dashboard.execute();
    _dtcDisplay.execute();

    // Heartbeat at 500ms (every 10th tick)
    ++_heartbeatTickCounter;
    if (_heartbeatTickCounter >= 10u)
    {
        _heartbeatTickCounter = 0u;
        _codec.encodeTxHeartbeat();
    }
}

} // namespace systems
} // namespace icu
} // namespace taktflow
