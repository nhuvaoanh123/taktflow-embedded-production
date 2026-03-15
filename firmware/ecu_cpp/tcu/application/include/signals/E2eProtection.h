// Taktflow Systems - E2E protection (CRC-8 SAE J1850)

#pragma once

#include <cstdint>

namespace taktflow
{
namespace tcu
{
namespace signals
{

class E2eProtection
{
public:
    static uint8_t crc8(uint8_t const* data, uint8_t length, uint8_t dataId);
    static void protect(uint8_t* frameData, uint8_t dlc, uint8_t dataId, uint8_t& aliveCounter);
    static bool validate(uint8_t const* frameData, uint8_t dlc, uint8_t dataId);
};

} // namespace signals
} // namespace tcu
} // namespace taktflow
