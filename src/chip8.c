#include "stdlib.h"
#include "stdint.h"
#include <stdio.h>
#include <time.h>
#include <string.h>
#include "chip8.h"

#define START_ADDRESS 0x200
#define FONTSET_START_ADDRESS 0x050
#define FONTSET_SIZE 80
#define SPRITE_WIDTH 8

struct Chip8
{
    uint8_t registers[16];
    uint8_t memory[4096];
    uint8_t stack_pointer;
    uint8_t d_timer; //delay
    uint8_t s_timer; //sound
    uint8_t keypad[16];
    uint16_t index;
    uint16_t program_counter;
    uint16_t opcode;
    uint16_t stack[16];
    uint32_t video[64 * 32];
};

uint8_t fontset[FONTSET_SIZE] =
{
	0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
	0x20, 0x60, 0x20, 0x20, 0x70, // 1
	0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
	0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
	0x90, 0x90, 0xF0, 0x10, 0x10, // 4
	0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
	0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
	0xF0, 0x10, 0x20, 0x40, 0x40, // 7
	0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
	0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
	0xF0, 0x90, 0xF0, 0x90, 0x90, // A
	0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
	0xF0, 0x80, 0x80, 0x80, 0xF0, // C
	0xE0, 0x90, 0x90, 0x90, 0xE0, // D
	0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
	0xF0, 0x80, 0xF0, 0x80, 0x80  // F
};

void chip_init(Chip8 *chip8)
{
    srand(time(NULL));

    //initi the program_counter
    chip8->program_counter = START_ADDRESS;

    // load the fontset in the 0x050 address
    for(unsigned int i = 0; i < FONTSET_SIZE; ++i){
        chip8->memory[FONTSET_START_ADDRESS + 1] = fontset[i];
    }
}

static void decode_0x0_instructions(Chip8 *chip8){
    switch((chip8->opcode & 0x000Fu)){
        case 0x0:
            OP_00E0(chip8);
            break;
        
        case 0xE:
            OP_00EE(chip8);
            break;
        default:
            break;
    }
}

static void decode_0x8_instructions(Chip8 *chip8){
    switch((chip8->opcode & 0x000Fu)){
        case 0x0:
            OP_8xy0(chip8);
            break;
        
        case 0x1:
            OP_8xy1(chip8);
            break;

        case 0x2:
            OP_8xy2(chip8);
            break;

        case 0x3:
            OP_8xy3(chip8);
            break;

        case 0x4:
            OP_8xy4(chip8);
            break;

        case 0x5:
            OP_8xy5(chip8);
            break;

        case 0x6:
            OP_8xy6(chip8);
            break;

        case 0x07:
            OP_8xy7(chip8);
            break;

        case 0xE:
            OP_8xyE(chip8);
            break;

        default:
            break;
    }
}

static void decode_0xE_instructions(Chip8 *chip8){
    switch((chip8->opcode & 0x000Fu)){
        case 0xE:
            OP_Ex9E(chip8);
            break;
        
        case 0x1:
            OP_ExA1(chip8);
            break;

        default:
            break;
    }
}

static void decode_0xF_instructions(Chip8 *chip8){
    switch((chip8->opcode & 0x00FFu)){
        case 0x07:
            OP_Fx07(chip8);
            break;

        case 0x0A:
            OP_Fx0A(chip8);
            break;
        
        case 0x15:
            OP_Fx15(chip8);
            break;
        
        case 0x18:
            OP_Fx18(chip8);
            break;
        
        case 0x1E:
            OP_Fx1E(chip8);
            break;
        
        case 0x29:
            OP_Fx29(chip8);
            break;
        
        case 0x33:
            OP_Fx33(chip8);
            break;
        
        case 0x55:
            OP_Fx55(chip8);
            break;
        
        case 0x65:
            OP_Fx65(chip8);
            break;
        
        default:
            break;
    }

}

void cycle_instruction(Chip8 *chip8)
{
    chip8->opcode = (chip8->memory[chip8->program_counter << 8u] | chip8->memory[chip8->program_counter + 1]);
    chip8->program_counter += 2;

    // first switch for the MSB 
    switch((chip8->opcode & 0xF000u) >> 12u)
    {
        case 0x0:
            decode_0x0_instructions(chip8);
            break;

        case 0x1:
            OP_1nnn(chip8);
            break;

        case 0x2:
            OP_2nnn(chip8);
            break;

        case 0x3:
            OP_3xkk(chip8);
            break;

        case 0x4:
            OP_4xkk(chip8);
            break;

        case 0x5:
            OP_5xy0(chip8);
            break;

        case 0x6:
            OP_6xkk(chip8);
            break;

        case 0x7:
            OP_7xkk(chip8);
            break;

        case 0x8:
            decode_0x8_instructions(chip8);
            break;

        case 0x9:
            OP_9xy0(chip8);
            break;

        case 0xA:
            OP_Annn(chip8);
            break;
            
        case 0xB:
            OP_Bnnn(chip8);
            break;

        case 0xC:
            OP_Cxkk(chip8);
            break;

        case 0xD:
            OP_Dxyn(chip8);
            break;

        case 0xE:
            decode_0xE_instructions(chip8);
            break;

        case 0xF:
            decode_0xF_instructions(chip8);
            break;

        default:
            printf("Error: Unknown Instruction");
            break;
    }

    if(chip8->d_timer > 0){
        --chip8->d_timer;
    }

    if(chip8->s_timer > 0){
        --chip8->s_timer;
    }
}

// TODO: proper error handling
void load_rom(Chip8* chip8, char const *filename)
{
    FILE *fp = fopen(filename, "rb");
    if(fp != NULL) return; // need proper error handling here

    fseek(fp, 0, SEEK_END);
    uint32_t size = ftell(fp);
    char *buffer = malloc(size);

    if(!buffer) return; // need proper error handling here
    fseek(fp, 0, SEEK_SET);
    fread(buffer, 1, size,fp);
    fclose(fp);

    for(uint32_t i = 0; i < size; ++i){
        chip8->memory[START_ADDRESS + i] = buffer[i];
    }

    free(buffer);
    buffer = NULL;
}

uint8_t generate_random_seed(void)
{
    uint8_t min = 0;
    uint8_t max = 255;

    return (rand() % (max = min + 1)) + min; //produce a random number between 0 to 255;
}

void OP_00E0(Chip8 *chip8)
{
    memset(chip8->video, 0, sizeof(chip8->video));
}

void OP_00EE(Chip8 *chip8)
{
    chip8->stack_pointer--;
    chip8->program_counter = chip8->stack[chip8->stack_pointer]; 
}

void OP_1nnn(Chip8 *chip8)
{
    uint16_t address = chip8->opcode & 0x0FFFu;
    chip8->program_counter = address;
}

void OP_2nnn(Chip8 *chip8)
{
    chip8->stack[chip8->stack_pointer] = chip8->program_counter;
    chip8->stack_pointer++;

    uint16_t address = chip8->opcode & 0x0FFFu;
    chip8->program_counter = address;
}

void OP_3xkk(Chip8 *chip8)
{
    uint8_t Vx = (chip8->opcode & 0x0F00u) >> 8;
    uint8_t byte = (chip8->opcode & 0x00FFu);

    if(chip8->registers[Vx] == byte){
        chip8->program_counter += 2;
    }
}

void OP_4xkk(Chip8 *chip8)
{
    uint8_t Vx = (chip8->opcode & 0x0F00u) >> 8;
    uint8_t byte = (chip8->opcode & 0x00FFu);

    if(chip8->registers[Vx] != byte){
        chip8->program_counter += 2;
    }

}

void OP_5xy0(Chip8 *chip8)
{
    uint8_t Vx = (chip8->opcode & 0x0F00u) >> 8;
    uint8_t Vy = (chip8->opcode & 0x00F0u) >> 4;

    if(chip8->registers[Vx] == chip8->registers[Vy]){
        chip8->program_counter += 2;
    }
}

void OP_6xkk(Chip8 *chip8)
{
    uint8_t Vx = (chip8->opcode & 0x0F00u) >> 8;
    uint8_t byte = (chip8->opcode & 0x00FFu);
    chip8->registers[Vx] = byte;
}

void OP_7xkk(Chip8 *chip8)
{
    uint8_t Vx = (chip8->opcode & 0x0F00u) >> 8;
    uint8_t byte = (chip8->opcode & 0x00FFu);

    chip8->registers[Vx] = chip8->registers[Vx] + byte;
}

// not sure if tama ang aking magiging interpretation sa insturction na ito
void OP_8xy0(Chip8 *chip8)
{
    uint8_t Vx = (chip8->opcode & 0x0F00u) >> 8;
    uint8_t Vy = (chip8->opcode & 0x00F0u) >> 4;

    chip8->registers[Vx] = chip8->registers[Vy];
}

void OP_8xy1(Chip8 *chip8)
{
    uint8_t Vx = (chip8->opcode & 0x0F00u) >> 8;
    uint8_t Vy = (chip8->opcode & 0x00F0u) >> 4;

    chip8->registers[Vx] = chip8->registers[Vx] | chip8->registers[Vy];
}

void OP_8xy2(Chip8 *chip8)
{
    uint8_t Vx = (chip8->opcode & 0x0F00u) >> 8;
    uint8_t Vy = (chip8->opcode & 0x00F0u) >> 4;

    chip8->registers[Vx] = chip8->registers[Vx] & chip8->registers[Vy];
}

void OP_8xy3(Chip8 *chip8)
{
    uint8_t Vx = (chip8->opcode & 0x0F00u) >> 8;
    uint8_t Vy = (chip8->opcode & 0x00F0u) >> 4;

    chip8->registers[Vx] = chip8->registers[Vx] ^ chip8->registers[Vy];
}

void OP_8xy4(Chip8 *chip8)
{
    uint8_t Vx = (chip8->opcode & 0x0F00u) >> 8;
    uint8_t Vy = (chip8->opcode & 0x00F0u) >> 4;

    uint16_t sum = chip8->registers[Vx] + chip8->registers[Vy];

    // based sa instruction
    if(sum > 255){
        chip8->registers[15] = 1; //refers to the Vf register
        chip8->registers[Vx] = (sum & 0x00FFu);
    }
    else{
        chip8->registers[15] = 0; //refers to the Vf register
    }
}

void OP_8xy5(Chip8 *chip8)
{
    uint8_t Vx = (chip8->opcode & 0x0F00u) >> 8;
    uint8_t Vy = (chip8->opcode & 0x00F0u) >> 4;

    if(chip8->registers[Vx] > chip8->registers[Vy]){
        chip8->registers[15] = 1; //refers to the Vf register
    }
    else{
        chip8->registers[15] = 0; //refers to the Vf register
    }

    chip8->registers[Vx] = chip8->registers[Vx] - chip8->registers[Vy];
}

void OP_8xy6(Chip8 *chip8)
{
    uint8_t Vx = (chip8->opcode & 0x0F00u) >> 8;
    uint8_t bit_test = (Vx & 0x0001u);

    if(bit_test == 1){
        chip8->registers[15] = 1; //refers to the Vf register
    }
    else{
        chip8->registers[15] = 0; //refers to the Vf register
    }

    chip8->registers[Vx] = chip8->registers[Vx] >> 1;
}

void OP_8xy7(Chip8 *chip8)
{
    uint8_t Vx = (chip8->opcode & 0x0F00u) >> 8;
    uint8_t Vy = (chip8->opcode & 0x00F0u) >> 4;

    if(chip8->registers[Vx] < chip8->registers[Vy]){
        chip8->registers[15] = 1; //refers to the Vf register
    }
    else{
        chip8->registers[15] = 0; //refers to the Vf register
    }

    chip8->registers[Vx] = chip8->registers[Vy] - chip8->registers[Vx];
}

void OP_8xyE(Chip8 *chip8)
{
    uint8_t Vx = (chip8->opcode & 0x0F00u) >> 8;
    uint8_t bit_test = (Vx & 0x0001u);

    if(bit_test == 1){
        chip8->registers[15] = 1; //refers to the Vf register
    }
    else{
        chip8->registers[15] = 0; //refers to the Vf register
    }

    chip8->registers[Vx] = chip8->registers[Vx] << 1;
}

void OP_9xy0(Chip8 *chip8)
{
    uint8_t Vx = (chip8->opcode & 0x0F00u) >> 8;
    uint8_t Vy = (chip8->opcode & 0x00F0u) >> 4;

    if(chip8->registers[Vx] != chip8->registers[Vy]){
        chip8->program_counter += 2;
    }
}

void OP_Annn(Chip8 *chip8)
{
    uint16_t address = chip8->opcode & 0x0FFFu;
    chip8->index = address;

}

void OP_Bnnn(Chip8 *chip8)
{
    uint16_t address = chip8->opcode & 0x0FFFu;
    chip8->program_counter = address + chip8->registers[0];
}

void OP_Cxkk(Chip8 *chip8)
{
    uint8_t Vx = (chip8->opcode & 0x0F00u) >> 8;
    uint8_t byte = (chip8->opcode & 0x00FFu);
    chip8->registers[Vx] = byte & generate_random_seed();
}

void OP_Dxyn(Chip8 *chip8)
{
    uint8_t n_byte = chip8->opcode & 0x000Fu;
    uint8_t Vx = (chip8->opcode & 0x0F00u) >> 8;
    uint8_t Vy = (chip8->opcode & 0x00F0u) >> 4;

    //video wrapping algo
    uint8_t x = chip8->registers[Vx] % VIDEO_WIDTH;
    uint8_t y = chip8->registers[Vy] % VIDEO_HEIGHT;

    chip8->registers[15] = 0; //reset the VF register

    for(int row = 0; row < n_byte; ++row){

        uint8_t sprite_byte = chip8->memory[chip8->index + row];

        for(int col = 0; col < SPRITE_WIDTH; ++col){
            
            //gets the individual pixel by shifting the 0x80u depending on what col is now
            uint8_t sprite_pixel = sprite_byte & (0x80u >> col); 
            
            uint32_t *screen_pixel = chip8->video[(y + col) * VIDEO_HEIGHT + (x + row)];

            if(sprite_pixel){
                if(*screen_pixel == 0xFFFFFFFF){ //collision check
                    chip8->registers[15] = 1;
                }
                *screen_pixel = 0xFFFFFFFF;
            }
        }
    }
}

void OP_Ex9E(Chip8 *chip8)
{
    uint8_t Vx = (chip8->opcode & 0x0F00u) >> 8;
    uint8_t key_reg = chip8->keypad[Vx];

    if(chip8->keypad[key_reg]){
        chip8->program_counter += 2;
    }

}

void OP_ExA1(Chip8 *chip8)
{
    uint8_t Vx = (chip8->opcode & 0x0F00u) >> 8;
    uint8_t key_reg = chip8->keypad[Vx];

    if(!chip8->keypad[key_reg]){
        chip8->program_counter += 2;
    }
}

void OP_Fx07(Chip8 *chip8)
{
    uint8_t Vx = (chip8->opcode & 0x0F00u) >> 8;
    chip8->registers[Vx] = chip8->d_timer;

}

void OP_Fx0A(Chip8 *chip8)
{
    uint8_t Vx = (chip8->opcode & 0x0F00u) >> 8;
    
    if(chip8->keypad[0]){
        chip8->keypad[Vx] = 0;
    }

    else if(chip8->keypad[1]){
        chip8->keypad[Vx] = 1;        
    }

    else if(chip8->keypad[2]){
        chip8->keypad[Vx] = 2;
    }

    else if(chip8->keypad[3]){
        chip8->keypad[Vx] = 3;
    }

    else if(chip8->keypad[4]){
        chip8->keypad[Vx] = 4;    
    }

    else if(chip8->keypad[5]){
        chip8->keypad[Vx] = 5;    
    }

    else if(chip8->keypad[6]){
        chip8->keypad[Vx] = 6;
    }

    else if(chip8->keypad[7]){
        chip8->keypad[Vx] = 7;    
    }

    else if(chip8->keypad[8]){
        chip8->keypad[Vx] = 8;
    }

    else if(chip8->keypad[9]){
        chip8->keypad[Vx] = 9;    
    }

    else if(chip8->keypad[0xAu]){
        chip8->keypad[Vx] = 0xAu;    
    }

    else if(chip8->keypad[0xBu]){
        chip8->keypad[Vx] = 0xBu;    
    }

    else if(chip8->keypad[0xCu]){
        chip8->keypad[Vx] = 0xCu;
    }

    else if(chip8->keypad[0xDu]){
        chip8->keypad[Vx] = 0xDu;
    }

    else if(chip8->keypad[0xEu]){
        chip8->keypad[Vx] = 0xEu;
    }

    else if(chip8->keypad[0xFu]){
        chip8->keypad[Vx] = 0xFu;
    }
}

void OP_Fx15(Chip8 *chip8)
{
    uint8_t Vx = (chip8->opcode & 0x0F00u) >> 8;
    chip8->d_timer = chip8->registers[Vx];    
}

void OP_Fx18(Chip8 *chip8)
{
    uint8_t Vx = (chip8->opcode & 0x0F00u) >> 8;
    chip8->s_timer = chip8->registers[Vx];    
}

void OP_Fx1E(Chip8 *chip8)
{
    uint8_t Vx = (chip8->opcode & 0x0F00u) >> 8;
    chip8->index = chip8->index + chip8->registers[Vx]; 
}

void OP_Fx29(Chip8 *chip8)
{
    uint8_t Vx = (chip8->opcode & 0x0F00u) >> 8;
    uint8_t digit = chip8->registers[Vx];

    chip8->index = FONTSET_START_ADDRESS + (digit * 5);     
}

void OP_Fx33(Chip8 *chip8)
{
    uint8_t Vx = (chip8->opcode & 0x0F00u) >> 8;
    
    uint8_t hundred = Vx / 100;
    chip8->memory[chip8->index] = hundred;
    
    uint8_t tens = (Vx / 10) % 10;
    chip8->memory[chip8->index + 1] = tens;
    
    uint8_t ones = Vx % 10;
    chip8->memory[chip8->index + 2] = ones;
    
}

void OP_Fx55(Chip8 *chip8)
{
    uint8_t Vx = (chip8->opcode & 0x0F00u) >> 8;

    for(uint8_t i = 0; i < Vx; ++i){
        chip8->memory[chip8->index + i] = chip8->registers[i];
    }
}

void OP_Fx65(Chip8 *chip8)
{
    uint8_t Vx = (chip8->opcode & 0x0F00u) >> 8;

    for(uint8_t i = 0; i < Vx; ++i){
        chip8->registers[i] = chip8->memory[chip8->index + i];
    }
}
