#include "../headers/color.hpp"
#include <algorithm>
#include <math.h>

namespace board::animate::color::mutate {

      /* Step color */
      constexpr void step(display::color &dest, const display::color &target, const std::uint8_t step_size) {

            dest.r += std::clamp(static_cast<std::int32_t>(target.r) - static_cast<std::int32_t>(dest.r), -static_cast<std::int32_t>(step_size), static_cast<std::int32_t>(step_size));
            dest.g += std::clamp(static_cast<std::int32_t>(target.g) - static_cast<std::int32_t>(dest.g), -static_cast<std::int32_t>(step_size), static_cast<std::int32_t>(step_size));
            dest.b += std::clamp(static_cast<std::int32_t>(target.b) - static_cast<std::int32_t>(dest.b), -static_cast<std::int32_t>(step_size), static_cast<std::int32_t>(step_size));
            return;
      }

      /* Invert color */
      constexpr void invert(display::color &dest) {

            dest.r = static_cast<std::uint8_t>(255u - dest.r);
            dest.g = static_cast<std::uint8_t>(255u - dest.g);
            dest.b = static_cast<std::uint8_t>(255u - dest.b);
            return;
      }

      /* Blend colors */
      constexpr void blend(display::color &dest, const display::color &src, const std::uint8_t alpha = 128u) {

            dest.r = static_cast<std::uint8_t>((dest.r * (255u - alpha) + src.r * alpha) / 255u);
            dest.g = static_cast<std::uint8_t>((dest.g * (255u - alpha) + src.g * alpha) / 255u);
            dest.b = static_cast<std::uint8_t>((dest.b * (255u - alpha) + src.b * alpha) / 255u);
            return;
      }

      /* Add colors  */
      constexpr void add(display::color &result, const display::color &src) {

            result.r = std::min(255u, static_cast<std::uint32_t>(result.r) + static_cast<std::uint32_t>(src.r));
            result.g = std::min(255u, static_cast<std::uint32_t>(result.g) + static_cast<std::uint32_t>(src.g));
            result.b = std::min(255u, static_cast<std::uint32_t>(result.b) + static_cast<std::uint32_t>(src.b));
            return;
      }

      /* Grayscale */
      constexpr void gray_scale(display::color &dest) {

            dest.r = dest.g = dest.b = static_cast<std::uint8_t>((54 * dest.r + 183 * dest.g + 18 * dest.b) >> 8);
            return;
      }

      /* Tint color */
      constexpr void tint(display::color &dest, const display::color &tint) {
            dest.r = static_cast<std::uint8_t>((dest.r * tint.r) / 255u);
            dest.g = static_cast<std::uint8_t>((dest.g * tint.g) / 255u);
            dest.b = static_cast<std::uint8_t>((dest.b * tint.b) / 255u);
            return;
      }

} // namespace board::animate::color::mutate
