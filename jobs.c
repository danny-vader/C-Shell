#include "headers.h"
#include "jobs.h"
#include "misc.h"

extern int argc;
extern char *argv[MAX_SIZE];
extern int bg_proc_count;
extern node bg_proc_list_head;

node init_node()
{
    node temp = (node)malloc(sizeof(struct list_node));
    temp->proc_num = 1;
    temp->pid = -1;
    temp->command = (char *)malloc(MAX_SIZE);
    temp->next = NULL;
    return temp;
}

node init_list()
{
    bg_proc_count = 0;
    node temp = init_node();
    return temp;
}

void add_node(pid_t pid)
{
    node itr = bg_proc_list_head;

    if (bg_proc_count == 0)
    {
        itr->pid = pid;
        bg_proc_count++;

        strcpy(itr->command, argv[0]);
        for (int i = 1; i < argc; i++)
        {
            strcat(itr->command, " ");
            strcat(itr->command, argv[i]);
        }

        return;
    }

    bool flag = false;
    while (itr->next != NULL)
    {
        if (itr->pid == -1)
        {
            flag = true;
            break;
        }

        itr = itr->next;
    }

    if (!flag)
    {
        if (itr->pid == -1)
        {
            flag = true;
        }
    }

    if (flag)
    {
        itr->pid = pid;
        bg_proc_count++;

        strcpy(itr->command, argv[0]);
        for (int i = 1; i < argc; i++)
        {
            strcat(itr->command, " ");
            strcat(itr->command, argv[i]);
        }
    }
    else
    {
        node temp = init_node();

        temp->proc_num = ++bg_proc_count;
        temp->pid = pid;

        strcpy(temp->command, argv[0]);
        for (int i = 1; i < argc; i++)
        {
            strcat(temp->command, " ");
            strcat(temp->command, argv[i]);
        }

        itr->next = temp;
    }

    return;
}

void remove_node(pid_t pid)
{
    node itr = bg_proc_list_head;

    while (itr != NULL && itr->pid != pid)
    {
        itr = itr->next;
    }

    if (itr == NULL)
    {
        return;
    }

    itr->pid = -1;
    free(itr->command);
    itr->command = (char *)malloc(MAX_SIZE);
    bg_proc_count--;

    return;
}

char bg_proc_status(pid_t pid)
{
    char proc_stat[MAX_SIZE];
    char procFile[MAX_SIZE];
    sprintf(procFile, "/proc/%d/stat", pid);
    FILE *statFile = fopen(procFile, "r");

    if (statFile == NULL)
    {
        red();
        perror("Error: jobs");
        reset_error();
        return;
    }

    int i = 0;
    while (i < 3)
    {
        char temp[MAX_SIZE];
        fscanf(statFile, "%s", temp);
        if (i == 2)
        {
            strcpy(proc_stat, temp);
        }
        i++;
    }

    return proc_stat[0];
}

int cmp_jobs(const void *a, const void *b)
{
    jobs_node_ptr p1 = (jobs_node_ptr)a;
    jobs_node_ptr p2 = (jobs_node_ptr)b;

    return strcmp(p1->command, p2->command);
}

void jobs()
{
    bool r_flag = false;
    bool s_flag = false;
    int arguments = argc - 1;

    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "-r") == 0)
        {
            r_flag = true;
            arguments--;
        }

        if (strcmp(argv[i], "-s") == 0)
        {
            s_flag = true;
            arguments--;
        }
    }

    if (arguments > 0)
    {
        red();
        if (!r_flag && !s_flag)
        {
            fprintf(stderr, "Error: jobs: invalid argument type\n");
        }
        else
        {
            fprintf(stderr, "Error: jobs: too many arguments\n");
        }
        reset_error();
    }

    struct jobs_node bg_procs[bg_proc_count];
    node itr = bg_proc_list_head;

    for (int i = 0; i < bg_proc_count; i++)
    {
        while (itr->pid == -1)
        {
            itr = itr->next;
        }

        bg_procs[i].proc_num = itr->proc_num;
        bg_procs[i].state = bg_proc_status(itr->pid);
        bg_procs[i].command = (char *)malloc(sizeof(MAX_SIZE));
        strcpy(bg_procs[i].command, itr->command);
        bg_procs[i].pid = itr->pid;
        itr = itr->next;
    }

    qsort(bg_procs, bg_proc_count, sizeof(struct jobs_node), cmp_jobs);

    for (int i = 0; i < bg_proc_count; i++)
    {
        if (bg_procs[i].state == 'T' && !r_flag)
        {
            printf("[%d] ", bg_procs[i].proc_num);
            printf("Stopped ");
            printf("%s ", bg_procs[i].command);
            printf("[%d]\n", bg_procs[i].pid);
        }
        else if (!s_flag)
        {
            printf("[%d] ", bg_procs[i].proc_num);
            printf("Running ");
            printf("%s ", bg_procs[i].command);
            printf("[%d]\n", bg_procs[i].pid);
        }
    }
}