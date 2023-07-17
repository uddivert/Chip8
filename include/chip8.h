#include <stdint.h>
#include "stack.h"

#ifndef CHIP8_H
#define CHIP8_H
struct chip8 {
    uint8_t memory[4096]; 
    uint8_t varReg[16];
    uint16_t regI;
    uint16_t instruction;
    uint16_t opcode;
    uint8_t delayTimer;
    uint8_t soundTimer;
    int16_t progCounter;
    uint8_t *display[64 * 32];
    struct Stack stack;
}; // chip8

extern uint16_t fonts[80]; 
int load_Memory(struct chip8* c8, char* fileName);
int load_keys(char *fileName);
void hexdump(const void* data, size_t size);
int varRegDump(struct chip8* c8, int i); 

/*
void chipStack(uint16_t addr);
void soundTimer();
void dispTimer();
*/
#endif /* CHIP8_H */
