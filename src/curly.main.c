#include <curlyc.h>
#include <mpc.h>

#include <stdlib.h>

int main()
{
    mpc_parser_t* Number = mpc_new("number");
    mpc_parser_t* Symbol = mpc_new("symbol");
    mpc_parser_t* Sexpr = mpc_new("sexpr");
    mpc_parser_t* Qexpr = mpc_new("qexpr");
    mpc_parser_t* Expr = mpc_new("expr");
    mpc_parser_t* Curly = mpc_new("curly");


    mpca_lang(MPCA_LANG_DEFAULT,
        "                                                               \
            number      : /-?[0-9]+/ ;                                  \
            symbol      : /[a-zA-Z0-9_+\\-*\\/\\\\=<>!&]+/ ;             \
            sexpr       : '(' <expr>* ')' ;                             \
            qexpr       : '{' <expr>* '}' ;                             \
            expr        : <number> | <symbol> | <sexpr> | <qexpr> ;     \
            curly       : /^/ <expr>* /$/ ;                             \
        ",
        Number, Symbol, Sexpr, Qexpr, Expr, Curly);


    puts("Curly v0.0.17");
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
    mpc_cleanup(6, Number, Symbol, Sexpr, Qexpr, Expr, Curly);

    return 0;
}