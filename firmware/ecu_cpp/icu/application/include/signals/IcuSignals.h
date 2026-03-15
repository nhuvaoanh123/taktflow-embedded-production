// Taktflow Systems - ICU typed signal struct (replaces RTE signal buffer)

#pragma once

#include <cstdint>

namespace taktflow
{
namespace icu
{
namespace signals
{

/// Vehicle state enumeration (matches CVC state machine)
enum VehicleState : uint8_t
{
    VEHICLE_STATE_INIT     = 0,
    VEHICLE_STATE_RUN      = 1,
    VEHICLE_STATE_DEGRADED = 2,
    VEHICLE_STATE_LIMP     = 3,
    VEHICLE_STATE_SAFE_STOP = 4,
    VEHICLE_STATE_SHUTDOWN = 5,
};

/// Temperature zone for dashboard gauge
enum TempZone : uint8_t
{
    TEMP_ZONE_GREEN  = 0,
    TEMP_ZONE_YELLOW = 1,
    TEMP_ZONE_ORANGE = 2,
    TEMP_ZONE_RED    = 3,
};

/// Battery zone for dashboard gauge
enum BatteryZone : uint8_t
{
    BATT_ZONE_GREEN  = 0,
    BATT_ZONE_YELLOW = 1,
    BATT_ZONE_RED    = 2,
};

/// Warning flags bitmask
static constexpr uint8_t DASH_WARN_CHECK_ENGINE = 0x01u;
static constexpr uint8_t DASH_WARN_TEMPERATURE  = 0x02u;
static constexpr uint8_t DASH_WARN_BATTERY      = 0x04u;
static constexpr uint8_t DASH_WARN_ESTOP        = 0x08u;
static constexpr uint8_t DASH_WARN_OVERCURRENT  = 0x10u;

/// Temperature thresholds (deg C)
static constexpr uint16_t TEMP_THRESHOLD_YELLOW = 60u;
static constexpr uint16_t TEMP_THRESHOLD_ORANGE = 80u;
static constexpr uint16_t TEMP_THRESHOLD_RED    = 100u;

/// Battery thresholds (millivolts)
static constexpr uint16_t BATT_THRESHOLD_YELLOW = 11000u;
static constexpr uint16_t BATT_THRESHOLD_RED    = 10000u;

/// Heartbeat timeout (ticks at 50ms = 200ms)
static constexpr uint8_t HB_TIMEOUT_TICKS = 4u;

/// Heartbeat E2E and ECU ID
static constexpr uint8_t HEARTBEAT_ECU_ID     = 0x04u;
static constexpr uint8_t HEARTBEAT_E2E_DATA_ID = 0x31u;

/// DTC buffer constants
static constexpr uint8_t DTC_MAX_ENTRIES = 16u;

/// Typed signal struct — owned by IcuSystem, shared by reference
struct IcuSignals
{
    // RX signals (written by CanFrameCodec from CAN frames)
    uint8_t  vehicleState{0};
    uint16_t motorSpeedRpm{0};
    uint8_t  torquePct{0};
    uint16_t motorTempC{0};
    uint16_t batteryVoltageMv{0};
    uint8_t  estopActive{0};
    uint8_t  overcurrentFlag{0};

    // DTC broadcast (from 0x500)
    uint32_t dtcCode{0};
    uint8_t  dtcStatus{0};
    uint8_t  dtcEcuSource{0};
    bool     dtcUpdated{false};

    // Heartbeat alive counters (from 0x010, 0x011, 0x012)
    uint8_t cvcAliveCounter{0};
    uint8_t fzcAliveCounter{0};
    uint8_t rzcAliveCounter{0};

    // TX signals (written by heartbeat logic)
    uint8_t txAliveCounter{0};
};

} // namespace signals
} // namespace icu
} // namespace taktflow
