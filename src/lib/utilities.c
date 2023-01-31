#include <utilities.h>
#include <types.h>


#ifdef _WIN32
    #include <stdio.h>

    static char buffer[2048];

    char* readline(char* prompt) 
    {
        fputs(prompt, stdout);
        fgets(buffer, 2048, stdin);
        char* cpy = malloc(strlen(buffer) + 1);
        strcpy(cpy, buffer);
        cpy[strlen(cpy)-1] = '\0';
        return cpy;
    }

    void add_history(char* unused) {}
#endif


char* ltype_name(int t)
{
    switch (t)
    {
        case LVAL_FUN:
            return "Function";

        case LVAL_NUM:
            return "Number";

        case LVAL_ERR:
            return "Error";

        case LVAL_SYM:
            return "Symbol";

        case LVAL_STR:
            return "String";

        case LVAL_SEXPR:
            return "S-Expression";

        case LVAL_QEXPR:
            return "Q-Expression";

        default:
            return "Unknown";
    }
}
