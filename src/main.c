#include <unistd.h>
#include <getopt.h>
#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <stdio.h>
#include <signal.h>
#include "chip8.h"
#include "isa.h"
#include "stack.h"
#include "debugger.h"
#include "gui.h"

struct chip8 c8;
struct Stack* stack;
pthread_t guithread_id;
void (*cpuTable[16])(struct chip8* c8) = 
{
    _f0, _f1, cpuNull, cpuNull,
    cpuNull, cpuNull, _f6, _f7,
    cpuNull, cpuNull, _fA, cpuNull,
    cpuNull, _fD, cpuNull, cpuNull
};

void fetch();
void execute();
void *guiThread(void *vargp);
void sigint_handler(int signum);

int main(int argc, char* argv[]) 
{
    // Register the SIGINT handler
    signal(SIGINT, sigint_handler);

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

        /* args manipulation to get args into array*/
        void *args[argc + 1];
        args[0] = &argc;
        for (int i = 1; i <= argc; i++) {
            args[i] = argv[i - 1];
        }

        // make gui thread
        pthread_create(&guithread_id, NULL, guiThread, args); 

        while (c8.progCounter)
        {
            fetch();
            execute();
            debPrint(&c8);
        } // while
    } // while
    pthread_join(guithread_id, NULL);
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

/**
 * @brief initializes guiInit and the arguments going into it
 * 
 * @param vargp void pointer. Actually holds array of args
 * @return void* 
 */
void *guiThread(void *vargp) {
    int argc = *((int*)vargp);
    char** argv = (char**)vargp + sizeof(int);

    guiInit(argc, argv); // Call your GUI initialization function with argc and argv
    return NULL;
}

void sigint_handler(int signum) {
    printf("Control+C pressed. Performing cleanup and exiting...\n");
    pthread_join(guithread_id, NULL);
    destroyStack(stack);
    quitDeb();
    exit(EXIT_FAILURE);
}