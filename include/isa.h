#include "chip8.h"

#ifndef ISA_H 
#define ISA_H 

void cls(); 
void ret(struct chip8* c8);
void jmp (struct chip8* c8);
void set(struct chip8* c8);
#endif /*ISA_H*/
