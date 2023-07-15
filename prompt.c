#include "headers.h"
#include "misc.h"
#include "prompt.h"

extern char HOME[MAX_SIZE];
extern char CWD[MAX_SIZE];
extern char USERNAME[MAX_SIZE];
extern char SYSTEM_NAME[MAX_SIZE];
extern bool fgtime;
extern double proc_time;

void prompt()
{
    if (strcmp(CWD, HOME) == 0)
    {
        yellow();
        printf("<");
        green();
        printf("%s@%s:", USERNAME, SYSTEM_NAME);
        blue();
        printf("~");
        reset();
        if (proc_time >= 1)
        {
            printf(" took %lfs", proc_time);
        }
        yellow();
        printf("> ");
        reset();
    }
    else if(strstr(CWD, HOME) != NULL && CWD[strlen(HOME)] == '/')
    {
        int breakpoint = strlen(HOME);
        char extension[strlen(CWD) - breakpoint + 1];
        strcpy(extension, &CWD[breakpoint]);
        yellow();
        printf("<");
        green();
        printf("%s@%s:", USERNAME, SYSTEM_NAME);
        blue();
        printf("~%s", extension);
        reset();
        if (proc_time >= 1)
        {
            printf(" took %lfs", proc_time);
        }
        yellow();
        printf("> ");
        reset();
    }
        else if (strlen(CWD) < strlen(HOME) || strlen(CWD) >= strlen(HOME))
    {
        yellow();
        printf("<");
        green();
        printf("%s@%s:", USERNAME, SYSTEM_NAME);
        blue();
        printf("%s", CWD);
        reset();
        if (proc_time >= 1)
        {
            printf(" took %lfs", proc_time);
        }
        yellow();
        printf("> ");
        reset();
    }
}