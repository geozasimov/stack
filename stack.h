//!@file
#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <limits.h>

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

//------------------------------------------------
//! Enum which describes all error codes and their numbers.
//!
//! @param [in] STK_IS_NULL_PTR   
//! @param [in] DATA_IS_NULL_PTR  
//! @param [in] STK_DESTROYED     
//! @param [in] STK_OVERFL        
//! @param [in] STK_UNDERFL       
//! @param [in] STK_DOUBLE_CTED   
//! @param [in] STRCT_CANARY_BAD  
//! @param [in] DATA_CANARY_BAD   
//! @param [in] HASH_BAD          
//! @param [in] CAPACITY_LARG_SIZE
//!
//------------------------------------------------

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

//------------------------------------------------
//! Define which describes the general check of the stack.
//!
//------------------------------------------------

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

//------------------------------------------------
//! Define which describes a general stack check during stack resizing.
//!
//------------------------------------------------

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

//------------------------------------------------
//! Define which describes the general check of the stack during the poping of the element.
//!
//------------------------------------------------

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

//------------------------------------------------
//! Function that describes the stack constructor.
//!
//! @param [in] stk stk-stack pointer
//!
//! @return zero
//!
//------------------------------------------------

int StackCtor(Stack* stk);

//------------------------------------------------
//! Function that describes the stack destructor.
//!
//! @param [in] stk stk-stack pointer
//!
//! @return zero
//!
//------------------------------------------------

int StackDtor(Stack* stk);

//------------------------------------------------
//! Function that describes the process of changing the stack size.
//!
//! @param [in] stk stk-stack pointer
//! @param [out] new_adress new_adress-new stack pointer
//!
//! @return new stack pointer
//!
//------------------------------------------------

data_t* StackResize(Stack* stk);

//------------------------------------------------
//! Function that describes the process of pushing an element onto the stack.
//!
//! @param [in] stk stk-stack pointer
//! @param [in] value value-value of the element that is pushed onto the stack
//!
//! @return zero
//!
//------------------------------------------------

int StackPush(Stack* stk, data_t value);

//------------------------------------------------
//! Function that describes the process of popping the value of an element from the top of the stack.
//!
//! @param [in] stk stk-stack pointer
//! @param [out]  data_pop data_pop-value of the element that is popped from the top of the stack
//!
//! @return value of the element that is popped from the top of the stack
//!
//------------------------------------------------

data_t StackPop(Stack* stk);

//------------------------------------------------
//! Function that describes the process of checking the stack.
//!
//! @param [in] stk stk-stack pointer
//! @param [out] Errors Error-number of errors in the stack
//!
//! @return number of errors in the stack
//!
//------------------------------------------------

int StackErrorCheck(Stack* stk);

//------------------------------------------------
//! Function that describes the process of checking the stack while the constructor is running.
//!
//! @param [in] stk stk-stack pointer
//!
//! @return zero
//!
//------------------------------------------------

int StackCtorCheck (Stack* stk);

//------------------------------------------------
//! Function that describes the process of checking the stack while the destructor is running.
//!
//! @param [in] stk stk-stack pointer
//!
//! @return zero
//!
//------------------------------------------------

int StackDtorCheck (Stack* stk);

//------------------------------------------------
//! Function that writes all errors to the Dump.txt file.
//!
//! @param [in] stk stk-stack pointer
//! @param [in] current_file-the file in which the error occurred
//! @param [in] current_function-the function in which the error occurred
//!
//! @return nothing
//!
//------------------------------------------------

void StackDump (Stack* stk, const char* current_file, const char* current_function);

//------------------------------------------------
//! Function that describes the stack hashing process.
//!
//! @param [in] stk stk-stack pointer
//! @param [out] hash hash-stack hash value
//!
//! @return stack hash value
//!
//------------------------------------------------

size_t StackHash (Stack* stk);

//------------------------------------------------
//! Function that describes the process of hashing a separate piece of memory.
//!
//! @param [in] memory memory-pointer to memory area
//! @param [in] size_memory size_memory-memory area size
//! @param [out] hash hash-memory area hash values
//!
//! @return memory area hash values
//!
//------------------------------------------------

size_t Hash (void* memory, size_t size_memory);

//------------------------------------------------
//! Function that describes the stack breaking process.
//!
//! @param [in] stk stk-stack pointer
//!
//! @return nothing
//!
//------------------------------------------------

int StackDestroy(Stack* stk);
