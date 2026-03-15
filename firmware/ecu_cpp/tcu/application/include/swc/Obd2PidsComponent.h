// Taktflow Systems - OBD-II PID handler SWC (SAE J1979)

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

class Obd2PidsComponent
{
public:
    Obd2PidsComponent(signals::TcuSignals& signals, DtcStoreComponent& dtcStore);

    void init();
    void handleRequest(uint8_t mode, uint8_t pid, uint8_t* response, uint8_t& len);

private:
    void handleMode01(uint8_t pid, uint8_t* response, uint8_t& len);
    void handleMode03(uint8_t* response, uint8_t& len);
    void handleMode04(uint8_t* response, uint8_t& len);
    void handleMode09(uint8_t pid, uint8_t* response, uint8_t& len);

    signals::TcuSignals& _signals;
    DtcStoreComponent& _dtcStore;
};

} // namespace swc
} // namespace tcu
} // namespace taktflow
