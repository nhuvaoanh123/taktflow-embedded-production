// Taktflow Systems - CAN frame encode/decode (replaces Com + ComBridge + CanIf)

#pragma once

#include "signals/BcmSignals.h"

#include <can/canframes/CANFrame.h>
#include <can/transceiver/ICanTransceiver.h>

namespace taktflow
{
namespace bcm
{
namespace signals
{

class CanFrameCodec
{
public:
    CanFrameCodec(BcmSignals& signals, ::can::ICanTransceiver& transceiver);

    CanFrameCodec(CanFrameCodec const&) = delete;
    CanFrameCodec& operator=(CanFrameCodec const&) = delete;

    /// Decode an incoming CAN frame into the signal struct
    void decodeRx(::can::CANFrame const& frame);

    /// Encode and transmit Light_Status (0x400)
    void encodeTxLightStatus();

    /// Encode and transmit Indicator_State (0x401)
    void encodeTxIndicatorState();

    /// Encode and transmit Door_Lock_Status (0x402)
    void encodeTxDoorLockStatus();

    /// Encode and transmit BCM_Heartbeat (0x016)
    void encodeTxHeartbeat();

private:
    BcmSignals& _signals;
    ::can::ICanTransceiver& _transceiver;
    uint8_t _heartbeatAliveCounter{0};
    uint8_t _lightStatusAliveCounter{0};
    uint8_t _indicatorAliveCounter{0};
    uint8_t _doorLockAliveCounter{0};
};

} // namespace signals
} // namespace bcm
} // namespace taktflow
