#include <editline/readline.h>
#include <editline/history.h>

#include <stdio.h>
#include <stdlib.h>


int main(int argc, char** argv)
{
    puts("Curly v0.0.1");
    puts("Press Ctrl+C to exit.\n");

    while(1)
    {
        char* input = readline("curly> ");

        add_history(input);

        printf("No you're a %s!\n", input);

        free(input);
    }

    return 0;
}