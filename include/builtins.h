#ifndef CURLY_BUILTINS_H
#define CURLY_BUILTINS_H

#include <types.h>


//////////////////////////
/// Builtin Evaluaters ///
//////////////////////////

/// \brief Dispatches the builtin function `f` to the lval `a`.
///
/// \details Dispatches the builtin function `f` to the lval `a`.
/// Returns an error if `f` is not a builtin function.
///
/// \param e - type: lenv*
/// \param a - type: lval*
/// \param f - type: char*
/// \return lval*
lval* builtin(lenv* e, lval* a, char* func);


/////////////////////////
/// Builtin Operators ///
/////////////////////////

/// \brief Evaluates the lval `a` as an operator.
///
/// \details Evaluates the lval `a` as an operator.
/// Returns an error if `a`'s children are not of type
/// LVAL_NUM.
///
/// \param e - type: lenv*
/// \param a - type: lval*
/// \param op - type: char*
/// \return lval*
lval* builtin_op(lenv* e, lval* a, char* op);


////////////////////////////////////
/// Builtin Arithmatic Operators ///
////////////////////////////////////

/// \brief Built-in addition operator.
///
/// \details Curly's built-in addition operator.
/// 
/// \param e - type: lenv*
/// \param a - type: lval*
/// \return lval*
lval* builtin_add(lenv* e, lval* a);


/// \brief Built-in subtraction operator.
///
/// \details Curly's built-in subtraction operator.
///
/// \param e - type: lenv*
/// \param a - type: lval*
/// \return lval*
lval* builtin_sub(lenv* e, lval* a);


/// \brief Built-in multiplication operator.
///
/// \details Curly's built-in multiplication operator.
///
/// \param e - type: lenv*
/// \param a - type: lval*
/// \return lval*
lval* builtin_mul(lenv* e, lval* a);


/// \brief Built-in division operator.
///
/// \details Curly's built-in division operator.
///
/// \param e - type: lenv*
/// \param a - type: lval*
/// \return lval*
lval* builtin_div(lenv* e, lval* a);


//////////////////////////////
/// Builtin List Operators ///
//////////////////////////////

/// \brief Returns the head of a Q-Expression.
///
/// \details Returns the head of a Q-Expression
/// and discards the tail.
///
/// \param e - type: lenv*
/// \param a - type: lval*
/// \return lval*
lval* builtin_head(lenv* e, lval* a);


/// \brief Returns the tail of a Q-Expression.
///
/// \details Returns the tail of a Q-Expression
/// and discards the head.
///
/// \param e - type: lenv*
/// \param a - type: lval*
/// \return lval*
lval* builtin_tail(lenv* e, lval* a);


/// \brief Converts a S-Expression to a Q-Expression.
///
/// \details Converts a S-Expression to a Q-Expression
/// and returns it.
///
/// \param e - type: lenv*
/// \param a - type: lval*
/// \return lval*
lval* builtin_list(lenv* e, lval* a);


/// \brief Evaluates a Q-Expression as an S-Expression.
///
/// \details Evaluates a Q-Expression as an S-Expression
/// using lval_eval. Returns an error if the Q-Expression 
/// contains another Q-Expression.
///
/// \param e - type: lenv*
/// \param a - type: lval*
/// \return lval*
lval* builtin_eval(lenv* e, lval* a);

/// \brief Joins multiple Q-Expressions into a single Q-Expression.
///
/// \details Joins multiple Q-Expressions into a single Q-Expression
/// using lval_join returns the resulting Q-Expression.
///
/// \param e - type: lenv*
/// \param a - type: lval*
/// \return lval*
lval* builtin_join(lenv* e, lval* a);


//////////////////////////////////
/// Builtin Function Operators ///
//////////////////////////////////

/// \brief Defines the lambda operator
///
/// \details Defines the lambda operator
/// as the `\` character.
///
/// \param e - type: lenv*
/// \param a - type: lval*
/// \return lval*
lval* builtin_lambda(lenv* e, lval* a);


/// FIX
/// \brief Allows the definition of new functions in 
/// the environment.
///
/// \details Allows the definition of new functions in 
/// the environment.
///
/// \param e - type: lenv*
/// \param name - type: char*
/// \return lval*
lval* builtin_def(lenv* e, lval* a);


/// TODO
lval* builtin_put(lenv* e, lval* a);


/// TODO
lval* builtin_var(lenv* e, lval* a, char* func);


//////////////////////////
/// Ordering Operators ///
//////////////////////////

/// TODO
lval* builtin_ord(lenv* e, lval* a, char* op);


/// TODO
lval* builtin_gt(lenv* e, lval* a);


/// TODO
lval* builtin_lt(lenv* e, lval* a);


/// TODO
lval* builtin_ge(lenv* e, lval* a);


/// TODO
lval* builtin_le(lenv* e, lval* a);


//////////////////////////
/// Equality Operators ///
//////////////////////////

/// TODO
lval* builtin_cmp(lenv* e, lval* a, char* op);


/// TODO
lval* builtin_eq(lenv* e, lval* a);


/// TODO
lval* builtin_ne(lenv* e, lval* a);


////////////////////////////
/// Comparison Functions ///
////////////////////////////

/// TODO
lval* builtin_if(lenv* e, lval* a);

#endif  /// CURLY_BUILTINS_H
