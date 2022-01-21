#include <stdlib.h>
#include <stdio.h>
#include <string.h>


#include "stack.h"

int StackCtor(Stack* stk, int basic_size)
{
    if (stk == NULL)
    {
        printf("Error %d", EMPTY_PTR);
        return -1;
    }
    memset(stk, 0, sizeof(Stack));
    stk -> data = (elem_t*)calloc(basic_size, sizeof(elem_t));
    stk -> size = 0;
    return 0;

}

int StackDtor(Stack* stk)
{
    if (stk -> capacity == 0)
    {
        printf("Error %d", CAPACITY_NULL);
        return -1;
    }
    memset(stk, 0, stk -> capacity * sizeof(elem_t));
    free(stk);
    return 0;
}

int StackResize(Stack* stk, bool mode)
{
    if (mode == RESIZELARGER)
    {
        stk -> capacity++; 
        stk -> data = (elem_t*)realloc(stk -> data, sizeof(elem_t));
    }
    else if (mode == RESIZESMALLER)
    {
        stk -> capacity--; 
        stk -> data = (elem_t*)realloc(stk -> data, sizeof(elem_t));
    }
    else
    {
        return -1;
    }
    return 0;
}

elem_t StackPush(Stack* stk, elem_t value)
{
    if (StackCheck != -1)
    {
        return -1;
    }
    StackResize(stk, RESIZELARGER);
    stk -> data[stk -> size] = value;
    stk -> size++;
    return 0;
}

elem_t StackPop(Stack* stk)
{   
    stk -> size--;
    elem_t elem_pop = stk -> data[stk -> size];
    StackResize(stk, RESIZESMALLER);
    return elem_pop;
}

int StackCheck(Stack* stk)
{
    if (stk == NULL)
    {
        printf("Error %d", EMPTY_PTR);
        return EMPTY_PTR;
    }
    else if (stk -> capacity > stk -> size)
    {
        printf("Error %d", CAPACITY_LARG_SIZE);
        return CAPACITY_LARG_SIZE;
    }
    else if (stk -> size < 0)
    {
        printf("Error %d", SIZE_IS_NEGATIVE);
        return SIZE_IS_NEGATIVE;
    }
    else
    {
        return -1;
    }
}
