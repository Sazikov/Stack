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
    SIZERR = 4,
    CAPERR = 8,
    SIZENCAPERR = 16,
    LEFTCANERR = 32,
    RIGHTCANERR = 64,
    LEFTDATACANERR = 128,
    RIGHTDATACANERR = 256,
    DATAHASHERR = 512,
    STACKHASHERR = 1024
};

enum SysErrors
{
    MEMERR = -1
};

int StackCtor(struct stack* stk, const int capacity);

elem_t StackPop(struct stack* stk);

int StackPush(struct stack* stk, const elem_t elem);

int StackDtor(struct stack* stk);

void print(const long x);

void print(const char* x);

void print(const char x);

void print(const double x);

void print(const int x);

