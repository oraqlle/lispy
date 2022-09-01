#ifndef CURLY_WINDOWS_H
#define CURLY_WINDOWS_H

#ifdef _WIN32

    char* readline(char* prompt);

    void add_history(char* unused);

#else
    #include <editline/readline.h>
    #include <editline/history.h>
#endif

#endif  /// CURLY_WINDOWS_H
