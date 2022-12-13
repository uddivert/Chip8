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

int load_Memory(char* fileName)
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
    
    //memcpy(memory,fonts,sizeof(fonts)); // load fonts to first 512 bytes
    memcpy(memory + 0X050,fonts,sizeof(fonts)); // load fonts to $050 - $09f bc apparently thats popular to do
    fread(memory + 0x200,1,st.st_size, fp); // set the rom to memory at $200

    return 1;
   // fread(memory + 0x200, 1, sizeof(memory) - 0x200, fp);
    
} // load_Memory
int main() {
    return 0;
}
