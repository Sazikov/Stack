//#define HASH_DEBUG


static int StackIncrease (struct stack* stk);

static int Poison (struct stack* stk, const int left, const int right);

static int StackDecrease (struct stack* stk);

static int StackVerificator (struct stack* stk);

static int StackDump (struct stack* stk, const int line, const char* func, const char* file);

#ifndef HASH_DEBUG
    static long My_Data_Hash (struct stack *stk);

    static long My_Stack_Hash (struct stack *stk);
#endif
