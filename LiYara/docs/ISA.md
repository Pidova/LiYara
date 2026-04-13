# ISA
Instruction Set Architecture for LiYara-VM

## Operand sizes
-   union{u32(A), u64(Ax)}
-   u32{B, C, D, E, AUX}


## Opcodes
 Opcode  |Mnemonic        | Description                         | Operands                                 |
|--------|----------------|-------------------------------------|------------------------------------------|
| 0x0    | OP_NOP         | No operation.                       | N/A                                      |
| 0x1    | OP_SET_RGB     | Set color at XY coordinate.         | Ax: {X, Y, R, G, B} (Packed)             |
| 0x2    | OP_UPDATE      | Commit buffer to hardware.          | N/A                                      |
| 0x3    | OP_DELAY       | Sleep for N milliseconds.           | A: Duration (ms)                         |
| 0x4    | OP_SET_VAR     | Assign RGB to a variable ID.        | A: ID, B: R, C: G, D: B                  |
| 0x5    | OP_SET_SPEED   | Set global playback speed.          | A: Speed                                 |
| 0x6    | OP_SET_VPIXEL  | Draw pixel using variable color.    | A: X, B: Y, C: Var ID                    |
| 0x7    | OP_MOVEV       | Copy value from one var to another. | A: Dest ID, B: Source ID                 |
| 0x8    | OP_MOVEP       | Move pixel data across coordinates. | A: Dest X, B: Dest Y, C: Src X, D: Src Y |
| 0x9    | OP_RENDERFRAME | Render full frame from matrix data. | A: Instruction Count                     |
