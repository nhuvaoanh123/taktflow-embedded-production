// Taktflow Systems - DTC Store SWC (64-entry buffer with freeze-frame + aging)

#pragma once

#include "signals/TcuSignals.h"

#include <cstdint>

namespace taktflow
{
namespace tcu
{
namespace swc
{

struct DtcStoreEntry
{
    uint32_t dtcCode{0};
    uint8_t  status{0};
    uint16_t agingCounter{0};
    uint16_t ff_speed{0};
    uint16_t ff_current{0};
    uint16_t ff_voltage{0};
    uint8_t  ff_temp{0};
    uint32_t ff_timestamp{0};
};

class DtcStoreComponent
{
public:
    explicit DtcStoreComponent(signals::TcuSignals& signals);

    void init();
    void execute();

    uint8_t getCount() const { return _count; }
    DtcStoreEntry const* getByIndex(uint8_t index) const;
    uint8_t getByMask(uint8_t statusMask, uint32_t* dtcCodes, uint8_t maxCount) const;
    void clear();
    void add(uint32_t dtcCode, uint8_t status);

private:
    uint8_t findByCode(uint32_t code) const;
    void captureFreeze(DtcStoreEntry& entry);

    signals::TcuSignals& _signals;

    DtcStoreEntry _entries[signals::DTC_MAX_ENTRIES];
    uint8_t _count{0};
    uint32_t _tickCounter{0};
    uint32_t _lastBroadcast{0};
};

} // namespace swc
} // namespace tcu
} // namespace taktflow
