#include "execute.h"
#include "headers.h"
#include "history.h"
#include "init.h"
#include "jobs.h"
#include "pipe.h"
#include "prompt.h"
#include "tokenize.h"

extern char HOME[MAX_SIZE];
extern char CWD[MAX_SIZE];
extern char CURR_CMD[MAX_SIZE];
extern int commands;
extern char *LIST_CMD[MAX_SIZE];
extern int bg_proc_count;
extern node bg_proc_list_head;
extern double proc_time;
extern int stdin_fileno;
extern int stdout_fileno;

extern int alphasort();

void die(const char *s)
{
    perror(s);
    exit(1);
}

struct termios orig_termios;

void disableRawMode()
{
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1)
    {
        die("tcsetattr");
    }
}

void enableRawMode()
{
    if (tcgetattr(STDIN_FILENO, &orig_termios) == -1)
    {
        die("tcgetattr");
    }
    atexit(disableRawMode);
    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ICANON | ECHO);
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1)
    {
        die("tcsetattr");
    }
}
int cmp_string(const void *a, const void *b)
{
    return strcmp(*(const char **)a, *(const char **)b);
}

int min(int a, int b)
{
    return a < b ? a : b;
}

bool isPrefix(const char *a, const char *b)
{
    return strncmp(a, b, strlen(a)) == 0;
}

void BG_HANDLER()
{
    if (bg_proc_count == 0)
    {
        return;
    }

    int status;
    pid_t pid = waitpid(-1, &status, WNOHANG);

    if (pid > 0)
    {
        node itr = bg_proc_list_head;
        while (itr->pid != pid)
        {
            itr = itr->next;
        }

        if (!WIFSTOPPED(status))
        {
            if (WIFEXITED(status))
            {
                printf("\n");
                fflush(stdout);
                printf("%s exited with pid = %d normally\n", itr->command, itr->pid);
            }
            else
            {
                printf("\n");
                fflush(stdout);
                printf("%s exited with pid = %d abnormally\n", itr->command, itr->pid);
            }

            remove_node(pid);
        }
    }

    return;
}

int main()
{
    init_shell();
    signal(SIGINT, SIG_IGN);
    signal(SIGTSTP, SIG_IGN);
    signal(SIGTTIN, SIG_IGN);
    signal(SIGTTOU, SIG_IGN);
    signal(SIGCHLD, BG_HANDLER);

    while (1)
    {
        proc_time = 0;
        char c;
        bool flag;
        int ptr;

        setbuf(stdout, NULL);
        enableRawMode();
        if (!flag)
        {
            prompt();
            memset(CURR_CMD, '\0', MAX_SIZE);
            ptr = 0;
        }
        flag = false;

        while (read(STDIN_FILENO, &c, 1) == 1)
        {
            if (iscntrl(c))
            {
                if (c == 4)
                {
                    printf("\n");
                    exit(0);
                }
                else if (c == 9)
                {
                    char word[MAX_SIZE];
                    memset(word, '\0', MAX_SIZE);
                    char file_path[MAX_SIZE];
                    memset(file_path, '\0', MAX_SIZE);

                    char temp[MAX_SIZE];
                    strcpy(temp, CURR_CMD);

                    char *tokenize = strtok(temp, " ");
                    while (tokenize != NULL)
                    {
                        strcpy(file_path, tokenize);
                        tokenize = strtok(NULL, ";");
                    }

                    int index_slash = -1;
                    for (int i = 0; i < strlen(file_path); i++)
                    {
                        if (file_path[i] == '/')
                        {
                            index_slash = i;
                        }
                    }

                    char file[MAX_SIZE];
                    memset(file, '\0', MAX_SIZE);
                    if (index_slash == -1)
                    {
                        strcpy(word, file_path);
                        strcpy(file, "./");
                    }
                    else
                    {
                        if (index_slash != strlen(file_path) - 1)
                        {
                            strcpy(word, &file_path[index_slash + 1]);
                            if (file_path[0] == '~')
                            {
                                strcpy(file, HOME);
                                strncat(file, &file_path[1], index_slash);
                            }
                            else
                            {
                                strncpy(file, file_path, index_slash + 1);
                            }
                        }
                        else
                        {
                            strcpy(file, file_path);
                        }
                    }

                    struct dirent **contents;
                    int count_contents = scandir(file, &contents, NULL, alphasort);
                    struct stat fileStat;

                    char *dir_contents[MAX_SIZE];
                    for (int i = 0; i < MAX_SIZE; i++)
                    {
                        dir_contents[i] = (char *)malloc(MAX_SIZE);
                        memset(dir_contents[i], '\0', MAX_SIZE);
                    }

                    if (count_contents <= 0)
                    {
                        continue;
                    }
                    else
                    {
                        int counter = 0;
                        int count = 0;

                        while (counter < count_contents)
                        {
                            if (word[0] != '\0' && isPrefix(word, contents[counter]->d_name) && contents[counter]->d_name[0] != '.')
                            {
                                strcpy(dir_contents[count++], contents[counter]->d_name);
                            }
                            else if (word[0] == '\0' && contents[counter]->d_name[0] != '.')
                            {
                                strcpy(dir_contents[count++], contents[counter]->d_name);
                            }

                            counter++;
                        }

                        if (count == 0)
                        {
                            continue;
                        }

                        if (count == 1)
                        {
                            printf("%s", &dir_contents[0][strlen(word)]);
                            strcat(CURR_CMD, &dir_contents[0][strlen(word)]);
                            ptr += strlen(&dir_contents[0][strlen(word)]);

                            stat(dir_contents[0], &fileStat);
                            if (S_ISDIR(fileStat.st_mode))
                            {
                                printf("/");
                                strcat(CURR_CMD, "/");
                                ptr++;
                            }
                            else
                            {
                                printf(" ");
                            }
                        }
                        else
                        {
                            char *sort_contents[count];
                            for (int i = 0; i < count; i++)
                            {
                                sort_contents[i] = (char *)malloc(MAX_SIZE);
                            }
                            for (int i = 0; i < count; i++)
                            {
                                strcpy(sort_contents[i], dir_contents[i]);
                            }
                            qsort(sort_contents, count, sizeof(const char *), cmp_string);
                            char common[MAX_SIZE];
                            memset(common, '\0', MAX_SIZE);
                            int index = 0;
                            while (index < min(strlen(sort_contents[0]), strlen(sort_contents[count - 1])) && sort_contents[0][index] == sort_contents[count - 1][index])
                            {
                                index++;
                            }
                            strncpy(common, sort_contents[0], index);
                            if (strcmp(word, common) == 0)
                            {
                                printf("\n");
                                for (int i = 0; i < count; i++)
                                {
                                    printf("%s\n", sort_contents[i]);
                                }
                                prompt();
                                printf("%s", CURR_CMD);
                                flag = true;
                                break;
                            }
                            else
                            {
                                printf("%s", &common[strlen(word)]);
                                strcat(CURR_CMD, &common[strlen(word)]);
                                ptr += strlen(&common[strlen(word)]);
                            }
                        }
                    }
                }
                else if (c == 10)
                {
                    CURR_CMD[ptr++] = c;
                    printf("%c", c);
                    break;
                }
                else if (c == 127)
                {
                    if (ptr > 0)
                    {
                        CURR_CMD[--ptr] = '\0';
                        printf("\b \b");
                    }
                }
                else
                {
                    CURR_CMD[ptr++] = c;
                    printf("%c", c);
                }
            }
            else
            {
                CURR_CMD[ptr++] = c;
                printf("%c", c);
            }
        }
        disableRawMode();
        if (flag)
        {
            continue;
        }

        add_to_history();
        tokenize();

        for (int i = 0; i < commands; i++)
        {
            if (piping(i) == 1)
            {
                continue;
            }
            else if (piping(i) == 0)
            {
                execute(LIST_CMD[i]);
            }

            dup2(stdin_fileno, STDIN_FILENO);
            dup2(stdout_fileno, STDOUT_FILENO);
        }
    }
}