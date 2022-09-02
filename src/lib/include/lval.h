#ifndef CURLY_LVAL_H
#define CURLY_LVAL_H

#include <builtins.h>
#include <lenv.h>
#include <types.h>


///////////////////////////
/// `lval` Constructors ///
///////////////////////////

/// \brief Creates an lval of type LVAL_NUM.
///
/// \details Creates an lval of type LVAL_NUM 
/// and sets the value to the provided number `x`.
/// 
/// \param x - type: long
/// \return lval* 
lval* lval_num(long x);


/// \brief Creates an lval of type LVAL_ERR.
///
/// \details Creates an lval of type LVAL_ERR
/// and sets the value to the provided error
/// message `x`.
/// 
/// \param m - type: char*
/// \return lval* 
lval* lval_err(char* fmt, ...);


/// \brief Creates an lval of type LVAL_SYM.
///
/// \details Creates an lval of type LVAL_SYM
/// and sets the value to the provided symbol
/// or operator `x`.
///
/// \param x - type: char*
/// \return lval*
lval* lval_sym(char* s);


/// \brief Creates an lval of type LVAL_SEXPR.
///
/// \details Creates an lval of type LVAL_SEXPR
/// which is an empty S-Expression.
///
/// \return lval*
lval* lval_sexpr(void);


/// \brief Creates an lval of type LVAL_QEXPR.
///
/// \details Creates an lval of type LVAL_QEXPR
/// which is an empty Q-Expression.
///
/// \return lval*
lval* lval_qexpr(void);


/// \brief Constructs an lval of type LVAL_FUN.
///
/// \details Constructs an lval of type LVAL_FUN
/// and sets the value to the provided builtin
/// function `func`.
///
/// \param func - type: lbuiltin
/// \return lval*
lval* lval_fun(lbuiltin func);


/// \brief Constructs a lambda lval.
///
/// \details Constructs a lambda lval.
///
/// \param formals - type: lval*
/// \param body - type: lval*
/// \return lval*
lval* lval_lambda(lval* formals, lval* body);


/////////////////////////
/// `lval` Destructor ///
/////////////////////////

/// \brief Frees an lval.
///
/// \details Frees an lval and all of its children
/// (if any) as well as any other allocated resources.
///
/// \param v - type: lval*
void lval_del(lval* v);


//////////////////////
/// `lval` Methods ///
//////////////////////

/// \brief Adds the child `x` to the parent `v`.
///
/// \details Adds the child `x` to the parent `v`.
/// Allocates memory to `v`'s `cell` array and 
/// assigns `x` to the last slot of the array.
///
/// \param v - type: lval*
/// \param x - type: lval*
/// \return lval*
lval* lval_add(lval* v, lval* x);


/// \brief Copies an lval.
///
/// \details Copies an lval and all of its children
/// (if any) as well as any other allocated resources.
///
/// \param v - type: lval*
/// \return lval*
lval* lval_copy(lval* v);


/// \brief Pops the ith element off of the lval `v`.
///
/// \details Pops the ith element off of the lval `v`
/// and moves the receding elements up. Returns the
/// popped element.
///
/// \param v - type: lval*
/// \param i - type: int
/// \return lval*
lval* lval_pop(lval* v, int i);


/// \brief Takes the ith element off of the lval `v`.
///
/// \details Takes the ith element off of the lval `v`
/// and discards all other values. Returns the taken 
/// element.
///
/// \param v - type: lval*
/// \param i - type: int
/// \return lval*
lval* lval_take(lval* v, int i);

/// \brief Evaluates the lval `v`.
///
/// \details Evaluates the lval `v`.
/// Returns `v` as-is it is not an 
/// S-Expression.
///
/// \param e - type: lenv*
/// \param v - type: lval*
/// \return lval*
lval* lval_eval(lenv* e, lval* v);


/// \brief Evaluates the lval `v` as an S-Expression.
///
/// \details Evaluates the lval `v` as an S-Expression.
/// Returns `v` as-is if it has no children or returns the
/// child if it only has one child. Returns an error if the
/// child does not start with a symbol.
///
/// \param e - type: lenv*
/// \param v - type: lval*
/// \return lval*
lval* lval_eval_sexpr(lenv* e, lval* v);


/// \brief Joins the Q-Expression `y` to `x`.
///
/// \details Joins the Q-Expression `y` to `x` 
/// by continually popping the first element 
/// off `y` and adding it to `x`. Returns `x`.
///
/// \param x - type: lval*
/// \param y - type: lval*
/// \return lval*
lval* lval_join(lval* x, lval* y);

#endif  /// CURLY_LVAL_H
