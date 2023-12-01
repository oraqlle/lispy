#include <lispy.h>

#include <replxx.h>

#include <errno.h>
#include <stdlib.h>
#include <stdio.h>


int main(int argc, char* argv[])
{
    lenv* denv = lenv_new();
    lenv_add_builtins(denv);
    lval* pre = load_prelude(denv);

    Replxx* replxx = replxx_init();
    replxx_install_window_change_handler(replxx);

    if (argc == 1)
    {

        puts("Lispy v0.3.1");
        puts("Press Ctrl+D to exit.\n");

        while(1)
        {
            char const* input = NULL;
            do {
                input = replxx_input(replxx, "Lispy> ");
            } while ((input == NULL) && (errno == EAGAIN));

            if (input == NULL) {
			    printf("\n");
			    break;
		    }

            if (*input != '\0') {
                replxx_history_add(replxx, input);

                int pos = 0;
                lval* expr = lval_read_expr(input, &pos, '\0');
                lval* evald_expr = lval_eval(denv, expr);
                lval_println(evald_expr);
                lval_del(evald_expr);
            }
        }
    }

    if (argc >= 2){
        for (int i = 1; i < argc; ++i)
        {
            lval* args = lval_add(lval_sexpr(), lval_str(argv[i]));
            lval* loaded = builtin_load(denv, args);

            if (loaded->type == LVAL_ERR){
                lval_println(loaded);
            }

            lval_del(loaded);
        }
    }

    replxx_end( replxx );
    lval_del(pre);
    lenv_del(denv);

    return 0;
}
