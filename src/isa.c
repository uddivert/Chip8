#include <stdio.h>
#include <stdint.h>
#include "isa.h"
#include "chip8.h"

void cls(struct chip8 c8)
{

} // cls

/* The interpreter sets the program counter to the address at the top of the stack, then subtracts 1 from the stack pointer.*/
void ret(struct chip8 c8) {
} // ret
