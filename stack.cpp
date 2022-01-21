#include "stack.h"

const int START_CAPACITY = 10;

int Errors = 0;

int StackCtor(Stack* stk)
{
    if (stk == NULL)
    {
        printf("Error %d", STK_IS_NULL_PTR);
        return -1;
    }
    memset(stk, 0, sizeof(Stack));
    stk->data = (elem_t*)calloc(START_CAPACITY, sizeof(elem_t));
    stk->size = 0;
    stk->capacity = START_CAPACITY;
    return 0;

}

int StackDtor(Stack* stk)
{
    if (stk->capacity == 0)
    {
        printf("Error %d", STACK_UNDERFL);
        return -1;
    }
    memset(stk, 0, stk->capacity * sizeof(elem_t));
    free(stk);
    return 0;
}

int StackResize(Stack* stk, bool mode)
{
    if (mode == RESIZELARGER)
    {
        stk->capacity++; 
        stk->data = (elem_t*)realloc(stk->data, sizeof(elem_t));
    }
    else if (mode == RESIZESMALLER)
    {
        stk->capacity--; 
        stk->data = (elem_t*)realloc(stk->data, sizeof(elem_t));
    }
    else
    {
        return -1;
    }
    return 0;
}

elem_t StackPush(Stack* stk, elem_t value)
{
    if (StackCheck(stk) != -1)
    {
        return -1;
    }

    StackResize(stk, RESIZELARGER);
    stk->data[stk -> size] = value;
    stk->size++;
    return 0;

    if (StackCheck(stk) != -1)
    {
        return -1;
    }
}

elem_t StackPop(Stack* stk)
{   
    if (StackCheck(stk) != -1)
    {
        return -1;
    }

    stk->size--;
    elem_t elem_pop = stk->data[stk->size];
    StackResize(stk, RESIZESMALLER);
    return elem_pop;

    if (StackCheck(stk) != -1)
    {
        return -1;
    }
}

int StackCheck(Stack* stk)
{
    if (stk == NULL)
    {
///        printf("Error %d", STK_IS_NULL_PTR);
        return STK_IS_NULL_PTR;
    }
    else if (stk->capacity > stk->size)
    {
///        printf("Error %d", CAPACITY_LARG_SIZE);
        return CAPACITY_LARG_SIZE;
    }
    else if (stk->size < 0)
    {
///        printf("Error %d", SIZE_IS_NEGATIVE);
        return SIZE_IS_NEGATIVE;
    }
    else
    {
        return -1;
    }
}


void StackDump (Stack* stack, const char* current_file, const char* current_function)
{
	FILE* dump_file = fopen("Dump.txt", "a"); //TODO передавать pid в имя файла

    if (errors != 0)
    {
        fprintf(dump_file, "\n______________________________\n");
    }

	if ((errors & STK_IS_NULL_PTR) == STK_IS_NULL_PTR)
	{
		fprintf(dump_file, "\n-In file: %s; In function: %s\n Stack is nullptr: %d\n",
			current_file, current_function, errors & STK_IS_NULL_PTR);
	}

	if ((errors & DATA_IS_NULL_PTR) == DATA_IS_NULL_PTR)
	{
		fprintf(dump_file, "\n-In file: %s; In function: %s\n Stack data is nullptr: %d\n",
			current_file, current_function, errors & DATA_IS_NULL_PTR);
	}

	if ((errors & STK_DESTROYED) == STK_DESTROYED)
	{
		fprintf(dump_file, "\n-In file: %s; In function: %s\n Stack destroyed: %d\n",
			current_file, current_function, errors & STK_DESTROYED);
	}

	if ((errors & STK_UNDERFL) == STK_UNDERFL)
	{
		fprintf(dump_file, "\n-In file: %s; In function: %s\n Stack underflowed: %d\n",
			current_file, current_function, errors & STK_UNDERFL);
	}

	if ((errors & STK_OVERFL) == STK_OVERFL)
	{
		fprintf(dump_file, "\n-In file: %s; In function: %s\n Stack overflowed: %d\n",
			current_file, current_function, errors & STK_OVERFL);
	}

	if ((errors & STK_DOUBLE_CTED) == STK_DOUBLE_CTED)
	{
		fprintf(dump_file, "\n-In file: %s; In function: %s\n Stack already constructed: %d\n",
			current_file, current_function, errors & STK_DOUBLE_CTED);
	}

    if ((errors & STRCT_CANARY_BAD) == STRCT_CANARY_BAD)
    {
        fprintf(dump_file, "\n-In file: %s; In function: %s\n Canary of Structure \"Stack\" has been changed: "
                           "Error code: %d\n", current_file, current_function, errors & STRCT_CANARY_BAD);
    }

    if ((errors & DATA_CANARY_BAD) == DATA_CANARY_BAD)
    {
        fprintf(dump_file, "\n-In file: %s; In function: %s\n Data canary has been changed illegally : Error code: %d\n",
                current_file, current_function, errors & DATA_CANARY_BAD);
    }

    if ((errors & HASH_BAD) == HASH_BAD)
    {
        fprintf(dump_file, "\n-In file: %s; In function: %s\n Stack hash has been changed illegally. Error code: %d\n",
                current_file, current_function, errors & HASH_BAD);
    }

    if ((errors & CAPACITY_LARG_SIZE) == CAPACITY_LARG_SIZE)
    {
        fprintf(dump_file, "\n-In file: %s; In function: %s\n Stack's capacity is larger than Stack's size. Error code: %d\n",
                current_file, current_function, errors & CAPACITY_LARG_SIZE);
    }

    //TODO универсальная печать стека
    if (errors != 0)
    {
        fprintf(dump_file, "\n Stack: \n");
        for (int i = 0; i < stack->capacity; i++)
        {
#ifdef FLOAT_DATA
            if ( i == stack->size)
                fprintf(dump_file, "\n Other data: \n");

            fprintf(dump_file, "[%d]: %f\n", i, stack->data[i]);
#endif
#ifdef INT_DATA
            if ( i == stack->size)
                fprintf(dump_file, "\n Other data: \n");

            fprintf(dump_file, "[%d]: %d\n", i, stack->data[i]);
#endif
        }
    }

	fclose(dump_file);
}

size_t StackHash (Stack* stack)
{
    if (stack->data == nullptr || stack->data < (data_t*)4000)
    {
        return 0;
    }

    size_t hash = 2139062143;

    for (int i = 0; i < stack->capacity; i++)
    {
        hash = 37 * hash + (size_t)stack->data[i];
    }

    hash = 37 * hash + (size_t)stack->capacity;
    hash = 37 * hash + (size_t)stack->data;
    hash = 37 * hash + (size_t)stack->size;

    return hash;
}