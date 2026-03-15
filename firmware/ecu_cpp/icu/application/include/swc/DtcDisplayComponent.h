// Taktflow Systems - DTC Display SWC (circular DTC buffer)

#pragma once

#include "signals/IcuSignals.h"

#include <cstdint>

namespace taktflow
{
namespace icu
{
namespace swc
{

struct DtcEntry
{
    uint32_t dtcCode{0};
    uint8_t  status{0};
    uint32_t timestamp{0};
};

class DtcDisplayComponent
{
public:
    explicit DtcDisplayComponent(signals::IcuSignals& signals);

    void init();
    void execute();

    uint8_t getCount() const { return _count; }
    DtcEntry const& getEntry(uint8_t index) const;
    void clearAll();

private:
    uint8_t findByCode(uint32_t code) const;

    signals::IcuSignals& _signals;

    DtcEntry _entries[signals::DTC_MAX_ENTRIES];
    uint8_t _count{0};
    uint8_t _writeIndex{0};
    uint32_t _tickCounter{0};
    uint32_t _lastBroadcast{0};
};

} // namespace swc
} // namespace icu
} // namespace taktflow
