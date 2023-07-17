#include "chip8.h"

#ifndef ISA_H 
#define ISA_H 

void cls(); 
void ret(struct chip8* c8);
void jmp(struct chip8* c8);
void setVx(struct chip8* c8);
void add(struct chip8* c8);
void setRi(struct chip8* c8);
void display(struct chip8 *c8); 
#endif /*ISA_H*/
