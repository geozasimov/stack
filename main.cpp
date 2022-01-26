#include "stack.h"


int main()
{
	Stack stk = {};
	
	StackCtor(&stk);
	
	StackPush(&stk, 1200.2);
	StackPush(&stk, 1300.3);
	StackPush(&stk, 1400.4);
	 
	printf("%d\n", stk.size);

	int Size = stk.size;

	for (int i = 0; i < Size; i++)
	{
		printf("%lf\n", StackPop(&stk));
	
	printf("%d\n", stk.size);

	StackDtor(&stk);

	return 0;
}
