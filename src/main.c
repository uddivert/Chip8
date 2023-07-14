#include <unistd.h>
#include <stdio.h>
#include "chip8.h"
#include "isa.h"
#include "stack.h"

int main(int argc, char* argv[]) 
{
    int option;
    struct chip8 c8;
    while((option = getopt(argc, argv, "f:F:")) != -1)
    { 
        switch(option)
        {
            case 'f':
            case 'F':
                load_Memory(&c8, optarg);
               // printMem(&c8);
                break;
            case '?': //used for some unknown options
                printf("%c is an unknown option\n", optopt);
                break;
            case ':':
                printf("Option -%c requires an argument\n", optopt);
            break;
        } // switch
    } // while
} // main
