#include "headers.h"
#include "misc.h"
#include "pinfo.h"

extern char HOME[MAX_SIZE];
extern int argc;
extern char *argv[MAX_SIZE];
extern pid_t SHELL_PID;

void pinfo()
{
    if (argc > 2)
    {
        red();
        error("Error: pinfo: too many arguments\n");
        return;
    }

    pid_t pid;

    if (argc == 2)
    {
        pid = atoi(argv[1]);
        if (pid <= 0)
        {
            red();
            fprintf(stderr, "Error: pinfo: invalid argument type\n");
            reset_error();
            return;
        }
    }
    else
    {
        pid = SHELL_PID;
    }

    char proc_stat[MAX_SIZE];
    int memory;
    char exec_path[MAX_SIZE];
    pid_t pgpid;
    pid_t tgpid;

    char procFile[MAX_SIZE];
    sprintf(procFile, "/proc/%d/stat", pid);
    FILE *statFile = fopen(procFile, "r");

    if (statFile == NULL)
    {
        red();
        perror("Error: pinfo");
        reset_error();
        return;
    }

    int i = 0;
    while (i < 23)
    {
        char temp[MAX_SIZE];
        fscanf(statFile, "%s", temp);
        if (i == 2)
        {
            strcpy(proc_stat, temp);
        }
        if (i == 4)
        {
            pgpid = atoi(temp);
        }
        if (i == 7)
        {
            tgpid = atoi(temp);
        }
        if (i == 22)
        {
            memory = atoi(temp);
        }
        i++;
    }

    printf("pid : %d\n", pid);
    if (pgpid == tgpid)
    {
        printf("process Status : {%c+}\n", proc_stat[0]);
    }
    else
    {
        printf("process Status : {%c}\n", proc_stat[0]);
    }
    printf("memory : %d {Virtual Memory}\n", memory);

    char exeFile[MAX_SIZE];
    sprintf(exeFile, "/proc/%d/exe", pid);
    FILE *execFile = fopen(exeFile, "r");

    int len = readlink(exeFile, exec_path, MAX_SIZE);

    if (len < 0)
    {
        red();
        perror("Error: pinfo");
        reset_error();
        return;
    }

    exec_path[len] = '\0';

    if (strcmp(exec_path, HOME) == 0)
    {
        printf("executable Path : ~\n", exec_path);
    }
    else if (strlen(exec_path) < strlen(HOME) || strlen(exec_path) >= strlen(HOME) && strstr(exec_path, HOME) == NULL)
    {
        printf("executable Path : %s\n", exec_path);
    }
    else
    {
        int breakpoint = strlen(HOME);
        char extension[MAX_SIZE];
        strcpy(extension, &exec_path[breakpoint]);
        printf("executable Path : ~%s\n", extension);
    }
}