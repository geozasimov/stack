#include "stack.h"

static const int    START_CAPACITY = 10 ;
static const int    CAPACITY_STEP = 2;
static const size_t CANARY_CONSTANT = 0xDED32BAD;
static const data_t*   UNAVAILABLE_ADR = (data_t*) 1;

int errors = 0;

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

    return 0;

#ifdef HASH_DEFF
    stk->hash = StackHash(stk);
#endif

    STACK_GENERAL_CHECK(StackErrorCheck(stk));
}

int StackDtor(Stack* stk)
{
    STACK_GENERAL_CHECK(StackDtorCheck(stk));

    memset(stk, 0, stk->capacity * sizeof(data_t));

#ifdef CANARY_DEFF
    stk->data = (data_t*)((canary_t*)stk->data - 1);
#endif

    free(stk->data);

	stk->size     = -1;
	stk->capacity = 0;
	stk->data     = (data_t*) UNAVAILABLE_ADR;

#ifdef CANARY_DEFF
    stk->canary_left  = 0;
    stk->canary_right = 0;
#endif

	return 0;
}

data_t* StackResize (Stack* stk)
{
    STACK_RESIZE_ERROR_CHECK();

	data_t* new_adress = stk->data;

	if (stk->size >= stk->capacity)
	{
        stk->capacity *= CAPACITY_STEP;

#ifdef CANARY_DEFF

        stk->data = (data_t*)((canary_t*)stk->data - 1);

        new_adress = (data_t*) realloc(stk->data,
                                       stk->capacity * sizeof(data_t) + 2 * sizeof(canary_t));

        ((canary_t*)new_adress)[0] = CANARY_CONSTANT;

        new_adress = (data_t*)((canary_t*)new_adress + 1);

        *(canary_t*)(new_adress + stk->capacity) = CANARY_CONSTANT;
#else
        new_adress = (data_t*) realloc(stk->data,
                                       (stk->capacity) * sizeof(data_t));
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

    memset (new_adress + stk->size, (int)0,
            (stk->capacity - stk->size) * sizeof(data_t));

    if (!new_adress)
        return stk->data;
    else
	    return new_adress;
}

int StackPush(Stack* stk, data_t value)
{
    STACK_GENERAL_CHECK(StackErrorCheck(stk));

    stk->data = StackResize(stk);
    stk->data[stk -> size] = value;
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
    stk->data = StackResize(stk);

    STACK_GENERAL_CHECK(StackErrorCheck(stk));

#ifdef HASH_DEFF
    stk->hash = StackHash(stk);
#endif    

    return elem_pop;
}

int StackErrorCheck (Stack* stk)
{
	if (stk == nullptr)
		errors |= STK_IS_NULL_PTR;

	if (stk->data == nullptr)
		errors |= DATA_IS_NULL_PTR;

	if (stk->data == UNAVAILABLE_ADR)
		errors |= STK_DESTROYED;

    if ((errors & STK_IS_NULL_PTR)  == STK_IS_NULL_PTR ||
        (errors & DATA_IS_NULL_PTR) == DATA_IS_NULL_PTR ||
        (errors & STK_DESTROYED)    == STK_DESTROYED)
        return errors;

	if (stk->size < 0)
		errors |= STK_UNDERFL;

	if (stk->size > stk->capacity)
		errors |= STK_OVERFL;

#ifdef HASH_DEFF
    if (stk->hash != StackHash(stk))
        errors |= HASH_BAD;
#endif

#ifdef CANARY_DEFF
    if (stk->canary_left != CANARY_CONSTANT || stk->canary_right != CANARY_CONSTANT)
        errors |= STRCT_CANARY_BAD;

    if (((canary_t*)stk->data)[-1] != CANARY_CONSTANT ||
    *(canary_t*)(stk->data + stk->capacity) != CANARY_CONSTANT)
        errors |= DATA_CANARY_BAD;
#endif

    return errors;
}


int StackCtorCheck (Stack* stk)
{
	if (stk->data != nullptr && stk->data != UNAVAILABLE_ADR)
	{
		errors |= STK_DOUBLE_CTED;
		return errors;
	}
	else
		return 0;
}

int StackDtorCheck (Stack* stk)
{
    if (stk == nullptr)
        errors |= STK_IS_NULL_PTR;

    if (stk->data == nullptr)
        errors |= DATA_IS_NULL_PTR;

    if (stk->data == UNAVAILABLE_ADR)
        errors |= STK_DESTROYED;

    if ((errors & STK_DESTROYED) == STK_DESTROYED)
        return errors;

    return errors;
}


void StackDump (Stack* stk, const char* current_file, const char* current_function)
{
	FILE* dump_file = fopen("Dump.txt", "a"); 

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

    if (errors != 0)
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

    size_t hash = 2139062143;

    for (int i = 0; i < stk->capacity; i++)
    {
        hash = 37 * hash + (size_t)stk->data[i];
    }

    hash = 37 * hash + (size_t)stk->capacity;
    hash = 37 * hash + (size_t)stk->data;
    hash = 37 * hash + (size_t)stk->size;

    return hash;
}

int DestroyStack(Stack* stk)
{
    *(int*)&stk = 0;
    ((int*)&stk)[1] = 0;
    return 0;
}