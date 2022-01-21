
typedef double elem_t;

struct Stack
{
	int size;
	int capacity;
	elem_t* data;
};

enum StackErrors
{
	EMPTY_PTR = 0,
	CAPACITY_NULL = 1,
	CAPACITY_LARG_SIZE = 2,
	SIZE_IS_NEGATIVE = 3
	
};

enum ResizeTypes
{
	RESIZESMALLER = 0,
	RESIZELARGER = 1
};


int StackCtor(Stack* stk, int basic_size);
int StackDtor(Stack* stk);
int StackResize(Stack* stk, bool mode);
elem_t StackPush(Stack* stk, elem_t value);
elem_t StackPop(Stack* stk);
int StackCheck(Stack* stk);
