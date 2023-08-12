#include <ncurses.h>
#include "debugger.h"

int main1(int argc, char *argv[])
{
    int ch;
    debInit();

    while ((ch = getch()) != KEY_F(1))
    {
        for (int i = 0; i < 255; i++)
        {
        //    debPrint();
            getch();
        }

    }
    quitDeb();
    return 0;
}
