#include <lval.h>

#include <builtin.h>
#include <io.h>
#include <lenv.h>
#include <macros.h>
#include <utilities.h>

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

///////////////////////////
/// `lval` Constructors ///
///////////////////////////

lval* lval_num(const long num)
{
    lval* nnumval = malloc(sizeof(lval));
    nnumval->type = LVAL_NUM;
    nnumval->num = num;
    return nnumval;
}

lval* lval_err(const char* fmt, ...)
{
    lval* nerrval = malloc(sizeof(lval));
    nerrval->type = LVAL_ERR;

    va_list var_list;
    va_start(var_list, fmt);

    nerrval->err = malloc(MAX_ERR_STR_SIZE);
    vsnprintf(nerrval->err, MAX_ERR_STR_SIZE - 1, fmt, var_list); // NOLINT(clang-analyzer-security.insecureAPI.DeprecatedOrUnsafeBufferHandling, clang-diagnostic-format-nonliteral)

    nerrval->err = realloc(nerrval->err, strlen(nerrval->err) + 1);

    va_end(var_list);
    return nerrval;
}

lval* lval_sym(const char* sym)
{
    lval* nsymval = malloc(sizeof(lval));
    nsymval->type = LVAL_SYM;
    nsymval->sym = malloc(strlen(sym) + 1);
    strcpy(nsymval->sym, sym); // NOLINT(clang-analyzer-security.insecureAPI.strcpy)
    return nsymval;
}

lval* lval_str(const char* str)
{
    lval* nstrval = malloc(sizeof(lval));
    nstrval->type = LVAL_STR;
    nstrval->str = malloc(strlen(str) + 1);
    strcpy(nstrval->str, str); // NOLINT(clang-analyzer-security.insecureAPI.strcpy)
    return nstrval;
}

lval* lval_sexpr(void)
{
    lval* nsexprval = malloc(sizeof(lval));
    nsexprval->type = LVAL_SEXPR;
    nsexprval->count = 0;
    nsexprval->cell = NULL;
    return nsexprval;
}

lval* lval_qexpr(void)
{
    lval* nqexprval = malloc(sizeof(lval));
    nqexprval->type = LVAL_QEXPR;
    nqexprval->count = 0;
    nqexprval->cell = NULL;
    return nqexprval;
}

lval* lval_fun(lbuiltin func)
{
    lval* nfunval = malloc(sizeof(lval));
    nfunval->type = LVAL_FUN;
    nfunval->builtin = func;
    return nfunval;
}

lval* lval_lambda(lval* formals, lval* body) // NOLINT(bugprone-easily-swappable-parameters)
{
    lval* nlambdaval = malloc(sizeof(lval));
    nlambdaval->type = LVAL_FUN;

    nlambdaval->builtin = NULL;

    nlambdaval->env = lenv_new();

    nlambdaval->formals = formals;
    nlambdaval->body = body;
    return nlambdaval;
}

/////////////////////////
/// `lval` Destructor ///
/////////////////////////

void lval_del(lval* obj)
{
    switch (obj->type) {
    case LVAL_NUM:
        break;

    case LVAL_ERR:
        free(obj->err);
        break;

    case LVAL_SYM:
        free(obj->sym);
        break;

    case LVAL_STR:
        free(obj->str);
        break;

    case LVAL_FUN:
        if (!obj->builtin) {
            lenv_del(obj->env);
            lval_del(obj->formals);
            lval_del(obj->body);
        }
        break;

    case LVAL_QEXPR:
    case LVAL_SEXPR:
        for (unsigned i = 0; i < obj->count; i++) {
            lval_del(obj->cell[i]);
        }

        free(obj->cell);
        break;
    }

    free(obj);
}

//////////////////////
/// `lval` Methods ///
//////////////////////

lval* lval_add(lval* parent, lval* child)
{
    parent->count++;
    parent->cell = realloc(parent->cell, sizeof(lval*) * parent->count);
    parent->cell[parent->count - 1] = child;
    return parent;
}

lval* lval_copy(const lval* obj)
{
    lval* nval = malloc(sizeof(lval));
    nval->type = obj->type;

    switch (obj->type) {
    case LVAL_FUN:
        if (obj->builtin) {
            nval->builtin = obj->builtin;
        } else {
            nval->builtin = NULL;
            nval->env = lenv_copy(obj->env);
            nval->formals = lval_copy(obj->formals);
            nval->body = lval_copy(obj->body);
        }
        break;

    case LVAL_NUM:
        nval->num = obj->num;
        break;

    case LVAL_ERR:
        nval->err = malloc(strlen(obj->err) + 1);
        strcpy(nval->err, obj->err); // NOLINT(clang-analyzer-security.insecureAPI.strcpy)
        break;

    case LVAL_SYM:
        nval->sym = malloc(strlen(obj->sym) + 1);
        strcpy(nval->sym, obj->sym); // NOLINT(clang-analyzer-security.insecureAPI.strcpy)
        break;

    case LVAL_STR:
        nval->str = malloc(strlen(obj->str) + 1);
        strcpy(nval->str, obj->str); // NOLINT(clang-analyzer-security.insecureAPI.strcpy)
        break;

    case LVAL_SEXPR:
    case LVAL_QEXPR:
        nval->count = obj->count;
        nval->cell = malloc(sizeof(lval*) * nval->count);
        for (unsigned i = 0; i < nval->count; i++) {
            nval->cell[i] = lval_copy(obj->cell[i]);
        }

        break;
    }

    return nval;
}

lval* lval_pop(lval* obj, unsigned ith)
{
    if (obj->count == 0) {
        return lval_err("Symbol: '%s' has no children", obj->sym);
    }

    lval* popd = obj->cell[ith];
    memmove(&obj->cell[ith], &obj->cell[ith + 1], sizeof(lval*) * (obj->count - ith - 1)); // NOLINT(clang-analyzer-security.insecureAPI.DeprecatedOrUnsafeBufferHandling)
    obj->count--;
    obj->cell = realloc(obj->cell, sizeof(lval*) * obj->count);

    return popd;
}

lval* lval_take(lval* obj, unsigned ith)
{
    lval* popd = lval_pop(obj, ith);
    lval_del(obj);
    return popd;
}

lval* lval_eval(lenv* env, lval* obj)
{
    if (obj->type == LVAL_SYM) {
        lval* sub = lenv_get(env, obj);
        lval_del(obj);
        return sub;
    }

    if (obj->type == LVAL_SEXPR) {
        return lval_eval_sexpr(env, obj);
    }

    return obj;
}

lval* lval_call(lenv* env, lval* func, lval* arg)
{
    if (func->builtin) {
        return func->builtin(env, arg);
    }

    unsigned given = arg->count;
    unsigned total = func->formals->count;

    while (arg->count) {
        if (func->formals->count == 0) {
            lval_del(arg);
            return lval_err("Function passed too many arguments. "
                            "Got %i, Expected %i. ",
                given, total);
        }

        lval* sym = lval_pop(func->formals, 0);

        if (strcmp(sym->sym, "&") == 0) {
            if (func->formals->count != 1) {
                lval_del(arg);

                return lval_err("Function format invalid. "
                                "Symbol '&' not followed by single symbol.");
            }

            lval* nextsym = lval_pop(func->formals, 0);
            lenv_put(func->env, nextsym, builtin_list(env, arg));
            lval_del(sym);
            lval_del(nextsym);
            break;
        }

        lval* popd = lval_pop(arg, 0);
        lenv_put(func->env, sym, popd);

        lval_del(sym);
        lval_del(popd);
    }

    lval_del(arg);

    if (func->formals->count > 0 && strcmp(func->formals->cell[0]->sym, "&") == 0) {

        if (func->formals->count != 2) {
            return lval_err("Function format invalid. "
                            "Symbol '&' not followed by single symbol.");
        }

        lval_del(lval_pop(func->formals, 0));

        lval* sym = lval_pop(func->formals, 0);
        lval* val = lval_qexpr();

        lenv_put(func->env, sym, val);
        lval_del(sym);
        lval_del(val);
    }

    if (func->formals->count == 0) {
        func->env->par = env;
        return builtin_eval(func->env, lval_add(lval_sexpr(), lval_copy(func->body)));
    }

    return lval_copy(func);
}

lval* lval_eval_sexpr(lenv* env, lval* sexpr)
{
    for (unsigned i = 0; i < sexpr->count; i++) {
        sexpr->cell[i] = lval_eval(env, sexpr->cell[i]);
    }

    for (unsigned i = 0; i < sexpr->count; i++) {
        if (sexpr->cell[i]->type == LVAL_ERR) {
            return lval_take(sexpr, i);
        }
    }

    if (sexpr->count == 0) {
        return sexpr;
    }

    if (sexpr->count == 1) {
        return lval_take(sexpr, 0);
    }

    lval* func = lval_pop(sexpr, 0);

    if (func->type != LVAL_FUN) {
        lval* err = lval_err("S-Expression starts with incorrect type. ",
            "Got %s, Expected %s. ",
            ltype_name(func->type), ltype_name(LVAL_FUN));

        lval_del(func);
        lval_del(sexpr);
        return err;
    }

    lval* result = lval_call(env, func, sexpr);
    lval_del(func);

    return result;
}

lval* lval_join(lval* l_arg, lval* r_arg)
{
    while (r_arg->count) {
        l_arg = lval_add(l_arg, lval_pop(r_arg, 0));
    }

    lval_del(r_arg);
    return l_arg;
}

int lval_eq(lval* l_arg, lval* r_arg)
{
    if (l_arg->type != r_arg->type) {
        return 0;
    }

    switch (l_arg->type) {
    case LVAL_NUM:
        return (l_arg->num == r_arg->num);

    case LVAL_ERR:
        return (strcmp(l_arg->err, r_arg->err) == 0);

    case LVAL_SYM:
        return (strcmp(l_arg->sym, r_arg->sym) == 0);

    case LVAL_STR:
        return (strcmp(l_arg->str, r_arg->str) == 0);

    case LVAL_FUN:
        if (l_arg->builtin || r_arg->builtin) {
            return (l_arg->builtin == r_arg->builtin);
        }

        return (lval_eq(l_arg->formals, r_arg->formals)
            && lval_eq(l_arg->body, r_arg->body));

    case LVAL_QEXPR:
    case LVAL_SEXPR:
        if (l_arg->count != r_arg->count) {
            return 0;
        }

        for (unsigned i = 0; i < l_arg->count; ++i) {
            if (!lval_eq(l_arg->cell[i], r_arg->cell[i])) {
                return 0;
            }
        }

        return 1;

        break;

    default:
        printf("[Internal] Compare Error - Unknown Type ID: %d", l_arg->type);
        break;
    }

    return 0;
}

////////////////////
/// Prelude Load ///
////////////////////

lval* load_prelude(lenv* env)
{
#define PRELUDE_PATH_SIZE 100

    char prelude_path[PRELUDE_PATH_SIZE];

#ifdef _WIN32
    const char* envvar = "USERPROFILE";
#else
    const char* envvar = "HOME";
#endif /// _WIN32

    if (!getenv(envvar)) // NOLINT(concurrency-mt-unsafe)
    {
        fprintf(stderr, "The environment variable %s was not found.\n", envvar);
        exit(1); // NOLINT(concurrency-mt-unsafe)
    }

    if (snprintf(prelude_path, PRELUDE_PATH_SIZE, "%s/.lispy/stdlib/prelude.lpy", getenv(envvar)) >= PRELUDE_PATH_SIZE) // NOLINT(concurrency-mt-unsafe, clang-analyzer-security.insecureAPI.DeprecatedOrUnsafeBufferHandling)
    {
        fprintf(stderr, "PRELUDE_PATH_SIZE of %d was too small. Aborting\n", PRELUDE_PATH_SIZE);
        exit(1); // NOLINT(concurrency-mt-unsafe)
    }

    lval* prelude = lval_add(lval_sexpr(), lval_str(prelude_path));
    lval* prld = builtin_load(env, prelude);

    if (prld->type == LVAL_ERR) {
        lval_println(prld);
    }

    return prld;
}
