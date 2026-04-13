#pragma once
#include "color.hpp"
#include "defs.hpp"
#include <bitset>
#include <cstdint>

namespace board::display {

      struct pixel {

            pixel() = default;
            pixel(const x_axis x, const y_axis y = 0u)
                : x(x), y(y) {};

            x_axis x = 0u;
            y_axis y = 0u;
      };

      struct diode_data {
            pixel p;
            color c;
            std::uint8_t brightness = 255u;
      };

} // namespace board::display