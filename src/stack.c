#include <stdlib.h>
#include "stack.h"

struct Stack* initStack(uint8_t size)
{
    struct Stack* stack = malloc(sizeof(struct Stack));
    stack -> size = size;
    stack -> array = malloc(sizeof(int) * size);
    stack -> top = -1;
    return stack;
} // initStack

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
        return;
    stack -> array[++stack -> top] = item;
} // push

int pop(struct Stack* stack)
{
    if (isEmpty(stack)) 
        return;
    stack-> array[-- stack -> top];
} // pop

int peep(struct Stack* stack) 
{
    stack -> array[stack -> top];
} // peep

int main()
{
    struct Stack* stack = initStack(100);
    push(stack, 10);
    peep(stack);
    push(stack, 20);
    peep(stack);
    push(stack, 30);
    peep(stack);
    pop(stack);
    peep(stack);
    pop(stack);
    peep(stack);
    pop(stack);
    peep(stack);
} // main
