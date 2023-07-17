#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "isa.h"
#include "chip8.h"

void cls() 
{
    printf("\e[1;1H\e[2J");
} // cls

void ret(struct chip8* c8)
{
   c8 -> progCounter = peep(&c8 -> stack);
   pop(&c8 -> stack);
} // ret

void jmp (struct chip8* c8)
{
    int jLoc = c8 -> instruction % 1000;
    c8 -> progCounter = jLoc; 
} // jmp

void setVx(struct chip8* c8)
{
    //use bit shifts
    int index = (c8 -> instruction >> 8) & 0xF;
    int value = c8 -> instruction & 0xFF;
    c8 -> varReg[index] = value;
} // set

void add (struct chip8* c8) 
{
    //use bit shifts
    int index = (c8 -> instruction >> 8) & 0xF;
    int value = c8 -> instruction & 0xFF;
    c8 -> varReg[index] += value;
    //printf("Instruction: %X, Index: %d, Value: %d\n",c8 -> instruction, index, value);
} // add

void setRi(struct chip8* c8)
{
    int value = c8 -> instruction & 0xFFF;
    c8 -> regI = value;
} // setRi

void display(struct chip8 *c8) 
{
    int height = c8->instruction & 0x000F;
    uint8_t sprite[height][8];
    int x = (c8->instruction & 0x0F00) >> 8;
    int y = (c8->instruction & 0x00F0) >> 4;

    c8->varReg[0x0F] = 0; // Reset VF (carry) flag

    memcpy(sprite, &(c8->memory[c8->regI]), height * 8);

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
} // draw
