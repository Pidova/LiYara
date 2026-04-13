#pragma once
#include "../../../../defs/shared.hpp"
#include <numbers>

namespace board::animate::move {

      /* Move X */
      template <sprite_n n>
      constexpr void move_x(sprite<n> &s, const std::int8_t c = 1) {
            for (auto &d : s) {
                  d.p.x += c;
            }
            return;
      }

      /* Move Y */
      template <sprite_n n>
      constexpr void move_y(sprite<n> &s, const std::int8_t c = 1) {
            for (auto &d : s) {
                  d.p.y += c;
            }
            return;
      }

      /* Rotate */
      template <sprite_n n>
      constexpr void rotate(sprite<n> &s, const std::uint16_t degrees) {

            const auto r = degrees * std::numbers::pi / 180.0;
            const auto cs = cos(r);
            const auto sn = sin(r);
            for (auto &d : s) {

                  const double x = d.p.x;
                  const double y = d.p.y;
                  d.p.x = static_cast<std::int32_t>(x * cs - y * sn);
                  d.p.y = static_cast<std::int32_t>(x * sn + y * cs);
            }
            return;
      }
} // namespace board::animate::move