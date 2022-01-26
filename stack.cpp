#include "stack.h"

static const int        START_CAPACITY  = 1;
static const int        CAPACITY_STEP   = 2;
static const size_t     CANARY_CONSTANT = 0xDED32BAD;
static const data_t*    UNAVAILABLE_ADR = (data_t*) 1;

int Errors = 0;

#define CANARY_DEFF
#define HASH_DEFF

int StackCtor(Stack* stk)
{
    STACK_GENERAL_CHECK(StackCtorCheck(stk));

    stk->capacity = START_CAPACITY;

#ifdef CANARY_DEFF
    stk->data = (data_t*) calloc(stk->capacity * sizeof(data_t) + 2 * sizeof(canary_t), sizeof(char));

    stk->canary_left  = CANARY_CONSTANT;
    stk->canary_right = CANARY_CONSTANT;

    *((canary_t*)stk->data) = CANARY_CONSTANT;
    stk->data = (data_t*)((canary_t*)stk->data + 1);
    *(canary_t*)(stk->data + stk->capacity) = CANARY_CONSTANT;
#else
    memset(stk, 0, sizeof(Stack));
    stk->data = (data_t*)calloc(stk->capacity, sizeof(data_t));
#endif

    stk->size = 0;

#ifdef HASH_DEFF
    stk->hash = StackHash(stk);
#endif

    STACK_GENERAL_CHECK(StackErrorCheck(stk));

    return 0;
}

int StackDtor(Stack* stk)
{
    STACK_GENERAL_CHECK(StackDtorCheck(stk));

#ifdef CANARY_DEFF
    stk->data = (data_t*)((canary_t*)stk->data - 1);
#endif
	stk->size     = -1;
	stk->capacity = 0;
	stk->data     = (data_t*) UNAVAILABLE_ADR;

    free(stk->data);

#ifdef CANARY_DEFF
    stk->canary_left  = 0;
    stk->canary_right = 0;
#endif

    memset(stk, 0, stk->capacity * sizeof(data_t));

	return 0;
}

data_t* StackResize (Stack* stk)
{
    STACK_RESIZE_ERROR_CHECK();

	data_t* new_adress = stk->data;

    data_t* old_adress = stk->data;

	if (stk->size >= stk->capacity)
	{
        stk->capacity *= CAPACITY_STEP;

#ifdef CANARY_DEFF
        
        old_adress = (data_t*)((canary_t*)stk->data - 1);

        new_adress = (data_t*) realloc(old_adress, stk->capacity * sizeof(data_t) + 2 * sizeof(canary_t));

        ((canary_t*)new_adress)[0] = CANARY_CONSTANT;

        new_adress = (data_t*)((canary_t*)new_adress + 1);

        *(canary_t*)(new_adress + stk->capacity) = CANARY_CONSTANT;
#else
        new_adress = (data_t*) realloc(stk->data, (stk->capacity) * sizeof(data_t));
#endif
	}
	else if (stk->size * 2 * CAPACITY_STEP == stk->capacity)
	{
		stk->capacity /= CAPACITY_STEP;

#ifdef CANARY_DEFF

        stk->data = (data_t*)((canary_t*)stk->data - 1);

        new_adress = (data_t*) realloc(stk->data,
                                       stk->capacity * sizeof(data_t) + 2*sizeof(canary_t));


        ((canary_t*)new_adress)[0] = CANARY_CONSTANT;

        new_adress = (data_t*)((canary_t*)new_adress + 1);

        *(canary_t*)(new_adress + stk->capacity) = CANARY_CONSTANT;
#else
        new_adress = (data_t*) realloc(stk->data,
                                       (stk->capacity) * sizeof(data_t));
#endif
    }

    memset (new_adress + stk->size, (int)0, (stk->capacity - stk->size) * sizeof(data_t));

    if (!new_adress)
        return stk->data;
    else
	    return new_adress;
}

int StackPush(Stack* stk, data_t value)
{
    STACK_GENERAL_CHECK(StackErrorCheck(stk));

    if (stk->size >= stk->capacity)
    {
        stk->data = StackResize(stk);
    }

    stk->data[stk->size] = value;
    stk->size++;
    
#ifdef HASH_DEFF
    stk->hash = StackHash(stk);
#endif

    STACK_GENERAL_CHECK(StackErrorCheck(stk));

    return 0;
}

data_t StackPop(Stack* stk)
{   
    STACK_POP_ERROR_CHECK();

    stk->size--;
    data_t elem_pop = stk->data[stk->size];

    if (stk->size >= stk->capacity)
    {
        stk->data = StackResize(stk);
    }

#ifdef HASH_DEFF
    stk->hash = StackHash(stk);
#endif    

    STACK_GENERAL_CHECK(StackErrorCheck(stk));

    return elem_pop;
}

int StackErrorCheck (Stack* stk)
{
	if (stk == nullptr)
		Errors |= STK_IS_NULL_PTR;

	if (stk->data == nullptr)
		Errors |= DATA_IS_NULL_PTR;

	if (stk->data == UNAVAILABLE_ADR)
		Errors |= STK_DESTROYED;

    if ((Errors & STK_IS_NULL_PTR)  == STK_IS_NULL_PTR ||
        (Errors & DATA_IS_NULL_PTR) == DATA_IS_NULL_PTR ||
        (Errors & STK_DESTROYED)    == STK_DESTROYED)
        return Errors;

	if (stk->size < 0)
		Errors |= STK_UNDERFL;

	if (stk->size > stk->capacity)
		Errors |= STK_OVERFL;

#ifdef HASH_DEFF
    if (stk->hash != StackHash(stk))
        Errors |= HASH_BAD;
#endif

#ifdef CANARY_DEFF
    if (stk->canary_left != CANARY_CONSTANT || stk->canary_right != CANARY_CONSTANT)
        Errors |= STRCT_CANARY_BAD;

    if (((canary_t*)stk->data)[-1] != CANARY_CONSTANT ||
    *(canary_t*)(stk->data + stk->capacity) != CANARY_CONSTANT)
        Errors |= DATA_CANARY_BAD;
#endif

    return Errors;
}


int StackCtorCheck (Stack* stk)
{
	if (stk->data != nullptr && stk->data != UNAVAILABLE_ADR)
	{
		Errors |= STK_DOUBLE_CTED;
		return Errors;
	}
	else
		return 0;
}

int StackDtorCheck (Stack* stk)
{
    if (stk == nullptr)
        Errors |= STK_IS_NULL_PTR;

    if (stk->data == nullptr)
        Errors |= DATA_IS_NULL_PTR;

    if (stk->data == UNAVAILABLE_ADR)
        Errors |= STK_DESTROYED;

    if ((Errors & STK_DESTROYED) == STK_DESTROYED)
        return Errors;

    return Errors;
}


void StackDump (Stack* stk, const char* current_file, const char* current_function)
{
	FILE* dump_file = fopen("Dump.txt", "a"); 

    if (Errors != 0)
    {
        fprintf(dump_file, "\n______________________________\n");
    }

	if ((Errors & STK_IS_NULL_PTR) == STK_IS_NULL_PTR)
	{
		fprintf(dump_file, "\n-In file: %s; In function: %s\n Stack is nullptr: %d\n",
			current_file, current_function, Errors & STK_IS_NULL_PTR);
	}

	if ((Errors & DATA_IS_NULL_PTR) == DATA_IS_NULL_PTR)
	{
		fprintf(dump_file, "\n-In file: %s; In function: %s\n Stack data is nullptr: %d\n",
			current_file, current_function, Errors & DATA_IS_NULL_PTR);
	}

	if ((Errors & STK_DESTROYED) == STK_DESTROYED)
	{
		fprintf(dump_file, "\n-In file: %s; In function: %s\n Stack destroyed: %d\n",
			current_file, current_function, Errors & STK_DESTROYED);
	}

	if ((Errors & STK_UNDERFL) == STK_UNDERFL)
	{
		fprintf(dump_file, "\n-In file: %s; In function: %s\n Stack underflowed (wrong size of stack): %d\n",
			current_file, current_function, Errors & STK_UNDERFL);
	}

	if ((Errors & STK_OVERFL) == STK_OVERFL)
	{
		fprintf(dump_file, "\n-In file: %s; In function: %s\n Stack overflowed (wrong capacity of stack): %d\n",
			current_file, current_function, Errors & STK_OVERFL);
	}

	if ((Errors & STK_DOUBLE_CTED) == STK_DOUBLE_CTED)
	{
		fprintf(dump_file, "\n-In file: %s; In function: %s\n Stack already constructed: %d\n",
			current_file, current_function, Errors & STK_DOUBLE_CTED);
	}

    if ((Errors & STRCT_CANARY_BAD) == STRCT_CANARY_BAD)
    {
        fprintf(dump_file, "\n-In file: %s; In function: %s\n Canary of Structure \"Stack\" has been changed: "
                           "Error code: %d\n", current_file, current_function, Errors & STRCT_CANARY_BAD);
    }

    if ((Errors & DATA_CANARY_BAD) == DATA_CANARY_BAD)
    {
        fprintf(dump_file, "\n-In file: %s; In function: %s\n Data canary has been changed illegally : Error code: %d\n",
                current_file, current_function, Errors & DATA_CANARY_BAD);
    }

    if ((Errors & HASH_BAD) == HASH_BAD)
    {
        fprintf(dump_file, "\n-In file: %s; In function: %s\n Stack hash has been changed illegally. Error code: %d\n",
                current_file, current_function, Errors & HASH_BAD);
    }

    if ((Errors & CAPACITY_LARG_SIZE) == CAPACITY_LARG_SIZE)
    {
        fprintf(dump_file, "\n-In file: %s; In function: %s\n Stack's capacity is larger than Stack's size. Error code: %d\n",
                current_file, current_function, Errors & CAPACITY_LARG_SIZE);
    }

    if (Errors != 0)
    {
        fprintf(dump_file, "\n Stack: \n");
        for (int i = 0; i < stk->capacity; i++)
        {
#ifdef FLOAT_DATA
            if ( i == stk->size)
                fprintf(dump_file, "\n Other data: \n");

            fprintf(dump_file, "[%d]: %f\n", i, stk->data[i]);
#endif
#ifdef INT_DATA
            if ( i == stk->size)
                fprintf(dump_file, "\n Other data: \n");

            fprintf(dump_file, "[%d]: %d\n", i, stk->data[i]);
#endif
        }
    }

	fclose(dump_file);
}

size_t StackHash (Stack* stk)
{
    if (stk->data == nullptr || stk->data < (data_t*)4000)
    {
        return 0;
    }
    
    size_t old_hash = stk->hash;

    stk->hash = 0;

    size_t hash = Hash(&stk, sizeof(stk));

    stk->hash = old_hash;

    return hash;
}

size_t Hash (void* memory, size_t size_memory)
{
    size_t hash = 2139062143;

    for (int i = 0; i < size_memory; i++)
    {
        hash = 37 * hash + ((unsigned char*)memory)[i];
    }
    return hash;
}

int StackDestroy(Stack* stk)
{
    *(int*)&stk = 0;
    ((int*)&stk)[1] = 0;
    stk->size = -1;
    stk->capacity = -2;
    stk->data = 0;

    return 0;
}
