// Taktflow Systems - TCU CAN frame encode/decode

#include "signals/CanFrameCodec.h"
#include "signals/E2eProtection.h"

#include <cstring>

namespace taktflow
{
namespace tcu
{
namespace signals
{

CanFrameCodec::CanFrameCodec(TcuSignals& signals, ::can::ICanTransceiver& transceiver)
: _signals(signals)
, _transceiver(transceiver)
{}

void CanFrameCodec::decodeRx(::can::CANFrame const& frame)
{
    uint32_t const id = frame.getId();
    uint8_t const* data = frame.getPayload();

    switch (id)
    {
        case 0x001u: // EStop_Broadcast
            _signals.estopActive = data[2];
            break;

        case 0x010u: // CVC_Heartbeat
            _signals.cvcAliveCounter = data[2];
            break;

        case 0x100u: // Vehicle_State
            _signals.vehicleState = (data[2] >> 0) & 0x0Fu;
            break;

        case 0x300u: // Motor_Status
            _signals.torquePct = data[1];
            _signals.motorSpeedRpm =
                static_cast<uint16_t>(data[3]) | (static_cast<uint16_t>(data[4]) << 8u);
            break;

        case 0x301u: // Motor_Current
            _signals.motorCurrentMa =
                static_cast<uint16_t>(data[0]) | (static_cast<uint16_t>(data[1]) << 8u);
            break;

        case 0x302u: // Motor_Temperature
            _signals.motorTempC =
                static_cast<uint16_t>(data[0]) | (static_cast<uint16_t>(data[1]) << 8u);
            break;

        case 0x303u: // Battery_Status
            _signals.batteryVoltageMv =
                static_cast<uint16_t>(data[0]) | (static_cast<uint16_t>(data[1]) << 8u);
            break;

        case 0x500u: // DTC_Broadcast
            _signals.dtcCode =
                (static_cast<uint32_t>(data[0]) << 16) |
                (static_cast<uint32_t>(data[1]) << 8) |
                static_cast<uint32_t>(data[2]);
            _signals.dtcStatus = data[3];
            _signals.dtcEcuSource = data[4];
            _signals.dtcUpdated = true;
            break;

        default:
            break;
    }
}

void CanFrameCodec::encodeTxHeartbeat()
{
    uint8_t frameData[4] = {0};
    frameData[2] = _signals.txAliveCounter;
    frameData[3] = HEARTBEAT_ECU_ID;

    E2eProtection::protect(frameData, 4u, HEARTBEAT_E2E_DATA_ID, _heartbeatAliveCounter);
    _signals.txAliveCounter = (_signals.txAliveCounter + 1u) & 0xFFu;

    ::can::CANFrame frame;
    frame.setId(0x015u);
    frame.setPayloadLength(4u);
    uint8_t* payload = frame.getPayload();
    std::memcpy(payload, frameData, 4u);
    _transceiver.write(frame);
}

} // namespace signals
} // namespace tcu
} // namespace taktflow
