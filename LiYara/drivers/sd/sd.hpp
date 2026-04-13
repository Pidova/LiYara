#pragma once

#include "../../board/pins/pins.hpp"

#ifdef LIYARA_EXTRAS_SD_ENABLE
#include <SD.h>
#include <SPI.h>

namespace driver::sd {

      struct handler {

            handler(bool &ffailed, const board::pins::GP::handler::pin &sck, const board::pins::GP::handler::pin &miso, const board::pins::GP::handler::pin &mosi, board::pins::GP::handler::pin &cs) {
                  cs.set_m<board::pins::GP::handler::mode::output>();
                  cs.digital_high();
                  SPI.begin(sck.get_pn(), miso.get_pn(), mosi.get_pn(), cs.get_pn());
                  delay(300);
                  ffailed = !SD.begin(cs.get_pn());
            }

            bool open_file(const char *const filename);
            std::string list_dir(const char *const dir) const;
            std::size_t get_file_size();
            std::size_t read_bytes(std::uint8_t *buffer, const std::size_t buffer_size);
            void reset_buffer();
            void close_file();

          private:
            File f;
      };
} // namespace driver::sd

#endif