#include <builtins.h>
#include <macros.h>
#include <utilities.h>

#include <string.h>

//////////////////////////
/// Builtin Evaluaters ///
//////////////////////////

lval* builtin(lenv* e, lval* a, char* func)
{
    if (strcmp(func, "list") == 0)
        return builtin_list(e, a);

    if (strcmp(func, "head") == 0)
        return builtin_head(e, a);

    if (strcmp(func, "tail") == 0)
        return builtin_tail(e, a);

    if (strcmp(func, "eval") == 0)
        return builtin_eval(e, a);

    if (strcmp(func, "join") == 0)
        return builtin_join(e, a);

    if (strstr("+-*/", func))
        return builtin_op(e, a, func);

    lval_del(a);
    return lval_err("Unknown function!");
}


/////////////////////////
/// Builtin Operators ///
/////////////////////////

lval* builtin_op(lenv* e, lval* a, char* op)
{
    for (int i = 0; i < a->count; i++)
        LASSERT_TYPE(op, a, i, LVAL_NUM);

    lval* x = lval_pop(a, 0);

    if ((strcmp(op, "-") == 0) && a->count == 0)
        x->num = -x->num;

    while (a->count > 0)
    {
        lval* y = lval_pop(a, 0);

        if (strcmp(op, "+") == 0)
            x->num += y->num;

        if (strcmp(op, "-") == 0)
            x->num -= y->num;

        if (strcmp(op, "*") == 0)
            x->num *= y->num;

        if (strcmp(op, "/") == 0)
        {
            if (y->num == 0)
            {
                lval_del(x);
                lval_del(y);
                x = lval_err("Division by zero!");
                break;
            }

            x->num /= y->num;
        }

        lval_del(y);
    }

    lval_del(a);
    return x;
}


////////////////////////////////////
/// Builtin Arithmatic Operators ///
////////////////////////////////////

lval* builtin_add(lenv* e, lval* a)
{ 
    return builtin_op(e, a, "+"); 
}


lval* builtin_sub(lenv* e, lval* a)
{ 
    return builtin_op(e, a, "-"); 
}


lval* builtin_mul(lenv* e, lval* a)
{ 
    return builtin_op(e, a, "*"); 
}


lval* builtin_div(lenv* e, lval* a)
{ 
    return builtin_op(e, a, "/"); 
}


//////////////////////////////
/// Builtin List Operators ///
//////////////////////////////

lval* builtin_head(lenv* e, lval* a)
{
    LASSERT_NUM("head", a, 1);
    LASSERT_TYPE("head", a, 0, LVAL_QEXPR);
    LASSERT_NOT_EMPTY("head", a, 0);

    lval* v = lval_take(a, 0);

    while (v->count > 1)
        lval_del(lval_pop(v, 1));

    return v;
}


lval* builtin_tail(lenv* e, lval* a)
{
    LASSERT_NUM("tail", a, 1);
    LASSERT_TYPE("tail", a, 0, LVAL_QEXPR);
    LASSERT_NOT_EMPTY("tail", a, 0);
    
    lval* v = lval_take(a, 0);
    lval_del(lval_pop(v, 0));
    return v;
}


lval* builtin_list(lenv* e, lval* a)
{
    LASSERT_NUM("eval", a, 1);
    LASSERT_TYPE("eval", a, 0, LVAL_QEXPR);

    a->type = LVAL_QEXPR;
    return a;
}


lval* builtin_eval(lenv* e, lval* a)
{
    LASSERT(a, a->count == 1, "Function 'eval' passed too many arguments!");
    LASSERT(a, a->cell[0]->type == LVAL_QEXPR, "Function 'eval' passed incorrect type!");

    lval* x = lval_take(a, 0);
    x->type = LVAL_SEXPR;
    return lval_eval(e, x);
}


lval* builtin_join(lenv* e, lval* a)
{
    for (int i = 0; i < a->count; i++)
    LASSERT_TYPE("join", a, i, LVAL_QEXPR);
    
    lval* x = lval_pop(a, 0);
    
    while (a->count)
    {
        lval* y = lval_pop(a, 0);
        x = lval_join(x, y);
    }
    
    lval_del(a);
    return x;
}


//////////////////////////////////
/// Builtin Function Operators ///
//////////////////////////////////

lval* builtin_def(lenv* e, lval* a)
{
    return builtin_var(e, a, "def");
}


lval* builtin_put(lenv* e, lval* a)
{
    return builtin_var(e, a, "=");
}


lval* builtin_var(lenv* e, lval* a, char* func)
{
    LASSERT_TYPE(func, a, 0, LVAL_QEXPR);

    lval* syms = a->cell[0];

    for (int i = 0; i < syms->count; ++i)
        LASSERT(a, (syms->cell[i]->type == LVAL_SYM),
                "Function '%s' cannot define non-symbol. "
                "Got %s, Expected %s.", func,
                ltype_name(syms->cell[i]->type),
                ltype_name(LVAL_SYM));

    LASSERT(a, (syms->count == a->count - 1),
            "Function %s passed too many arguments for symbols. "
            "Got %i, Expected %i.", func, syms->count, a->count - 1);

    for (int i = 0; i < syms->count; ++i)
    {
        if (strcmp(func, "def") == 0)
            lenv_def(e, syms->cell[i], a->cell[i + 1]);

        if (strcmp(func, "=") == 0)
            lenv_put(e, syms->cell[i], a->cell[i + 1]);
    }

    lval_del(a);
    return lval_sexpr();
}


lval* builtin_lambda(lenv* e, lval* a)
{
    LASSERT_NUM("\\", a, 2);
    LASSERT_TYPE("\\", a, 0, LVAL_QEXPR);
    LASSERT_TYPE("\\", a, 1, LVAL_QEXPR);

    for (int i = 0; i < a->cell[0]->count; i++)
        LASSERT(a, (a->cell[0]->cell[i]->type == LVAL_SYM),
                "Cannot define non-symbol. Got %s, Expected %s.",
                ltype_name(a->cell[0]->cell[i]->type),ltype_name(LVAL_SYM));

    lval* formals = lval_pop(a, 0);
    lval* body = lval_pop(a, 0);
    lval_del(a);
    return lval_lambda(formals, body);
}
