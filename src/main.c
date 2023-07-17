#include <unistd.h>
#include <stdio.h>
#include "chip8.h"
#include "isa.h"
#include "stack.h"

void test();

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
        //c8.varReg[0] = 0xFF;
        //printf("varReg: %d, varRegDump: %d",c8.varReg[0], varRegDump(&c8, 0));
        test(c8);
    } // while
} // main

void test(struct chip8 c8) 
{
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
        /* Test SETVX */
        c8.instruction = 0x6010;
        setVx(&c8);
        printf("setVX: %s\n", varRegDump(&c8, 0) == 0x10 ? "Success" : "Failure");
        /* Test ADD */
        c8.instruction = 0x7111;
        add(&c8);
        printf("add %s\n",varRegDump(&c8, 1) == 0x11 ? "Success" : "Failure");
        /* Test SETRI */
        c8.instruction = 0xA010;
        setRi(&c8);
        printf("SetRi %s\n",c8.regI == 0x010 ? "Success" : "Failure");
        /* Test DISP */
        c8.instruction = 0xDF3F;
        display(&c8);

} // debug
