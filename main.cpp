#include "stack.h"


int main()
{
	Stack stk = {};
	
	StackCtor(&stk);


	StackPush(&stk, 1);
	StackPush(&stk, 2);
	StackPush(&stk, 3);

	double x = StackPop(&stk);
	printf("%lf\n", x);

	StackDtor(&stk);

	return 0;
}
