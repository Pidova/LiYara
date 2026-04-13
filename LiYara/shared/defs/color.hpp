#pragma once
#include <cstdint>
#include <optional>

namespace board::display {

      struct color {
            std::uint8_t r = 0u; /* Red */
            std::uint8_t g = 0u; /* Green */
            std::uint8_t b = 0u; /* Blue */
      };

      namespace colors {

            static constexpr auto RED = color(255u, 0u, 0u);
            static constexpr auto GREEN = color(0u, 255u, 0u);
            static constexpr auto BLUE = color(0u, 0u, 255u);
            static constexpr auto ORANGE = color(255u, 165u, 0u);
            static constexpr auto YELLOW = color(255u, 255u, 0u);
            static constexpr auto CYAN = color(0u, 255u, 255u);
            static constexpr auto MAGENTA = color(255u, 0u, 255u);
            static constexpr auto WHITE = color(255u, 255u, 255u);
            static constexpr auto BLACK = color(0u, 0u, 0u);
            static constexpr auto GRAY = color(128u, 128u, 128u);
            static constexpr auto PURPLE = color(128u, 0u, 128u);
            static constexpr auto PINK = color(255u, 192u, 203u);
      } // namespace colors
} // namespace board::display
