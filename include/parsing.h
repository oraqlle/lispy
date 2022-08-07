#include <mpc.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _WIN32

    char* readline(char* prompt);

    void add_history(char* unused);

#else
    #include <editline/readline.h>
    #include <editline/history.h>
#endif

/// \brief Represents a Lisp Value 
/// 
/// A `lval` consists of a:
/// - type      : int corresponding to an enum value 
/// - num       : long coresonding to a number
/// - err       : char* corresponding to an error message (optional)
/// - sym       : char* corresponding to a symbol or operator (optional)
/// - count     : int corresponding to the number of elements in the `cell` array
/// - cell      : lval** corresponding to an array of lvals
typedef struct lval 
{
    int type;
    long num;
    char* err;
    char* sym;
    int count;
    struct lval** cell;
} lval;

/// \brief Enum for possible lval types
///
/// The possible lval types are:
/// - LVAL_ERR : Error type
/// - LVAL_NUM : Number type
/// - LVAL_SYM : Symbol type
/// - LVAL_SEXPR : S-Expression type
enum { LVAL_ERR, LVAL_NUM, LVAL_SYM, LVAL_SEXPR };

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
lval* lval_err(char* m);

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

/// \brief Frees an lval.
///
/// \details Frees an lval and all of its children
/// (if any) as well as any other allocated resources.
///
/// \param v - type: lval*
void lval_del(lval* v);

/// \brief Adds the child `x` to the parent `v`.
///
/// \details Adds the child `x` to the parent `v`.
/// Allocates memory to `v`'s `cell` array and 
/// assigns `x` to the last slot of the array.
///
/// \param v - type: lval*
/// \param x - type: lval*
lval* lval_add(lval* v, lval* x);

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

/// \brief Evaluates the lval `v` as an operator.
///
/// \details Evaluates the lval `v` as an operator.
/// Returns an error if `a`'s children are not of type
/// LVAL_NUM.
///
/// \param a - type: lval*
/// \param op - type: char*
lval* builtin_op(lval* a, char* op);

/// \brief Evaluates the lval `v`.
///
/// \details Evaluates the lval `v`.
/// Returns `v` as-is it is not an 
/// S-Expression.
///
/// \param v - type: lval*
/// \return lval*
lval* lval_eval(lval* v);

/// \brief Evaluates the lval `v` as an S-Expression.
///
/// \details Evaluates the lval `v` as an S-Expression.
/// Returns `v` as-is if it has no children or returns the
/// child if it only has one child. Returns an error if the
/// child does not start with a symbol.
///
/// \param v - type: lval*
/// \return lval*
lval* lval_eval_sexpr(lval* v);
