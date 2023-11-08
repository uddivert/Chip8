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

/**
 * @brief Destroys the stack and deallocate's memory
 * 
 * @param stack 
 */
void destroyStack(struct Stack* stack) 
{
    free(stack-> array);
    free(stack);
} // destroyStack

/**
 * @brief Returns if stack is full
 * 
 * @param stack 
 * @return int bool true or false
 */
int isFull(const struct Stack* stack)
{
    return stack -> top == stack -> size -1;
} // isFull


/**
 * @brief Returns if stack is empty
 * 
 * @param stack 
 * @return int bool true or false
 */
int isEmpty(const struct Stack* stack)
{
    return stack -> top == -1;
} // isEmpty 

/**
 * @brief Push value to stack
 * 
 * @param stack 
 * @param item 
 * @return int error code
 */
int push(struct Stack* stack, int item) 
{
    if(isFull(stack))
        return -1;
    stack -> array[++stack -> top] = item;
    return 0;
} // push

/**
 * @brief  Pop value from stack
 * 
 * @param stack 
 * @return int error code
 */
int pop(struct Stack* stack)
{
    if (isEmpty(stack)) 
        return -1;
    return stack-> array[-- stack -> top];
} // pop

/**
 * @brief Peep top value from stack
 * 
 * @param stack 
 * @return int error code
 */
int peep(struct Stack* stack) 
{
    return stack -> array[stack -> top];
} // peep
