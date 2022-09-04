#include <curlyc.h>
#include <mpc.h>

#include <stdlib.h>

int main()
{
    Number = mpc_new("number");
    Symbol = mpc_new("symbol");
    String = mpc_new("string");
    Comment = mpc_new("comment");
    Sexpr = mpc_new("sexpr");
    Qexpr = mpc_new("qexpr");
    Expr = mpc_new("expr");
    Curly = mpc_new("curly");


    mpca_lang(MPCA_LANG_DEFAULT,
        "                                                           \
            number      : /-?[0-9]+/ ;                              \
            symbol      : /[a-zA-Z0-9_+\\-*\\/\\\\=<>!&]+/ ;        \
            string      : /\"(\\\\.|[^\"])*\"/ ;                    \
            comment     : /;[^\\r\\n]*/ ;                           \
            sexpr       : '(' <expr>* ')' ;                         \
            qexpr       : '{' <expr>* '}' ;                         \
            expr        : <number>  | <symbol> | <string>           \
                        | <comment> | <sexpr>  | <qexpr>  ;         \
            curly       : /^/ <expr>* /$/ ;                         \
        ",
        Number, Symbol, String, Comment, Sexpr, Qexpr, Expr, Curly);


    puts("Curly v0.0.19");
    puts("Press Ctrl+C to exit.\n");

    lenv* e = lenv_new();
    lenv_add_builtins(e);

    while(1)
    {
        char* input = readline("curly> ");

        add_history(input);

        mpc_result_t r;

        if (mpc_parse("<stdin>", input, Curly, &r))
        {
            lval* x = lval_eval(e, lval_read(r.output));
            lval_println(x);
            lval_del(x);
        }
        else
        {
            mpc_err_print(r.error);
            mpc_err_delete(r.error);
        }

        free(input);
    }

    lenv_del(e);
    mpc_cleanup(8, Number, Symbol, String, Comment, Sexpr, Qexpr, Expr, Curly);

    return 0;
}