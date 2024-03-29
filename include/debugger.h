#include <ncurses.h>
#include <string.h>
#include "chip8.h"

#ifndef DEBUGGER_H 
#define DEBUGGER_H 
extern WINDOW *pCounter;
extern WINDOW *registers;
extern WINDOW *timers;
extern WINDOW *hexWin;
extern WINDOW *banner;
extern WINDOW *extra;

WINDOW *create_newwin(int height, int width, int starty, int startx);
void destroy_win(WINDOW *local_win);
void debInit();
void debPrint(struct chip8* c8);
void quitDeb();
void hexdump(const void* data, size_t size);
void loadBanner(void);
void printExtra(const char* format, ...);
#endif