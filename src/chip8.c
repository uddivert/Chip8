#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include "chip8.h"

/*
 * Memory Map
 *
 * RAM = 4096 Bytes
 * Index register = 12 bits
 * program Counter = 12 bits
 */
uint16_t fonts[80] = {
    0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
    0x20, 0x60, 0x20, 0x20, 0x70, // 1
    0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
    0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
    0x90, 0x90, 0xF0, 0x10, 0x10, // 4
    0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
    0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
    0xF0, 0x10, 0x20, 0x40, 0x40, // 7
    0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
    0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
    0xF0, 0x90, 0xF0, 0x90, 0x90, // A
    0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
    0xF0, 0x80, 0x80, 0x80, 0xF0, // C
    0xE0, 0x90, 0x90, 0x90, 0xE0, // D
    0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
    0xF0, 0x80, 0xF0, 0x80, 0x80  // F
}; // fonts

int load_Memory(struct chip8 c8, char* fileName)
{
    FILE *fp; 
    fp = fopen(fileName, "rb"); // read binary
    if(fp == NULL) {
        perror("File not found");
        return errno;
    } // if

    /* ONLY WORKS ON POSIX COMPILIANT SYSTEMS */
    struct stat st;
    stat(fileName, &st); // to get size of file
    
    memcpy(c8.memory + 0x050, fonts, sizeof(fonts)); // load fonts to $050 - $09f bc apparently thats popular to do
    //fread(memory + 0x200, 1, st.st_size, fp); // set the rom to memory at $200
    memcpy(c8.memory + 0x200, fp, st.st_size);
    return 1;
   // fread(memory + 0x200, 1, sizeof(memory) - 0x200, fp);
} // load_Memory

int load_keys(char* fileName) 
{
    strcat(fileName, "../conf");
    FILE *fp;
    fp = fopen(fileName, "r");  

    if(fp == NULL) {
        perror("File not found");
        return errno;
    } // if
    return 0;
} // load_keys
