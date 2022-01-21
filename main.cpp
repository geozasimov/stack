#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "stack.h"


int main()
{
	Stack stk = {};
	int error = NULL;
	StackCtor(&stk, 10);


	StackPush(&stk, 1);
	StackPush(&stk, 2);
	StackPush(&stk, 3);

	double x = StackPop(&stk);
	printf("%lf\n", x);

	StackDtor(&stk);

	return 0;
}
