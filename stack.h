#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <limits.h>

typedef double elem_t;

struct Stack
{
	int size;
	int capacity;
	elem_t* data;
};

enum StackErrors
{
	STK_IS_NULL_PTR    = 1,
	DATA_IS_NULL_PTR   = 2,
	STK_DESTROYED      = 4,
	STK_OVERFL         = 8,
	STK_UNDERFL        = 16,
	STK_DOUBLE_CTED    = 32,
    STRCT_CANARY_BAD   = 64,
    DATA_CANARY_BAD    = 128,
    HASH_BAD           = 256,
	CAPACITY_LARG_SIZE = 512,
};

enum ResizeTypes
{
	RESIZESMALLER = 0,
	RESIZELARGER = 1
};

#define FLOAT_DATA

#ifdef FLOAT_DATA
typedef double data_t;
#endif

#ifdef INT_DATA
typedef int data_t;
#endif

int StackCtor(Stack* stk);
int StackDtor(Stack* stk);
int StackResize(Stack* stk, bool mode);
elem_t StackPush(Stack* stk, elem_t value);
elem_t StackPop(Stack* stk);
int StackCheck(Stack* stk);
void StackDump (Stack* stack, const char* current_file, const char* current_function);
size_t StackHash (Stack* stack);

