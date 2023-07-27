#include <curses.h>
#include "gui.h"
#include "chip8.h"

int screenHeight;
int screenWidth;
WINDOW* displayWindow;
WINDOW* window1;
WINDOW* window2;
WINDOW* window3;

void ncurseInit()
{
    initscr(); // Start curses mode
    getmaxyx(stdscr, screenHeight, screenWidth);

    // Create the display window (64x32)
    displayWindow = newwin(32, 64, 0, 0);
    box(displayWindow, 0, 0);

    // Calculate the total width available for the three windows
    int totalWidthForThreeWindows = screenWidth - 64 - 3;

    // Determine the width of each of the three windows
    int windowWidth = totalWidthForThreeWindows / 3;

    // Create three additional windows (equally spaced)
    window1 = newwin(screenHeight, windowWidth, 0, 64);
    window2 = newwin(screenHeight, windowWidth, 0, 64 + windowWidth + 1);
    window3 = newwin(screenHeight, windowWidth, 0, 64 + 2 * (windowWidth + 1));

    // Box all four windows
    box(displayWindow, 0, 0);
    box(window1, 0, 0);
    box(window2, 0, 0);
    box(window3, 0, 0);

    // Refresh all windows
    wrefresh(displayWindow);
    wrefresh(window1);
    wrefresh(window2);
    wrefresh(window3);
}

void destroyGui() 
{
    endwin(); // End curses mode
}

void display(struct chip8* c8)
{
    static int currentLine = 1; // Initialize the current line for window1

    // Update content in the displayWindow (64x32)
    // ...

    // Box and refresh the displayWindow
    box(displayWindow, 0, 0);
    wrefresh(displayWindow);

    // Update content in window1
    mvwprintw(window1, 1, 1, "Program Counter");
    mvwprintw(window1, currentLine, 1, "%.2X", c8->progCounter);
    currentLine++; // Increment the current line for window1

    // If the current line exceeds the window height, reset it to 1
    if (currentLine > screenHeight - 2)
        currentLine = 2;

    // Box and refresh window1
    box(window1, 0, 0);
    wrefresh(window1);

    // Update content in window2
    mvwprintw(window2, 1, 1, "op code");
    mvwprintw(window2, currentLine, 1, "0x%.4X", c8->opcode);

    // Box and refresh window2
    box(window2, 0, 0);
    wrefresh(window2);

    // Update content in window3
    mvwprintw(window3, 1, 1, "Registers");
    for (int i = 0; i < 16; i ++) 
    {
        mvwprintw(window3, i + 1 , 1, "Register %i: %X",i, varRegDump(c8, i));
    } 
    mvwprintw(window3, 17 , 1, "Register I: %X", c8 -> regI);

    // Box and refresh window3
    box(window3, 0, 0);
    wrefresh(window3);
}
