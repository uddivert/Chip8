#include "chip8.h"

#ifndef ISA_H 
#define ISA_H 

void cpuNull(struct chip8* c8);
void _f0(struct chip8* c8);
void _f1(struct chip8* c8);
void _f6(struct chip8* c8);
void _f7(struct chip8* c8);
void _fA(struct chip8* c8);
void _fD(struct chip8 *c8); 
#endif /*ISA_H*/
