#include <builtin.h>
#include <lenv.h>
#include <lval.h>

#include <stdlib.h>
#include <string.h>

///////////////////////////
/// `lenv` Constructors ///
///////////////////////////

lenv* lenv_new(void)
{
    lenv* env = malloc(sizeof(lenv));

    env->par = NULL;

    env->count = 0;
    env->syms = NULL;
    env->vals = NULL;

    return env;
}

//////////////////////////
/// `lenv` Destructors ///
//////////////////////////

void lenv_del(lenv* env)
{
    for (unsigned i = 0; i < env->count; i++) {
        free(env->syms[i]);
        lval_del(env->vals[i]);
    }

    env->par = NULL;

    free(env->syms);
    free(env->vals);
    free(env);
}

//////////////////////
/// `lenv` Methods ///
//////////////////////

lval* lenv_get(lenv* env, const lval* key)
{
    for (unsigned i = 0; i < env->count; i++) {
        if (strcmp(env->syms[i], key->sym) == 0) {
            return lval_copy(env->vals[i]);
        }
    }

    if (env->par) {
        return lenv_get(env->par, key);
    }

    return lval_err("Unbound symbol '%s'", key->sym);
}

void lenv_put(lenv* env, const lval* key, lval* value)
{
    for (unsigned i = 0; i < env->count; i++) {
        if (strcmp(env->syms[i], key->sym) == 0) {
            lval_del(env->vals[i]);
            env->vals[i] = lval_copy(value);
            return;
        }
    }

    env->count++;
    env->vals = realloc(env->vals, sizeof(lval*) * env->count);
    env->syms = realloc(env->syms, sizeof(char*) * env->count);

    env->vals[env->count - 1] = lval_copy(value);
    env->syms[env->count - 1] = malloc(strlen(key->sym) + 1);
    strcpy(env->syms[env->count - 1], key->sym); // NOLINT(clang-analyzer-security.insecureAPI.strcpy)
}

lenv* lenv_copy(const lenv* env)
{
    lenv* nenv = malloc(sizeof(lenv));
    nenv->par = env->par;
    nenv->count = env->count;

    nenv->syms = malloc(sizeof(char*) * nenv->count);
    nenv->vals = malloc(sizeof(lval*) * nenv->count);

    for (unsigned i = 0; i < env->count; i++) {
        nenv->syms[i] = malloc(strlen(env->syms[i]) + 1);
        strcpy(nenv->syms[i], env->syms[i]); // NOLINT(clang-analyzer-security.insecureAPI.strcpy)
        nenv->vals[i] = lval_copy(env->vals[i]);
    }

    return nenv;
}

void lenv_def(lenv* env, const lval* key, lval* value)
{
    while (env->par) {
        env = env->par;
    }

    lenv_put(env, key, value);
}

void lenv_add_builtin(lenv* env, const char* name, lbuiltin func)
{
    lval* key = lval_sym(name);
    lval* value = lval_fun(func);
    lenv_put(env, key, value);
    lval_del(key);
    lval_del(value);
}

void lenv_add_builtins(lenv* env)
{
    lenv_add_builtin(env, "load", builtin_load);
    lenv_add_builtin(env, "print", builtin_print);
    // lenv_add_builtin(env, "input", builtin_);
    lenv_add_builtin(env, "error", builtin_error);

    lenv_add_builtin(env, "\\", builtin_lambda);
    lenv_add_builtin(env, "def", builtin_def);
    lenv_add_builtin(env, "=", builtin_put);

    lenv_add_builtin(env, "list", builtin_list);
    lenv_add_builtin(env, "head", builtin_head);
    lenv_add_builtin(env, "tail", builtin_tail);
    lenv_add_builtin(env, "eval", builtin_eval);
    lenv_add_builtin(env, "join", builtin_join);

    lenv_add_builtin(env, "+", builtin_add);
    lenv_add_builtin(env, "-", builtin_sub);
    lenv_add_builtin(env, "*", builtin_mul);
    lenv_add_builtin(env, "/", builtin_div);

    lenv_add_builtin(env, "if", builtin_if);
    lenv_add_builtin(env, "==", builtin_eq);
    lenv_add_builtin(env, "!=", builtin_ne);
    lenv_add_builtin(env, ">", builtin_gt);
    lenv_add_builtin(env, "<", builtin_lt);
    lenv_add_builtin(env, ">=", builtin_ge);
    lenv_add_builtin(env, "<=", builtin_le);
}
