#include "bg.h"
#include "background.h"
#include "cd.h"
#include "discover.h"
#include "echo.h"
#include "fg.h"
#include "foreground.h"
#include "headers.h"
#include "history.h"
#include "jobs.h"
#include "ls.h"
#include "pinfo.h"
#include "pwd.h"
#include "redirection.h"
#include "sig.h"

extern int argc;
extern char *argv[MAX_SIZE];

void execute(char *command)
{

    if (redirection(command) == false)
    {
        return;
    }

    if (strcmp(argv[0], "\0") == 0)
    {
    }
    else if (strcmp(argv[0], "exit") == 0)
    {
        exit(0);
    }
    else if (strcmp(argv[0], "bg") == 0)
    {
        bg();
    }
    else if (strcmp(argv[0], "cd") == 0)
    {
        cd();
    }
    else if (strcmp(argv[0], "discover") == 0)
    {
        discover();
    }
    else if (strcmp(argv[0], "echo") == 0)
    {
        echo();
    }
    else if (strcmp(argv[0], "fg") == 0)
    {
        fg();
    }
    else if (strcmp(argv[0], "history") == 0)
    {
        history();
    }
    else if (strcmp(argv[0], "jobs") == 0)
    {
        jobs();
    }
    else if (strcmp(argv[0], "ls") == 0)
    {
        ls();
    }
    else if (strcmp(argv[0], "pinfo") == 0)
    {
        pinfo();
    }
    else if (strcmp(argv[0], "pwd") == 0)
    {
        pwd();
    }
    else if (strcmp(argv[0], "sig") == 0)
    {
        sig();
    }
    else if (argv[0][0] == '&')
    {
        for (int i = 1; i < argc; i++)
        {
            argv[i - 1] = argv[i];
        }
        argc--;
        background();
    }
    else
    {
        foreground();
    }

    return;
}