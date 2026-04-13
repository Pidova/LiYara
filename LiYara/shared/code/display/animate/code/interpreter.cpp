#include "../../../../shared.hpp"
#include <chrono>
#include <cstring>
#include <iostream>
#include <thread>

namespace board::animate::interpreter {

      bool stop_int = false;

      inline void read_buffer(vm_state &state) {

            std::uint16_t offset = 0u;
            if (state.fs.buffer_pc && state.fs.buffer_pc < configs::interpreter::MAX_READ_BUFFER_LEN) {
                  offset = configs::interpreter::MAX_READ_BUFFER_LEN - state.fs.buffer_pc;
                  std::memmove(state.data, state.data + state.fs.buffer_pc, offset);
            }
            state.fs.buffer_pc = 0u;
            state.fs.read_buffer(state, configs::interpreter::MAX_READ_BUFFER_LEN - offset, offset);
      }
      void run(display::displayer &disp, vm_state &state) {

            static std::uint8_t buffer[configs::interpreter::MAX_READ_BUFFER_LEN];
            if (state.fs) {
                  state.data = buffer;
                  state.len = state.fs.get_file_size(state);
                  read_buffer(state);
            }
            while (state.pc < state.len) {

                  if (state.fs && (state.fs.buffer_pc + ISA::MAX_INST_SIZE) > configs::interpreter::MAX_READ_BUFFER_LEN) {
                        read_buffer(state);
                  }
                  const auto dism = disassemble_instruction(state);

                  if (stop_int) {
                        state.fs.buffer_pc = 0u;
                        state.pc = 0u;
                        stop_int = false;
                        return;
                  }
                  if (dism.first == interpreter::disassembling_error::fsbuffer_not_inrange || dism.first == interpreter::disassembling_error::fsbuffer_overrun) {
                        ++state.fs.buffer_pc;
                        continue;
                  }
                  if (dism.first != interpreter::disassembling_error::none) {
                        return;
                  }
                  const auto &inst = dism.second;

                  if (state.hook && !state.hook(disp, state, inst)) {
                        state.pc += inst.size;
                        continue;
                  }
                  switch (inst.opcode) {
                        case ISA::opcodes::OP_DELAY: {

                              const auto ms = std::min(static_cast<std::uint32_t>(inst.operands.A / state.play_back_speed), state.max_delay_ms);
                              if (!ms) {
                                    break;
                              }
#ifdef ARDUINO
                              delay(ms);
#else
                              std::this_thread::sleep_for(std::chrono::milliseconds(ms));
#endif
                              state.pc += inst.size;
                              break;
                        }
                        case ISA::opcodes::OP_SET_RGB: {

                              board::display::pixel p;
                              board::display::color c;
                              display::unpack(inst.operands.Ax, p, c);
                              disp.set_color(p, c, state.max_brightness);
                              state.pc += inst.size;
                              break;
                        }
                        case ISA::opcodes::OP_UPDATE: {
                              disp.update();
                              state.pc += inst.size;
                              break;
                        }
                        case ISA::opcodes::OP_SET_SPEED: {

                              state.play_back_speed = inst.operands.A;
                              state.pc += inst.size;
                              break;
                        }
                        case ISA::opcodes::OP_SET_VAR: {

                              state.vars[inst.operands.A] = board::display::color{
                                  static_cast<std::uint8_t>(inst.operands.B),
                                  static_cast<std::uint8_t>(inst.operands.C),
                                  static_cast<std::uint8_t>(inst.operands.D)};
                              state.pc += inst.size;
                              break;
                        }
                        case ISA::opcodes::OP_SET_VPIXEL: {

                              disp.set_color(board::display::pixel(inst.operands.A, inst.operands.B), state.vars[inst.operands.A], state.max_brightness);
                              state.pc += inst.size;
                              break;
                        }
                        case ISA::opcodes::OP_MOVEV: {

                              state.vars[inst.operands.A] = state.vars[inst.operands.B];
                              state.pc += inst.size;
                              break;
                        }
                        case ISA::opcodes::OP_MOVEP: {

                              if (const auto &c = disp.get_color(board::display::pixel(inst.operands.C, inst.operands.D)); c) {
                                    disp.set_color(board::display::pixel(inst.operands.A, inst.operands.B), *c, state.max_brightness);
                              }
                              state.pc += inst.size;
                              break;
                        }
                        case ISA::opcodes::OP_RENDERFRAME: {

                              state.pc += inst.size;
                              if (state.pc + (inst.operands.A * 3u) >= state.len) {
                                    break;
                              }
                              for (auto i = 0u; i < inst.operands.A; ++i) {
                                    const auto px = board::display::pixel(i % disp.display_length(), i / disp.display_heigth());
                                    if (!disp.contain(px)) {
                                          break;
                                    }
                                    disp.set_color(px,
                                        board::display::color(state.data[state.pc + (i * 3u)],
                                            state.data[state.pc + (i * 3u) + 1u],
                                            state.data[state.pc + (i * 3u) + 2u]),
                                        state.max_brightness);
                              }
                              state.pc += inst.operands.A * 3u;
                              break;
                        }
                        default: {
                              state.pc += inst.size;
                              break;
                        }
                  }
                  state.fs.buffer_pc += inst.size;
            }
            state.fs.buffer_pc = 0u;
            state.pc = 0u;
            return;
      }
} // namespace board::animate::interpreter
