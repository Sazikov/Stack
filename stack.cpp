#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <math.h>
#include <typeinfo>
#include <string.h>

#include "..\Config.h"
#include "..\stack.h"
#include "..\static_functions.h"

//#define CANARY_DEBUG
//#define HASH_DEBUG


FILE* logs = fopen("logs.txt", "w");

const int POISON = 777777;
const elem_t CANARY = 0xDED32;
int canary = 0;


int StackCtor(struct stack* stk, const int capacity)
{
    if (stk == NULL)
    {
        stk->errors |= STACKPTRERR;
        StackDump(stk, __LINE__, __PRETTY_FUNCTION__, __FILE__);
        return STACKPTRERR;
    }


    #ifndef CANARY_DEBUG
        canary = 1;
    #endif

    void *temp_ptr = calloc(capacity + 2 * canary, sizeof(elem_t));
    if (temp_ptr == NULL)
        exit (MEMERR);

    stk->data = (elem_t*) temp_ptr;
    stk->capacity = capacity;
    stk->size = 0;

    #ifndef HASH_DEBUG
        stk->Data_Hash = My_Data_Hash (stk);
        stk->Stack_Hash = My_Stack_Hash (stk);
    #endif

    #ifndef CANARY_DEBUG
        stk->left_canary = CANARY;
        stk->data[0] = CANARY;
        stk->data[stk->capacity + 1] = CANARY;
        stk->right_canary = CANARY;
    #endif


    FillWPoison(stk, 1, stk->capacity + canary);

    return NOERR;
}

int FillWPoison(struct stack* stk, const int left, const int right)
{
    if (stk == NULL)
    {
        stk->errors |= STACKPTRERR;
        StackDump(stk, __LINE__, __PRETTY_FUNCTION__, __FILE__);
        return STACKPTRERR;
    }

    for (int i = left; i < right; i++)
    {
        stk->data[i] = POISON;
    }

    #ifndef HASH_DEBUG
        stk->Data_Hash = My_Data_Hash (stk);
        stk->Stack_Hash = My_Stack_Hash (stk);
    #endif

    return NOERR;
}

elem_t StackPop(struct stack* stk)
{
    if (stk == NULL)
    {
        stk->errors |= STACKPTRERR;
        StackDump(stk, __LINE__, __PRETTY_FUNCTION__, __FILE__);
        return STACKPTRERR;
    }

    StackErr(stk);
    StackDump(stk, __LINE__, __PRETTY_FUNCTION__, __FILE__);

    elem_t value = stk->data[stk->size];
    if (value == POISON || value == CANARY || stk->size <= 0)
        exit (0);
    stk->data[stk->size] = POISON;
    stk->size--;

    #ifndef HASH_DEBUG
        stk->Data_Hash = My_Data_Hash (stk);
        stk->Stack_Hash = My_Stack_Hash (stk);
    #endif

    if (stk->size < stk->capacity/4)
    {
        stk->capacity /= 2;
        StackShrink(stk);
    }

    #ifndef HASH_DEBUG
        stk->Data_Hash = My_Data_Hash (stk);
        stk->Stack_Hash = My_Stack_Hash (stk);
    #endif

    StackErr(stk);
    StackDump(stk, __LINE__, __PRETTY_FUNCTION__, __FILE__);
    return value;
}

int StackPush(struct stack* stk, const elem_t elem)
{
    if (stk == NULL)
    {
        stk->errors |= STACKPTRERR;
        StackDump(stk, __LINE__, __PRETTY_FUNCTION__, __FILE__);
        exit (STACKPTRERR);
    }

    StackErr(stk);
    StackDump(stk, __LINE__, __PRETTY_FUNCTION__, __FILE__);

    if (stk->size == stk->capacity)
    {
        stk->capacity *= 2;
        StackRealloc(stk);
        FillWPoison(stk, stk->size + 1, stk->capacity + 1);
    }

    stk->data[stk->size + 1] = elem;
    stk->size++;

    #ifndef HASH_DEBUG
        stk->Data_Hash = My_Data_Hash (stk);
        stk->Stack_Hash = My_Stack_Hash (stk);
    #endif

    StackErr(stk);
    StackDump(stk, __LINE__, __PRETTY_FUNCTION__, __FILE__);

    return NOERR;
}

 int StackRealloc(struct stack* stk)
{
    if (stk == NULL)
    {
        stk->errors |= STACKPTRERR;
        StackDump(stk, __LINE__, __PRETTY_FUNCTION__, __FILE__);
        return STACKPTRERR;
    }

    void *temp_ptr = realloc(stk->data, (stk->capacity + 1) * sizeof(elem_t));
    if (temp_ptr  == NULL)
        exit (MEMERR);

    stk->data = (elem_t*) temp_ptr;

    #ifndef CANARY_DEBUG
        stk->data[stk->capacity + 1] = CANARY;
    #endif
    #ifndef HASH_DEBUG
        stk->Data_Hash = My_Data_Hash (stk);
        stk->Stack_Hash = My_Stack_Hash (stk);
    #endif

    return NOERR;
}

int StackShrink(struct stack* stk)
{
    if (stk == NULL)
    {
        stk->errors |= STACKPTRERR;
        StackDump(stk, __LINE__, __PRETTY_FUNCTION__, __FILE__);
        return STACKPTRERR;
    }


    void *temp_ptr = realloc(stk->data,  (stk->capacity + 1) * sizeof(elem_t));
    if (temp_ptr  == NULL)
        exit (MEMERR);

    stk->data = (elem_t*) temp_ptr;

    #ifndef CANARY_DEBUG
        stk->data[stk->capacity + 1] = CANARY;
    #endif
    #ifndef HASH_DEBUG
        stk->Data_Hash = My_Data_Hash (stk);
        stk->Stack_Hash = My_Stack_Hash (stk);
    #endif

    return NOERR;
}

int StackErr(struct stack* stk)
{
    if (stk == NULL)
        {
            stk->errors |= STACKPTRERR;
            StackDump(stk, __LINE__, __PRETTY_FUNCTION__, __FILE__);
            return STACKPTRERR;
        }

    if (stk->data == NULL)
        stk->errors |= DATAERR;
    if (isnan(stk->size))
        stk->errors |= SIZERR;
    if (isnan(stk->capacity))
        stk->errors |= CAPERR;
    if (stk->size > stk->capacity)
        stk->errors |= SIZENCAPERR;
    #ifndef CANARY_DEBUG
        if (stk->left_canary != CANARY)
            stk->errors |= LEFTCANERR;
        if (stk->right_canary != CANARY)
            stk->errors |= RIGHTCANERR;
        if (stk->data[0] != CANARY)
            stk->errors |= LEFTDATACANERR;
        if (stk->data[stk->capacity + 1] != CANARY)
            stk->errors |= RIGHTDATACANERR;
    #endif
    #ifndef HASH_DEBUG
        if (stk->Data_Hash != My_Data_Hash (stk))
            stk->errors |= DATAHASHERR;
        if (stk->Stack_Hash != My_Stack_Hash (stk))
            stk->errors |= STACKHASHERR;
    #endif


    return NOERR;
}

int StackDump(struct stack* stk, const int line, const char* func, const char* file)
{
    if (stk == NULL)
    {
        fprintf(logs, "ERROR: NULL Pointer to a stack");
        return STACKPTRERR;
    }

    fprintf(logs, "\n%s at ", func);
    fprintf(logs, "%s", file);
    fprintf(logs, "(%d)\n", line);
    fprintf(logs, "Stack[%p] ", (void*) &stk);

    if (stk->errors == 0)
        fprintf(logs, "(OK)\n");
    else
        fprintf(logs, "-------------(ERROR CODE = %d)---------------\n", stk->errors);

    fprintf(logs, "Data_Hash = %ld\n", stk->Data_Hash);
    fprintf(logs, "{\n\t size = %i\n", stk->size);
    fprintf(logs, "\tcapacity = %i\n", stk->capacity);
    fprintf(logs, "\tdata[%p]\n\t{\n", (void*) stk->data);

    if (stk->data != NULL)
    {
        for (int i = 0; i < stk->capacity + 1; i++)
        {
            if (stk->data[i] != POISON)
            {
                fprintf(logs, "\t\t*[%d] = ", i - 1);
                print(stk->data[i]);
            }
            else
                fprintf(logs, "\t\t[%d] = POISON\n", i - 1);
        }
    }

    fprintf(logs, "\t}\n");
    fprintf(logs, "}\n");

    return NOERR;
}

int StackDtor(struct stack* stk)
{
    if (stk == NULL)
    {
        stk->errors |= STACKPTRERR;
        StackDump(stk, __LINE__, __PRETTY_FUNCTION__, __FILE__);
        return STACKPTRERR;
    }


    stk->capacity = 0xDED32DED;
    stk->data = NULL;
    stk->size = -1;

    return NOERR;
}

void print(const int x)
{
    fprintf(logs, "%d\n", x);
}

void print(const double x)
{
    fprintf(logs, "%lg\n", x);
}

void print(const char x)
{
    fprintf(logs, "%c\n", x);
}

void print(const char* x)
{
    fprintf(logs, "%p\n", x);
}

void print(const long x)
{
    fprintf(logs, "%ld\n", x);
}

#ifndef HASH_DEBUG
long My_Data_Hash (struct stack *stk)
{
    long Hash = 0;

    for (int i = 1; i < stk->capacity; i++)
    {
        Hash += stk->data[i] * i;
    }

    return Hash;
}
#endif


#ifndef HASH_DEBUG
long My_Stack_Hash (struct stack *stk)
{
    return stk->left_canary + stk->size + stk->capacity + stk->errors + stk->Data_Hash + stk->right_canary + (long int)stk->data;
}
#endif
