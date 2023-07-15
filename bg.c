#include "bg.h"
#include "headers.h"
#include "misc.h"

extern int argc;
extern char *argv[MAX_SIZE];
extern int bg_proc_count;
extern node bg_proc_list_head;

void bg()
{
    if (argc != 2)
    {
        if (argc < 2)
        {
            error("Error: bg: too less arguments\n");
        }
        else
        {
            error("Error: bg: too many arguments\n");
        }

        return;
    }

    int job_num = atoi(argv[1]);
    if (job_num <= 0)
    {
        error("Error: bg: invalid argument type\n");
        return;
    }

    if (bg_proc_count == 0)
    {
        error("Error: bg: no background job with the given job number exists\n");
        return;
    }

    pid_t pid = -1;
    node itr = bg_proc_list_head;

    while (itr != NULL)
    {
        if (itr->proc_num == job_num)
        {
            pid = itr->pid;
            break;
        }

        itr = itr->next;
    }

    if (pid == -1)
    {
        error("Error: bg: no background job with the given job number exists\n");
        return;
    }

    if (kill(pid, SIGCONT) < 0)
    {
        red();
        perror("Error: bg");
        reset_error();
    }

    return;
}