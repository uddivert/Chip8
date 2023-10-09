#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include "chip8.h"
#include "isa.h"
#include "stack.h"
#include "debugger.h"

struct chip8 c8;
void (*cpuTable[16])(struct chip8* c8) = 
{
    _f0, _f1, cpuNull, cpuNull,
    cpuNull, cpuNull, _f6, _f7,
    cpuNull, cpuNull, _fA, cpuNull,
    cpuNull, _fD, cpuNull, cpuNull
};

void fetch();
void execute();

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
                debInit();
                // printMem(&c8);
                break;
            case '?': //used for some unknown options
                printf("%c is an unknown option\n", optopt);
                break;
            case ':':
                printf("Option -%c requires an argument\n", optopt);
            break;
        } // switch  
        while (c8.progCounter)
        {
            fetch();
            execute();
            debPrint(&c8);
        } // while
    } // while
    destroyStack(stack);
    quitDeb();
} // main

/**
 * @brief Instruction fetch stage of pipeline
 * 
 */
void fetch()
{
    c8.opcode = (c8.memory[c8.progCounter] << 8) + c8.memory[c8.progCounter + 1] ;
    c8.progCounter += 2;
} // fetch

/**
 * @brief Instruction execute stage of pipeline
 * TODO: make instruction only clock length long
 * 
 */
void execute()
{
    clock_t start, end;
    double cpu_time_used;
    start = clock();
    #ifdef DEBUG
    getchar();
    #endif
    cpuTable[(c8.opcode &0xF000) >> 12](&c8);
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    if (cpu_time_used < 2000) {
        usleep (1852 - cpu_time_used);
    }
} // 
