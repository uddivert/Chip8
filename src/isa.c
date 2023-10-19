#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
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

/**
 * @brief CLS 
 * Clear the display.
 * 
 * RET
 * Return from a subroutine.
 * 
 * The interpreter sets the program counter 
 * to the address at the top of the stack,
 * then subtracts 1 from the stack pointer.
 * 
 * @param c8 
 */
void _f0(struct chip8* c8)
{
    if ((c8 -> opcode & 0xF)== 0)  // if 00E0 CLS
    {
        
    } else {
        c8 -> progCounter = peep(&c8 -> stack); // 00EE RET
        pop(&c8 -> stack);
    } // else
} // ret

/**
 * @brief JP addr
 * Jump to location nnn.
 * The interpreter sets the program counter to nnn.
 * 
 * @param c8 
 */
void _f1(struct chip8* c8)
{
    c8 -> progCounter = c8 -> opcode & 0xFFF; // 1NNN JP 
} // jmp

/**
 * @brief CALL addr
 * Call subroutine at nnn.
 * The interpreter increments the stack pointer, then puts the current PC on the top of the stack. The PC is then set to nnn.
 * 
 * @param c8 
 */
void _f2(struct chip8* c8) 
{
    int addr = c8 -> opcode & 0xFFF;
    push(&c8 -> stack, c8 -> progCounter); 
    c8 -> progCounter = addr;
} // Call

/**
 * @brief SE Vx, byte 
 * Skip next instruction if Vx = kk.
 * The interpreter compares register Vx to kk, 
 * and if they are equal, increments the program counter by 2.

 * @param c8 
 */
void _f3(struct chip8* c8) 
{
    int index = (c8 -> opcode >> 8) & 0xF;
    int value = c8 -> opcode & 0xFF;
    if (value == c8 -> varReg[index]) {
        c8 -> progCounter +=2;
    } // if
} // skip

/**
 * @brief  4xkk - SNE Vx, byte
 * Skip next instruction if Vx != kk.
 * The interpreter compares register Vx to kk, 
 * and if they are not equal, increments the program counter by 2. 
 * 
 * @param c8 
 */
void _f4(struct chip8* c8)
{
    int index = (c8 -> opcode >> 8) & 0xF;
    int value = c8 -> opcode & 0xFF;
    if (!(c8->varReg[index] == value)) {
        c8 ->progCounter += 2;
    } // if
} // skip

/**
 * @brief 5xy0 - SE Vx, Vy
 * Skip next instruction if Vx = Vy.
 * The interpreter compares register Vx to register Vy, 
 * and if they are equal, increments the program counter by 2.
 * 
 * @param c8 
 */
void _f5(struct chip8* c8)
{
    int x = (c8 -> opcode >> 8) & 0xF;
    int y = (c8 -> opcode >> 4) & 0xF;
    if (c8 -> varReg[x] == c8 -> varReg[y]) {
        c8 -> progCounter += 2;
    } // if 
} // _f5

/**
 * @brief LD Vx, byte
 * Set Vx = kk.
 * The interpreter puts the value kk into register Vx.
 * 
 * @param c8 
 */
void _f6(struct chip8* c8)
{
    // 3xkk - SE Vx, Byte
    int index = (c8 -> opcode >> 8) & 0xF;
    int value = c8 -> opcode & 0xFF;
    c8 -> varReg[index] = value;
} // set

/**
 * @brief ADD Vx, byte
 * Set Vx = Vx + kk.
 * Adds the value kk to the value of register Vx, 
 * then stores the result in Vx. 
 * 
 * @param c8 
 */
void _f7(struct chip8* c8) 
{
    int index = (c8 -> opcode >> 8) & 0xF;
    int value = c8 -> opcode & 0xFF;
    c8 -> varReg[index] += value;
} // add

/**
 * @brief  8xy? - ? Vx, Vy
 * 
 * @param c8 
 */
void _f8(struct chip8 *c8)
{
    int x = (c8 -> opcode >> 8) & 0xF;
    int y = (c8 -> opcode >> 4) & 0xF;
    int option = c8 -> opcode & 0xF;
    switch (option)
    {
        /**
         * @brief 8xy0 - LD Vx, Vy
         * Set Vx = Vy.
         * Stores the value of register Vy in register Vx. 
         */
        case 0x0:
            c8 -> varReg[x] = c8 -> varReg[y];
            break;

        /**
         * @brief xy1 - OR Vx, Vy
         * Set Vx = Vx OR Vy.
         * Performs a bitwise OR on the values of Vx and Vy,
         * then stores the result in Vx. 
         */
        case 0x1:
            c8 -> varReg[x] |= c8 -> varReg[y];
            break;
            
        /**
         * @brief 8xy2 - AND Vx, Vy
         * Set Vx = Vx AND Vy.
         * Performs a bitwise AND on the values of Vx and Vy,
         * then stores the result in Vx.
         */
        case 0x2:
            c8 -> varReg[x] &= c8 -> varReg[y];
            break;

        /**
         * @brief 
         * Set Vx = Vx XOR Vy.
         * Performs a bitwise exclusive OR on the values of Vx and Vy,
         * then stores the result in Vx.
         * 
         */
        case 0x3:
            c8 -> varReg[x] ^= c8 -> varReg[y];
            break;
        
        /**
         * @brief 8xy4 - ADD Vx, Vy
         * Set Vx = Vx + Vy, set VF = carry.
         * The values of Vx and Vy are added together.
         * If the result is greater than 8 bits (i.e., > 255,) VF is set to 1, otherwise 0.
         * Only the lowest 8 bits of the result are kept,
         * and stored in Vx.
         */
        case 0x4:
            c8 -> varReg[0xF] = (c8 -> varReg[y] + c8 -> varReg[x]) > 256;
            c8 -> varReg[x] = c8 -> varReg[y] + c8 -> varReg[x];
            break;

        /**
         * @brief 8xy5 - SUB Vx, Vy
         * Set Vx = Vx - Vy, set VF = NOT borrow.
         * If Vx > Vy, then VF is set to 1, otherwise 0. 
         * Then Vy is subtracted from Vx, and the results stored in Vx.
         */
        case 0x5:
            c8 -> varReg[0xF] = c8 -> varReg[x] > c8 -> varReg[y] ? 1 : 0;
            c8 -> varReg[x] = c8 -> varReg[x] - c8 -> varReg[y];
            break;
        
        /**
         * @brief  8xy6 - SHR Vx {, Vy}
         * Set Vx = Vx SHR 1.
         * If the least-significant bit of Vx is 1,
         * then VF is set to 1, otherwise 0. Then Vx is divided by 2.
         */
        case 0x6:
            c8 -> varReg[0xF] = (c8 -> varReg[x] & 0xF) == 1 ?  1 : 0;
            if (c8 -> varReg[0xF]) {
                c8 -> varReg[x] = c8 -> varReg[x] >> 1;
            } // if
            break;

        /**
         * @brief 8xy7 - SUBN Vx, Vy
         * Set Vx = Vy - Vx, set VF = NOT borrow.
         * If Vy > Vx, then VF is set to 1, otherwise 0.
         * Then Vx is subtracted from Vy, and the results stored in Vx. 
         */
        case 0x7:
            c8 -> varReg[0xF] = c8 -> varReg[x] > c8 -> varReg[y] ? 1 : 0;
            c8 -> varReg[x] = c8 -> varReg[y] - c8 -> varReg[x];
            break;
        
        /**
         * @brief 8xyE - SHL Vx {, Vy}
         * Set Vx = Vx SHL 1.
         * If the most-significant bit of Vx is 1, 
         * then VF is set to 1, otherwise to 0. 
         * Then Vx is multiplied by 2.
         */
        case 0xE:
            c8 -> varReg[0xF] = ((c8 -> varReg[x] >> 4) & 0xF) == 1 ?  1 : 0;
            if (c8 -> varReg[0xF]) {
                c8 -> varReg[x] = c8 -> varReg[x] << 1;
            } // if
            break;

        default:
            break;
    }
} // ? 

/**
 * @brief 9xy0 - SNE Vx, Vy
 * Skip next instruction if Vx != Vy.
 * The values of Vx and Vy are compared, 
 * and if they are not equal,
 * the program counter is increased by 2.
 * 
 * @param c8 
 */
void _f9(struct chip8* c8)
{
    int x = (c8 -> opcode >> 8) & 0xF;
    int y = (c8 -> opcode >> 4) & 0xF;
    if (c8 -> varReg[x] != c8 -> varReg[y]) {
        c8 -> progCounter +=2;
    } // if
} // SNE

/**
 * @brief LD I, addr * The value of register I is set to nnn * 
 * @param c8 
 */
void _fA(struct chip8* c8)
{
    int value = c8 -> opcode & 0xFFF;
    c8 -> regI = value;
} // setRi

/**
 * @brief Bnnn - JP V0, addr
 * Jump to location nnn + V0.
 * The program counter is set to nnn plus the value of V0. 
 * @param c8 
 */
void _fB(struct chip8* c8)
{
    int addr = c8 -> opcode & 0xFFF;
    c8 -> progCounter = addr + c8 -> varReg[0];
} // Branch

/**
 * @brief Cxkk - RND Vx, byte
 * Set Vx = random byte AND kk.
 * The interpreter generates a random number from 0 to 255
 * which is then ANDed with the value kk. The results are stored in Vx.
 * See instruction 8xy2 for more information on AND.
 */
void _fC(struct chip8* c8) 
{
    int index = (c8 -> opcode >> 8) & 0xF;
    int val = c8 -> opcode & 0xFF;
    srand(time(NULL));
    int random = rand() % 255;
    c8 -> varReg[index] = random & val;
} // RND

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
