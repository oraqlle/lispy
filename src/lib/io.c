#include <io.h>
#include <parser.h>

#include <stdio.h>
#include <string.h>

///////////////////////
/// `lval` Printing ///
///////////////////////

void lval_print(lval* v)
{
    switch (v->type)
    {
        case LVAL_NUM:
            printf("%li", v->num);
            break;

        case LVAL_ERR:
            printf("Error: %s", v->err);
            break;
        
        case LVAL_SYM:
            printf("%s", v->sym);
            break;

        case LVAL_STR:
            lval_print_str(v);
            break;

        case LVAL_FUN:
            if (v->builtin)
                printf("<builtin>");
            else
            {
                printf("(\\ ");
                lval_print(v->formals);
                putchar(' ');
                lval_print(v->body);
                putchar(')');
            }
            break;

        case LVAL_SEXPR:
            lval_expr_print(v, '(', ')');
            break;

        case LVAL_QEXPR:
            lval_expr_print(v, '{', '}');
            break;
    }
}


void lval_println(lval* v)
{
    lval_print(v);
    putchar('\n');
}


///////////////////////////
/// Expression Printing ///
///////////////////////////

void lval_expr_print(lval* v, char open, char close)
{
    putchar(open);

    for (int i = 0; i < v->count; i++)
    {
        lval_print(v->cell[i]);

        if (i != v->count - 1)
            putchar(' ');
    }

    putchar(close);
}


/////////////////
/// String IO ///
/////////////////

void lval_print_str(lval* v)
{
    putchar('"');

    for (int i = 0; i < strlen(v->str); i++)
        if (strchr("\a\b\f\n\r\t\v\\\'\"", v->str[i]))
            printf("%s", lval_str_escape(v->str[i]));
        else
            putchar(v->str[i]);

    putchar('"');
}
