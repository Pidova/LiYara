#pragma once
#include "../../../../defs/shared.hpp"
#include <algorithm>
#include <math.h>

namespace board::animate::color {

      /* Fill sprite color */
      template <sprite_n n>
      constexpr void fill(sprite<n> &s, const display::color &c) {
            for (auto &d : s) {
                  d.c = c;
            }
            return;
      }

      template <sprite_n n>
      constexpr void fill_gradient_from(sprite<n> &s, const display::color &start, const display::color &end, const display::pixel &p) {

            /* Get dist */
            float max_dist = 0.0f;
            for (const auto &d : s) {
                  const auto dx = static_cast<float>(d.p.x) - static_cast<float>(p.x);
                  const auto dy = static_cast<float>(d.p.y) - static_cast<float>(p.y);
                  const auto dist = sqrt(dx * dx + dy * dy);
                  if (dist > max_dist) {
                        max_dist = dist;
                  }
            }

            /* Interpolate */
            if (max_dist == 0.0f) {
                  max_dist = 1.0f;
            }
            for (auto &d : s) {

                  const auto dx = static_cast<float>(d.p.x) - static_cast<float>(p.x);
                  const auto dy = static_cast<float>(d.p.y) - static_cast<float>(p.y);
                  auto t = sqrt(dx * dx + dy * dy) / max_dist;
                  t = std::clamp(t, 0.0f, 1.0f);

                  d.c.r = static_cast<std::uint8_t>(start.r + t * (end.r - start.r));
                  d.c.g = static_cast<std::uint8_t>(start.g + t * (end.g - start.g));
                  d.c.b = static_cast<std::uint8_t>(start.b + t * (end.b - start.b));
            }
            return;
      }

} // namespace board::animate::color