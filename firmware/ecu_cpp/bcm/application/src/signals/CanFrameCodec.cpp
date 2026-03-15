// Taktflow Systems - CAN frame encode/decode implementation
// Bit positions match Com_Cfg_Bcm.c signal config table

#include "signals/CanFrameCodec.h"

#include "signals/E2eProtection.h"

#include <cstring>

namespace taktflow
{
namespace bcm
{
namespace signals
{

CanFrameCodec::CanFrameCodec(BcmSignals& signals, ::can::ICanTransceiver& transceiver)
: _signals(signals)
, _transceiver(transceiver)
{
}

void CanFrameCodec::decodeRx(::can::CANFrame const& frame)
{
    uint32_t const id = frame.getId();
    uint8_t const* p = frame.getPayload();

    switch (id)
    {
    case 0x001u: // EStop_Broadcast
    {
        // E2E header in bytes 0-1, estop_active at bit 16 (byte 2), 8 bits
        if (E2eProtection::validate(p, frame.getPayloadLength()))
        {
            _signals.estopActive = p[2];
        }
        break;
    }
    case 0x100u: // Vehicle_State
    {
        // E2E in bytes 0-1, vehicleState at bit 16 (byte 2 bits 0-3), 4 bits
        if (E2eProtection::validate(p, frame.getPayloadLength()))
        {
            _signals.vehicleState = p[2] & 0x0Fu;
        }
        break;
    }
    case 0x300u: // Motor_Status
    {
        // E2E in bytes 0-1, motorSpeedRpm at bit 24 (bytes 3-4), 16 bits LE
        if (E2eProtection::validate(p, frame.getPayloadLength()))
        {
            _signals.motorSpeedRpm = static_cast<uint16_t>(
                static_cast<uint16_t>(p[3]) | (static_cast<uint16_t>(p[4]) << 8u));
        }
        break;
    }
    case 0x350u: // Body_Control_Cmd
    {
        // No E2E on body control cmd (DLC 4, simple signals)
        // headlightCmd at bit 0, 2 bits
        _signals.headlightCmd = p[0] & 0x03u;
        // taillightCmd at bit 2, 1 bit (not used by BCM SWCs but decoded)
        // hazardCmd at bit 3, 1 bit
        _signals.hazardCmd = (p[0] >> 3u) & 0x01u;
        // turnSignalCmd at bit 4, 2 bits
        _signals.turnSignalCmd = (p[0] >> 4u) & 0x03u;
        // doorLockCmd at bit 6, 1 bit
        _signals.doorLockCmd = (p[0] >> 6u) & 0x01u;
        break;
    }
    default:
        break;
    }
}

void CanFrameCodec::encodeTxLightStatus()
{
    // CAN 0x400, DLC 4
    // E2E in bytes 0-1, signals start at byte 2
    // headlightOn at bit 0 (of signal area), tailLightOn at bit 1
    uint8_t payload[4] = {0};
    // Signal data in bytes 2-3
    payload[2] = static_cast<uint8_t>(
        (_signals.headlightOn & 0x01u) | ((_signals.tailLightOn & 0x01u) << 1u));
    // E2E protect: dataId for Light_Status TX
    E2eProtection::protect(payload, 4u, 0x05u, _lightStatusAliveCounter);
    ++_lightStatusAliveCounter;

    ::can::CANFrame frame(LIGHT_STATUS_CAN_ID, payload, 4u);
    _transceiver.write(frame);
}

void CanFrameCodec::encodeTxIndicatorState()
{
    // CAN 0x401, DLC 4
    // leftIndicator bit 0, rightIndicator bit 1, hazardActive bit 2, blinkState bit 3
    uint8_t payload[4] = {0};
    payload[2] = static_cast<uint8_t>(
        (_signals.leftIndicator & 0x01u)
        | ((_signals.rightIndicator & 0x01u) << 1u)
        | ((_signals.hazardActive & 0x01u) << 2u));
    E2eProtection::protect(payload, 4u, 0x06u, _indicatorAliveCounter);
    ++_indicatorAliveCounter;

    ::can::CANFrame frame(INDICATOR_CAN_ID, payload, 4u);
    _transceiver.write(frame);
}

void CanFrameCodec::encodeTxDoorLockStatus()
{
    // CAN 0x402, DLC 2
    // frontLeftLock bit 0
    uint8_t payload[4] = {0};
    payload[2] = _signals.frontLeftLock & 0x01u;
    // Use DLC 4 to match E2E pattern (bytes 0-1 = E2E, 2-3 = data)
    E2eProtection::protect(payload, 4u, 0x07u, _doorLockAliveCounter);
    ++_doorLockAliveCounter;

    ::can::CANFrame frame(DOOR_LOCK_CAN_ID, payload, 4u);
    _transceiver.write(frame);
}

void CanFrameCodec::encodeTxHeartbeat()
{
    // CAN 0x016, DLC 4
    // E2E header bytes 0-1, aliveCounter byte 2, ecuId byte 3
    uint8_t payload[4] = {0};
    payload[2] = _heartbeatAliveCounter;
    payload[3] = HEARTBEAT_ECU_ID;
    E2eProtection::protect(payload, 4u, 0x00u, _heartbeatAliveCounter);
    ++_heartbeatAliveCounter;

    ::can::CANFrame frame(HEARTBEAT_CAN_ID, payload, 4u);
    _transceiver.write(frame);
}

} // namespace signals
} // namespace bcm
} // namespace taktflow
