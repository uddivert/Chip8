#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "isa.h"
#include "chip8.h"
#include "debugger.h"

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
    int index = (c8 -> opcode >> 8) & 0xF;
    int value = c8 -> opcode & 0xFF;
    c8 -> varReg[index] += value;
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
    int height = c8-> opcode& 0x000F;
    uint8_t x = (c8->opcode & 0x0F00) >> 8;
    uint8_t y = (c8->opcode & 0x00F0) >> 4;
    x = c8 -> varReg[x] & 63; // xcoord is VX
    y =  c8 -> varReg[y] & 31; // ycoord is VY
    c8-> varReg[0x0F] = 0; // Reset VF (carry) flag
    
    // Loop through each pixel in the row
    for (int row = 0; row < height; row++) {
        if (row + y >= 32) break; // overflow
        uint8_t spriteData = c8->memory[c8->regI + row];
        for (int col = 0; col < 8; col++) {
            if (col + x >= 64) break; // overflow
            uint8_t pixel = (spriteData >> (7 - col)) & 0x1;
            if(!pixel) continue;
            if (c8 -> display[row + y][col + x] ==1) {
                c8 ->varReg[0x0F] = 1;
            }
            c8 -> display[row + y][col + x] ^=1;
        } // for
    } // for
} // draw
