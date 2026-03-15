// Taktflow Systems - ICU bus ID name mapping

#include "busid/BusId.h"

namespace common
{
namespace busid
{

#define BUS_ID_NAME(BUS) \
    case ::busid::BUS: return #BUS

char const* BusIdTraits::getName(uint8_t index)
{
    switch (index)
    {
        BUS_ID_NAME(CAN_0);
        default: return "INVALID";
    }
}

} // namespace busid
} // namespace common
