#include "sd.hpp"
#ifdef LIYARA_EXTRAS_SD_ENABLE

namespace driver::sd {

      bool handler::open_file(const char *const filename) {
            if (this->f) {
                  this->f.close();
            }
            this->f = SD.open(filename, FILE_READ);
            this->f.seek(0);
            return this->f;
      }
      std::string handler::list_dir(const char *const dir) const {

            auto root = SD.open(dir);
            if (!root || !root.isDirectory()) {
                  return "";
            }
            std::string result = "";
            while (auto file = root.openNextFile()) {
                  if (!file.isDirectory()) {
                        result += std::string(file.name()) + "\n";
                  }
            }
            return result;
      }
      std::size_t handler::get_file_size() {
            return this->f.size();
      }
      std::size_t handler::read_bytes(std::uint8_t *buffer, const std::size_t buffer_size) {
            if (!this->f) {
                  return 0u;
            }
            return this->f.readBytes((char *)buffer, buffer_size);
      }
      void handler::reset_buffer() {
            if (!this->f) {
                  return;
            }
            this->f.seek(0);
            return;
      }
      void handler::close_file() {
            if (this->f) {
                  this->f.close();
            }
            return;
      }
} // namespace driver::sd

#endif