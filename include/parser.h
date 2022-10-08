#ifndef LIX_PARSER_H
#define LIX_PARSER_H

#include <lval.h>


/// TODO
lval* lval_read_expr(char* s, int* i, char end);


/// TODO
lval* lval_read(char* s, int* i);


/// TODO
lval* lval_read_str(char*s , int* i);


/// TODO
lval* lval_read_sym(char* s, int* i);


/// TODO
char lval_str_unescape(char x);


/// TODO
char* lval_str_escape(char x);


#endif  // LIX_PARSER_H
