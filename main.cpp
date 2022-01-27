#include "stack.h"


int main()
{
	Stack stk = {};	
	StackCtor(&stk);
	
	StackPush(&stk, 1200.2);
	StackPush(&stk, 1300.3);
															stk.size = -1;	
	StackPush(&stk, 1400.4);
	
	//printf("%d\n", stk.size);

	//int Size = stk.size;

	//for (int i = 0; i < Size; i++)
	//{
	//	printf("%lf\n", StackPop(&stk));
	//}

	//printf("%d\n", stk.size);

	//int StackAdd(Stack* stk)
	//{
	//	data_t a = StackPop(stk);
	//	data_t b = StackPop(stk);
	//
	//	StackPush(stk, a + b);
	//	
	//	return 0;
	//}

	//int StackSub(Stack* stk)
	//{
	//	data_t a = StackPop(stk);
	//	data_t b = StackPop(stk);
	//
	//	StackPush(stk, a - b);
	//	
	//	return 0;
	//}

	//int StackMul(Stack* stk)
	//{
	//	data_t a = StackPop(stk);
	//	data_t b = StackPop(stk);
	//
	//	StackPush(stk, a * b);
	//	
	//	return 0;
	//}

	//int StackDiv(Stack* stk)
	//{
	//	data_t a = StackPop(stk);
	//	data_t b = StackPop(stk);
	//
	//	StackPush(stk, a / b);
	//	
	//	return 0;
	//}

	StackDtor(&stk);

	return 0;

	
}
