#include <builtins.h>
#include <io.h>
#include <macros.h>
#include <parser.h>
#include <types.h>
#include <utilities.h>

#include <stdio.h>
#include <string.h>

//////////////////////////
/// Builtin Evaluators ///
//////////////////////////

lval* builtin(lenv* env, lval* arg, char* func)
{
    if (strcmp(func, "list") == 0){
        return builtin_list(env, arg);
    }

    if (strcmp(func, "head") == 0){
        return builtin_head(env, arg);
    }

    if (strcmp(func, "tail") == 0){
        return builtin_tail(env, arg);
    }

    if (strcmp(func, "eval") == 0){
        return builtin_eval(env, arg);
    }

    if (strcmp(func, "join") == 0){
        return builtin_join(env, arg);
    }

    if (strstr("+-*/", func)){
        return builtin_op(env, arg, func);
    }

    lval_del(arg);
    return lval_err("Unknown function!");
}


/////////////////////////
/// Builtin Operators ///
/////////////////////////

lval* builtin_op(lenv* env, lval* arg, char* operand)
{
    for (unsigned i = 0; i < arg->count; i++){
        LASSERT_TYPE(operand, arg, i, LVAL_NUM)
    }

    lval* l_expr = lval_pop(arg, 0);

    if ((strcmp(operand, "-") == 0) && arg->count == 0){
        l_expr->num = -l_expr->num;
    }

    while (arg->count > 0)
    {
        lval* r_expr = lval_pop(arg, 0);

        if (strcmp(operand, "+") == 0)
            l_expr->num += r_expr->num;

        if (strcmp(operand, "-") == 0)
            l_expr->num -= r_expr->num;

        if (strcmp(operand, "*") == 0)
            l_expr->num *= r_expr->num;

        if (strcmp(operand, "/") == 0)
        {
            if (r_expr->num == 0)
            {
                lval_del(l_expr);
                lval_del(r_expr);
                l_expr = lval_err("Division by zero!");
                break;
            }

            l_expr->num /= r_expr->num;
        }

        lval_del(r_expr);
    }

    lval_del(arg);
    return l_expr;
}


////////////////////////////////////
/// Builtin Arithmetic Operators ///
////////////////////////////////////

lval* builtin_add(lenv* env, lval* arg)
{ 
    return builtin_op(env, arg, "+"); 
}


lval* builtin_sub(lenv* env, lval* arg)
{ 
    return builtin_op(env, arg, "-"); 
}


lval* builtin_mul(lenv* env, lval* arg)
{ 
    return builtin_op(env, arg, "*"); 
}


lval* builtin_div(lenv* env, lval* arg)
{ 
    return builtin_op(env, arg, "/"); 
}


//////////////////////////////
/// Builtin List Operators ///
//////////////////////////////

lval* builtin_head(lenv* env, lval* arg)
{
    LASSERT_NUM("head", arg, 1);
    LASSERT_TYPE("head", arg, 0, LVAL_QEXPR);
    LASSERT_NOT_EMPTY("head", arg, 0);

    lval* taken = lval_take(arg, 0);

    while (taken->count > 1)
        lval_del(lval_pop(taken, 1));

    return taken;
}


lval* builtin_tail(lenv* env, lval* arg)
{
    LASSERT_NUM("tail", arg, 1);
    LASSERT_TYPE("tail", arg, 0, LVAL_QEXPR);
    LASSERT_NOT_EMPTY("tail", arg, 0);
    
    lval* taken = lval_take(arg, 0);
    lval_del(lval_pop(taken, 0));
    return taken;
}


lval* builtin_list(lenv* env, lval* arg)
{
    arg->type = LVAL_QEXPR;
    return arg;
}


lval* builtin_eval(lenv* env, lval* arg)
{
    LASSERT(arg, arg->count == 1, "Function 'eval' passed too many arguments!");
    LASSERT(arg, arg->cell[0]->type == LVAL_QEXPR, "Function 'eval' passed incorrect type!");

    lval* taken = lval_take(arg, 0);
    taken->type = LVAL_SEXPR;
    return lval_eval(env, taken);
}


lval* builtin_join(lenv* env, lval* arg)
{
    for (unsigned i = 0; i < arg->count; i++)
    LASSERT_TYPE("join", arg, i, LVAL_QEXPR);
    
    lval* l_arg = lval_pop(arg, 0);
    
    while (arg->count)
    {
        lval* r_arg = lval_pop(arg, 0);
        l_arg = lval_join(l_arg, r_arg);
    }
    
    lval_del(arg);
    return l_arg;
}


//////////////////////////////////
/// Builtin Function Operators ///
//////////////////////////////////

lval* builtin_def(lenv* env, lval* arg)
{
    return builtin_var(env, arg, "def");
}


lval* builtin_put(lenv* env, lval* arg)
{
    return builtin_var(env, arg, "=");
}


lval* builtin_var(lenv* env, lval* arg, char* func)
{
    LASSERT_TYPE(func, arg, 0, LVAL_QEXPR);

    const lval* syms = arg->cell[0];

    for (unsigned i = 0; i < syms->count; ++i)
        LASSERT(arg, (syms->cell[i]->type == LVAL_SYM),
                "Function '%s' cannot define non-symbol. "
                "Got %s, Expected %s.", func,
                ltype_name(syms->cell[i]->type),
                ltype_name(LVAL_SYM))

    LASSERT(arg, (syms->count == arg->count - 1),
            "Function %s passed too many arguments for symbols. "
            "Got %i, Expected %i.", func, syms->count, arg->count - 1)

    for (int i = 0; i < syms->count; ++i)
    {
        if (strcmp(func, "def") == 0)
            lenv_def(env, syms->cell[i], arg->cell[i + 1]);

        if (strcmp(func, "=") == 0)
            lenv_put(env, syms->cell[i], arg->cell[i + 1]);
    }

    lval_del(arg);
    return lval_sexpr();
}


lval* builtin_lambda(lenv* env, lval* arg)
{
    LASSERT_NUM("\\", arg, 2);
    LASSERT_TYPE("\\", arg, 0, LVAL_QEXPR);
    LASSERT_TYPE("\\", arg, 1, LVAL_QEXPR);

    for (int i = 0; i < arg->cell[0]->count; i++)
        LASSERT(arg, (arg->cell[0]->cell[i]->type == LVAL_SYM),
                "Cannot define non-symbol. Got %s, Expected %s.",
                ltype_name(arg->cell[0]->cell[i]->type), ltype_name(LVAL_SYM));

    lval* formals = lval_pop(arg, 0);
    lval* body = lval_pop(arg, 0);
    lval_del(arg);
    return lval_lambda(formals, body);
}


//////////////////////////
/// Ordering Operators ///
//////////////////////////

lval* builtin_ord(lenv* env, lval* arg, char* operand)
{
    LASSERT_NUM(operand, arg, 2);
    LASSERT_TYPE(operand, arg, 0, LVAL_NUM);
    LASSERT_TYPE(operand, arg, 1, LVAL_NUM);

    int rint = 0;

    if (strcmp(operand, ">") == 0) {
        rint = (arg->cell[0]->num > arg->cell[1]->num);
    }

    if (strcmp(operand, "<") == 0) {
        rint = (arg->cell[0]->num < arg->cell[1]->num);
    }

    if (strcmp(operand, ">=") == 0) {
        rint = (arg->cell[0]->num >= arg->cell[1]->num);
    }

    if (strcmp(operand, "<=") == 0) {
        rint = (arg->cell[0]->num <= arg->cell[1]->num);
    }

    lval_del(arg);
    return lval_num(rint);
}


lval* builtin_gt(lenv* env, lval* arg)
{
    return builtin_ord(env, arg, ">");
}


lval* builtin_lt(lenv* env, lval* arg)
{
    return builtin_ord(env, arg, "<");
}


lval* builtin_ge(lenv* env, lval* arg)
{
    return builtin_ord(env, arg, ">=");
}

lval* builtin_le(lenv* env, lval* arg)
{
    return builtin_ord(env, arg, "<=");
}


//////////////////////////
/// Equality Operators ///
//////////////////////////

lval* builtin_cmp(lenv* env, lval* arg, char* operand)
{
    LASSERT_NUM(operand, arg, 2)

    int result = 0;

    if (strcmp(operand, "==") == 0){ 
        result = lval_eq(arg->cell[0], arg->cell[1]);
    }

    if (strcmp(operand, "!=") == 0) {
        result = !lval_eq(arg->cell[0], arg->cell[1]);
    }

    lval_del(arg);
    return lval_num(result);
}


lval* builtin_eq(lenv* env, lval* arg)
{
    return builtin_cmp(env, arg, "==");
}


lval* builtin_ne(lenv* env, lval* arg)
{
    return builtin_cmp(env, arg, "!=");
}


////////////////////////////
/// Comparison Functions ///
////////////////////////////

/// TODO
lval* builtin_if(lenv* env, lval* arg)
{
    LASSERT_NUM("if", arg, 3)
    LASSERT_TYPE("if", arg, 0, LVAL_NUM)
    LASSERT_TYPE("if", arg, 1, LVAL_QEXPR)
    LASSERT_TYPE("if", arg, 2, LVAL_QEXPR)

    lval* evald_expr = NULL;

    arg->cell[1]->type = LVAL_SEXPR;
    arg->cell[2]->type = LVAL_SEXPR;

    if (arg->cell[0]->num) {
        evald_expr = lval_eval(env, lval_pop(arg, 1));
    } else {
        evald_expr = lval_eval(env, lval_pop(arg, 2));
    }

    lval_del(arg);
    return evald_expr;
}


////////////////////////////
/// Builtin IO functions ///
////////////////////////////

lval* builtin_load(lenv* env, lval* arg)
{
    LASSERT_NUM("load", arg, 1)
    LASSERT_TYPE("load", arg, 0, LVAL_STR)

    FILE* file = fopen(arg->cell[0]->str, "rb");

    if (file == NULL)
    {
        lval* err = lval_err("Could not load library %s", arg->cell[0]->str);
        lval_del(arg);
        return err;
    }

    fseek(file, 0, SEEK_END);
    long length = ftell(file);
    fseek(file, 0, SEEK_SET);
    char* input = calloc(length + 1, 1);
    fread(input, 1, length, file);
    fclose(file);

    int pos = 0;
    lval* expr = lval_read_expr(input, &pos, '\0');
    free(input);

    if (expr->type != LVAL_ERR){
        while (expr->count)
        {
            lval* evald_expr = lval_eval(env, lval_pop(expr, 0));
            if (evald_expr->type == LVAL_ERR){
                lval_println(evald_expr);
            }
            
            lval_del(evald_expr);
        }
    }    else{
        lval_println(expr);
    }

    lval_del(expr);
    lval_del(arg);

    return lval_sexpr();
}


lval* builtin_print(lenv* env, lval* arg)
{
    for (unsigned i = 0; i < arg->count; ++i)
    {
        lval_print(arg->cell[i]);
        putchar(' ');
    }

    putchar('\n');
    lval_del(arg);

    return lval_sexpr();
}


/// TODO
// lval* builtin_input(lenv* env, lval* arg);


lval* builtin_error(const lenv* env, lval* arg)
{
    LASSERT_NUM("error", arg, 1)
    LASSERT_TYPE("error", arg, 0, LVAL_STR)

    lval* err = lval_err(arg->cell[0]->str);
    lval_del(arg);

    return err;
}
