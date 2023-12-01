#ifndef LISPY_TYPES_H
#define LISPY_TYPES_H


struct lval;
typedef struct lval lval;


struct lenv;
typedef struct lenv lenv;

typedef lval*(*lbuiltin)(lenv*, lval*);

// typedef lval*(*builtinload)(lenv*, lval*, mpc_parser_t*);


/// @brief Represents a Lisp Value 
/// 
/// A `lval` consists of a:
/// - type      : int corresponding to an enum value 
/// - num       : long corresponding to a number
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
    char* str;

    lbuiltin builtin;
    lenv* env;
    lval* formals;
    lval* body;

    unsigned int count;
    struct lval** cell;
} lval;


/// @brief Enum for possible lval types
///
/// The possible lval types are:
/// - LVAL_ERR : Error type
/// - LVAL_NUM : Number type
/// - LVAL_SYM : Symbol type
/// - LVAL_STR : String type
/// - LVAL_FUN : Function type
/// - LVAL_SEXPR : S-Expression type
/// - LVAL_QEXPR : Q-Expression type
enum { LVAL_ERR, LVAL_NUM, LVAL_SYM, LVAL_STR, 
       LVAL_FUN, LVAL_SEXPR, LVAL_QEXPR };


typedef struct lenv 
{
    lenv* par;
    
    unsigned int count;
    char** syms;
    lval** vals;
} lenv;

#endif  // LISPY_TYPES_H
