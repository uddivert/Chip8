#include "chip8.h"

#ifndef ISA_H 
#define ISA_H 

void cpuNull(struct chip8* c8);
void _f0(struct chip8* c8);
void _f1(struct chip8* c8);
void _f2(struct chip8* c8);
void _f3(struct chip8* c8);
void _f4(struct chip8* c8);
void _f5(struct chip8* c8);
void _f6(struct chip8* c8);
void _f7(struct chip8* c8);
void _f8(struct chip8* c8);
void _fA(struct chip8* c8);
void _fD(struct chip8 *c8); 
#endif /*ISA_H*/
