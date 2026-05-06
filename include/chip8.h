#ifndef CHIP8_H
#define CHIP8_H

#define VIDEO_WIDTH 64
#define VIDEO_HEIGHT 32

typedef struct Chip8 Chip8;

void chip_init(Chip8* chip8);
void load_rom(Chip8* chip8, char const * filename);
uint8_t generate_random_seed(void);

void OP_00E0(Chip8* chip8); // 00E0 - CLS: Clear the display
void OP_00EE(Chip8* chip8); // 00EE - RET: Return from subroutine

void OP_1nnn(Chip8* chip8); // 1nnn - JP addr
void OP_2nnn(Chip8* chip8); // 2nnn - CALL addr

void OP_3xkk(Chip8* chip8); // 3xkk - SE Vx, byte
void OP_4xkk(Chip8* chip8); // 4xkk - SNE Vx, byte
void OP_5xy0(Chip8* chip8); // 5xy0 - SE Vx, Vy

void OP_6xkk(Chip8* chip8); // 6xkk - LD Vx, byte
void OP_7xkk(Chip8* chip8); // 7xkk - ADD Vx, byte

void OP_8xy0(Chip8* chip8); // 8xy0 - LD Vx, Vy
void OP_8xy1(Chip8* chip8); // 8xy1 - OR Vx, Vy
void OP_8xy2(Chip8* chip8); // 8xy2 - AND Vx, Vy
void OP_8xy3(Chip8* chip8); // 8xy3 - XOR Vx, Vy
void OP_8xy4(Chip8* chip8); // 8xy4 - ADD Vx, Vy
void OP_8xy5(Chip8* chip8); // 8xy5 - SUB Vx, Vy
void OP_8xy6(Chip8* chip8); // 8xy6 - SHR Vx {, Vy}
void OP_8xy7(Chip8* chip8); // 8xy7 - SUBN Vx, Vy
void OP_8xyE(Chip8* chip8); // 8xyE - SHL Vx {, Vy}

void OP_9xy0(Chip8* chip8); // 9xy0 - SNE Vx, Vy
void OP_Annn(Chip8* chip8); // Annn - LD I, addr
void OP_Bnnn(Chip8* chip8); // Bnnn - JP V0, addr

void OP_Cxkk(Chip8* chip8); // Cxkk - RND Vx, byte
void OP_Dxyn(Chip8* chip8); // Dxyn - DRW Vx, Vy, nibble

void OP_Ex9E(Chip8* chip8); // Ex9E - SKP Vx
void OP_ExA1(Chip8* chip8); // ExA1 - SKNP Vx

void OP_Fx07(Chip8* chip8); // Fx07 - LD Vx, DT
void OP_Fx0A(Chip8* chip8); // Fx0A - LD Vx, K
void OP_Fx15(Chip8* chip8); // Fx15 - LD DT, Vx
void OP_Fx18(Chip8* chip8); // Fx18 - LD ST, Vx
void OP_Fx1E(Chip8* chip8); // Fx1E - ADD I, Vx
void OP_Fx29(Chip8* chip8); // Fx29 - LD F, Vx
void OP_Fx33(Chip8* chip8); // Fx33 - LD B, Vx
void OP_Fx55(Chip8* chip8); // Fx55 - LD [I], Vx
void OP_Fx65(Chip8* chip8); // Fx65 - LD Vx, [I]
#endif