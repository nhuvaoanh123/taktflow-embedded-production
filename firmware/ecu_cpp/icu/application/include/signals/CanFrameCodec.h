// Taktflow Systems - ICU CAN frame encode/decode

#pragma once

#include "signals/IcuSignals.h"

#include <can/canframes/CANFrame.h>
#include <can/transceiver/ICanTransceiver.h>

namespace taktflow
{
namespace icu
{
namespace signals
{

class CanFrameCodec
{
public:
    CanFrameCodec(IcuSignals& signals, ::can::ICanTransceiver& transceiver);

    void decodeRx(::can::CANFrame const& frame);
    void encodeTxHeartbeat();

private:
    IcuSignals& _signals;
    ::can::ICanTransceiver& _transceiver;
    uint8_t _heartbeatAliveCounter{0};
};

} // namespace signals
} // namespace icu
} // namespace taktflow
