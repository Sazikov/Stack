//#define HASH_DEBUG


static int StackRealloc(struct stack* stk);

static int FillWPoison(struct stack* stk, const int left, const int right);

static int StackShrink(struct stack* stk);

static int StackErr(struct stack* stk);

static int StackDump(struct stack* stk, const int line, const char* func, const char* file);

#ifndef HASH_DEBUG
    static long My_Data_Hash (struct stack *stk);

    static long My_Stack_Hash (struct stack *stk);
#endif
