#include "headers.h"
#include "misc.h"
#include "redirection.h"

extern int argc;
extern char *argv[MAX_SIZE];

bool redirection(char *command)
{
    bool input = false;
    bool output = false;
    bool append = false;

    char input_file[MAX_SIZE];
    char output_file[MAX_SIZE];

    char *tokenize = strtok(command, " \t\r\n");
    argc = 0;
    while (tokenize != NULL)
    {
        argv[argc++] = tokenize;
        if (strcmp(argv[argc - 1], "<") == 0)
        {
            char *in;
            input = true;
            argc--;
            argv[argc] = NULL;
            tokenize = strtok(NULL, " \t\n");
            in = tokenize;
            strcpy(input_file, in);
            tokenize = strtok(NULL, " \t\n");
        }
        else if (strcmp(argv[argc - 1], ">") == 0)
        {
            char *out;
            output = true;
            argc--;
            argv[argc] = NULL;
            tokenize = strtok(NULL, " \t\n");
            out = tokenize;
            strcpy(output_file, out);
            tokenize = strtok(NULL, " \t\n");
        }
        else if (strcmp(argv[argc - 1], ">>") == 0)
        {
            char *out;
            output = true;
            append = true;
            argc--;
            argv[argc] = NULL;
            tokenize = strtok(NULL, " \t\n");
            out = tokenize;
            strcpy(output_file, out);
            tokenize = strtok(NULL, " \t\n");
        }
        else
        {
            tokenize = strtok(NULL, " \t\n");
        }
        argv[argc] = NULL;
    }

    if (input)
    {
        int fd_in = open(input_file, O_RDONLY);
        if (fd_in < 0)
        {
            red();
            perror("Error: input redirection");
            reset_error();
            return false;
        }

        if (dup2(fd_in, STDIN_FILENO) < 0)
        {
            red();
            perror("Error: input redirection");
            reset_error();
            return false;
        }

        close(fd_in);
    }

    if (output)
    {
        int fd_out;
        if (append)
        {
            fd_out = open(output_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
        }
        else
        {
            fd_out = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        }

        if (fd_out < 0)
        {
            red();
            perror("Error: output redirection");
            reset_error();
            return false;
        }

        if (dup2(fd_out, STDOUT_FILENO) < 0)
        {
            red();
            perror("Error: output redirection");
            reset_error();
            return false;
        }

        close(fd_out);
    }

    return 1;
}