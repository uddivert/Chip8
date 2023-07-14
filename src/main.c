#include <unistd.h>
#include <stdio.h>
#include "chip8.h"
#include "isa.h"
#include "stack.h"

int main(int argc, char* argv[]) 
{
    int option;
    struct chip8 c8;

    // Set up stack
    uint8_t sSize = 64;
    struct Stack* stack = initStack(sSize); // 64 BYTE SIZE
    c8.stack = *stack;

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
        
        #ifdef DEBUG
        /* Test CLS */
        printf("cls Failure\n");
        cls();
        printf("cls Success\n");
        /* Test RET */
        c8.progCounter = 0x11;
        int16_t stackVal = peep(&c8.stack);
        ret(&c8);
        printf("ret %s\n", stackVal == c8.progCounter ? "Success": "Failure");
        /* Test JMP */
        printf("jmp %s\n", stackVal == c8.progCounter ? "Success": "Failure");
        jmp(&c8);
        /* Test SET */
        push(&c8.stack, 111);
        stackVal = peep(&c8.stack);
        set(&c8);
        printf("set %s\n",c8.varReg[stackVal / 100 % 10] == stackVal/ 10 % 10 * 10 + stackVal % 10 ? "Success" : "Failure");
        #endif

    } // while
} // main

