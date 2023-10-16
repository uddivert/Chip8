#include "debugger.h"
#include "chip8.h"
#include <locale.h>

WINDOW *pCounter;
WINDOW *Registers;
WINDOW *hexWin;
WINDOW *banner;

const char* bannerLines[] = {
	"██╗   ██╗██████╗ ██████╗ ██╗██╗   ██╗███████╗██████╗ ████████╗", 
	"██║   ██║██╔══██╗██╔══██╗██║██║   ██║██╔════╝██╔══██╗╚══██╔══╝", 
	"██║   ██║██║  ██║██║  ██║██║██║   ██║█████╗  ██████╔╝   ██║   ", 
	"██║   ██║██║  ██║██║  ██║██║╚██╗ ██╔╝██╔══╝  ██╔══██╗   ██║   ",
	"╚██████╔╝██████╔╝██████╔╝██║ ╚████╔╝ ███████╗██║  ██║   ██║   ",
	" ╚═════╝ ╚═════╝ ╚═════╝ ╚═╝  ╚═══╝  ╚══════╝╚═╝  ╚═╝   ╚═╝   "
	}; // loadBanner

/**
 * @brief creates debugger and debugger inteface
 * 
 */
void debInit()
{
	setlocale(LC_ALL, "");
	initscr();			/* Start curses mode 		*/
	noecho();			/* No echo */
	cbreak();			/* Line buffering disabled, Pass on
					 * everty thing to me 		*/
	keypad(stdscr, TRUE);		/* I need that nifty F1 	*/

	printw("Press F1 to exit");
	refresh();

	pCounter = create_newwin(3, 10, 1, 0);
	Registers = create_newwin(19, 30, 5, 0);
	hexWin = create_newwin(6, 58, 5, 31);
	banner = create_newwin(9, 64, 11,31);
    wborder(banner, ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ');
} // guiT_init()

/**
 * @brief  Loads the fun banner I made :)
 * 
 */
void loadBanner()
{
	int numLines = sizeof(bannerLines) / sizeof(bannerLines[0]);
    for (int i = 0; i < numLines; i++)
    {
        mvwprintw(banner, i + 2, 1, "%s", bannerLines[i]);
    }
    wrefresh(banner);
} // loadBanner

/**
 * @brief prints entire debugger screen using NCURSES
 * 
 * @param c8 
 */
void debPrint(struct chip8* c8)
{
	mvwprintw(pCounter, 1, 1,"0x%.02X", c8 -> progCounter); // update program counter
	
	for (int i = 0; i < 16; i ++) 
	{
		mvwprintw(Registers, 1 + i, 1, "Register %X: Ox%.02X", i, varRegDump(c8, i));
	} // for

    hexdump(c8 -> memory + c8 -> progCounter, 64); // show hexDump
	loadBanner();

	mvwprintw(Registers, 17, 1, "Register I: Ox%.04X", c8 -> regI);
	wrefresh(pCounter);
	wrefresh(Registers);
	wrefresh(hexWin);
} // debPrint

/**
 * @brief destroyer for windows
 * 
 * @param local_win 
 */
void destroy_win(WINDOW *local_win)
{	
	/* box(local_win, ' ', ' '); : This won't produce the desired
	 * result of erasing the window. It will leave it's four corners 
	 * and so an ugly remnant of window. 
	 */
    wborder(local_win, '|', '|', '-', '-', '+', '+', '+', '+');
	/* The parameters taken are 
	 * 1. win: the window on which to operate
	 * 2. ls: character to be used for the left side of the window 
	 * 3. rs: character to be used for the right side of the window 
	 * 4. ts: character to be used for the top side of the window 
	 * 5. bs: character to be used for the bottom side of the window 
	 * 6. tl: character to be used for the top left corner of the window 
	 * 7. tr: character to be used for the top right corner of the window 
	 * 8. bl: character to be used for the bottom left corner of the window 
	 * 9. br: character to be used for the bottom right corner of the window
	 */
	wrefresh(local_win);
	delwin(local_win);
}

/**
 * @brief Cleanly closes the debugger
 * 
 */
void quitDeb()
{
	destroy_win(pCounter);
	destroy_win(Registers);
    endwin();
} // quitDeb

/**
 * @brief Create a new window
 * 
 * @param height Height of the window
 * @param width Width of the window
 * @param starty y position of where the window needs to be placed
 * @param startx x position of wher the window needs to be placed
 * @return WINDOW* returns pointer to the window
 */
WINDOW *create_newwin(int height, int width, int starty, int startx)
{	WINDOW *local_win;

	local_win = newwin(height, width, starty, startx);
	box(local_win, 0 , 0);		/* 0, 0 gives default characters 
					 * for the vertical and horizontal
					 * lines			*/
	wrefresh(local_win);		/* Show that box 		*/

	return local_win;
}

/**
 * @brief Remade hexdump for my screen
 * 
 * @param data pointer to the data needed to be dumped
 * @param size size of the data
 */
void hexdump(const void* data, size_t size)
{
    const unsigned char* bytes = (const unsigned char*)data;
    size_t i, j;

    for (i = 0; i < size; i += 16)
    {
        mvwprintw(hexWin, i / 16 + 1, 1, "%06zu: ", i); // prints line numbers

        // Print hex values
        for (j = 0; j < 16; j++)
        {
            if (i + j < size)
                mvwprintw(hexWin, i / 16 + 1, 9 + j * 3, "%02X ", bytes[i + j]);
            else
                mvwprintw(hexWin, i / 16 + 1, 9 + j * 3, "   ");

            if (j % 8 == 7)
                mvwprintw(hexWin, i / 16 + 1, 9 + 8 * 3 + j / 8, " "); // Add extra space after 8 bytes
        }

        mvwprintw(hexWin, i / 16 + 1, 9 + 8 * 3 + 2, " ");

        // Print ASCII values
        for (j = 0; j < 16; j++)
        {
            if (i + j < size)
            {
                unsigned char ch = bytes[i + j];
                mvwprintw(hexWin, i / 16 + 1, 9 + 8 * 3 + 3 + j, "%c", (ch >= 32 && ch <= 126) ? ch : '.');
            }
            else
            {
                mvwprintw(hexWin, i / 16 + 1, 9 + 8 * 3 + 3 + j, " ");
            }
        }
    }
}
