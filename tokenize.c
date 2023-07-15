#include "headers.h"
#include "tokenize.h"

extern char CURR_CMD[MAX_SIZE];
extern int commands;
extern char *LIST_CMD[MAX_SIZE];

void tokenize()
{
    commands = 0;

    if (CURR_CMD[strlen(CURR_CMD) - 1] == '\n')
    {
        CURR_CMD[strlen(CURR_CMD) - 1] = '\0';
    }

    char *temp[MAX_SIZE];
    int count = 0;

    char *tokenize = strtok(CURR_CMD, ";");
    while (tokenize != NULL)
    {
        temp[count++] = tokenize;
        tokenize = strtok(NULL, ";");
    }

    for (int i = 0; i < count; i++)
    {
        int bg_count = 0;

        for (int j = 0; j < strlen(temp[i]); j++)
        {
            if (temp[i][j] == '&')
            {
                bg_count++;
            }
        }

        if (bg_count != 0)
        {
            char *bg_tokenize = strtok(temp[i], "&");
            int j = 0;

            while (j < bg_count && bg_tokenize != NULL)
            {
                char *bg = (char *)malloc(MAX_SIZE);
                LIST_CMD[commands] = bg_tokenize;
                strcpy(bg, "& ");
                strcat(bg, LIST_CMD[commands]);
                LIST_CMD[commands] = (char *)malloc(MAX_SIZE);
                strcpy(LIST_CMD[commands], bg);
                bg_tokenize = strtok(NULL, "&");
                commands++;
                j++;
                free(bg);
            }

            while (bg_tokenize != NULL)
            {
                LIST_CMD[commands] = bg_tokenize;
                bg_tokenize = strtok(NULL, "&");
                commands++;
            }
        }
        else
        {
            LIST_CMD[commands] = temp[i];
            commands++;
        }
    }
}