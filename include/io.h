#ifndef CURLY_IO_H
#define CURLY_IO_H

#include <mpc.h>
#include <lval.h>


/////////////////////////
/// `lval` IO Methods ///
/////////////////////////

/// \brief Returns lval number type from the AST.
///
/// \details Reads a value from the AST and 
/// returns the result as an lval LVAL_NUM type
/// or an LVAL_ERR lval if the value is not an 
/// number.
///
/// \param v - type: mpc_ast_t*
/// \return lval*
lval* lval_read_num(mpc_ast_t* t);


/// \brief Parses the AST.
///
/// \details Parses the AST and returns the resulting
/// as an lval's.
///
/// \param t - type: mpc_ast_t*
/// \return lval*
lval* lval_read(mpc_ast_t* t);


/// \brief Prints the lval
///
/// \details Prints the lval to stdout.
/// 
/// \param v - type: lval* 
void lval_print(lval* v);


/// \brief Prints the lval to stdout with a newline.
///
/// \param v - type: lval*
void lval_println(lval* v);


/// \brief Prints the lval as an expression to stdout.
///
/// \details Prints the lval as an expression surrounded
/// by the `open` and `close` parameters.
///
/// \param v - type: lval*
/// \param open - type: char
/// \param close - type: char
void lval_expr_print(lval* v, char open, char close);


/// TODO
void lval_print_str(lval* v);

#endif  /// CURLY_IO_H
