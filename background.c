#include "background.h"
#include "headers.h"
#include "jobs.h"
#include "misc.h"

extern int argc;
extern char *argv[MAX_SIZE];
extern int bg_proc_count;
extern node bg_proc_list_head;

void background()
{
    pid_t pid = fork();

    if (pid == 0)
    {
        setpgid(pid, pid);

        if (execvp(argv[0], argv) < 0)
        {
            red();
            perror("Error");
            reset_error();
            kill(pid, SIGTERM);
            return;
        }
    }
    else if (pid < 0)
    {
        red();
        perror("Error");
        reset_error();
        return;
    }
    else
    {
        add_node(pid);
        node itr = bg_proc_list_head;

        while (itr->pid != pid)
        {
            itr = itr->next;
        }

        printf("[%d] %d\n", itr->proc_num, itr->pid);
    }
}