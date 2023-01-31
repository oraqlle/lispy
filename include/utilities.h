#ifndef LISPY_UTILITIES_H
#define LISPY_UTILITIES_H

#include <types.h>


#ifdef _WIN32

    char* readline(char* prompt);

    void add_history(char* unused);

#else
    #include <editline/readline.h>
    #include <editline/history.h>
#endif


/// \brief Returns the name of a type
///
/// \details Returns a string of of the passed
/// type's name.
///
/// \param t - type int
/// \return char*
char* ltype_name(int t);

#endif  /// LISPY_UTILITIES_H
