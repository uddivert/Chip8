#include <stdlib.h>
#include "stack.h"

/**
 * @brief Array based stack data structure
 * 
 * @param size 
 * @return struct Stack* 
 */
struct Stack* initStack(uint8_t size)
{
    struct Stack* stack = malloc(sizeof(struct Stack));
    stack -> size = size;
    stack -> array = malloc(sizeof(int) * size);
    stack -> top = -1;
    return stack;
} // initStack

void destroyStack(struct Stack* stack) 
{
    free(stack-> array);
    free(stack);
} // destroyStack

int isFull(struct Stack* stack)
{
    return stack -> top == stack -> size -1;
} // isFull


int isEmpty(struct Stack* stack)
{
    return stack -> top == -1;
} // isEmpty 

int push(struct Stack* stack, int item) 
{
    if(isFull(stack))
        return -1;
    stack -> array[++stack -> top] = item;
    return 0;
} // push

int pop(struct Stack* stack)
{
    if (isEmpty(stack)) 
        return -1;
    return stack-> array[-- stack -> top];
} // pop

int peep(struct Stack* stack) 
{
    return stack -> array[stack -> top];
} // peep
