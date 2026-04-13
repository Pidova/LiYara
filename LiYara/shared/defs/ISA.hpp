#pragma once
#include <cstdint>

namespace board::ISA {

      /* 
          Byte | Hint | Operands  (A, B, C, D, E, AUX) MAX SIZE IS 32 BITS
          ----------------------------------------------------------------
      */
      enum class opcodes : std::uint8_t {
            OP_NOP = 0x0,        /* 0x0 | No operation */
            OP_SET_RGB = 0x1,    /* 0x1 | Set RGB value to XY position | (X(BYTE), Y(BYTE), R(BYTE), G(BYTE), B(BYTE))[Ax] */
            OP_UPDATE = 0x2,     /* 0x2 | Update LEDS */
            OP_DELAY = 0x3,      /* 0x3 | Sleep for ms | MS(WORD)[A] */
            OP_SET_VAR = 0x4,    /* 0x4 | Set variable | ID(BYTE)[A], R(BYTE)[B], G(BYTE)[C], B(BYTE)[D]  */
            OP_SET_SPEED = 0x5,  /* 0x5 | Set playback speed | SPEED(DWORD)[A] */
            OP_SET_VPIXEL = 0x6, /* 0x6 | Set pixel based on variable | X(BYTE)[A], Y(BYTE)[B], ID(BYTE)[C] */
            OP_MOVEV = 0x7,      /* 0x7 | Move var | DEST(BYTE)[A], SRC(BYTE)[B]  */
            OP_MOVEP = 0x8,      /* 0x8 | Move pixel | X_DEST(BYTE)[A], Y_DEST(BYTE)[B], X_SRC(BYTE)[C], Y_SRC(BYTE)[D]  */
            OP_RENDERFRAME = 0x9 /* 0x9 | Renders frame with pixel matrix, Given matrix fills display | INSTRUCTION_COUNT(DWORD)[A] : { R(BYTE), G(BYTE), B(BYTE) }*/
      };

      /* Instruction data */
      struct instruction {

            struct {

                  union {
                        std::uint32_t A;  /* Operand A */
                        std::uint64_t Ax; /* Extended operand A */
                  };
                  std::uint32_t B = 0u;   /* Operand B */
                  std::uint32_t C = 0u;   /* Operand C */
                  std::uint32_t D = 0u;   /* Operand D */
                  std::uint32_t E = 0u;   /* Operand E */
                  std::uint32_t AUX = 0u; /* Auxiliary Operand */
            } operands;

            opcodes opcode = opcodes::OP_NOP; /* Opcode */
            std::uint8_t size = 0u;           /* Instruction size (Including opcode) */
      };

      static constexpr auto MAX_INST_SIZE = sizeof(instruction) - 1u; /* Max instruction byte size */

      namespace misc {

            /* Gets operand size */
            inline constexpr std::uint8_t operand_size(const opcodes i) {
                  switch (i) {
                        case opcodes::OP_DELAY:
                        case opcodes::OP_MOVEV: {
                              return 2u;
                        }
                        case opcodes::OP_SET_VPIXEL: {
                              return 3u;
                        }
                        case opcodes::OP_SET_VAR:
                        case opcodes::OP_MOVEP:
                        case opcodes::OP_SET_SPEED:
                        case opcodes::OP_RENDERFRAME: {
                              return 4u;
                        }
                        case opcodes::OP_SET_RGB: {
                              return 5u;
                        }
                        default: {
                              return 0u;
                        }
                  }
            }
      } // namespace misc

} // namespace board::ISA