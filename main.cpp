#include <stdio.h>
#include <typeinfo>

#include "src\stack.h"


int main()
{
    struct stack stk = {0, NULL, 0, 0, 0, 0, 0, 0, 0};

    StackCtor(&stk, 10);
    StackPush (&stk, 15);
    StackPop (&stk);

    return 0;
}
