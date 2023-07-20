#include <unistd.h>
#include <stdio.h>
#include "chip8.h"
#include "isa.h"
#include "stack.h"

struct chip8 c8;
void cpuNull(struct chip8* c8)
{

}
void (*cpuTable[16])(struct chip8* c8) = 
{
    _f0, _f1, cpuNull, cpuNull,
    cpuNull, cpuNull, _f6, _f7,
    cpuNull, cpuNull, _fA, cpuNull,
    cpuNull, _fD, cpuNull, cpuNull
};

void fetch();
void execute();
void draw();

int main(int argc, char* argv[]) 
{
    int option;

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
        //test(c8);
        //cpuTable[0](&c8);
        while (c8.progCounter < 0xFFF)
        {
            fetch();
            execute();
            //draw();
        } // while
    } // while
    destroyStack(stack);
} // main

void fetch()
{

    printf("%.4X\n", c8.progCounter);
    c8.opcode = (c8.memory[c8.progCounter] << 8) + c8.memory[c8.progCounter + 1] ;
    printf("op code: 0x%.4X\n", c8.opcode);
    hexdump(c8.memory + c8.progCounter, 50);
    c8.progCounter += 2;
} // fetch

void execute()
{
    //printf("val: %X", (c8.opcode &0xF000) >> 12);
    getchar();
    cpuTable[(c8.opcode &0xF000) >> 12](&c8);
} // 

void draw() 
{
    for(int i = 0; i < 64; i ++) 
    {
        for (int j = 0; j < 32; j ++)
        {
            printf("%c", c8.display[i][j]);
        } // for
        printf("\n");
    } // for
} // draw
