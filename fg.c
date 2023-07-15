#include "fg.h"
#include "headers.h"
#include "jobs.h"
#include "misc.h"

extern int argc;
extern char *argv[MAX_SIZE];
extern int bg_proc_count;
extern node bg_proc_list_head;

void fg()
{
    if (argc != 2)
    {
        if (argc < 2)
        {
            error("Error: fg: too less arguments\n");
        }
        else
        {
            error("Error: fg: too many arguments\n");
        }

        return;
    }

    int job_num = atoi(argv[1]);
    if (job_num <= 0)
    {
        error("Error: fg: invalid argument type\n");
        return;
    }

    if (bg_proc_count == 0)
    {
        error("Error: fg: no background job with the given job number exists\n");
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
        error("Error: fg: no background job with the given job number exists\n");
        return;
    }

    signal(SIGTTIN, SIG_IGN);
    signal(SIGTTOU, SIG_IGN);
    tcsetpgrp(0, getpgid(pid));

    if (kill(pid, SIGCONT) < 0)
    {
        red();
        perror("Error: fg");
        reset_error();
        return;
    }

    int status;
    waitpid(pid, &status, WUNTRACED);
    tcsetpgrp(0, getpgid(0));
    signal(SIGTTIN, SIG_DFL);
    signal(SIGTTOU, SIG_DFL);
    remove_node(pid);

    return;
}