#ifndef _JOBS_H
#define _JOBS_H

typedef struct jobs_node
{
    int proc_num;
    char state;
    char *command;
    pid_t pid;
} * jobs_node_ptr;

node init_node();
node init_list();
void add_node(pid_t pid);
void remove_node(pid_t pid);
char bg_proc_status(pid_t pid);
void jobs();

#endif