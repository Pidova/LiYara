#include "../../shared.hpp"

namespace board::display::mutate {

      void fill(displayer &disp, const color &col, const std::optional<std::uint8_t> &brightness) {

            for (auto &c : disp.modules) {
                  for (auto &r : c) {
                        if (r) {
                              (*r).set(col, brightness);
                        }
                  }
            }
            return;
      }

      namespace x {

            void copy(displayer &disp, const x_axis dest, const x_axis src) {
                  if (!disp.contain_x(dest) || !disp.contain_x(src)) {
                        return;
                  }
                  disp.modules[dest] = disp.modules[src];
                  return;
            }

            void swap(displayer &disp, const x_axis dest, const x_axis src) {

                  if (!disp.contain_x(dest) || !disp.contain_x(src)) {
                        return;
                  }
                  std::swap(disp.modules[dest], disp.modules[src]);
                  return;
            }
            void swap(displayer &disp, const x_range &dest, const x_range &src) {

                  if (!disp.contain_x(dest) || !disp.contain_x(src)) {
                        return;
                  }

                  if (std::abs(dest.second - dest.first) != std::abs(src.second - src.first)) {
                        return;
                  }

                  for (auto i = dest.first, j = dest.first;; i += (dest.second >= dest.first) ? 1 : -1, j += (src.second >= src.first) ? 1 : -1) {
                        std::swap(disp.modules[i], disp.modules[j]);
                        if (i == dest.second) {
                              break;
                        }
                  }
                  return;
            }

            void mirror(displayer &disp, const x_range &range) {

                  if (!disp.contain_x(range)) {
                        return;
                  }
                  auto start = range.first;
                  auto end = range.second;
                  while (start < end) {
                        std::swap(disp.modules[start++], disp.modules[end--]);
                  }
                  return;
            }

      } // namespace x

} // namespace board::display::mutate