#include <lix.h>

#include <stdlib.h>


int main(int argc, char* argv[])
{
    lenv* e = lenv_new();
    lenv_add_builtins(e);
    lval* p = load_prelude(e);

    if (argc == 1)
    {

        puts("Lix v0.3.1");
        puts("Press Ctrl+C to exit.\n");

        while(1)
        {
            char* input = readline("lix> ");

            add_history(input);

            int pos = 0;
            lval* expr = lval_read_expr(input, &pos, '\0');
            lval* x = lval_eval(e, expr);
            lval_println(x);
            lval_del(x);

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

    return 0;
}
