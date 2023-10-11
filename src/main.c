#include <unistd.h>
#include <getopt.h>
#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include "chip8.h"
#include "isa.h"
#include "stack.h"
#include "debugger.h"
#include "gui.h"

struct chip8 c8;
struct Stack* stack;
void (*cpuTable[16])(struct chip8* c8) = 
{
    _f0, _f1, cpuNull, cpuNull,
    cpuNull, cpuNull, _f6, _f7,
    cpuNull, cpuNull, _fA, cpuNull,
    cpuNull, _fD, cpuNull, cpuNull
};

void* loop(void* arg);
void fetch();
void execute();

int main(int argc, char* argv[]) 
{
    int option;

    // Set up stack
    uint8_t sSize = 64;
    stack = initStack(sSize); // 64 BYTE SIZE
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
                printf("%c is an unknown option\n", option);
                break;
            case ':':
                printf("Option -%c requires an argument\n", option);
            break;
        } // switch  
    } // while
    pthread_t emuthreadid;
    pthread_create(&emuthreadid, NULL, loop, NULL);

    guiInit(argc, argv);

    destroyStack(stack);
    quitDeb();
} // main

/**
 * @brief main loop of the program.
 * Will be another thread.
 * 
 */
void* loop(void* arg)
{
    while (c8.progCounter)
    {
        fetch();
        execute();
        debPrint(&c8);
        screenFill();
        glutPostRedisplay();
    } // while
    return NULL;
}

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
 * 
 */
void execute()
{
    clock_t start, end;
    double cpu_time_used;
    start = clock();
    //TODO Add mutex
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