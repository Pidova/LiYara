#include "../../../../shared.hpp"

namespace board::animate::interpreter {

      std::pair<disassembling_error, ISA::instruction> disassemble_instruction(const vm_state &state) {

            ISA::instruction result;

            if (state.pc >= state.len) {
                  return {disassembling_error::pc_not_in_range, result};
            }

            auto pc = state.fs ? state.fs.buffer_pc : state.pc;
            if (state.fs && pc >= configs::interpreter::MAX_READ_BUFFER_LEN) {
                  return {disassembling_error::fsbuffer_not_inrange, result};
            }
            const auto cpc = pc;
            result.opcode = static_cast<ISA::opcodes>(state.data[pc++]);

            const auto size = ISA::misc::operand_size(result.opcode);
            if (pc + size > state.len) {
                  return {disassembling_error::invalid_op_size, result};
            }
            if (state.fs && state.fs.buffer_pc + size > configs::interpreter::MAX_READ_BUFFER_LEN) {
                  return {disassembling_error::fsbuffer_overrun, result};
            }
            switch (result.opcode) {
                  case ISA::opcodes::OP_DELAY: {
                        result.operands.A = (static_cast<std::uint16_t>(state.data[pc]) | static_cast<std::uint16_t>(state.data[pc + 1u])) << 8;
                        break;
                  }
                  case ISA::opcodes::OP_SET_RGB: {
                        result.operands.Ax = 0u;
                        for (auto i = 0u; i < 8u; ++i) {
                              result.operands.Ax |= static_cast<std::uint64_t>(state.data[pc + i]) << (8u * i);
                        }
                        break;
                  }
                  case ISA::opcodes::OP_SET_VAR: {
                        result.operands.A = state.data[pc];
                        result.operands.B = state.data[pc + 1u];
                        result.operands.C = state.data[pc + 2u];
                        result.operands.D = state.data[pc + 3u];
                        break;
                  }
                  case ISA::opcodes::OP_RENDERFRAME: {
                        result.operands.A = state.data[pc];
                        break;
                  }
                  case ISA::opcodes::OP_SET_SPEED: {
                        result.operands.Ax =
                            (static_cast<std::uint32_t>(state.data[pc]) << 0u) |
                            (static_cast<std::uint32_t>(state.data[pc + 1u]) << 8u) |
                            (static_cast<std::uint32_t>(state.data[pc + 2u]) << 16u) |
                            (static_cast<std::uint32_t>(state.data[pc + 3u]) << 24u);
                        break;
                  }
                  case ISA::opcodes::OP_SET_VPIXEL: {
                        result.operands.A = state.data[pc];
                        result.operands.B = state.data[pc + 1u];
                        result.operands.C = state.data[pc + 2u];
                        break;
                  }
                  case ISA::opcodes::OP_MOVEV: {
                        result.operands.A = state.data[pc];
                        result.operands.B = state.data[pc + 1u];
                        break;
                  }
                  case ISA::opcodes::OP_MOVEP: {
                        result.operands.A = state.data[pc];
                        result.operands.B = state.data[pc + 1u];
                        result.operands.C = state.data[pc + 2u];
                        result.operands.D = state.data[pc + 3u];
                        break;
                  }
                  default: {
                        break;
                  }
            }
            pc += size;
            result.size = static_cast<std::uint8_t>(pc - cpc);
            return {disassembling_error::none, result};
      }
} // namespace board::animate::interpreter