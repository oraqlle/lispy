#include <parser.h>

#include <errno.h>
#include <string.h>

lval* lval_read_expr(const char* str, int* idx, char end)
{
    lval* rexpr = (end == '}') ? lval_qexpr() : lval_sexpr();

    while (str[*idx] != end) {
        lval* rval = lval_read(str, idx);

        if (rval->type == LVAL_ERR) {
            lval_del(rexpr);
            return rval;
        }

        lval_add(rexpr, rval);
    }

    (*idx)++;

    return rexpr;
}

lval* lval_read(const char* str, int* idx)
{
    while (strchr(" \t\v\r\n;", str[*idx]) && str[*idx] != '\0') {
        if (str[*idx] == ';') {
            while (str[*idx] != '\n' && str[*idx] != '\0') {
                (*idx)++;
            }
        }

        (*idx)++;
    }

    lval* rexpr = NULL;

    if (str[*idx] == '\0') {
        return lval_err("Unexpected end of input");
    }

    if (str[*idx] == '(') {
        (*idx)++;
        rexpr = lval_read_expr(str, idx, ')');
    } else if (str[*idx] == '{') {
        (*idx)++;
        rexpr = lval_read_expr(str, idx, '}');
    } else if (strchr(
                   "abcdefghijklmnopqrstuvwxyz"
                   "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
                   "0123456789_+-*\\/=<>!&",
                   str[*idx])) {
        rexpr = lval_read_sym(str, idx);
    } else if (strchr("\"", str[*idx])) {
        rexpr = lval_read_str(str, idx);
    } else {
        rexpr = lval_err("Unexpected character %c", str[*idx]);
    }

    while (strchr(" \t\v\r\n", str[*idx]) && str[*idx] != '\0') {
        if (str[*idx] == ';') {
            while (str[*idx] != '\n' && str[*idx] != '\0') {
                (*idx)++;
            }
        }

        (*idx)++;
    }

    return rexpr;
}

lval* lval_read_str(const char* str, int* idx)
{
    char* part = calloc(1, 1);

    (*idx)++;

    while (str[*idx] != '"') {
        char chr = str[*idx];

        if (chr == '\0') {
            free(part);
            return lval_err("Unexpected end of input");
        }

        if (chr == '\\') {
            (*idx)++;

            if (strchr("abfnrtv\\\'\"", str[*idx])) {
                chr = lval_str_unescape(str[*idx]);
            } else {
                free(part);
                return lval_err("Invalid escape sequence \\%c", str[*idx]);
            }
        }

        char* old_part = part;
        part = realloc(part, strlen(part) + 2);

        if (!part) {
            free(old_part);
            exit(1); // NOLINT(concurrency-mt-unsafe)
        }

        part[strlen(part) + 1] = '\0';
        part[strlen(part) + 0] = chr;
        (*idx)++;
    }

    (*idx)++;

    lval* rexpr = lval_str(part);

    free(part);

    return rexpr;
}

lval* lval_read_sym(const char* str, int* idx)
{
    char* part = calloc(1, 1);

    while (strchr(
               "abcdefghijklmnopqrstuvwxyz"
               "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
               "0123456789_+-*\\/=<>!&",
               str[*idx])
        && str[*idx] != '\0') {
        char* old_part = part;
        part = realloc(part, strlen(part) + 2);

        if (!part) {
            free(old_part);
            exit(1); // NOLINT(concurrency-mt-unsafe)
        }

        part[strlen(part) + 1] = '\0';
        part[strlen(part) + 0] = str[*idx];
        (*idx)++;
    }

    int is_num = strchr("-0123456789", part[0]) != NULL;

    for (size_t j = 1; j < strlen(part); j++) {
        if (strchr("0123456789", part[j]) == NULL) {
            is_num = 0;
            break;
        }
    }

    if (strlen(part) == 1 && part[0] == '-') {
        is_num = 0;
    }

    lval* rexpr = NULL;

    if (is_num) {
        errno = 0;
        long num = strtol(part, NULL, 10); // NOLINT(readability-magic-numbers)
        rexpr = (errno != ERANGE) ? lval_num(num) : lval_err("Invalid Number %s", part);
    } else {
        rexpr = lval_sym(part);
    }

    free(part);

    return rexpr;
}

char lval_str_unescape(char chr)
{
    switch (chr) {
    case 'a':
        return '\a';
    case 'b':
        return '\b';
    case 'f':
        return '\f';
    case 'n':
        return '\n';
    case 'r':
        return '\r';
    case 't':
        return '\t';
    case 'v':
        return '\v';
    case '\\':
        return '\\';
    case '\'':
        return '\'';
    case '\"':
        return '\"';
    default:
        return '\0';
    }
}

const char* lval_str_escape(char chr)
{
    switch (chr) {
    case '\a':
        return "\\a";
    case '\b':
        return "\\b";
    case '\f':
        return "\\f";
    case '\n':
        return "\\n";
    case '\r':
        return "\\r";
    case '\t':
        return "\\t";
    case '\v':
        return "\\v";
    case '\\':
        return "\\\\";
    case '\'':
        return "\\\'";
    case '\"':
        return "\\\"";
    default:
        return "";
    }
}
