//#define HASH_DEBUG


static int StackRealloc (struct stack* stk, const int newCapacity);

static int Poison (struct stack* stk, const int left, const int right);

static int StackVerificator (struct stack* stk);

static void StackDump (struct stack* stk, const int line, const char* func, const char* file);



#ifndef HASH_DEBUG
    static long My_Data_Hash (struct stack *stk);

    static long My_Stack_Hash (struct stack *stk);
#endif
