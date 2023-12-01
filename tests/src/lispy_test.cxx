#include <catch2/catch_test_macros.hpp>

extern "C" {
    #include <lispy.h>

    #include <replxx.h>

    #include <errno.h>
    #include <stdlib.h>
}

TEST_CASE("Lispy Test", "[library]")
{
    lenv* denv = lenv_new();
    lenv_add_builtins(denv);
    lval* pre = load_prelude(denv);

    Replxx* replxx = replxx_init();
    replxx_install_window_change_handler(replxx);

    // if (argc == 1)
    // {

    //     puts("Lispy v0.3.1");
    //     puts("Press Ctrl+C to exit.\n");

    //     while(1)
    //     {
    //         char const* input = NULL;
    //         do {
    //             input = replxx_input(replxx, "Lispy> ");
    //         } while ((input == NULL) && (errno == EAGAIN));

    //         if (input == NULL) {
    //              printf("\n");
    //              break;
    //         }

    //         if (*input != '\0') {
    //             replxx_history_add(replxx, input);

    //             int pos = 0;
    //             lval* expr = lval_read_expr(input, &pos, '\0');
    //             lval* x = lval_eval(denv, expr);
    //             lval_println(x);
    //             lval_del(x);
    //         }

    //         free(input);
    //     }
    // }

    // if (argc >= 2) {
    //     for (int i = 1; i < argc; ++i)
    //     {
    //         lval* argrgs = lval_add(lval_sexpr(), lval_str(argv[i]));
    //         lval* x = builtin_load(denv, args);

    //         if (x->type == LVAL_ERR)
    //             lval_println(x);

    //         lval_del(x);
    //     }
    // }

    replxx_end( replxx );
    lval_del(pre);
    lenv_del(denv);
}
