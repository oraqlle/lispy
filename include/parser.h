#ifndef LISPY_PARSER_H
#define LISPY_PARSER_H

#include <lval.h>


/// TODO
lval* lval_read_expr(const char* str, int* idx, char end);


/// TODO
lval* lval_read(const char* str, int* idx);


/// TODO
lval* lval_read_str(const char* str, int* idx);


/// TODO
lval* lval_read_sym(const char* str, int* idx);


/// TODO
char lval_str_unescape(char chr);


/// TODO
const char* lval_str_escape(char chr);


#endif  // LISPY_PARSER_H
