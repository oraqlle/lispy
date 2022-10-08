#include <lix.h>
#include <mpc.h>

#include <stdlib.h>


int main(int argc, char* argv[])
{
    mpc_parser_t* Number = mpc_new("number");
    mpc_parser_t* Symbol = mpc_new("symbol");
    mpc_parser_t* String = mpc_new("string");
    mpc_parser_t* Comment = mpc_new("comment");
    mpc_parser_t* Sexpr = mpc_new("sexpr");
    mpc_parser_t* Qexpr = mpc_new("qexpr");
    mpc_parser_t* Expr = mpc_new("expr");
    mpc_parser_t* Lix = mpc_new("lix");


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
            lix         : /^/ <expr>* /$/ ;                         \
        ",
        Number, Symbol, String, Comment, Sexpr, Qexpr, Expr, Lix);

    lenv* e = lenv_new(Lix);
    lenv_add_builtins(e);
    lval* p = load_prelude(e);

    if (argc == 1)
    {

        puts("Lix v0.1.1");
        puts("Press Ctrl+C to exit.\n");

        while(1)
        {
            char* input = readline("lix> ");

            add_history(input);

            int pos = 0;
            lval* expr = lval_read_expr(input, &pos, '\0');
            lval* x = lval_eval(e, expr);

            free(input);
        }
    }

    if (argc >= 2)
        for (int i = 1; i < argc; ++i)
        {
            lval* args = lval_add(lval_sexpr(), lval_str(argv[i]));
            lval* x = builtin_load(e, args);

            if (x->type == LVAL_ERR)
                lval_println(x);

            lval_del(x);
        }

    lval_del(p);
    lenv_del(e);
    mpc_cleanup(8, Number, Symbol, String, Comment, Sexpr, Qexpr, Expr, Lix);

    return 0;
}
