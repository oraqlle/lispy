#include <lenv.h>

struct lenv 
{
    int count;
    char** syms;
    lval** vals;
};