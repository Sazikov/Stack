#include "Config.h"

struct stack
{
    long int left_canary;
    elem_t* data;
    int size;
    int capacity;
    int errors;
    long Data_Hash;
    long Stack_Hash;
    long int right_canary;

};

enum StackErrors
{
    NOERR = 0,
    STACKPTRERR = 1,
    DATAERR = 2,
    SIZEBIGGERTHANCAPERR = 4,
    LEFTCANERR = 8,
    RIGHTCANERR = 16,
    LEFTDATACANERR = 32,
    RIGHTDATACANERR = 64,
    DATAHASHERR = 128,
    STACKHASHERR = 256
};

enum SysErrors
{
    MEMERR = -1
};

int StackCtor(struct stack* stk, const int capacity);

elem_t StackPop(struct stack* stk);

int StackPush(struct stack* stk, const elem_t elem);

int StackDtor(struct stack* stk);


