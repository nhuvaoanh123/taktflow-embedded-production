// Taktflow Systems - UDS Server SWC implementation

#include "swc/UdsServerComponent.h"

#include <cstring>
#include <cstdlib>

namespace taktflow
{
namespace tcu
{
namespace swc
{

static char const DEFAULT_VIN[] = "TAKTFLOW000000001";
static char const SW_VERSION[] = "1.0.0";
static char const HW_VERSION[] = "REV-A";

UdsServerComponent::UdsServerComponent(signals::TcuSignals& signals, DtcStoreComponent& dtcStore)
: _signals(signals)
, _dtcStore(dtcStore)
{
    std::memcpy(_vin, DEFAULT_VIN, signals::VIN_LENGTH);
    _vin[signals::VIN_LENGTH] = '\0';
}

void UdsServerComponent::init()
{
    _session = 0x01u;
    _securityLevel = 0u;
    _seed = 0u;
    _sessionTimer = 0u;
    _securityAttempts = 0u;
    _lockoutTimer = 0u;
    std::memcpy(_vin, DEFAULT_VIN, signals::VIN_LENGTH);
    _vin[signals::VIN_LENGTH] = '\0';
}

void UdsServerComponent::execute()
{
    // Session timeout management
    if (_session != 0x01u)
    {
        ++_sessionTimer;
        if (_sessionTimer >= signals::UDS_SESSION_TIMEOUT_TICKS)
        {
            _session = 0x01u;
            _securityLevel = 0u;
            _sessionTimer = 0u;
        }
    }

    // Security lockout countdown
    if (_lockoutTimer > 0u)
    {
        --_lockoutTimer;
    }
}

void UdsServerComponent::processRequest(uint8_t const* reqData, uint16_t reqLen,
                                         uint8_t* rspData, uint16_t& rspLen)
{
    if (reqLen < 1u)
    {
        return;
    }

    _sessionTimer = 0u; // reset timeout on any request

    uint8_t const sid = reqData[0];

    switch (sid)
    {
        case 0x10u: handleSessionControl(reqData, reqLen, rspData, rspLen); break;
        case 0x27u: handleSecurityAccess(reqData, reqLen, rspData, rspLen); break;
        case 0x22u: handleReadDid(reqData, reqLen, rspData, rspLen); break;
        case 0x2Eu: handleWriteDid(reqData, reqLen, rspData, rspLen); break;
        case 0x14u: handleClearDtc(reqData, reqLen, rspData, rspLen); break;
        case 0x19u: handleReadDtc(reqData, reqLen, rspData, rspLen); break;
        case 0x3Eu: handleTesterPresent(reqData, reqLen, rspData, rspLen); break;
        default:    sendNrc(sid, 0x11u, rspData, rspLen); break; // serviceNotSupported
    }
}

void UdsServerComponent::handleSessionControl(uint8_t const* req, uint16_t len,
                                               uint8_t* rsp, uint16_t& rspLen)
{
    if (len < 2u) { sendNrc(0x10u, 0x13u, rsp, rspLen); return; }

    uint8_t const subFunc = req[1] & 0x7Fu;
    if (subFunc >= 0x01u && subFunc <= 0x03u)
    {
        _session = subFunc;
        _securityLevel = 0u;
        _sessionTimer = 0u;
        rsp[0] = 0x50u;
        rsp[1] = subFunc;
        rspLen = 2u;
    }
    else
    {
        sendNrc(0x10u, 0x12u, rsp, rspLen); // subFunctionNotSupported
    }
}

void UdsServerComponent::handleSecurityAccess(uint8_t const* req, uint16_t len,
                                               uint8_t* rsp, uint16_t& rspLen)
{
    if (len < 2u) { sendNrc(0x27u, 0x13u, rsp, rspLen); return; }
    if (_lockoutTimer > 0u) { sendNrc(0x27u, 0x37u, rsp, rspLen); return; }

    uint8_t const subFunc = req[1];

    if (subFunc == 0x01u || subFunc == 0x03u)
    {
        // Request seed
        _seed = static_cast<uint32_t>(std::rand()) | 1u; // ensure non-zero
        rsp[0] = 0x67u;
        rsp[1] = subFunc;
        rsp[2] = static_cast<uint8_t>(_seed >> 24);
        rsp[3] = static_cast<uint8_t>(_seed >> 16);
        rsp[4] = static_cast<uint8_t>(_seed >> 8);
        rsp[5] = static_cast<uint8_t>(_seed);
        rspLen = 6u;
    }
    else if (subFunc == 0x02u || subFunc == 0x04u)
    {
        // Send key
        if (len < 6u) { sendNrc(0x27u, 0x13u, rsp, rspLen); return; }

        uint32_t const key =
            (static_cast<uint32_t>(req[2]) << 24) |
            (static_cast<uint32_t>(req[3]) << 16) |
            (static_cast<uint32_t>(req[4]) << 8) |
            static_cast<uint32_t>(req[5]);

        uint32_t expectedKey;
        if (subFunc == 0x02u)
        {
            expectedKey = _seed ^ signals::UDS_SECURITY_LEVEL1_XOR;
        }
        else
        {
            uint32_t tmp = _seed ^ signals::UDS_SECURITY_LEVEL3_XOR;
            expectedKey = (tmp << 8) | (tmp >> 24); // rotate left 8
        }

        if (key == expectedKey)
        {
            _securityLevel = (subFunc == 0x02u) ? 1u : 3u;
            _securityAttempts = 0u;
            rsp[0] = 0x67u;
            rsp[1] = subFunc;
            rspLen = 2u;
        }
        else
        {
            ++_securityAttempts;
            if (_securityAttempts >= signals::UDS_MAX_SECURITY_ATTEMPTS)
            {
                _lockoutTimer = signals::UDS_SECURITY_LOCKOUT_TICKS;
                _securityAttempts = 0u;
            }
            sendNrc(0x27u, 0x35u, rsp, rspLen); // invalidKey
        }
    }
    else
    {
        sendNrc(0x27u, 0x12u, rsp, rspLen);
    }
}

void UdsServerComponent::handleReadDid(uint8_t const* req, uint16_t len,
                                        uint8_t* rsp, uint16_t& rspLen)
{
    if (len < 3u) { sendNrc(0x22u, 0x13u, rsp, rspLen); return; }

    uint16_t const did = (static_cast<uint16_t>(req[1]) << 8) | req[2];

    rsp[0] = 0x62u;
    rsp[1] = req[1];
    rsp[2] = req[2];

    switch (did)
    {
        case 0xF190u: // VIN
            std::memcpy(&rsp[3], _vin, signals::VIN_LENGTH);
            rspLen = 3u + signals::VIN_LENGTH;
            break;

        case 0xF195u: // SW Version
            std::memcpy(&rsp[3], SW_VERSION, 5u);
            rspLen = 8u;
            break;

        case 0xF191u: // HW Version
            std::memcpy(&rsp[3], HW_VERSION, 5u);
            rspLen = 8u;
            break;

        case 0x0100u: // Vehicle Speed
        {
            uint16_t speed = static_cast<uint16_t>(
                (static_cast<uint32_t>(_signals.motorSpeedRpm) * 60u) / 1000u);
            rsp[3] = static_cast<uint8_t>(speed >> 8);
            rsp[4] = static_cast<uint8_t>(speed);
            rspLen = 5u;
            break;
        }
        case 0x0101u: // Motor Temp
            rsp[3] = static_cast<uint8_t>(_signals.motorTempC >> 8);
            rsp[4] = static_cast<uint8_t>(_signals.motorTempC);
            rspLen = 5u;
            break;

        case 0x0102u: // Battery Voltage
            rsp[3] = static_cast<uint8_t>(_signals.batteryVoltageMv >> 8);
            rsp[4] = static_cast<uint8_t>(_signals.batteryVoltageMv);
            rspLen = 5u;
            break;

        case 0x0103u: // Motor Current
            rsp[3] = static_cast<uint8_t>(_signals.motorCurrentMa >> 8);
            rsp[4] = static_cast<uint8_t>(_signals.motorCurrentMa);
            rspLen = 5u;
            break;

        case 0x0104u: // Motor RPM
            rsp[3] = static_cast<uint8_t>(_signals.motorSpeedRpm >> 8);
            rsp[4] = static_cast<uint8_t>(_signals.motorSpeedRpm);
            rspLen = 5u;
            break;

        default:
            sendNrc(0x22u, 0x31u, rsp, rspLen); // requestOutOfRange
            break;
    }
}

void UdsServerComponent::handleWriteDid(uint8_t const* req, uint16_t len,
                                         uint8_t* rsp, uint16_t& rspLen)
{
    if (len < 3u) { sendNrc(0x2Eu, 0x13u, rsp, rspLen); return; }
    if (_securityLevel < 1u) { sendNrc(0x2Eu, 0x33u, rsp, rspLen); return; }

    uint16_t const did = (static_cast<uint16_t>(req[1]) << 8) | req[2];

    if (did == 0xF190u && len >= 3u + signals::VIN_LENGTH)
    {
        std::memcpy(_vin, &req[3], signals::VIN_LENGTH);
        _vin[signals::VIN_LENGTH] = '\0';
        rsp[0] = 0x6Eu;
        rsp[1] = req[1];
        rsp[2] = req[2];
        rspLen = 3u;
    }
    else
    {
        sendNrc(0x2Eu, 0x31u, rsp, rspLen);
    }
}

void UdsServerComponent::handleClearDtc(uint8_t const* /* req */, uint16_t /* len */,
                                         uint8_t* rsp, uint16_t& rspLen)
{
    if (_securityLevel < 1u) { sendNrc(0x14u, 0x33u, rsp, rspLen); return; }

    _dtcStore.clear();
    rsp[0] = 0x54u;
    rspLen = 1u;
}

void UdsServerComponent::handleReadDtc(uint8_t const* req, uint16_t len,
                                        uint8_t* rsp, uint16_t& rspLen)
{
    if (len < 2u) { sendNrc(0x19u, 0x13u, rsp, rspLen); return; }

    uint8_t const subFunc = req[1];

    if (subFunc == 0x01u) // reportNumberOfDTCByStatusMask
    {
        uint8_t const mask = (len >= 3u) ? req[2] : 0xFFu;
        uint32_t codes[64];
        uint8_t count = _dtcStore.getByMask(mask, codes, 64u);

        rsp[0] = 0x59u;
        rsp[1] = 0x01u;
        rsp[2] = 0xFFu; // availability mask
        rsp[3] = 0x00u; // format
        rsp[4] = static_cast<uint8_t>(count >> 8);
        rsp[5] = static_cast<uint8_t>(count);
        rspLen = 6u;
    }
    else if (subFunc == 0x02u) // reportDTCByStatusMask
    {
        uint8_t const mask = (len >= 3u) ? req[2] : 0xFFu;

        rsp[0] = 0x59u;
        rsp[1] = 0x02u;
        rsp[2] = 0xFFu;
        rspLen = 3u;

        for (uint8_t i = 0u; i < _dtcStore.getCount() && rspLen + 4u <= 64u; ++i)
        {
            DtcStoreEntry const* e = _dtcStore.getByIndex(i);
            if (e != nullptr && (e->status & mask) != 0u)
            {
                rsp[rspLen++] = static_cast<uint8_t>(e->dtcCode >> 16);
                rsp[rspLen++] = static_cast<uint8_t>(e->dtcCode >> 8);
                rsp[rspLen++] = static_cast<uint8_t>(e->dtcCode);
                rsp[rspLen++] = e->status;
            }
        }
    }
    else
    {
        sendNrc(0x19u, 0x12u, rsp, rspLen);
    }
}

void UdsServerComponent::handleTesterPresent(uint8_t const* req, uint16_t len,
                                              uint8_t* rsp, uint16_t& rspLen)
{
    (void)len;
    _sessionTimer = 0u;
    rsp[0] = 0x7Eu;
    rsp[1] = (len >= 2u) ? (req[1] & 0x7Fu) : 0x00u;
    rspLen = 2u;
}

void UdsServerComponent::sendNrc(uint8_t serviceId, uint8_t nrc, uint8_t* rsp, uint16_t& rspLen)
{
    rsp[0] = 0x7Fu;
    rsp[1] = serviceId;
    rsp[2] = nrc;
    rspLen = 3u;
}

} // namespace swc
} // namespace tcu
} // namespace taktflow
