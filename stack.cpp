#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <math.h>
#include <typeinfo>
#include <string.h>

#include "Config.h"
#include "stack.h"
#include "static_functions.h"

//#define CANARY_DEBUG
//#define HASH_DEBUG

#define VERIFICATOR(stack_name)                                                         \
{                                                                                       \
                    int res = StackVerificator (stack_name);                            \
                    if (res == STACKPTRERR)                                             \
                    {                                                                   \
                        PrintErrors (res, __LINE__, __PRETTY_FUNCTION__, __FILE__);     \
                        return res;                                                     \
                    }                                                                   \
                    StackDump (stack_name, __LINE__, __PRETTY_FUNCTION__, __FILE__);    \
}

#define ASSERT(res)  do {                                                               \
                        if (res != NOERR)                                               \
                            PrintErrors (res, __LINE__, __PRETTY_FUNCTION__, __FILE__); \
                        } while (0)


/*
if (condition)
    ASSERT(res)
else
    return 1;
*/
FILE* logs = fopen ("logs.txt", "w");

static int Canary = 0;

const elem_t POISON = 777777;
const elem_t CANARY = 0xDED32;

int StackCtor (struct stack* stk, const int capacity)
{
    if (stk == NULL)
        return STACKPTRERR;

    if (stk->created == Stack_CREATED)
        {
            PrintErrors (CREATED, __LINE__, __PRETTY_FUNCTION__, __FILE__);
            return CREATED;
        }
    else 
        stk->created = Stack_CREATED; 


    #ifndef CANARY_DEBUG
        Canary = 1;
    #endif

    stk->data = (elem_t*)calloc (capacity + Canary + 1, sizeof (elem_t));
    if (stk->data == NULL)
        return MEMERR;

    stk->capacity = capacity;
    stk->size = 0;

    #ifndef CANARY_DEBUG
        stk->left_canary = CANARY;
        stk->data [0] = CANARY;
        stk->data [stk->capacity + 1] = CANARY;
        stk->right_canary = CANARY;
    #endif
    
    #ifndef HASH_DEBUG
        stk->Data_Hash = My_Data_Hash (stk);
        stk->Stack_Hash = My_Stack_Hash (stk);
    #endif

    int reserr = Poison (stk, 1, stk->capacity + 1);
    ASSERT(reserr);

    #ifndef HASH_DEBUG
        stk->Data_Hash = My_Data_Hash (stk);
        stk->Stack_Hash = My_Stack_Hash (stk);
    #endif
    VERIFICATOR(stk)

    return NOERR;
}

int Poison (struct stack* stk, const int left, const int right)
{
    VERIFICATOR(stk)

    for (int i = left; i < right; i++)
    {
        stk->data[i] = POISON;
    }

    #ifndef HASH_DEBUG
        stk->Data_Hash = My_Data_Hash (stk);
        stk->Stack_Hash = My_Stack_Hash (stk);
    #endif

    VERIFICATOR(stk)

    return NOERR;
}

elem_t StackPop(struct stack* stk)
{
    VERIFICATOR(stk)

    elem_t value = stk->data[stk->size];
    if (stk->size - 1 < 0)
        return POISON;

    stk->data[stk->size] = POISON;
    stk->size--;

    #ifndef HASH_DEBUG
        stk->Data_Hash = My_Data_Hash (stk);
        stk->Stack_Hash = My_Stack_Hash (stk);
    #endif

    if (stk->size < stk->capacity / 4)
    {
        int reserr = StackRealloc (stk, stk->capacity / 2);
        ASSERT(reserr);
    }

    #ifndef HASH_DEBUG
        stk->Data_Hash = My_Data_Hash (stk);
        stk->Stack_Hash = My_Stack_Hash (stk);
    #endif
    VERIFICATOR(stk)

    return value;
}


int StackPush (struct stack* stk, const elem_t elem)
{

    VERIFICATOR(stk)

    if (stk->size == stk->capacity)
    {
        int reserr = StackRealloc (stk, stk->capacity * 2);
        ASSERT(reserr);
        reserr = Poison (stk, stk->size + 1, stk->capacity + 1);
        ASSERT(reserr);
    }

    stk->data[stk->size + 1] = elem;
    stk->size++;

    #ifndef HASH_DEBUG
        stk->Data_Hash = My_Data_Hash (stk);
        stk->Stack_Hash = My_Stack_Hash (stk);
    #endif

    VERIFICATOR(stk)

    return NOERR;
}

int StackRealloc (struct stack* stk, const int newCapacity) 
{
    VERIFICATOR(stk)

    stk->capacity = newCapacity;

    void *temp_ptr = realloc (stk->data, (stk->capacity + 1) * sizeof (elem_t));
    if (temp_ptr  == NULL)
        return MEMERR;

    stk->data = (elem_t*) temp_ptr;

    #ifndef CANARY_DEBUG
        stk->data[stk->capacity + 1] = CANARY;
    #endif
    #ifndef HASH_DEBUG
        stk->Data_Hash = My_Data_Hash (stk);
        stk->Stack_Hash = My_Stack_Hash (stk);
    #endif

    VERIFICATOR(stk)

    return NOERR;
}


int StackVerificator (struct stack* stk)
{
    if (stk == NULL)
        {
            return STACKPTRERR;
        }

    if (stk->data == NULL)                          stk->errors |= DATAERR;
    if (stk->size > stk->capacity)                  stk->errors |= SIZEBIGGERTHANCAPERR;
    if (stk->size < 0)                              stk->errors |= SIZEERR;

    #ifndef CANARY_DEBUG
        if (stk->left_canary != CANARY)             stk->errors |= LEFTCANERR;
        if (stk->right_canary != CANARY)            stk->errors |= RIGHTCANERR;
        if (stk->data[0] != CANARY)                 stk->errors |= LEFTDATACANERR;
        if (stk->data[stk->capacity + 1] != CANARY) stk->errors |= RIGHTDATACANERR;
    #endif

    #ifndef HASH_DEBUG
        if (stk->Data_Hash  != My_Data_Hash  (stk)) stk->errors |= DATAHASHERR;
        if (stk->Stack_Hash != My_Stack_Hash (stk)) stk->errors |= STACKHASHERR;
    #endif


    return NOERR;
}

void StackDump (struct stack* stk, const int line, const char* func, const char* file)
{

    fprintf (logs, "\n%s at ", func);
    fprintf (logs, "%s", file);
    fprintf (logs, "(%d)\n", line);
    fprintf (logs, "Stack[%p] ", (void*) &stk);

    if (stk->errors == 0)
        fprintf (logs, "(OK)\n");
    else
        fprintf (logs, "-------------(ERROR CODE = %d)---------------\n", stk->errors);

    fprintf (logs, "Data_Hash = %ld\n", stk->Data_Hash);
    fprintf (logs, "{\n\t size = %i\n", stk->size);
    fprintf (logs, "\tcapacity = %i\n", stk->capacity);
    fprintf (logs, "\tdata[%p]\n\t{\n", (void*) stk->data);

    if (stk->data != NULL)
    {
        for (int i = Canary - 1; i < stk->capacity + 2; i++)
        {
            if (stk->data[i] != POISON)
            {
                fprintf (logs, "\t\t*[%d] = %li\n", i - Canary, stk->data[i]);
            }
            else
                fprintf (logs, "\t\t[%d] = POISON\n", i - Canary);
        }
    }

    fprintf (logs, "\t}\n");
    fprintf (logs, "}\n");
}

int StackDtor (struct stack* stk)
{
    if (stk->created == Stack_Destructed)
    {
        PrintErrors (CREATED, __LINE__, __PRETTY_FUNCTION__, __FILE__);
        return CREATED;
    }
    VERIFICATOR(stk)

    stk->capacity = 0;
    free (stk->data);
    stk->size = 0;
    stk->created = -1;
    #ifndef CANARY_DEBUG
        stk->left_canary = 0;
        stk->right_canary = 0;
    #endif
    #ifndef HASH_DEBUG
        stk->Data_Hash = 0;
        stk->Stack_Hash = 0;
    #endif
    fclose(logs);
    return NOERR;
}



#ifndef HASH_DEBUG
long My_Data_Hash (struct stack *stk)
{
    long Hash = 0;

    for (int i = 1; i < stk->capacity; i++)
    {
        Hash ^= (stk->data[i] >> 16) ^ (stk->data[i] << 16);
    }

    return Hash;
}
#endif


#ifndef HASH_DEBUG
long My_Stack_Hash (struct stack *stk)
{
    return (stk->left_canary >> 16) ^ (stk->left_canary << 16) ^ (stk->size >> 16) ^ (stk->size << 16) ^ (stk->capacity >> 16) ^  (stk->capacity << 16) ^ (stk->errors >> 16) ^ (stk->errors << 16) ^ (stk->Data_Hash >> 16) ^ (stk->Data_Hash << 16) ^ (stk->right_canary >> 16) ^ (stk->right_canary << 16) ^ ((long int) *stk->data >> 16) ^ ((long int) *stk->data << 16) ^ (stk->created >> 16) ^ (stk->created << 16);
}
#endif


void PrintErrors (const int err, const int line, const char* func, const char* file)
{
    printf ("Error %i in line: %i in function: %s in file: %s\n", err, line, func, file);       
}
