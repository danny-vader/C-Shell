#include "headers.h"
#include "history.h"
#include "misc.h"

extern char HOME[MAX_SIZE];
extern char CURR_CMD[MAX_SIZE];
extern int commands;
extern char *LIST_CMD[MAX_SIZE];
extern int history_count;
extern char *HISTORY[MAX_CMDS];
extern int argc;
extern char *argv[MAX_SIZE];

void init_history_file()
{
    char path[MAX_SIZE];
    strcpy(path, HOME);
    strcat(path, "/");
    strcat(path, "history.txt");

    FILE *history_file = fopen(path, "r");
    history_count = 0;

    for (int i = 0; i < MAX_CMDS; i++)
    {
        HISTORY[i] = (char *)malloc(MAX_SIZE);
    }

    if (history_file == NULL)
    {
        FILE *history_file = fopen(path, "w");
        fclose(history_file);
        return;
    }

    size_t BUF = 0;
    char *BUFFER;
    while (getline(&HISTORY[history_count++], &BUF, history_file) != EOF)
    {
        continue;
    }

    fclose(history_file);
}

void add_to_history()
{
    if (history_count == MAX_CMDS && strcmp(HISTORY[history_count - 1], CURR_CMD) != 0)
    {
        for (int i = 1; i < MAX_CMDS; i++)
        {
            strcpy(HISTORY[i - 1], HISTORY[i]);
        }

        strcpy(HISTORY[MAX_CMDS - 1], CURR_CMD);
    }
    else if (history_count != 0 && strcmp(HISTORY[history_count - 1], CURR_CMD) != 0)
    {
        strcpy(HISTORY[history_count], CURR_CMD);
        history_count++;
    }
    else if (history_count == 0)
    {
        strcpy(HISTORY[history_count], CURR_CMD);
        history_count++;
    }

    char path[MAX_SIZE];
    strcpy(path, HOME);
    strcat(path, "/");
    strcat(path, "history.txt");
    remove(path);

    FILE *history_file = fopen(path, "w");

    for (int i = 0; i < history_count; i++)
    {
        fprintf(history_file, "%s", HISTORY[i]);
    }

    fclose(history_file);
}

void history()
{
    if (argc > 1)
    {
        error("Error: history: too many arguments\n");
        return;
    }

    if (history_count < 10)
    {
        for (int i = 0; i < history_count; i++)
        {
            printf("%s", HISTORY[i]);
        }
    }
    else
    {
        for (int i = history_count - 10; i < history_count; i++)
        {
            printf("%s", HISTORY[i]);
        }
    }
}