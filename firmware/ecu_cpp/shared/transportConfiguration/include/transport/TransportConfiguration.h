// Taktflow Systems - Transport layer configuration constants

#pragma once

#include <transport/TransportMessage.h>

#include <platform/estdint.h>

namespace transport
{
class TransportConfiguration
{
public:
    TransportConfiguration() = delete;

    static uint16_t const TESTER_RANGE_8BIT_START = 0x00E0U;
    static uint16_t const TESTER_RANGE_8BIT_END   = 0x00FDU;

    static uint16_t const TESTER_RANGE_DOIP_START = 0x0EE0U;
    static uint16_t const TESTER_RANGE_DOIP_END   = 0x0EFDU;

    static uint16_t const FUNCTIONAL_ALL_ISO14229 = 0x00DF;

    enum
    {
        INVALID_DIAG_ADDRESS = 0xFFU
    };

    static uint16_t const MAX_FUNCTIONAL_MESSAGE_PAYLOAD_SIZE = 6U;

    static uint16_t const DIAG_PAYLOAD_SIZE = 4095U;

    static uint8_t const NUMBER_OF_FULL_SIZE_TRANSPORT_MESSAGES = 5U;

    static uint8_t const MAXIMUM_NUMBER_OF_TRANSPORT_MESSAGES
        = NUMBER_OF_FULL_SIZE_TRANSPORT_MESSAGES * 8U;

    static bool isFunctionalAddress(uint16_t address);
    static bool isFunctionallyAddressed(TransportMessage const& message);
    static bool isTesterAddress(uint16_t address);
    static bool isFromTester(TransportMessage const& message);
};

inline bool TransportConfiguration::isFunctionalAddress(uint16_t const address)
{
    return (FUNCTIONAL_ALL_ISO14229 == address);
}

inline bool TransportConfiguration::isFunctionallyAddressed(TransportMessage const& message)
{
    return isFunctionalAddress(message.getTargetId());
}

inline bool TransportConfiguration::isTesterAddress(uint16_t const address)
{
    return (
        ((address >= TransportConfiguration::TESTER_RANGE_8BIT_START)
         && (address <= TransportConfiguration::TESTER_RANGE_8BIT_END))
        || ((address >= TransportConfiguration::TESTER_RANGE_DOIP_START)
            && (address <= TransportConfiguration::TESTER_RANGE_DOIP_END)));
}

inline bool TransportConfiguration::isFromTester(TransportMessage const& message)
{
    return isTesterAddress(message.getSourceId());
}

} // namespace transport
