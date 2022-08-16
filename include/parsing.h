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
struct lval;
typedef struct lval lval;


struct lenv;
typedef struct lenv lenv;

/// \brief Enum for possible lval types
///
/// The possible lval types are:
/// - LVAL_ERR : Error type
/// - LVAL_NUM : Number type
/// - LVAL_SYM : Symbol type
/// - LVAL_SEXPR : S-Expression type
enum { LVAL_ERR, LVAL_NUM, LVAL_SYM, 
       LVAL_FUN, LVAL_SEXPR, LVAL_QEXPR };


typedef lval*(*lbuiltin)(lenv*, lval*);

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

/// \brief Constructs an lval of type LVAL_FUN.
///
/// \details Constructs an lval of type LVAL_FUN
/// and sets the value to the provided builtin
/// function `func`.
///
/// \param func - type: lbuiltin
/// \return lval*
lval* lval_fun(lbuiltin func);

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
/// \param e - type: lenv*
/// \param a - type: lval*
/// \param op - type: char*
/// \return lval*
lval* builtin_op(lenv* e, lval* a, char* op);

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

/// \brief Constructs a new lenv.
///
/// \details Constructs a new lenv.
///
/// \return lenv*
lenv* lenv_new(void);

/// \brief Destroys the lenv `e`.
///
/// \details Destroys the lenv `e` by
/// freeing all of its entries.
///
/// \param e - type: lenv*
void lenv_del(lenv* e);

/// \brief Gets an lval from an lenv.
/// 
/// \details Returns a copy of the lval 
/// `k` from the lenv `e` if it exists 
/// otherwise returns an error.
///
/// \param e - type: lenv*
/// \param k - type: lval*
/// \return lval*
lval* lenv_get(lenv* e, lval* k);

/// \brief Sets an lval in an lenv.
///
/// \details Sets an lval in an lenv.
/// Replaces any existing entry with 
/// the same key.
///
/// \param e - type: lenv*
/// \param k - type: lval*
/// \param v - type: lval*
void lenv_put(lenv* e, lval* k, lval* v);

/// \brief Adds a builtin function to the environment.
///
/// \details Adds a builtin function to the environment
/// with the name `name` and the function signature `func`.
///
/// \param e - type: lenv*
/// \param name - type: char*
/// \param f - type: lbuiltin
void lenv_add_builtin(lenv* e, char* name, lbuiltin func);

/// \brief Adds all the builtin functions to the environment.
///
/// \details Adds all the builtin functions to the environment.
///
/// \param e - type: lenv*
void lenv_add_builtins(lenv* e);

/// \brief Allows the definition of new functions in the environment.
///
/// \details Allows the definition of new functions in the environment.
///
/// \param e - type: lenv*
/// \param name - type: char*
/// \return lval*
lval* builtin_def(lenv* e, lval* a);

/// \brief Returns the name of a type
///
/// \details Returns a string of of the passed
/// type's name.
///
/// \param t - type int
/// \return char*
char* ltype_name(int t);
