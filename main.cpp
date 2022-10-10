#include <stdio.h>
#include <typeinfo>

#include "..\stack.h"

int main()
{
    struct stack stk = {0, NULL, 0, 0, 0, 0, 0, 0};

    StackCtor(&stk, 6);

    for (int i = 0; i < 36; i++)
        StackPush(&stk, i);

    for (int i = 289; i > 0; i--)
        printf("%ld\n", StackPop(&stk));

    StackDtor(&stk);

    return 0;
}
