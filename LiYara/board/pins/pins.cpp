#include "pins.hpp"

namespace board::pins::GP::handler {

      void pin::init() const {
            pinMode(this->pn, this->m == mode::input ? INPUT : OUTPUT);
            return;
      }

      std::uint8_t pin::get_pn() const {
            return this->pn;
      }
      handler::mode pin::get_m() const {
            return this->m;
      }

      void pin::analog_high(const std::int32_t input) const {
            analogWrite(this->pn, input);
            return;
      }
      void pin::analog_low() const {
            analogWrite(this->pn, 0u);
            return;
      }
      std::optional<std::uint16_t> pin::analog_read() const {
            return this->f.fadc_capable ? std::optional(analogRead(this->pn)) : std::nullopt;
      }
      float pin::analog_read(const std::uint8_t samples) const {
            std::uint32_t sum = 0u;
            for (auto i = 0u; i < samples; ++i) {
                  sum += analogRead(this->pn);
            }
            return static_cast<float>(sum / samples);
      }

      void pin::digital_high() const {
            digitalWrite(this->pn, HIGH);
            return;
      }
      void pin::digital_low() const {
            digitalWrite(this->pn, LOW);
            return;
      }
      std::int32_t pin::digital_read() const {
            return digitalRead(this->pn);
      }

} // namespace board::pins::GP::handler