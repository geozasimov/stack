#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <limits.h>

//------------------------------------------------
//! Solves a square equation ax2 + bx + c = 0
//!
//! @param [in] a a-coefficient
//! @param [in] b b-coefficient
//! @param [in] c c-coefficient
//! @param [out] x1 Pointer to the 1st root
//! @param [out] x2 Pointer to the 2nd root
//!
//! @return Number of roots
//!
//! @note In case of infinite number of roots,
//! returns @ref NUMBER_INF_ROOTS.
//! Function changes x1 and x2, if Number of roots is not zero.
//! The variable x1 contains a root less than x2.
//------------------------------------------------

//------------------------------------------------
//! Define which specifies what type all elements on the stack are (in this case, the type is double).
//!
//!
//------------------------------------------------

#define FLOAT_DATA

//------------------------------------------------
//! Typedef which specifies that the type of all elements on the stack is float. 
//!
//!
//------------------------------------------------

#ifdef FLOAT_DATA
typedef double data_t;
#endif

//------------------------------------------------
//! Typedef which specifies that the type of all elements on the stack is int. 
//!
//!
//------------------------------------------------

#ifdef INT_DATA
typedef int data_t;
#endif

//------------------------------------------------
//! Typedef which indicates what type of canary is.
//!
//!
//------------------------------------------------

typedef size_t canary_t;

//------------------------------------------------
//! Typedef which indicates what type of hash is.
//!
//!
//------------------------------------------------

typedef size_t hash_t;

//------------------------------------------------
//! Structure that defines the stack: size, capacity, array pointer, hash, canary left and right for protection.
//!
//! @param [in] canary_left canary_left-left canary for protection
//! @param [in] size size-stack size
//! @param [in] capacity capacity-stack capacity
//! @param [in] data data-pointer to the beginning of the array
//! @param [in] hash b-coefficient
//! @param [in] canary_right canary_right-left right for protection
//!
//------------------------------------------------

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
    Errors = 0;                                         \
                                                        \
    check_function;                                     \
                                                        \
    StackDump(stk, __FILE__, __FUNCTION__);             \
                                                        \
    if (Errors != 0)                                    \
    {                                                   \
        return 1;                                       \
    }                                                   \
} while (0)

#define STACK_RESIZE_ERROR_CHECK()                      \
do                                                      \
{                                                       \
    Errors = 0;                                         \
                                                        \
	StackErrorCheck(stk);	                            \
                                                        \
    StackDump(stk, __FILE__, __FUNCTION__);             \
                                                        \
    if (Errors != 0)                                    \
    {                                                   \
        return nullptr;	                                \
    }                                                   \
} while (0)

#define STACK_POP_ERROR_CHECK()                         \
do                                                      \
{                                                       \
    Errors = 0;                                         \
                                                        \
	StackErrorCheck(stk);				                \
                                                        \
    if (stk->size <= 0)                                 \
    {                                                   \
        Errors |= STK_UNDERFL;                          \
    }                                                   \
                                                        \
    StackDump(stk, __FILE__, __FUNCTION__);             \
                                                        \
    if (Errors != 0)                                    \
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

size_t Hash (void* memory, size_t size_memory);

int StackDestroy(Stack* stk);

int StackAdd(Stack* stk);

int StackSub(Stack* stk);

int StackMul(Stack* stk);

int StackDiv(Stack* stk);