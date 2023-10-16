#include <ncurses.h>
#include <string.h>
#include "chip8.h"

#ifndef DEBUGGER_H 
#define DEBUGGER_H 
extern WINDOW *pCounter;
extern WINDOW *Registers;
extern WINDOW *hexWin;

WINDOW *create_newwin(int height, int width, int starty, int startx);
void destroy_win(WINDOW *local_win);
void debInit();
void debPrint(struct chip8* c8);
void quitDeb();
void hexdump(const void* data, size_t size);
#endif