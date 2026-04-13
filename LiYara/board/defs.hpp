#pragma once
#include <cstdint>

#define LIYARA_EXTRAS_SD_ENABLE 1

namespace board {

      enum class power_state : std::uint8_t {
            off = 0u, /* Nothing being emitted */
            on        /* Something is being emitted */
      };
}