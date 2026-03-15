// Taktflow Systems - ICU bus ID definitions

#pragma once

#include "common/busid/BusId.h"

#include <cstdint>

namespace busid
{
static constexpr uint8_t CAN_0    = 1;
static constexpr uint8_t SELFDIAG = 2;
static constexpr uint8_t LAST_BUS = SELFDIAG;

} // namespace busid
