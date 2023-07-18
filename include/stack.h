#include <stdint.h>

#ifndef STACK_H
#define STACK_H
struct Stack {
    int top;
    uint8_t size; 
    int* array;

}; // Stack

struct Stack* initStack(uint8_t size);
void destroyStack(struct Stack* stack);
int isFull(struct Stack* stack); 
int isEmpty(struct Stack* stack);
int push(struct Stack* stack, int item);
int pop(struct Stack* stack);
int peep(struct Stack* stack); 
#endif /* STACK_H */
