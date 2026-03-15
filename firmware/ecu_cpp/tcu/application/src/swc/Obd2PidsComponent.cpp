// Taktflow Systems - OBD-II PID handler implementation

#include "swc/Obd2PidsComponent.h"

#include <cstring>

namespace taktflow
{
namespace tcu
{
namespace swc
{

static char const DEFAULT_VIN[] = "TAKTFLOW000000001";

Obd2PidsComponent::Obd2PidsComponent(signals::TcuSignals& signals, DtcStoreComponent& dtcStore)
: _signals(signals)
, _dtcStore(dtcStore)
{}

void Obd2PidsComponent::init() {}

void Obd2PidsComponent::handleRequest(uint8_t mode, uint8_t pid,
                                       uint8_t* response, uint8_t& len)
{
    len = 0u;
    switch (mode)
    {
        case 0x01u: handleMode01(pid, response, len); break;
        case 0x03u: handleMode03(response, len); break;
        case 0x04u: handleMode04(response, len); break;
        case 0x09u: handleMode09(pid, response, len); break;
        default: break;
    }
}

void Obd2PidsComponent::handleMode01(uint8_t pid, uint8_t* response, uint8_t& len)
{
    response[0] = 0x41u;
    response[1] = pid;

    switch (pid)
    {
        case 0x00u: // Supported PIDs 01-20
            response[2] = 0x18u; // bits 28,27 → PIDs 04,05
            response[3] = 0x30u; // bits 20,19 → PIDs 0C,0D
            response[4] = 0x00u;
            response[5] = 0x00u;
            len = 6u;
            break;

        case 0x04u: // Engine load
            response[2] = static_cast<uint8_t>(
                (static_cast<uint16_t>(_signals.torquePct) * 255u) / 100u);
            len = 3u;
            break;

        case 0x05u: // Coolant temp (motor temp + 40 offset)
            response[2] = static_cast<uint8_t>(_signals.motorTempC + 40u);
            len = 3u;
            break;

        case 0x0Cu: // Engine RPM (value * 4, 2 bytes)
        {
            uint16_t const rpm4 = static_cast<uint16_t>(_signals.motorSpeedRpm * 4u);
            response[2] = static_cast<uint8_t>(rpm4 >> 8);
            response[3] = static_cast<uint8_t>(rpm4);
            len = 4u;
            break;
        }

        case 0x0Du: // Vehicle speed
        {
            uint16_t speed = static_cast<uint16_t>(
                (static_cast<uint32_t>(_signals.motorSpeedRpm) * 60u) / 1000u);
            if (speed > 255u) { speed = 255u; }
            response[2] = static_cast<uint8_t>(speed);
            len = 3u;
            break;
        }

        case 0x42u: // Control voltage (battery mV, 2 bytes)
            response[2] = static_cast<uint8_t>(_signals.batteryVoltageMv >> 8);
            response[3] = static_cast<uint8_t>(_signals.batteryVoltageMv);
            len = 4u;
            break;

        case 0x46u: // Ambient temp (fixed 25°C + 40 offset = 65)
            response[2] = 65u;
            len = 3u;
            break;

        default:
            len = 0u;
            break;
    }
}

void Obd2PidsComponent::handleMode03(uint8_t* response, uint8_t& len)
{
    // Report confirmed DTCs
    response[0] = 0x43u;
    uint8_t dtcCount = 0u;
    uint8_t pos = 1u;

    for (uint8_t i = 0u; i < _dtcStore.getCount() && dtcCount < 3u; ++i)
    {
        DtcStoreEntry const* e = _dtcStore.getByIndex(i);
        if (e != nullptr && (e->status & signals::DTC_STATUS_CONFIRMED) != 0u)
        {
            response[pos++] = static_cast<uint8_t>(e->dtcCode >> 8);
            response[pos++] = static_cast<uint8_t>(e->dtcCode);
            ++dtcCount;
        }
    }
    len = pos;
}

void Obd2PidsComponent::handleMode04(uint8_t* response, uint8_t& len)
{
    _dtcStore.clear();
    response[0] = 0x44u;
    len = 1u;
}

void Obd2PidsComponent::handleMode09(uint8_t pid, uint8_t* response, uint8_t& len)
{
    response[0] = 0x49u;
    response[1] = pid;

    if (pid == 0x02u) // VIN
    {
        response[2] = 0x01u; // data count
        std::memcpy(&response[3], DEFAULT_VIN, 17u);
        len = 20u;
    }
    else
    {
        len = 0u;
    }
}

} // namespace swc
} // namespace tcu
} // namespace taktflow
