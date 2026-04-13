#include "../../shared.hpp"

namespace board::display {

      namespace handler {

            namespace ws2812 {

#ifdef ARDUINO
                  Adafruit_NeoPixel get_driver(const std::uint8_t count, board::pins::GP::handler::pin &p) {
                        auto result = Adafruit_NeoPixel(count, p.get_pn(), NEO_GRB + NEO_KHZ800);
                        result.begin();
                        result.show();
                        return result;
                  }
#endif

                  pixel diode::get_pixel() const {
                        return this->d.p;
                  }
                  color diode::get_color() const {
                        return this->d.c;
                  }
                  std::uint8_t diode::get_brightness() const {
                        return this->d.brightness;
                  }
                  diode_data diode::get_data() const {
                        return this->d;
                  }
                  void diode::set_color(const color &col, const std::optional<std::uint8_t> &brightness) {

                        this->d.c = col;
                        this->d.brightness = brightness.value_or(this->d.brightness);
                        return;
                  }
                  std::array<std::uint8_t, 3u> diode::get_color_data() const {

                        std::array<std::uint8_t, 3u> result({0u});
                        result[0u] = this->d.c.g;
                        result[1u] = this->d.c.r;
                        result[2u] = this->d.c.b;
                        return result;
                  }

                  void module::set(const std::uint8_t idx, const color &col, const std::optional<std::uint8_t> &brightness) {

                        if (idx >= this->diodes.size()) {
                              return;
                        }
                        this->diodes[idx].set_color(col, brightness);
                        return;
                  }
                  void module::set(const color &col, const std::optional<std::uint8_t> &brightness) {

                        for (auto &i : this->diodes) {
                              i.set_color(col, brightness);
                        }
                        return;
                  }
                  std::optional<color> module::get(const std::uint8_t idx) const {

                        return idx >= this->diodes.size() ? std::nullopt : std::optional(this->diodes[idx].get_color());
                  }
                  void module::update() {

#ifdef ARDUINO
                        for (auto i = 0u; i < this->diodes.size(); ++i) {
                              const auto &diode = this->diodes[i];
                              const auto data = diode.get_color_data();
                              this->driver.setBrightness(diode.get_brightness());
                              this->driver.setPixelColor(i, this->driver.Color(data[1u], data[0u], data[2u]));
                        }
                        this->driver.show();
#endif
                        return;
                  }

            } // namespace ws2812

      } // namespace handler

      std::uint64_t pack(const pixel &p, const color &c) {
            std::uint64_t result = 0u;
            result |= static_cast<std::uint64_t>(p.x);
            result |= static_cast<std::uint64_t>(p.y) << 8;
            result |= static_cast<std::uint64_t>(c.r) << 16;
            result |= static_cast<std::uint64_t>(c.g) << 24;
            result |= static_cast<std::uint64_t>(c.b) << 32;
            return result;
      }
      void unpack(const std::uint64_t data, pixel &p, color &c) {
            p.x = data & 0xFF;
            p.y = (data >> 8) & 0xFF;
            c.r = (data >> 16) & 0xFF;
            c.g = (data >> 24) & 0xFF;
            c.b = (data >> 32) & 0xFF;
            return;
      }

      void displayer::add(const x_axis x, const y_axis y, handler::ws2812::module &m) {
            if (x >= configs::display::MAX_LENGTH || y >= configs::display::MAX_HEIGTH) {
                  return;
            }
            this->modules[x][y] = m;
            return;
      }
      void displayer::set_color(const pixel &p, const color &c, const std::optional<std::uint8_t> &brightness) {

            if (p.x >= configs::display::MAX_LENGTH || p.y >= (configs::display::MAX_HEIGTH * configs::display::MAX_MODULE_HEIGTH)) {
                  return;
            }
            if (auto &o = this->modules[p.x][std::uint8_t(p.y / configs::display::MAX_MODULE_HEIGTH)]; o) {
                  (*o).set(p.y % configs::display::MAX_MODULE_HEIGTH, c, this->static_brightness ? this->static_brightness : brightness);
            }
            return;
      }
      void displayer::set_color(const diode_data &d) {
            this->set_color(d.p, d.c, d.brightness);
            return;
      }
      void displayer::set_static_brightness(const std::optional<std::uint8_t> &b) {
            this->static_brightness = b;
            return;
      }
      std::optional<color> displayer::get_color(const pixel &p) const {

            if (p.x >= configs::display::MAX_LENGTH || p.y >= (configs::display::MAX_HEIGTH * configs::display::MAX_MODULE_HEIGTH)) {
                  return std::nullopt;
            }
            if (auto &o = this->modules[p.x][std::uint8_t(p.y / configs::display::MAX_MODULE_HEIGTH)]; o) {
                  return (*o).get(p.y % configs::display::MAX_MODULE_HEIGTH);
            }
            return std::nullopt;
      }
      void displayer::update() {

            if (this->updater) {
                  (*this->updater)(this);
                  return;
            }
            for (auto &c : this->modules) {
                  for (auto &r : c) {
                        if (r) {
                              (*r).update();
                        }
                  }
            }
            return;
      }
      bool displayer::contain_x(const x_axis x) const {
            return x < this->modules.size();
      }
      bool displayer::contain_x(const x_range &x) const {
            return this->contain_x(x.first) && this->contain_x(x.second);
      }
      bool displayer::contain_y(const y_axis y) const {
            return y < configs::display::MAX_HEIGTH;
      }
      bool displayer::contain_y(const y_range &y) const {
            return this->contain_y(y.first) && this->contain_y(y.second);
      }
      bool displayer::contain(const pixel &px) const {
            return this->contain_x(px.x) && this->contain_y(px.y);
      }

      std::uint8_t displayer::display_length() const {
            return configs::display::MAX_LENGTH;
      }
      std::uint8_t displayer::display_heigth() const {
            return configs::display::MAX_HEIGTH;
      }

} // namespace board::display