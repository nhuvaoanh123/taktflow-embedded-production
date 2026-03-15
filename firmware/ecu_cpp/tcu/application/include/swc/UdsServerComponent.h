// Taktflow Systems - UDS Server SWC (ISO 14229 diagnostic server)

#pragma once

#include "signals/TcuSignals.h"
#include "swc/DtcStoreComponent.h"

#include <cstdint>

namespace taktflow
{
namespace tcu
{
namespace swc
{

class UdsServerComponent
{
public:
    UdsServerComponent(signals::TcuSignals& signals, DtcStoreComponent& dtcStore);

    void init();
    void execute();

    void processRequest(uint8_t const* reqData, uint16_t reqLen,
                        uint8_t* rspData, uint16_t& rspLen);

    uint8_t getSession() const { return _session; }

private:
    void handleSessionControl(uint8_t const* req, uint16_t len, uint8_t* rsp, uint16_t& rspLen);
    void handleSecurityAccess(uint8_t const* req, uint16_t len, uint8_t* rsp, uint16_t& rspLen);
    void handleReadDid(uint8_t const* req, uint16_t len, uint8_t* rsp, uint16_t& rspLen);
    void handleWriteDid(uint8_t const* req, uint16_t len, uint8_t* rsp, uint16_t& rspLen);
    void handleClearDtc(uint8_t const* req, uint16_t len, uint8_t* rsp, uint16_t& rspLen);
    void handleReadDtc(uint8_t const* req, uint16_t len, uint8_t* rsp, uint16_t& rspLen);
    void handleTesterPresent(uint8_t const* req, uint16_t len, uint8_t* rsp, uint16_t& rspLen);

    void sendNrc(uint8_t serviceId, uint8_t nrc, uint8_t* rsp, uint16_t& rspLen);

    signals::TcuSignals& _signals;
    DtcStoreComponent& _dtcStore;

    uint8_t _session{0x01u};
    uint8_t _securityLevel{0};
    uint32_t _seed{0};
    uint16_t _sessionTimer{0};
    uint8_t _securityAttempts{0};
    uint16_t _lockoutTimer{0};

    char _vin[signals::VIN_LENGTH + 1];
};

} // namespace swc
} // namespace tcu
} // namespace taktflow
