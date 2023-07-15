#include "echo.h"
#include "headers.h"

extern int argc;
extern char *argv[MAX_SIZE];

void echo()
{
    for (int i = 1; i < argc - 1; i++)
    {
        printf("%s ", argv[i]);
    }
    printf("%s\n", argv[argc - 1]);
}