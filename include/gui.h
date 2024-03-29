#include <GL/freeglut.h>
#ifndef GUI_H
#define GUI_H
extern _Atomic int guiFlag;
extern int keyPress[16];
extern unsigned char keys[16];
void guiInit(int argc, char** argv);
void init(void);
void display(void);
void keyBoard(unsigned char key, int x, int y); 
void keyBoardUp(unsigned char key, int x, int y);
void screenFill(uint8_t grid[][64]);
#endif