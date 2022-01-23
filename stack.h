#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <limits.h>

#define FLOAT_DATA

#ifdef FLOAT_DATA
typedef double data_t;
#endif

#ifdef INT_DATA
typedef int data_t;
#endif


typedef size_t canary_t;
typedef size_t hash_t;

struct Stack
{
	canary_t 	canary_left;
	
	int 		size;
	int 		capacity;
	data_t* 	data;
	hash_t  	hash;

	canary_t 	canary_right;
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

//enum ResizeTypes
//{
//	RESIZESMALLER = 0,
//	RESIZELARGER = 1
//};


#define STACK_GENERAL_CHECK(check_function)             \
do                                                      \
{                                                       \
    errors = 0;                                         \
                                                        \
    check_function;                                     \
                                                        \
    StackDump(stk, __FILE__, __FUNCTION__);           \
                                                        \
    if (errors != 0)                                    \
    {                                                   \
        return 1;                                       \
    }                                                   \
} while (0)

#define STACK_RESIZE_ERROR_CHECK()                      \
do                                                      \
{                                                       \
    errors = 0;                                         \
                                                        \
	StackErrorCheck(stk);	                            \
                                                        \
    StackDump(stk, __FILE__, __FUNCTION__);           \
                                                        \
    if (errors != 0)                                    \
    {                                                   \
        return nullptr;	                                \
    }                                                   \
} while (0)

#define STACK_POP_ERROR_CHECK()                         \
do                                                      \
{                                                       \
    errors = 0;                                         \
                                                        \
	StackErrorCheck(stk);				                \
                                                        \
    if (stk->size <= 0)                               \
    {                                                   \
        errors |= STK_UNDERFL;                          \
    }                                                   \
                                                        \
    StackDump(stk, __FILE__, __FUNCTION__);           \
                                                        \
    if (errors != 0)                                    \
    {                                                   \
		return (data_t) 0xBEDABEDA;	                    \
    }                                                   \
} while (0)

int StackCtor(Stack* stk);

int StackDtor(Stack* stk);

data_t* StackResize(Stack* stk);

int StackPush(Stack* stk, data_t value);

data_t StackPop(Stack* stk);

int StackErrorCheck(Stack* stk);

int StackCtorCheck (Stack* stk);

int StackDtorCheck (Stack* stk);

void StackDump (Stack* stk, const char* current_file, const char* current_function);

size_t StackHash (Stack* stk);

int DestroyStack(Stack* stk);

int StackAdd(Stack* stk);

int StackSub(Stack* stk);

int StackMul(Stack* stk);

int StackDiv(Stack* stk);