// Taktflow Systems - Dashboard SWC (instrument cluster display logic)

#pragma once

#include "signals/IcuSignals.h"

#include <cstdint>

namespace taktflow
{
namespace icu
{
namespace swc
{

class DashboardComponent
{
public:
    explicit DashboardComponent(signals::IcuSignals& signals);

    void init();
    void execute();

    uint16_t getDisplaySpeed() const { return _displaySpeed; }
    uint8_t  getTorquePct() const { return _torquePct; }
    signals::TempZone getTempZone() const { return _tempZone; }
    signals::BatteryZone getBatteryZone() const { return _batteryZone; }
    uint8_t  getWarnings() const { return _warnings; }
    bool     isCvcHealthy() const { return _cvcHealthy; }
    bool     isFzcHealthy() const { return _fzcHealthy; }
    bool     isRzcHealthy() const { return _rzcHealthy; }

    static signals::TempZone computeTempZone(uint16_t tempC);
    static signals::BatteryZone computeBatteryZone(uint16_t millivolts);
    static char const* vehicleStateStr(uint8_t state);

private:
    void updateWarnings();
    void updateEcuHealth();

    signals::IcuSignals& _signals;

    uint16_t _displaySpeed{0};
    uint8_t  _torquePct{0};
    signals::TempZone _tempZone{signals::TEMP_ZONE_GREEN};
    signals::BatteryZone _batteryZone{signals::BATT_ZONE_GREEN};
    uint8_t  _warnings{0};

    bool _cvcHealthy{true};
    bool _fzcHealthy{true};
    bool _rzcHealthy{true};

    uint8_t _cvcLastAlive{0};
    uint8_t _fzcLastAlive{0};
    uint8_t _rzcLastAlive{0};

    uint8_t _cvcStaleCount{0};
    uint8_t _fzcStaleCount{0};
    uint8_t _rzcStaleCount{0};
};

} // namespace swc
} // namespace icu
} // namespace taktflow
