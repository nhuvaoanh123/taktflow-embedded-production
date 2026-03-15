// Taktflow Systems - BCM typed signal struct (replaces RTE signal buffer)

#pragma once

#include <cstdint>

namespace taktflow
{
namespace bcm
{
namespace signals
{

/// Vehicle state enum (matches CVC broadcast values)
enum VehicleState : uint8_t
{
    VSTATE_INIT    = 0u,
    VSTATE_READY   = 1u,
    VSTATE_DRIVING = 2u,
    VSTATE_DEGRADED = 3u,
    VSTATE_ESTOP   = 4u,
    VSTATE_FAULT   = 5u,
};

/// BCM configuration constants
static constexpr uint8_t  AUTO_LOCK_SPEED      = 10u;
static constexpr uint8_t  INDICATOR_FLASH_ON   = 33u;
static constexpr uint8_t  INDICATOR_FLASH_OFF  = 33u;
static constexpr uint8_t  HEARTBEAT_ECU_ID     = 0x06u;
static constexpr uint16_t HEARTBEAT_CAN_ID     = 0x016u;
static constexpr uint16_t LIGHT_STATUS_CAN_ID  = 0x400u;
static constexpr uint16_t INDICATOR_CAN_ID     = 0x401u;
static constexpr uint16_t DOOR_LOCK_CAN_ID     = 0x402u;

/// Shared signal struct — owned by BcmSystem, passed by reference to SWCs and codec
struct BcmSignals
{
    // RX inputs (written by CanFrameCodec from CAN frames)
    uint8_t  vehicleState{0};
    uint16_t motorSpeedRpm{0};
    uint8_t  headlightCmd{0};
    uint8_t  turnSignalCmd{0};
    uint8_t  hazardCmd{0};
    uint8_t  doorLockCmd{0};
    uint8_t  estopActive{0};

    // TX outputs (written by SWCs, read by CanFrameCodec for CAN TX)
    uint8_t headlightOn{0};
    uint8_t tailLightOn{0};
    uint8_t leftIndicator{0};
    uint8_t rightIndicator{0};
    uint8_t hazardActive{0};
    uint8_t frontLeftLock{0};
};

} // namespace signals
} // namespace bcm
} // namespace taktflow
