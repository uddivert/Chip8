#include <unistd.h>
#include <stdio.h>
#include "chip8.h"
#include "isa.h"
#include "stack.h"

void cpuNull(struct chip8* c8)
{
}
void fetch()
{
} // fetch

void execute()
{
} // execute`

int main(int argc, char* argv[]) 
{
    int option;
    struct chip8 c8;

    // Set up stack
    uint8_t sSize = 64;
    struct Stack* stack = initStack(sSize); // 64 BYTE SIZE
    c8.stack = *stack;

    void (*opCode[16])(struct chip8* c8) = 
    {
        _f0, _f1, cpuNull, cpuNull,
        cpuNull, cpuNull, _f6, _f7,
        cpuNull, cpuNull, _fA, cpuNull,
        cpuNull, _fD, cpuNull, cpuNull
    };

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
        //test(c8);
        //opCode[0](&c8);
    } // while
    destroyStack(stack);
} // main
