#include <stdint.h>
#include "stack.h"

#ifndef CHIP8_H
#define CHIP8_H
struct chip8 {
    uint8_t memory[4096]; 
    uint8_t varReg[16];
    uint8_t delayTimer;
    uint8_t soundTimer;
    int16_t progCounter;
    int *display[32];
    struct Stack stack;
}; // chip8

extern uint16_t fonts[80]; 
int load_Memory(struct chip8* c8, char* fileName);
int load_keys(char *fileName);
void hexdump(const void* data, size_t size);

/*
void chipStack(uint16_t addr);
void soundTimer();
void dispTimer();
*/
#endif /* CHIP8_H */
