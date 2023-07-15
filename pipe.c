#include "execute.h"
#include "headers.h"
#include "misc.h"
#include "pipe.h"

extern char *LIST_CMD[MAX_SIZE];
extern int argc;
extern char *argv[MAX_SIZE];
extern int stdin_fileno;
extern int stdout_fileno;

int piping(int i)
{
    if (LIST_CMD[i][0] == '\0')
    {
        return 0;
    }

    int count_pipes = 0;
    for (int j = 0; j < strlen(LIST_CMD[i]); j++)
    {
        if (LIST_CMD[i][j] == '|')
        {
            count_pipes++;
        }
    }

    if (count_pipes == 0)
    {
        return 0;
    }

    int count_pipe_commands = 0;
    char *pipe_commands[MAX_SIZE];

    char *tokenize = strtok(LIST_CMD[i], "|");
    while (tokenize != NULL)
    {
        pipe_commands[count_pipe_commands++] = tokenize;
        tokenize = strtok(NULL, "|");
    }

    if (count_pipe_commands != count_pipes + 1)
    {
        error("Error: pipe: invalid syntax for piping\n");
        return -1;
    }

    int pipe_tube[count_pipe_commands][2];
    for (int i = 0; i < count_pipe_commands; i++)
    {
        if (pipe(pipe_tube[i]) < 0)
        {
            red();
            perror("Error: pipe");
            reset_error();
            return -1;
        }
    }

    if (dup2(pipe_tube[0][1], STDOUT_FILENO) < 0)
    {
        red();
        perror("Error: pipe");
        reset_error();
        return -1;
    }

    execute(pipe_commands[0]);
    close(pipe_tube[0][1]);

    for (int i = 1; i < count_pipe_commands - 1; i++)
    {
        if (dup2(pipe_tube[i - 1][0], STDIN_FILENO) < 0)
        {
            red();
            perror("Error: pipe");
            reset_error();
            return -1;
        }

        if (dup2(pipe_tube[i][1], STDOUT_FILENO) < 0)
        {
            red();
            perror("Error: pipe");
            reset_error();
            return -1;
        }

        execute(pipe_commands[i]);
        close(pipe_tube[i][1]);
        close(pipe_tube[i - 1][0]);
    }

    if (dup2(pipe_tube[count_pipe_commands - 2][0], STDIN_FILENO) < 0)
    {
        red();
        perror("Error: pipe");
        reset_error();
        return -1;
    }

    if (dup2(stdout_fileno, STDOUT_FILENO) < 0)
    {
        red();
        perror("Error: pipe");
        reset_error();
        return -1;
    }

    execute(pipe_commands[count_pipe_commands - 1]);
    close(pipe_tube[count_pipe_commands - 2][0]);

    if (dup2(stdin_fileno, STDIN_FILENO) < 0)
    {
        red();
        perror("Error: pipe");
        reset_error();
        return -1;
    }

    if (dup2(stdout_fileno, STDOUT_FILENO) < 0)
    {
        red();
        perror("Error: pipe");
        reset_error();
        return -1;
    }

    return 1;
}