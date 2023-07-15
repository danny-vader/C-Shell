#include "foreground.h"
#include "headers.h"
#include "jobs.h"
#include "misc.h"

extern int argc;
extern char *argv[MAX_SIZE];
extern double proc_time;
extern int bg_proc_count;
extern node bg_proc_list_head;

void foreground()
{
    pid_t pid = fork();

    if (pid < 0)
    {
        red();
        perror("Error");
        reset_error();
        return;
    }
    else if (pid == 0)
    {
        setpgid(pid, pid);

        signal(SIGINT, SIG_DFL);
        signal(SIGTSTP, SIG_DFL);
        signal(SIGTTIN, SIG_DFL);
        signal(SIGTTOU, SIG_DFL);

        if (execvp(argv[0], argv) < 0)
        {
            red();
            perror("Error");
            reset_error();
            kill(pid, SIGTERM);
            return;
        }

        signal(SIGINT, SIG_IGN);
        signal(SIGTSTP, SIG_IGN);
        signal(SIGTTIN, SIG_IGN);
        signal(SIGTTOU, SIG_IGN);
    }
    else
    {
        int status;
        struct timespec begin, end;
        clock_gettime(CLOCK_REALTIME, &begin);

        signal(SIGTTIN, SIG_IGN);
        signal(SIGTTOU, SIG_IGN);

        tcsetpgrp(STDIN_FILENO, pid);

        waitpid(pid, &status, WUNTRACED);

        tcsetpgrp(STDIN_FILENO, getpgid(0));

        signal(SIGTTIN, SIG_DFL);
        signal(SIGTTOU, SIG_DFL);

        if (WIFSTOPPED(status))
        {
            add_node(pid);
        }

        clock_gettime(CLOCK_REALTIME, &end);
        double start = begin.tv_nsec;
        double finish = end.tv_nsec;
        double time = (end.tv_sec - begin.tv_sec) + ((end.tv_nsec - begin.tv_nsec) / 1000000000.0);
        proc_time += time;

        return;
    }
}