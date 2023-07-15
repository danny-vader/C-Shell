#include "headers.h"
#include "history.h"
#include "init.h"
#include "jobs.h"

extern pid_t SHELL_PID;
extern char HOME[MAX_SIZE];
extern char CWD[MAX_SIZE];
extern char PWD[MAX_SIZE];
extern char USERNAME[MAX_SIZE];
extern char SYSTEM_NAME[MAX_SIZE];
extern double proc_time;
extern int bg_proc_count;
extern node bg_proc_list_head;
extern int stdin_fileno;
extern int stdout_fileno;

void init_shell()
{
    SHELL_PID = getpid();
    getcwd(HOME, MAX_SIZE);
    getcwd(CWD, MAX_SIZE);
    getcwd(PWD, MAX_SIZE);
    getlogin_r(USERNAME, MAX_SIZE);
    gethostname(SYSTEM_NAME, MAX_SIZE);
    bg_proc_list_head = init_list();
    init_history_file();
    stdin_fileno = dup(STDIN_FILENO);
    stdout_fileno = dup(STDOUT_FILENO);
    proc_time = 0;
}