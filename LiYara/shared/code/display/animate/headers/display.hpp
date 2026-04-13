#pragma once
#include "../../../../defs/shared.hpp"

namespace board::animate::formatter {

      /* Moves sprite back on screen if it goes offscreen */
      template <sprite_n n>
      constexpr void format(sprite<n> &s, const std::size_t max_x, const std::size_t max_y) {
            for (auto &d : s) {
                  if (d.p.x >= max_x) {
                        d.p.x %= max_x;
                  }
                  if (d.p.y >= max_y) {
                        d.p.y %= max_y;
                  }
            }
            return;
      }

} // namespace board::animate::formatter