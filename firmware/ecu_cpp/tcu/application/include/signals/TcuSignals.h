// Taktflow Systems - TCU typed signal struct (replaces RTE signal buffer)

#pragma once

#include <cstdint>

namespace taktflow
{
namespace tcu
{
namespace signals
{

/// Heartbeat and ECU constants
static constexpr uint8_t HEARTBEAT_ECU_ID      = 0x07u;
static constexpr uint8_t HEARTBEAT_E2E_DATA_ID = 0x41u;

/// DTC constants
static constexpr uint8_t DTC_MAX_ENTRIES        = 64u;
static constexpr uint16_t DTC_AGING_CLEAR_CYCLES = 40u;

/// DTC status bits (ISO 14229)
static constexpr uint8_t DTC_STATUS_TEST_FAILED        = 0x01u;
static constexpr uint8_t DTC_STATUS_TEST_FAILED_THIS_OP = 0x02u;
static constexpr uint8_t DTC_STATUS_PENDING            = 0x04u;
static constexpr uint8_t DTC_STATUS_CONFIRMED          = 0x08u;

/// UDS constants
static constexpr uint16_t UDS_SESSION_TIMEOUT_TICKS   = 500u;
static constexpr uint16_t UDS_SECURITY_LOCKOUT_TICKS  = 1000u;
static constexpr uint8_t  UDS_MAX_SECURITY_ATTEMPTS   = 3u;
static constexpr uint32_t UDS_SECURITY_LEVEL1_XOR     = 0xA5A5A5A5u;
static constexpr uint32_t UDS_SECURITY_LEVEL3_XOR     = 0x5A5A5A5Au;
static constexpr uint8_t  VIN_LENGTH                  = 17u;

/// Data aggregator timeouts (ms)
static constexpr uint16_t TIMEOUT_VEHICLE_STATE = 500u;
static constexpr uint16_t TIMEOUT_MOTOR_CURRENT = 500u;
static constexpr uint16_t TIMEOUT_MOTOR_TEMP    = 2000u;
static constexpr uint16_t TIMEOUT_BATTERY       = 2000u;
static constexpr uint16_t TIMEOUT_DTC           = 2000u;
static constexpr uint16_t TIMEOUT_CVC_HB        = 200u;

/// Typed signal struct
struct TcuSignals
{
    // RX signals (written by CanFrameCodec)
    uint8_t  vehicleState{0};
    uint16_t motorSpeedRpm{0};
    uint8_t  torquePct{0};
    uint16_t motorTempC{0};
    uint16_t batteryVoltageMv{0};
    uint16_t motorCurrentMa{0};
    uint8_t  estopActive{0};

    // DTC broadcast (from 0x500)
    uint32_t dtcCode{0};
    uint8_t  dtcStatus{0};
    uint8_t  dtcEcuSource{0};
    bool     dtcUpdated{false};

    // CVC heartbeat
    uint8_t cvcAliveCounter{0};

    // TX signals
    uint8_t txAliveCounter{0};
};

} // namespace signals
} // namespace tcu
} // namespace taktflow
