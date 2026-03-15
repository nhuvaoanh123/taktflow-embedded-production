// Taktflow Systems - DTC Display SWC implementation

#include "swc/DtcDisplayComponent.h"

#include <cstring>

namespace taktflow
{
namespace icu
{
namespace swc
{

static DtcEntry const EMPTY_ENTRY{};

DtcDisplayComponent::DtcDisplayComponent(signals::IcuSignals& signals)
: _signals(signals)
{}

void DtcDisplayComponent::init()
{
    std::memset(_entries, 0, sizeof(_entries));
    _count = 0u;
    _writeIndex = 0u;
    _tickCounter = 0u;
    _lastBroadcast = 0u;
}

void DtcDisplayComponent::execute()
{
    ++_tickCounter;

    if (!_signals.dtcUpdated)
    {
        return;
    }
    _signals.dtcUpdated = false;

    uint32_t const code = _signals.dtcCode;
    uint8_t const status = _signals.dtcStatus;

    if (code == 0u)
    {
        return;
    }

    // Deduplicate: same code+status as last broadcast
    uint32_t const broadcast = (code << 8u) | status;
    if (broadcast == _lastBroadcast)
    {
        return;
    }
    _lastBroadcast = broadcast;

    // Check if already in buffer
    uint8_t const idx = findByCode(code);
    if (idx < signals::DTC_MAX_ENTRIES)
    {
        // Update existing entry
        _entries[idx].status = status;
        _entries[idx].timestamp = _tickCounter;
        return;
    }

    // New DTC
    if (_count < signals::DTC_MAX_ENTRIES)
    {
        // Buffer not full — append
        _entries[_count].dtcCode = code;
        _entries[_count].status = status;
        _entries[_count].timestamp = _tickCounter;
        ++_count;
    }
    else
    {
        // Buffer full — replace oldest
        uint32_t oldestTs = _entries[0].timestamp;
        uint8_t oldestIdx = 0u;
        for (uint8_t i = 1u; i < signals::DTC_MAX_ENTRIES; ++i)
        {
            if (_entries[i].timestamp < oldestTs)
            {
                oldestTs = _entries[i].timestamp;
                oldestIdx = i;
            }
        }
        _entries[oldestIdx].dtcCode = code;
        _entries[oldestIdx].status = status;
        _entries[oldestIdx].timestamp = _tickCounter;
    }
}

uint8_t DtcDisplayComponent::findByCode(uint32_t code) const
{
    for (uint8_t i = 0u; i < _count; ++i)
    {
        if (_entries[i].dtcCode == code)
        {
            return i;
        }
    }
    return signals::DTC_MAX_ENTRIES;
}

DtcEntry const& DtcDisplayComponent::getEntry(uint8_t index) const
{
    if (index < _count)
    {
        return _entries[index];
    }
    return EMPTY_ENTRY;
}

void DtcDisplayComponent::clearAll()
{
    std::memset(_entries, 0, sizeof(_entries));
    _count = 0u;
    _lastBroadcast = 0u;
}

} // namespace swc
} // namespace icu
} // namespace taktflow
