#include "cd.h"
#include "headers.h"
#include "misc.h"

extern char HOME[MAX_SIZE];
extern char CWD[MAX_SIZE];
extern char PWD[MAX_SIZE];
extern int argc;
extern char *argv[MAX_SIZE];

void cd()
{
    char dest[MAX_SIZE];

    if (argc > 2)
    {
        error("Error: cd: too many arguments\n");
        return;
    }

    if (argc == 1)
    {
        strcpy(dest, HOME);
    }
    else
    {
        if (strcmp(argv[1], "-") == 0)
        {
            strcpy(dest, PWD);
        }
        else if (strncmp(&argv[1][0], "~", 1) == 0)
        {
            strcpy(dest, HOME);
            strcat(dest, &argv[1][1]);
        }
        else
        {
            strcpy(dest, argv[1]);
        }
    }

    if (chdir(dest) < 0)
    {
        red();
        perror("Error: cd");
        reset_error();
    }

    strcpy(PWD, CWD);
    getcwd(CWD, MAX_SIZE);
}