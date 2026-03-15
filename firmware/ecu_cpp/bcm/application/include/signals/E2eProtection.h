// Taktflow Systems - E2E protection (CRC-8 SAE J1850 + alive counter)

#pragma once

#include <cstdint>

namespace taktflow
{
namespace bcm
{
namespace signals
{

class E2eProtection
{
public:
    /// CRC-8 SAE J1850 (poly 0x1D, init 0xFF) over data[0..len-1]
    static uint8_t computeCrc8(uint8_t const* data, uint8_t len);

    /// Write E2E header into PDU payload:
    ///   byte 0 bits 0-3: dataId, bits 4-7: aliveCounter
    ///   byte 1: CRC-8 over bytes 0 and 2..dlc-1
    static void protect(uint8_t* payload, uint8_t dlc, uint8_t dataId, uint8_t aliveCounter);

    /// Validate E2E CRC in PDU payload. Returns true if valid.
    static bool validate(uint8_t const* payload, uint8_t dlc);
};

} // namespace signals
} // namespace bcm
} // namespace taktflow
