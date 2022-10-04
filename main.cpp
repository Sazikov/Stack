#include <stdio.h>
#include <typeinfo>


#include "..\stack.h"

int main()
{

    struct stack stk = {0, NULL, 0, 0, 0, 0, 0, 0};

    StackCtor(&stk, 5);

    for (int i = 0; i < 25; i++)
        StackPush(&stk, i);

    for (int i = 29; i > 0; i--)
        printf("%ld\n", StackPop(&stk));

    StackDtor(&stk);

    return 0;
}
