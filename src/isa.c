#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include "isa.h"
#include "chip8.h"
#include "debugger.h"
#include "gui.h"

/**
 * @brief null values for cpuTable
 * 
 * @param c8 
 */
void cpuNull(struct chip8* c8)
{

}

/**
 * @brief CLS 
 * Clear the display.
 * 
 * RET
 * Return from a subroutine.
 * 
 * The interpreter sets the program counter 
 * to the address at the top of the stack,
 * then subtracts 1 from the stack pointer.
 * 
 * @param c8 
 */
void _f0(struct chip8* c8)
{
    int cmd = c8 -> opcode & 0xFFF;  // if 00E0 CLS
    switch (cmd) {
        case 0x0E0:
            // need to implement
            break;
        case 0x0EE:
            c8 -> progCounter = peep(&c8 -> stack); // 00EE RET
            pop(&c8 -> stack);
            break;
    } // switch
} // ret

/**
 * @brief JP addr
 * Jump to location nnn.
 * The interpreter sets the program counter to nnn.
 * 
 * @param c8 
 */
void _f1(struct chip8* c8)
{
    c8 -> progCounter = c8 -> opcode & 0xFFF; // 1NNN JP 
} // jmp

/**
 * @brief CALL addr
 * Call subroutine at nnn.
 * The interpreter increments the stack pointer, then puts the current PC on the top of the stack. The PC is then set to nnn.
 * 
 * @param c8 
 */
void _f2(struct chip8* c8) 
{
    int addr = c8 -> opcode & 0xFFF;
    if (!isFull(&c8 -> stack)) {
        push(&c8 -> stack, c8 -> progCounter); 
        c8 -> progCounter = addr;
    } // if 
} // Call

/**
 * @brief SE Vx, byte 
 * Skip next instruction if Vx = kk.
 * The interpreter compares register Vx to kk, 
 * and if they are equal, increments the program counter by 2.

 * @param c8 
 */
void _f3(struct chip8* c8) 
{
    int index = (c8 -> opcode >> 8) & 0xF;
    int value = c8 -> opcode & 0xFF;
    if (value == c8 -> varReg[index]) {
        c8 -> progCounter +=2;
    } // if
} // skip

/**
 * @brief  4xkk - SNE Vx, byte
 * Skip next instruction if Vx != kk.
 * The interpreter compares register Vx to kk, 
 * and if they are not equal, increments the program counter by 2. 
 * 
 * @param c8 
 */
void _f4(struct chip8* c8)
{
    int index = (c8 -> opcode >> 8) & 0xF;
    int value = c8 -> opcode & 0xFF;
    if (!(c8->varReg[index] == value)) {
        c8 ->progCounter += 2;
    } // if
} // skip

/**
 * @brief 5xy0 - SE Vx, Vy
 * Skip next instruction if Vx = Vy.
 * The interpreter compares register Vx to register Vy, 
 * and if they are equal, increments the program counter by 2.
 * 
 * @param c8 
 */
void _f5(struct chip8* c8)
{
    int x = (c8 -> opcode >> 8) & 0xF;
    int y = (c8 -> opcode >> 4) & 0xF;
    if (c8 -> varReg[x] == c8 -> varReg[y]) {
        c8 -> progCounter += 2;
    } // if 
} // _f5

/**
 * @brief LD Vx, byte
 * Set Vx = kk.
 * The interpreter puts the value kk into register Vx.
 * 
 * @param c8 
 */
void _f6(struct chip8* c8)
{
    // 3xkk - SE Vx, Byte
    int index = (c8 -> opcode >> 8) & 0xF;
    int value = c8 -> opcode & 0xFF;
    c8 -> varReg[index] = value;
} // set

/**
 * @brief ADD Vx, byte
 * Set Vx = Vx + kk.
 * Adds the value kk to the value of register Vx, 
 * then stores the result in Vx. 
 * 
 * @param c8 
 */
void _f7(struct chip8* c8) 
{
    int index = (c8 -> opcode >> 8) & 0xF;
    int value = c8 -> opcode & 0xFF;
    c8 -> varReg[index] += value;
} // add

/**
 * @brief  8xy? - ? Vx, Vy
 * 
 * @param c8 
 */
void _f8(struct chip8 *c8)
{
    int x = (c8 -> opcode >> 8) & 0xF;
    int y = (c8 -> opcode >> 4) & 0xF;
    int option = c8 -> opcode & 0xF;
    switch (option)
    {
        /**
         * @brief 8xy0 - LD Vx, Vy
         * Set Vx = Vy.
         * Stores the value of register Vy in register Vx. 
         */
        case 0x0:
            c8 -> varReg[x] = c8 -> varReg[y];
            break;

        /**
         * @brief xy1 - OR Vx, Vy
         * Set Vx = Vx OR Vy.
         * Performs a bitwise OR on the values of Vx and Vy,
         * then stores the result in Vx. 
         */
        case 0x1:
            c8 -> varReg[x] |= c8 -> varReg[y];
            break;
            
        /**
         * @brief 8xy2 - AND Vx, Vy
         * Set Vx = Vx AND Vy.
         * Performs a bitwise AND on the values of Vx and Vy,
         * then stores the result in Vx.
         */
        case 0x2:
            c8 -> varReg[x] &= c8 -> varReg[y];
            break;

        /**
         * @brief 
         * Set Vx = Vx XOR Vy.
         * Performs a bitwise exclusive OR on the values of Vx and Vy,
         * then stores the result in Vx.
         * 
         */
        case 0x3:
            c8 -> varReg[x] ^= c8 -> varReg[y];
            break;
        
        /**
         * @brief 8xy4 - ADD Vx, Vy
         * Set Vx = Vx + Vy, set VF = carry.
         * The values of Vx and Vy are added together.
         * If the result is greater than 8 bits (i.e., > 255,) VF is set to 1, otherwise 0.
         * Only the lowest 8 bits of the result are kept,
         * and stored in Vx.
         */
        case 0x4:
        {
            int temp = c8 -> varReg[x];
            c8 -> varReg[x] = (c8 -> varReg[y] + c8 -> varReg[x]) & 0xFF;
            c8 -> varReg[0xF] = c8 -> varReg[x] > temp ? 0 : 1;
            break;
        } // case

        /**
         * @brief 8xy5 - SUB Vx, Vy
         * Set Vx = Vx - Vy, set VF = NOT borrow.
         * If Vx > Vy, then VF is set to 1, otherwise 0. 
         * Then Vy is subtracted from Vx, and the results stored in Vx.
         */
        case 0x5: {
            int temp = c8 -> varReg[x];
            c8 -> varReg[x] = c8 -> varReg[x] - c8 -> varReg[y];
            c8 -> varReg[0xF] = c8 -> varReg[x] > temp ? 0 : 1;
            break;
        } // case
        
        /**
         * @brief  8xy6 - SHR Vx {, Vy}
         * Set Vx = Vx SHR 1.
         * If the least-significant bit of Vx is 1,
         * then VF is set to 1, otherwise 0. Then Vx is divided by 2.
         */
        case 0x6: {
            // Right shift Vx by 1
            uint8_t new_vf = c8 -> varReg[(c8 -> opcode & 0x0F0) >> 4] & 0x1;
            c8 -> varReg[(c8 -> opcode & 0x0F00) >> 8] = c8 -> varReg[(c8 -> opcode & 0x0F0) >> 4] >> 1;
            c8 -> varReg[0xf] = new_vf;
            break;
        } // case

        /**
         * @brief 8xy7 - SUBN Vx, Vy
         * Set Vx = Vy - Vx, set VF = NOT borrow.
         * If Vy > Vx, then VF is set to 1, otherwise 0.
         * Then Vx is subtracted from Vy, and the results stored in Vx. 
         */
        case 0x7:
            c8 -> varReg[x] = c8 -> varReg[y] - c8 -> varReg[x];
            c8 -> varReg[0xF] = c8 -> varReg[y] > c8 -> varReg[x] ? 1 : 0;
            break;
        
        /**
         * @brief 8xyE - SHL Vx {, Vy}
         * Set Vx = Vx SHL 1.
         * If the most-significant bit of Vx is 1, 
         * then VF is set to 1, otherwise to 0. 
         * Then Vx is multiplied by 2.
         */
        case 0xE: {
            // Left shift Vx by 1
            uint8_t new_vf = (c8 -> varReg[(c8 -> opcode & 0x0f0)>>4] >> 7) & 0x01;
            c8 -> varReg[(c8 -> opcode & 0x0F00) >> 8] = c8 -> varReg[(c8 -> opcode & 0x0F0) >> 4] << 1;
            c8 -> varReg[0xf] = new_vf;
            break;
        } // case

        default:
            break;
    }
} // ? 

/**
 * @brief 9xy0 - SNE Vx, Vy
 * Skip next instruction if Vx != Vy.
 * The values of Vx and Vy are compared, 
 * and if they are not equal,
 * the program counter is increased by 2.
 * 
 * @param c8 
 */
void _f9(struct chip8* c8)
{
    int x = (c8 -> opcode >> 8) & 0xF;
    int y = (c8 -> opcode >> 4) & 0xF;
    if (c8 -> varReg[x] != c8 -> varReg[y]) {
        c8 -> progCounter +=2;
    } // if
} // SNE

/**
 * @brief LD I, addr * The value of register I is set to nnn * 
 * @param c8 
 */
void _fA(struct chip8* c8)
{
    int value = c8 -> opcode & 0xFFF;
    c8 -> regI = value;
} // setRi

/**
 * @brief Bnnn - JP V0, addr
 * Jump to location nnn + V0.
 * The program counter is set to nnn plus the value of V0. 
 * @param c8 
 */
void _fB(struct chip8* c8)
{
    int addr = c8 -> opcode & 0xFFF;
    c8 -> progCounter = addr + c8 -> varReg[0];
} // Branch

/**
 * @brief Cxkk - RND Vx, byte
 * Set Vx = random byte AND kk.
 * The interpreter generates a random number from 0 to 255
 * which is then ANDed with the value kk. The results are stored in Vx.
 * See instruction 8xy2 for more information on AND.
 */
void _fC(struct chip8* c8) 
{
    int index = (c8 -> opcode >> 8) & 0xF;
    int val = c8 -> opcode & 0xFF;
    srand(time(NULL));
    int random = rand() % 256;
    c8 -> varReg[index] = random & val;
} // RND

/**
 * @brief  DRW Vx, Vy, nibble
 * 
 * @param c8  Chip 8 struct
 */
void _fD(struct chip8 *c8) 
{
    int height = c8-> opcode& 0x000F;
    uint8_t x = (c8->opcode & 0x0F00) >> 8;
    uint8_t y = (c8->opcode & 0x00F0) >> 4;
    x = c8 -> varReg[x] & 63; // xcoord is VX
    y =  c8 -> varReg[y] & 31; // ycoord is VY
    c8-> varReg[0x0F] = 0; // Reset VF (carry) flag
    
    // Loop through each pixel in the row
    for (int row = 0; row < height; row++) {
        if (row + y >= 32) break; // overflow
        uint8_t spriteData = c8->memory[c8->regI + row];
        for (int col = 0; col < 8; col++) {
            if (col + x >= 64) break; // overflow
            uint8_t pixel = (spriteData >> (7 - col)) & 0x1;
            if(!pixel) continue;
            if (c8 -> display[row + y][col + x] ==1) {
                c8 ->varReg[0x0F] = 1;
            }
            c8 -> display[row + y][col + x] ^=1;
        } // for
    } // for
} // draw

/**
 * @brief Ex?? - SK(?N)P Vx
 * 
 * @param c8 
 */
void _fE(struct chip8 *c8) {
    uint8_t index = (c8->opcode & 0x0F00) >> 8;
    switch(c8 -> opcode & 0x00FF) {
        /**
         * @brief Ex9E - SKP Vx
         * Skip next instruction if key with the
         * value of Vx is pressed.
         * Checks the keyboard, and if the key
         * corresponding to the value of Vx is currently
         * in the down position, PC is increased by 2.
         */
        case(0x9E):
            if (keyPress[c8 -> varReg[index]])
               c8 -> progCounter += 2; 
            break;
        /**
         * @brief ExA1 - SKNP Vx
         * Skip next instruction if key with
         * the value of Vx is not pressed.
         * Checks the keyboard, and if the key
         * corresponding to the value of Vx is currently
         * in the up position, PC is increased by 2. 
         */
        case(0xA1):
            if (!keyPress[c8 -> varReg[index]])
               c8 -> progCounter += 2; 
            break;
        default:
            break;
    }
}
/**
 * @brief Fx?? - LD ????
 * 
 * @param c8 
 */
void _fF(struct chip8 *c8) { 
    int index = (c8 -> opcode >> 8) & 0xF;
    printExtra("%x",index);
    switch(c8 -> opcode & 0x00FF) {
        case(0x07):
            /**
             * @brief LD Vx, DT
             * Set Vx = delay timer value.
             * The value of DT is placed into Vx.
             */
            c8 -> varReg[index] = c8 -> delayTimer;
            break;
        case(0x0A):
            /**
             * @brief LD Vx, K
             * Wait for a key press, store the value of the key in Vx.
             * All execution stops until a key is pressed,
             * then the value of that key is stored in Vx.
             */
            for (int i = 0; i < 16; i ++) {
                if (keyPress[i]) {
                    c8 -> varReg[index] = keys[i];
                } // if
                if (i == 16) {
                    i = 0;
                } // infinite loop
            } // for
            break;
        case(0x15):
            /**
             * @brief  LD DT, Vx
             * Set delay timer = Vx.
             * DT is set equal to the value of Vx.
             */
            c8 -> delayTimer = c8 -> varReg[index];
            break;
        case(0x18):
            /**
             * @brief LD ST, Vx
             * Set sound timer = Vx.
             * ST is set equal to the value of Vx.
             */
            c8 -> soundTimer= c8 -> varReg[index];
            break;
        case(0x1E):
            /**
             * @brief ADD I, Vx
             * Set I = I + Vx.
             * The values of I and Vx are added,
             * and the results are stored in I.
             */
            c8 -> regI += c8 -> varReg[index];
            break;
        case(0x29):
            /**
             * @brief LD F, Vx
             * Set I = location of sprite for digit Vx.
             * The value of I is set to the location 
             * for the hexadecimal sprite corresponding to the value of Vx. 
             */
            c8 -> regI = getFontAddr(c8, c8 -> varReg[index]);
            break;
        case(0x33):
            /**
             * @brief LD B, Vx 
             * Store BCD representation of Vx in memory locations I, I+1, and I+2.
             * The interpreter takes the decimal value of Vx, 
             * and places the hundreds digit in memory at location in I, 
             * the tens digit at location I+1, and the ones digit at location I+2.
             */
            {
                int bcd = c8->varReg[index];
                c8->memory[c8->regI] = bcd / 100;        // Hundreds digit
                c8->memory[c8->regI + 1] = (bcd / 10) % 10; // Tens digit
                c8->memory[c8->regI + 2] = bcd % 10;       // Ones digit
                break;
            }
        case(0x55):
            /**
             * @brief  LD [I], Vx
             * Store registers V0 through Vx in memory starting at location I.
             * The interpreter copies the values of registers V0 through Vx into memory,
             * starting at the address in I.
             */
            for (int i = 0; i <= index; i ++) {
               // c8 -> varReg[i] = c8->memory[c8->regI + i];
                c8->memory[c8->regI + i] = c8->varReg[i];
            }
            break;
        case(0x65):
            /**
             * @brief LD Vx, [I]
             * Read registers V0 through Vx from memory starting at location I.
             * The interpreter reads values from memory starting at location I
             * into registers V0 through Vx.
             */
            for (int i = 0; i <= index; i ++) {
                //c8->memory[c8->regI + i] = c8 -> varReg[i];
                c8 -> varReg[i] = c8->memory[c8->regI + i];
            }
            break;
        default:
            break;
    } // switch
} // ff