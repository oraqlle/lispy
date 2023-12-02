#ifndef LISPY_IO_H
#define LISPY_IO_H

#include <lval.h>

//////////////////////
/// `lval` Reading ///
//////////////////////

///////////////////////
/// `lval` Printing ///
///////////////////////

/// @brief Prints the lval
///
/// @details Prints the lval to stdout.
///
/// @param obj - type: const lval*
void lval_print(const lval* obj);

/// @brief Prints the lval to stdout with a newline.
///
/// @param obj - type: const lval*
void lval_println(const lval* obj);

///////////////////////////
/// Expression Printing ///
///////////////////////////

/// @brief Prints the lval as an expression to stdout.
///
/// @details Prints the lval as an expression surrounded
/// by the `open` and `close` parameters.
///
/// @param obj - type: const lval*
/// @param open - type: char
/// @param close - type: char
void lval_expr_print(const lval* obj, char open, char close);

/////////////////
/// String IO ///
/////////////////

/// TODO
void lval_print_str(const lval* obj);

#endif /// LISPY_IO_H
