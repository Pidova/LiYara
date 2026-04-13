#pragma once
#include "defs/shared.hpp"
#include "libs/boostpp/vector.hpp"
#ifdef ARDUINO
#include "../board/pins/pins.hpp"
#include <Adafruit_NeoPixel.h>
#endif
#include <array>
#include <cstdint>
#include <functional>
#include <optional>

namespace board {

      namespace display {

            namespace handler::ws2812 {

#ifdef ARDUINO
                  /* Gets Neo Pixel driver */
                  Adafruit_NeoPixel get_driver(const std::uint8_t count, board::pins::GP::handler::pin &p);
#endif

                  class diode {

                      public:
                        diode() = default;
                        diode(const pixel &p, const color &c, const std::uint8_t brightness = 255u)
                            : d({p, c, brightness}) {
                        }

                        /* Get */
                        pixel get_pixel() const;
                        color get_color() const;
                        std::uint8_t get_brightness() const;
                        diode_data get_data() const;

                        /* Color */
                        void set_color(const color &col, const std::optional<std::uint8_t> &brightness = std::nullopt);
                        std::array<std::uint8_t, 3u> get_color_data() const;

                      private:
                        diode_data d;
                  };

                  inline constexpr boost::fixed_vector<board::display::handler::ws2812::diode, configs::display::MAX_MODULE_HEIGTH> make_module_array(
                      const x_axis x, const y_axis count, const color &c, const std::uint8_t brightness = 255u) {

                        boost::fixed_vector<board::display::handler::ws2812::diode, configs::display::MAX_MODULE_HEIGTH> result;
                        for (auto i = 0u; i < count; ++i) {
                              result.emplace_back(board::display::pixel(x, i), c, brightness);
                        }
                        return result;
                  }

                  class module {

                      public:
                        module() = default;
#ifdef ARDUINO
                        module(const boost::fixed_vector<diode, configs::display::MAX_MODULE_HEIGTH> &diodes, const Adafruit_NeoPixel &driver) :diodes(diodes),
                            driver(driver) {

                              return;
                        }
#else
                        module(const boost::fixed_vector<diode, configs::display::MAX_MODULE_HEIGTH> &diodes) :diodes(diodes) {

                              return;
                        }
#endif

                        /* Set color */
                        void set(const std::uint8_t idx, const color &col, const std::optional<std::uint8_t> &brightness = std::nullopt);
                        void set(const color &col, const std::optional<std::uint8_t> &brightness = std::nullopt);

                        /* Get */
                        std::optional<color> get(const std::uint8_t idx) const;

                        /* Update Leds */
                        void update();
                        boost::fixed_vector<diode, configs::display::MAX_MODULE_HEIGTH> diodes;

                      private:
#ifdef ARDUINO
                        Adafruit_NeoPixel driver;
#endif
                  };
            } // namespace handler::ws2812

            /* Pack/Unpack */
            std::uint64_t pack(const pixel &p, const color &c);
            void unpack(const std::uint64_t data, pixel &p, color &c);

            class displayer {

                public:
                  displayer() = default;
                  displayer(const std::optional<std::function<void(displayer *)>> &updater)
                      : updater(updater) {
                        };

                  /* Registers */
                  void add(const x_axis x, const y_axis y, handler::ws2812::module &m);

                  template <animate::sprite_n n>
                  constexpr void add(const animate::sprite<n> &s) {
                        for (const auto &i : s) {
                              this->set_color(i);
                        }
                        return;
                  }

                  /* Set color */
                  void set_color(const pixel &p, const color &c, const std::optional<std::uint8_t> &brightness = std::nullopt);
                  void set_color(const diode_data &d);
                  void set_static_brightness(const std::optional<std::uint8_t> &b);

                  /* Get */
                  std::optional<color> get_color(const pixel &p) const;

                  /* Update LEDs */
                  void update();

                  /* Misc */
                  bool contain_x(const x_axis x) const;
                  bool contain_x(const x_range &x) const;
                  bool contain_y(const y_axis y) const;
                  bool contain_y(const y_range &y) const;
                  bool contain(const pixel &px) const;

                  /* Size */
                  std::uint8_t display_length() const;
                  std::uint8_t display_heigth() const;

                  std::array<std::array<std::optional<handler::ws2812::module>, configs::display::MAX_HEIGTH>, configs::display::MAX_LENGTH> modules;

                private:
                  std::optional<std::function<void(displayer *)>> updater = std::nullopt;
                  std::optional<std::uint8_t> static_brightness = std::nullopt;
            };

            /* WARNING. SHOULD ONLY BE USED IF ALL PIXELS HAVE BEEN SET */
            namespace mutate {

                  /* Fill display with a color */
                  void fill(displayer &disp, const color &col, const std::optional<std::uint8_t> &brightness = std::nullopt);

                  namespace x {

                        /* Copy src-> dest */
                        void copy(displayer &disp, const x_axis dest, const x_axis src);

                        /* Swap src -> dest */
                        void swap(displayer &disp, const x_axis dest, const x_axis src);
                        void swap(displayer &disp, const x_range &dest, const x_range &src);

                        /* Mirror x axis with range */
                        void mirror(displayer &disp, const x_range &range);
                  } // namespace x
            } // namespace mutate
      } // namespace display

      namespace animate {

            namespace interpreter {

                  extern bool stop_int;

                  struct vm_state {
                        std::uint8_t max_brightness = 255u;                                                                 /* Max brightness */
                        float play_back_speed = 1.0f;                                                                       /* Current animation playback speed */
                        std::array<board::display::color, 255u> vars{};                                                     /* RGB Vars */
                        std::uint8_t *data = nullptr;                                                                       /* Animation data */
                        std::size_t len = 0u;                                                                               /* Animation data length */
                        std::uint32_t max_delay_ms = 1000u;                                                                 /* Max delay in ms */
                        std::size_t pc = 0u;                                                                                /* Program counter */
                        std::function<bool(display::displayer &disp, vm_state &, const ISA::instruction &)> hook = nullptr; /* Instruction hook */

                        struct fs {
                              std::size_t buffer_pc = 0u;                                                                                                       /* Buffer program counter */
                              std::function<std::size_t(vm_state &)> get_file_size = nullptr;                                                                   /* Calls the function on file size request (Return file size) */
                              std::function<std::uint16_t(vm_state &, const std::uint16_t read_size, const std::uint16_t offset_buffer)> read_buffer = nullptr; /* Calls the function on buffer read (Returns read buffer size) */

                              operator bool() const {
                                    return get_file_size && read_buffer;
                              }
                        } fs;
                  };

                  /* Run interpreted animations */
                  void run(display::displayer &disp, vm_state &state);

                  /* Dissassemble to instruction */
                  enum class disassembling_error : std::uint8_t {
                        none,                 /* No error */
                        pc_not_in_range,      /* Program counter is out of range */
                        invalid_op_size,      /* Given opcode size doesnt fit in program space */
                        fsbuffer_not_inrange, /* FS buffer PC is out of range */
                        fsbuffer_overrun      /* FS pc buffer overan */
                  };
                  std::pair<disassembling_error, ISA::instruction> disassemble_instruction(const vm_state &state);
            } // namespace interpreter

      } // namespace animate

} // namespace board