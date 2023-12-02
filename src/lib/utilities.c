#include <types.h>
#include <utilities.h>

const char* ltype_name(int type_id)
{
    switch (type_id) {
    case LVAL_FUN:
        return "Function";

    case LVAL_NUM:
        return "Number";

    case LVAL_ERR:
        return "Error";

    case LVAL_SYM:
        return "Symbol";

    case LVAL_STR:
        return "String";

    case LVAL_SEXPR:
        return "S-Expression";

    case LVAL_QEXPR:
        return "Q-Expression";

    default:
        return "Unknown";
    }
}
