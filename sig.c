#include "headers.h"
#include "misc.h"
#include "sig.h"

extern int argc;
extern char *argv[MAX_SIZE];
extern int bg_proc_count;
extern node bg_proc_list_head;

void sig()
{
    if (argc != 3)
    {
        if (argc < 3)
        {
            error("Error: sig: too less arguments\n");
        }
        else
        {
            error("Error: sig: too many arguments\n");
        }

        return;
    }

    int job_num = atoi(argv[1]);
    int sig_num = atoi(argv[2]);
    if (job_num <= 0 || sig_num <= 0)
    {
        error("Error: sig: invalid argument type\n");
        return;
    }

    if (bg_proc_count == 0)
    {
        error("Error: sig: no background job with the given job number exists\n");
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
        error("Error: sig: no background job with the given job number exists\n");
        return;
    }

    if (kill(pid, sig_num) < 0)
    {
        red();
        perror("Error: sig");
        reset_error();
    }

    return;
}