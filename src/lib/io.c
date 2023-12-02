#include <io.h>
#include <parser.h>

#include <stdio.h>
#include <string.h>

///////////////////////
// `lval` Printing
///////////////////////

void lval_print(const lval* obj)
{
    switch (obj->type) {
    case LVAL_NUM:
        printf("%li", obj->num);
        break;

    case LVAL_ERR:
        printf("Error: %s", obj->err);
        break;

    case LVAL_SYM:
        printf("%s", obj->sym);
        break;

    case LVAL_STR:
        lval_print_str(obj);
        break;

    case LVAL_FUN:
        if (obj->builtin) {
            printf("<builtin>");
        } else {
            printf("(\\ ");
            lval_print(obj->formals);
            putchar(' ');
            lval_print(obj->body);
            putchar(')');
        }
        break;

    case LVAL_SEXPR:
        lval_expr_print(obj, '(', ')');
        break;

    case LVAL_QEXPR:
        lval_expr_print(obj, '{', '}');
        break;

    default:
        printf("[Internal] Print Error - Unknown Type ID: %d", obj->type);
        break;
    }
}

void lval_println(const lval* obj)
{
    lval_print(obj);
    putchar('\n');
}

///////////////////////////
// Expression Printing
///////////////////////////

void lval_expr_print(const lval* obj, char open, char close)
{
    putchar(open);

    for (unsigned i = 0; i < obj->count; i++) {
        lval_print(obj->cell[i]);

        if (i != obj->count - 1) {
            putchar(' ');
        }
    }

    putchar(close);
}

/////////////////
// String IO
/////////////////

void lval_print_str(const lval* obj)
{
    putchar('"');

    for (size_t i = 0; i < strlen(obj->str); i++) {
        if (strchr("\a\b\f\n\r\t\v\\\'\"", obj->str[i])) {
            printf("%s", lval_str_escape(obj->str[i]));
        } else {
            putchar(obj->str[i]);
        }
    }

    putchar('"');
}
