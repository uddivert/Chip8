#include <GL/freeglut.h>
#ifndef GUI_H
#define GUI_H
void guiInit(int argc, char** argv);
void init(void);
void display(void);
//void screenFill(void);
void screenFill(uint8_t grid[][64]);
#endif