#ifndef LISPY_BUILTINS_H
#define LISPY_BUILTINS_H

#include <types.h>

//////////////////////////
// Builtin Evaluators
//////////////////////////

/// @brief Dispatches the builtin function `f` to the lval `a`.
///
/// @details Dispatches the builtin function `f` to the lval `a`.
/// Returns an error if `f` is not a builtin function.
///
/// @param e - type: lenv*
/// @param a - type: lval*
/// @param f - type: char*
/// @return lval*
lval* builtin(lenv* env, lval* arg, const char* func);

/////////////////////////
// Builtin Operators
/////////////////////////

/// @brief Evaluates the lval `a` as an operator.
///
/// @details Evaluates the lval `a` as an operator.
/// Returns an error if `a`'s children are not of type
/// LVAL_NUM.
///
/// @param e - type: lenv*
/// @param a - type: lval*
/// @param op - type: char*
/// @return lval*
lval* builtin_op(lenv* env, lval* arg, const char* operand);

////////////////////////////////////
// Builtin Arithmetic Operators
////////////////////////////////////

/// @brief Built-in addition operator.
///
/// @details Curly's built-in addition operator.
///
/// @param e - type: lenv*
/// @param a - type: lval*
/// @return lval*
lval* builtin_add(lenv* env, lval* arg);

/// @brief Built-in subtraction operator.
///
/// @details Curly's built-in subtraction operator.
///
/// @param e - type: lenv*
/// @param a - type: lval*
/// @return lval*
lval* builtin_sub(lenv* env, lval* arg);

/// @brief Built-in multiplication operator.
///
/// @details Curly's built-in multiplication operator.
///
/// @param e - type: lenv*
/// @param a - type: lval*
/// @return lval*
lval* builtin_mul(lenv* env, lval* arg);

/// @brief Built-in division operator.
///
/// @details Curly's built-in division operator.
///
/// @param e - type: lenv*
/// @param a - type: lval*
/// @return lval*
lval* builtin_div(lenv* env, lval* arg);

//////////////////////////////
// Builtin List Operators
//////////////////////////////

/// @brief Returns the head of a Q-Expression.
///
/// @details Returns the head of a Q-Expression
/// and discards the tail.
///
/// @param e - type: lenv*
/// @param a - type: lval*
/// @return lval*
lval* builtin_head(lenv* env, lval* arg);

/// @brief Returns the tail of a Q-Expression.
///
/// @details Returns the tail of a Q-Expression
/// and discards the head.
///
/// @param e - type: lenv*
/// @param a - type: lval*
/// @return lval*
lval* builtin_tail(lenv* env, lval* arg);

/// @brief Converts a S-Expression to a Q-Expression.
///
/// @details Converts a S-Expression to a Q-Expression
/// and returns it.
///
/// @param e - type: lenv*
/// @param a - type: lval*
/// @return lval*
lval* builtin_list(lenv* env, lval* arg);

/// @brief Evaluates a Q-Expression as an S-Expression.
///
/// @details Evaluates a Q-Expression as an S-Expression
/// using lval_eval. Returns an error if the Q-Expression
/// contains another Q-Expression.
///
/// @param e - type: lenv*
/// @param a - type: lval*
/// @return lval*
lval* builtin_eval(lenv* env, lval* arg);

/// @brief Joins multiple Q-Expressions into a single Q-Expression.
///
/// @details Joins multiple Q-Expressions into a single Q-Expression
/// using lval_join returns the resulting Q-Expression.
///
/// @param e - type: lenv*
/// @param a - type: lval*
/// @return lval*
lval* builtin_join(lenv* env, lval* arg);

//////////////////////////////////
// Builtin Function Operators
//////////////////////////////////

/// FIX
/// @brief Defines the lambda operator
///
/// @details Defines the lambda operator
/// as the `\` character.
///
/// @param e - type: lenv*
/// @param a - type: lval*
/// @return lval*
lval* builtin_lambda(lenv* env, lval* arg);

/// FIX
/// @brief Allows the definition of new functions in
/// the environment.
///
/// @details Allows the definition of new functions in
/// the environment.
///
/// @param e - type: lenv*
/// @param name - type: char*
/// @return lval*
lval* builtin_def(lenv* env, lval* arg);

/// TODO
lval* builtin_put(lenv* env, lval* arg);

/// TODO
lval* builtin_var(lenv* env, lval* arg, const char* func);

//////////////////////////
// Ordering Operators
//////////////////////////

/// TODO
lval* builtin_ord(lenv* env, lval* arg, const char* operand);

/// TODO
lval* builtin_gt(lenv* env, lval* arg);

/// TODO
lval* builtin_lt(lenv* env, lval* arg);

/// TODO
lval* builtin_ge(lenv* env, lval* arg);

/// TODO
lval* builtin_le(lenv* env, lval* arg);

//////////////////////////
// Equality Operators
//////////////////////////

/// TODO
lval* builtin_cmp(lenv* env, lval* arg, const char* operand);

/// TODO
lval* builtin_eq(lenv* env, lval* arg);

/// TODO
lval* builtin_ne(lenv* env, lval* arg);

////////////////////////////
// Comparison Functions
////////////////////////////

/// TODO
lval* builtin_if(lenv* env, lval* arg);

////////////////////////////
// Builtin IO functions
////////////////////////////

/// TODO
lval* builtin_load(lenv* env, lval* arg);

/// TODO
lval* builtin_print(lenv* env, lval* arg);

/// TODO
// lval* builtin_input(lenv* env, lval* arg);

/// TODO
lval* builtin_error(lenv* env, lval* arg);

#endif /// LISPY_BUILTINS_H
