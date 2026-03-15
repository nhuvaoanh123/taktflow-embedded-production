// Taktflow Systems - TCU CAN frame encode/decode

#pragma once

#include "signals/TcuSignals.h"

#include <can/canframes/CANFrame.h>
#include <can/transceiver/ICanTransceiver.h>

namespace taktflow
{
namespace tcu
{
namespace signals
{

class CanFrameCodec
{
public:
    CanFrameCodec(TcuSignals& signals, ::can::ICanTransceiver& transceiver);

    void decodeRx(::can::CANFrame const& frame);
    void encodeTxHeartbeat();

private:
    TcuSignals& _signals;
    ::can::ICanTransceiver& _transceiver;
    uint8_t _heartbeatAliveCounter{0};
};

} // namespace signals
} // namespace tcu
} // namespace taktflow
