// Taktflow Systems - BCM main system implementation

#include "systems/BcmSystem.h"

namespace taktflow
{
namespace bcm
{
namespace systems
{

// -- BcmCanListener --

BcmSystem::BcmCanListener::BcmCanListener(signals::CanFrameCodec& codec)
: _codec(codec)
, _filter()
{
    // Accept CAN IDs used by BCM
    _filter.add(0x001u); // EStop_Broadcast
    _filter.add(0x100u); // Vehicle_State
    _filter.add(0x300u); // Motor_Status
    _filter.add(0x350u); // Body_Control_Cmd
}

void BcmSystem::BcmCanListener::frameReceived(::can::CANFrame const& frame)
{
    _codec.decodeRx(frame);
}

::can::IFilter& BcmSystem::BcmCanListener::getFilter() { return _filter; }

// -- BcmSystem --

BcmSystem::BcmSystem(::async::ContextType context, ::can::ICanTransceiver& transceiver)
: _context(context)
, _timeout()
, _transceiver(transceiver)
, _signals()
, _codec(_signals, transceiver)
, _lights(_signals)
, _indicators(_signals)
, _doorLock(_signals)
, _canListener(_codec)
{
    setTransitionContext(context);
}

void BcmSystem::init()
{
    _lights.init();
    _indicators.init();
    _doorLock.init();
    transitionDone();
}

void BcmSystem::run()
{
    _transceiver.addCANFrameListener(_canListener);
    ::async::scheduleAtFixedRate(
        _context, *this, _timeout, 10u, ::async::TimeUnit::MILLISECONDS);
    transitionDone();
}

void BcmSystem::shutdown()
{
    _timeout.cancel();
    _transceiver.removeCANFrameListener(_canListener);
    transitionDone();
}

void BcmSystem::execute()
{
    // Run SWCs (10ms tick)
    _lights.execute();
    _indicators.execute();

    // Door lock at 100ms (every 10th tick)
    ++_doorLockTickCounter;
    if (_doorLockTickCounter >= 10u)
    {
        _doorLockTickCounter = 0u;
        _doorLock.execute();
    }

    // Encode and transmit TX frames
    _codec.encodeTxLightStatus();
    _codec.encodeTxIndicatorState();
    _codec.encodeTxDoorLockStatus();

    // Heartbeat at 500ms (every 50th tick)
    ++_heartbeatTickCounter;
    if (_heartbeatTickCounter >= 50u)
    {
        _heartbeatTickCounter = 0u;
        _codec.encodeTxHeartbeat();
    }
}

} // namespace systems
} // namespace bcm
} // namespace taktflow
