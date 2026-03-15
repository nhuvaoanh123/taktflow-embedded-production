// Taktflow Systems - DTC Store SWC implementation

#include "swc/DtcStoreComponent.h"

#include <cstring>

namespace taktflow
{
namespace tcu
{
namespace swc
{

DtcStoreComponent::DtcStoreComponent(signals::TcuSignals& signals)
: _signals(signals)
{}

void DtcStoreComponent::init()
{
    std::memset(_entries, 0, sizeof(_entries));
    _count = 0u;
    _tickCounter = 0u;
    _lastBroadcast = 0u;
}

void DtcStoreComponent::execute()
{
    ++_tickCounter;

    // Auto-capture from DTC broadcast
    if (_signals.dtcUpdated)
    {
        _signals.dtcUpdated = false;

        uint32_t const code = _signals.dtcCode;
        uint8_t const status = _signals.dtcStatus;

        if (code != 0u)
        {
            uint32_t const broadcast = (code << 8u) | status;
            if (broadcast != _lastBroadcast)
            {
                _lastBroadcast = broadcast;
                add(code, status);
            }
        }
    }

    // Age existing DTCs
    for (uint8_t i = 0u; i < _count; ++i)
    {
        if ((_entries[i].status & signals::DTC_STATUS_TEST_FAILED) == 0u)
        {
            ++_entries[i].agingCounter;
            if (_entries[i].agingCounter >= signals::DTC_AGING_CLEAR_CYCLES)
            {
                // Remove by shifting
                for (uint8_t j = i; j + 1u < _count; ++j)
                {
                    _entries[j] = _entries[j + 1u];
                }
                std::memset(&_entries[_count - 1u], 0, sizeof(DtcStoreEntry));
                --_count;
                --i; // recheck this index
            }
        }
    }
}

void DtcStoreComponent::add(uint32_t dtcCode, uint8_t status)
{
    uint8_t const idx = findByCode(dtcCode);
    if (idx < signals::DTC_MAX_ENTRIES)
    {
        // Update existing
        _entries[idx].status = status;
        _entries[idx].agingCounter = 0u;
        captureFreeze(_entries[idx]);
        return;
    }

    // New entry
    if (_count < signals::DTC_MAX_ENTRIES)
    {
        _entries[_count].dtcCode = dtcCode;
        _entries[_count].status = status;
        _entries[_count].agingCounter = 0u;
        captureFreeze(_entries[_count]);
        ++_count;
    }
}

uint8_t DtcStoreComponent::findByCode(uint32_t code) const
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

void DtcStoreComponent::captureFreeze(DtcStoreEntry& entry)
{
    entry.ff_speed = static_cast<uint16_t>(
        (static_cast<uint32_t>(_signals.motorSpeedRpm) * 60u) / 1000u);
    entry.ff_current = _signals.motorCurrentMa;
    entry.ff_voltage = _signals.batteryVoltageMv;
    entry.ff_temp = static_cast<uint8_t>(_signals.motorTempC);
    entry.ff_timestamp = _tickCounter;
}

DtcStoreEntry const* DtcStoreComponent::getByIndex(uint8_t index) const
{
    if (index < _count) { return &_entries[index]; }
    return nullptr;
}

uint8_t DtcStoreComponent::getByMask(uint8_t statusMask, uint32_t* dtcCodes, uint8_t maxCount) const
{
    uint8_t found = 0u;
    for (uint8_t i = 0u; i < _count && found < maxCount; ++i)
    {
        if ((_entries[i].status & statusMask) != 0u)
        {
            dtcCodes[found] = _entries[i].dtcCode;
            ++found;
        }
    }
    return found;
}

void DtcStoreComponent::clear()
{
    std::memset(_entries, 0, sizeof(_entries));
    _count = 0u;
    _lastBroadcast = 0u;
}

} // namespace swc
} // namespace tcu
} // namespace taktflow
