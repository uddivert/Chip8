#include <curses.h>
#include "gui.h"
#include "chip8.h"

int screenHeight;
int screenWidth;
WINDOW* debugMenu;

void ncurseInit()
{
    initscr();			/* Start curses mode 		  */
    debugMenu = newwin(screenHeight, 21, 0, 0);
    getmaxyx(stdscr, screenHeight, screenWidth);
    box(debugMenu, 0, 0);
    wrefresh(debugMenu); // Refresh the debugMenu window
} // guiInit

void destroyGui() 
{
    endwin();			/* End curses mode		  */
} // destroyGui

void display(struct chip8* c8)
{
    mvwprintw(debugMenu,c8 -> progCounter % screenHeight, 1, "%.3X", c8 -> progCounter);
    mvwprintw(debugMenu,c8 -> progCounter % screenHeight, 6, "%.2X", c8 -> opcode);
    box(debugMenu, 0, 0);
    wrefresh(debugMenu); // Refresh the debugMenu window
} // display
