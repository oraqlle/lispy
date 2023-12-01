#ifndef LISPY_LVAL_H
#define LISPY_LVAL_H

#include <builtins.h>
#include <lenv.h>
#include <types.h>


///////////////////////////
/// `lval` Constructors ///
///////////////////////////

/// @brief Creates an lval of type LVAL_NUM.
///
/// @details Creates an lval of type LVAL_NUM 
/// and sets the obj to the provided number `num`.
/// 
/// @param num - type: const long
/// @return lval* 
lval* lval_num(long num);  // NOLINT(google-runtime-int)


/// @brief Creates an lval of type LVAL_ERR.
///
/// @details Creates an lval of type LVAL_ERR
/// and sets the obj to the provided error
/// message l_arg.
/// 
/// @param fmt - type: char*
/// @return lval* 
lval* lval_err(const char* fmt, ...);


/// @brief Creates an lval of type LVAL_SYM.
///
/// @details Creates an lval of type LVAL_SYM
/// and sets the obj to the provided symbol
/// or operator l_arg.
///
/// @param sym - type: char*
/// @return lval*
lval* lval_sym(const char* sym);


/// TODO
lval* lval_str(const char* str);


/// @brief Creates an lval of type LVAL_SEXPR.
///
/// @details Creates an lval of type LVAL_SEXPR
/// which is an empty S-Expression.
///
/// @return lval*
lval* lval_sexpr(void);


/// @brief Creates an lval of type LVAL_QEXPR.
///
/// @details Creates an lval of type LVAL_QEXPR
/// which is an empty Q-Expression.
///
/// @return lval*
lval* lval_qexpr(void);


/// @brief Constructs an lval of type LVAL_FUN.
///
/// @details Constructs an lval of type LVAL_FUN
/// and sets the obj to the provided builtin
/// function `func`.
///
/// @param func - type: lbuiltin
/// @return lval*
lval* lval_fun(lbuiltin func);


/// @brief Constructs a lambda lval.
///
/// @details Constructs a lambda lval.
///
/// @param formals - type: lval*
/// @param body - type: lval*
/// @return lval*
lval* lval_lambda(lval* formals, lval* body);


/////////////////////////
/// `lval` Destructor ///
/////////////////////////

/// @brief Frees an lval.
///
/// @details Frees an lval and all of its children
/// (if any) as well as any other allocated resources.
///
/// @param obj - type: lval*
void lval_del(lval* obj);


//////////////////////
/// `lval` Methods ///
//////////////////////

/// @brief Adds the child l_arg to the parent `obj`.
///
/// @details Adds the child l_arg to the parent `obj`.
/// Allocates memory to `obj`'s `cell` array and 
/// assigns l_arg to the last slot of the array.
///
/// @param parent - type: lval*
/// @param child - type: const lval*
/// @return lval*
lval* lval_add(lval* parent, lval* child);


/// @brief Copies an lval.
///
/// @details Copies an lval and all of its children
/// (if any) as well as any other allocated resources.
///
/// @param obj - type: lval*
/// @return lval*
lval* lval_copy(const lval* obj);


/// @brief Pops the ith element off of the lval `obj`.
///
/// @details Pops the ith element off of the lval `obj`
/// and moves the receding elements up. Returns the
/// popped element.
///
/// @param v - type: lval*
/// @param ith - type: int
/// @return lval*
lval* lval_pop(lval* obj, unsigned ith);


/// @brief Takes the ith element off of the lval `obj`.
///
/// @details Takes the ith element off of the lval `obj`
/// and discards all other objs. Returns the taken 
/// element.
///
/// @param obj - type: lval*
/// @param ith - type: int
/// @return lval*
lval* lval_take(lval* obj, unsigned ith);

/// @brief Evaluates the lval `obj`.
///
/// @details Evaluates the lval `obj`.
/// Returns `obj` as-is it is not an 
/// S-Expression.
///
/// @param env - type: lenv*
/// @param obj - type: lval*
/// @return lval*
lval* lval_eval(lenv* env, lval* obj);


/// TODO
lval* lval_call(lenv* env, lval* func, lval* arg);


/// @brief Evaluates the lval `obj` as an S-Expression.
///
/// @details Evaluates the lval `obj` as an S-Expression.
/// Returns `obj` as-is if it has no children or returns the
/// child if it only has one child. Returns an error if the
/// child does not start with a symbol.
///
/// @param env - type: lenv*
/// @param sexpr - type: lval*
/// @return lval*
lval* lval_eval_sexpr(lenv* env, lval* sexpr);


/// @brief Joins the Q-Expression r_arg to l_arg.
///
/// @details Joins the Q-Expression r_arg to l_arg 
/// by continually popping the first element 
/// off r_arg and adding it to l_arg. Returns l_arg.
///
/// @param l_arg - type: lval*
/// @param r_arg - type: lval*
/// @return lval*
lval* lval_join(lval* l_arg, lval* r_arg);


/// TODO
int lval_eq(lval* l_arg, lval* r_arg);


////////////////////
/// Prelude Load ///
////////////////////

/// TODO
lval* load_prelude(lenv* env);

#endif  /// LISPY_LVAL_H
