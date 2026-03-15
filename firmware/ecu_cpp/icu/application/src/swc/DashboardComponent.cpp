// Taktflow Systems - Dashboard SWC implementation

#include "swc/DashboardComponent.h"

namespace taktflow
{
namespace icu
{
namespace swc
{

DashboardComponent::DashboardComponent(signals::IcuSignals& signals)
: _signals(signals)
{}

void DashboardComponent::init()
{
    _displaySpeed = 0u;
    _torquePct = 0u;
    _tempZone = signals::TEMP_ZONE_GREEN;
    _batteryZone = signals::BATT_ZONE_GREEN;
    _warnings = 0u;
    _cvcHealthy = true;
    _fzcHealthy = true;
    _rzcHealthy = true;
    _cvcStaleCount = 0u;
    _fzcStaleCount = 0u;
    _rzcStaleCount = 0u;
}

void DashboardComponent::execute()
{
    // Compute display speed: speed = (rpm * 60) / 1000
    _displaySpeed = static_cast<uint16_t>(
        (static_cast<uint32_t>(_signals.motorSpeedRpm) * 60u) / 1000u);

    // Clamp torque to 0-100
    _torquePct = (_signals.torquePct > 100u) ? 100u : _signals.torquePct;

    // Temperature and battery zones
    _tempZone = computeTempZone(_signals.motorTempC);
    _batteryZone = computeBatteryZone(_signals.batteryVoltageMv);

    // Warning flags
    updateWarnings();

    // ECU health monitoring
    updateEcuHealth();
}

signals::TempZone DashboardComponent::computeTempZone(uint16_t tempC)
{
    if (tempC >= signals::TEMP_THRESHOLD_RED)
    {
        return signals::TEMP_ZONE_RED;
    }
    if (tempC >= signals::TEMP_THRESHOLD_ORANGE)
    {
        return signals::TEMP_ZONE_ORANGE;
    }
    if (tempC >= signals::TEMP_THRESHOLD_YELLOW)
    {
        return signals::TEMP_ZONE_YELLOW;
    }
    return signals::TEMP_ZONE_GREEN;
}

signals::BatteryZone DashboardComponent::computeBatteryZone(uint16_t millivolts)
{
    if (millivolts < signals::BATT_THRESHOLD_RED)
    {
        return signals::BATT_ZONE_RED;
    }
    if (millivolts <= signals::BATT_THRESHOLD_YELLOW)
    {
        return signals::BATT_ZONE_YELLOW;
    }
    return signals::BATT_ZONE_GREEN;
}

char const* DashboardComponent::vehicleStateStr(uint8_t state)
{
    switch (state)
    {
        case signals::VEHICLE_STATE_INIT:      return "INIT";
        case signals::VEHICLE_STATE_RUN:       return "RUN";
        case signals::VEHICLE_STATE_DEGRADED:  return "DEGRADED";
        case signals::VEHICLE_STATE_LIMP:      return "LIMP";
        case signals::VEHICLE_STATE_SAFE_STOP: return "SAFE_STOP";
        case signals::VEHICLE_STATE_SHUTDOWN:  return "SHUTDOWN";
        default:                               return "UNKNOWN";
    }
}

void DashboardComponent::updateWarnings()
{
    _warnings = 0u;

    // Check engine (any DTC active)
    if (_signals.dtcCode != 0u)
    {
        _warnings |= signals::DASH_WARN_CHECK_ENGINE;
    }

    // Temperature warning (orange or red zone)
    if (_tempZone >= signals::TEMP_ZONE_ORANGE)
    {
        _warnings |= signals::DASH_WARN_TEMPERATURE;
    }

    // Battery warning (red zone)
    if (_batteryZone == signals::BATT_ZONE_RED)
    {
        _warnings |= signals::DASH_WARN_BATTERY;
    }

    // E-stop active
    if (_signals.estopActive != 0u)
    {
        _warnings |= signals::DASH_WARN_ESTOP;
    }

    // Overcurrent
    if (_signals.overcurrentFlag != 0u)
    {
        _warnings |= signals::DASH_WARN_OVERCURRENT;
    }
}

void DashboardComponent::updateEcuHealth()
{
    // CVC heartbeat
    if (_signals.cvcAliveCounter != _cvcLastAlive)
    {
        _cvcLastAlive = _signals.cvcAliveCounter;
        _cvcStaleCount = 0u;
        _cvcHealthy = true;
    }
    else
    {
        if (_cvcStaleCount < 255u) { ++_cvcStaleCount; }
        if (_cvcStaleCount > signals::HB_TIMEOUT_TICKS) { _cvcHealthy = false; }
    }

    // FZC heartbeat
    if (_signals.fzcAliveCounter != _fzcLastAlive)
    {
        _fzcLastAlive = _signals.fzcAliveCounter;
        _fzcStaleCount = 0u;
        _fzcHealthy = true;
    }
    else
    {
        if (_fzcStaleCount < 255u) { ++_fzcStaleCount; }
        if (_fzcStaleCount > signals::HB_TIMEOUT_TICKS) { _fzcHealthy = false; }
    }

    // RZC heartbeat
    if (_signals.rzcAliveCounter != _rzcLastAlive)
    {
        _rzcLastAlive = _signals.rzcAliveCounter;
        _rzcStaleCount = 0u;
        _rzcHealthy = true;
    }
    else
    {
        if (_rzcStaleCount < 255u) { ++_rzcStaleCount; }
        if (_rzcStaleCount > signals::HB_TIMEOUT_TICKS) { _rzcHealthy = false; }
    }
}

} // namespace swc
} // namespace icu
} // namespace taktflow
