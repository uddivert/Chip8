#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "isa.h"
#include "chip8.h"

/**
 * @brief null values for cpuTable
 * 
 * @param c8 
 */
void cpuNull(struct chip8* c8)
{

}

void _f0(struct chip8* c8)
{
    if ((c8 -> opcode & 0xF)== 0)  // if 00E0 CLS
    {
        
    } else {
        c8 -> progCounter = peep(&c8 -> stack); // 00EE RET
        pop(&c8 -> stack);
    } // else
} // ret

void _f1(struct chip8* c8)
{
    c8 -> progCounter = c8 -> opcode & 0xFFF; // 1NNN JP 
} // jmp

void _f6(struct chip8* c8)
{
    // 3xkk - SE Vx, Byte
    int index = (c8 -> opcode >> 8) & 0xF;
    int value = c8 -> opcode & 0xFF;
    c8 -> varReg[index] = value;
} // set

void _f7(struct chip8* c8) 
{
    //7xkk - ADD Vx, byte
    int index = (c8 -> opcode >> 8) & 0xF;
    int value = c8 -> opcode & 0xFF;
    c8 -> varReg[index] += value;
    //printf("Instruction: %X, Index: %d, Value: %d\n",c8 -> opcode, index, value);
} // add

void _fA(struct chip8* c8)
{
    int value = c8 -> opcode & 0xFFF;
    c8 -> regI = value;
} // setRi

/**
 * @brief  DRW Vx, Vy, nibble
 * 
 * @param c8  Chip 8 struct
 */
void _fD(struct chip8 *c8) 
{
    printExtra("Opcode: %X\n", c8 -> opcode);

    int height = c8-> opcode& 0x000F;
    uint8_t x = (c8->opcode & 0x0F00) >> 8;
    uint8_t y = (c8->opcode & 0x00F0) >> 4;
    //x = c8 -> varReg[x & 63]; // xcoord is VX
    //y =  c7 -> varReg[y & 32]; // ycoord is VY

    c8->varReg[0x0F] = 0; // Reset VF (carry) flag
    
    for (int row = 0; row < height; row++) {
        uint8_t spriteData = c8->memory[c8->regI + row];

        // Loop through each pixel in the row
        for (int col = 0; col < 8; col++) {
            // for
            } // for 
        } // for
} // draw
