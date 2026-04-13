#pragma once

#include "../defs.hpp"
#include <Arduino.h>
#include <cstdint>
#include <driver/i2s.h>
#include <optional>

namespace board::pins {

      namespace GP {

            namespace handler {

                  /* Pin modes */
                  enum class mode : std::uint8_t {
                        input, /* Input mode */
                        output /* Output mode */
                  };
                  struct flags {
                        bool fconstant_mode = false; /* Mode cannot be changed? */
                        bool fadc_capable = false;   /* Port is analog to digitial capable? */
                  };

                  class pin {

                      public:
                        /* Constructors */
                        pin() = default;
                        constexpr pin(const std::uint8_t pn)
                            : pn(pn) {
                              return;
                        }
                        constexpr pin(const std::uint8_t pn, const flags &f)
                            : pn(pn), f(f) {
                              return;
                        }
                        constexpr pin(const std::uint8_t pn, const mode m)
                            : pn(pn), m(m) {
                              this->f.fconstant_mode = true;
                              return;
                        }
                        constexpr pin(const std::uint8_t pn, const mode m, const flags &f)
                            : pn(pn), m(m), f(f) {
                              this->f.fconstant_mode = true;
                              return;
                        }

                        /* Init port */
                        void init() const;

                        /* Get data  */
                        std::uint8_t get_pn() const; /* Get pin number */
                        handler::mode get_m() const; /* Get pin mode */

                        /* Set modes */
                        template <mode m>
                        void set_m() {
                              if (!this->f.fconstant_mode) {
                                    this->m = m;
                              }
                              pinMode(this->pn, this->m == mode::input ? INPUT : OUTPUT);
                              return;
                        }

                        /* Analog */
                        void analog_high(const std::int32_t input) const;    /* Analog set high */
                        void analog_low() const;                             /* Analog set low */
                        std::optional<std::uint16_t> analog_read() const;    /* Analog read */
                        float analog_read(const std::uint8_t samples) const; /* Analog read with samples */

                        /* Digital */
                        void digital_high() const;         /* Digital set  high */
                        void digital_low() const;          /* Digital set low */
                        std::int32_t digital_read() const; /* Digital read */

                      private:
                        flags f;              /* Flags */
                        mode m = mode::input; /* Pin mode */
                        std::uint8_t pn = 0u; /* Pin number */
                  };
            } // namespace handler

            static auto LED = handler::pin(2u, handler::mode::output); /* LED PIN */
            static auto IO4 = handler::pin(4u);                        /* General porpose pin (input/output) */
            static auto IO5 = handler::pin(5u);                        /* General porpose pin (input/output) */
            static auto IO12 = handler::pin(12u);                      /* General porpose pin (input/output): boot fails if pulled high, strapping pin */
            static auto IO13 = handler::pin(13u);                      /* General porpose pin (input/output) */
            static auto IO14 = handler::pin(14u);                      /* General porpose pin (input/output): outputs PWM signal at boot */
            static auto IO15 = handler::pin(15u);                      /* General porpose pin (input/output): outputs PWM signal at boot */
            static auto IO16 = handler::pin(16u);                      /* General porpose pin (input/output) */
            static auto IO17 = handler::pin(17u);                      /* General porpose pin (input/output) */
            static auto IO18 = handler::pin(18u);                      /* General porpose pin (input/output) */
            static auto IO19 = handler::pin(19u);                      /* General porpose pin (input/output) */
            static auto IO21 = handler::pin(21u);                      /* General porpose pin (input/output) */
            static auto IO22 = handler::pin(22u);                      /* General porpose pin (input/output) */
            static auto IO23 = handler::pin(23u);                      /* General porpose pin (input/output) */
            static auto IO25 = handler::pin(25u);                      /* General porpose pin (input/output) */
            static auto IO26 = handler::pin(26u);                      /* General porpose pin (input/output) */
            static auto IO27 = handler::pin(27u);                      /* General porpose pin (input/output) */

            namespace adc {

                  static auto IO32 = handler::pin(32u, handler::flags({.fadc_capable = true}));                       /* General porpose pin (input/output) */
                  static auto IO33 = handler::pin(33u, handler::flags({.fadc_capable = true}));                       /* General porpose pin (input/output) */
                  static auto IO34 = handler::pin(34u, handler::mode::input, handler::flags({.fadc_capable = true})); /* General porpose pin input only */
                  static auto IO35 = handler::pin(35u, handler::mode::input, handler::flags({.fadc_capable = true})); /* General porpose pin input only */
                  static auto IO36 = handler::pin(36u, handler::mode::input, handler::flags({.fadc_capable = true})); /* General porpose pin input only */
                  static auto IO39 = handler::pin(39u, handler::mode::input, handler::flags({.fadc_capable = true})); /* General porpose pin input only */
            } // namespace adc
      } // namespace GP

      namespace i2s {

      } // namespace i2s
} // namespace board::pins