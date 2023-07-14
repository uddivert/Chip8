#include <stdio.h>
#include <stdint.h>
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
    int jLoc = peep(&c8 -> stack) % 1000;
    c8 -> progCounter = jLoc; 
} // jmp

void set(struct chip8* c8)
{
    int instr = peep(&c8 ->stack);
    c8 -> varReg[instr / 100 % 10] = instr / 10 % 10 * 10 + instr % 10;
} // set
