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

void pSprite(struct chip8 *c8) 
{
} // pSprite

void _fD(struct chip8 *c8) 
{
    int height = c8-> opcode& 0x000F;
    uint8_t sprite[height];
    int x = (c8->opcode & 0x0F00) >> 8;
    int y = (c8->opcode & 0x00F0) >> 4;

    c8->varReg[0x0F] = 0; // Reset VF (carry) flag
    memcpy(sprite, &(c8->memory[c8->regI]), height);
    
    /* Print sprite data */

    //not getting proper sprite data
    // should be 8 x 15 need to change memcpy
    //printf("Register I:%X\n", c8 -> regI);

    for(int i = 0; i < height; i ++) {
        //printf("MD:%X\n", c8 -> memory[c8 -> regI +i]); 
    } // for


    printf("Sprite Data\nHeight: %d, X: %d, Y: %d\n", height, x, y);
    for (int row = 0; row < height; row ++) 
    {
     //       printf("%.2X\n", sprite[row]); // nl for spriteDump
            int sBit = (sprite[row]>> row) & 0xF;
            //printf("%d",sBit);
            //((sprite[row] >> row) & 0xF) ? printf("█ ") : printf(" ");
        //printf("\n"); // nl for sprite dump
    } // for
    getchar(); // pause on sprite dump

    /*
    for (int row = 0; row < height; row++)
    {
        for (int col = 0; col < 8; col++) 
        {
            int displayX = (x + col) % 64;
            int displayY = (y + row) % 32;

            if ((c8->display[displayX][displayY] ^ sprite[row][col]) == 0)
                c8->varReg[0x0F] = 1; // Set VF (carry) flag if collision occurs

            c8->display[displayX][displayY] ^= sprite[row][col];
        } // for
    } // for
    for(int i = 0; i < 64; i ++) 
    {
        for (int j = 0; j < 32; j ++)
        {
            printf("%c", c8 -> display[i][j]);
        } // for
        printf("\n");
    } // for
*/
} // draw
