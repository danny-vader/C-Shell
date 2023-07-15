#ifndef _HEADERS_H
#define _HEADERS_H

#include <ctype.h>
#include <dirent.h>
#include <fcntl.h>
#include <grp.h>
#include <pwd.h>
#include <signal.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/dir.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <termios.h>
#include <time.h>
#include <unistd.h>

#define CLOCK_REALTIME 0
#define MAX_CMDS 20
#define MAX_SIZE 1024

typedef struct list_node
{
    int proc_num;
    pid_t pid;
    char *command;
    struct list_node *next;
} * node;

#endif