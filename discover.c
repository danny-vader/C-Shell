#include "discover.h"
#include "headers.h"
#include "ls.h"
#include "misc.h"

extern char HOME[MAX_SIZE];
extern int argc;
extern char *argv[MAX_SIZE];

bool d_flag = false;
bool f_flag = false;
bool search = false;
bool found = false;

char search_file_name[MAX_SIZE];
char search_file_path[MAX_SIZE];

struct dirent *contents;
struct stat fileStat;

void printDir(char *path)
{
    DIR *dir = opendir(path);
    if (dir == NULL)
    {
        return;
    }

    while ((contents = readdir(dir)) != NULL)
    {
        char buffer[MAX_SIZE];
        strcpy(buffer, path);
        if (buffer[strlen(buffer) - 1] != '/')
        {
            strcat(buffer, "/");
        }

        if (strcmp(contents->d_name, ".") != 0 && strcmp(contents->d_name, "..") != 0)
        {
            strcat(buffer, contents->d_name);
            stat(buffer, &fileStat);

            if (d_flag && f_flag || !d_flag && !f_flag)
            {
                printf("%s\n", buffer);
                if (isDir(fileStat))
                {
                    printDir(buffer);
                }
            }
            else if (d_flag)
            {
                if (isDir(fileStat))
                {
                    printf("%s\n", buffer);
                    printDir(buffer);
                }
            }
            else if (f_flag)
            {
                if (!isDir(fileStat))
                {
                    printf("%s\n", buffer);
                }
                else
                {
                    printDir(buffer);
                }
            }
            else
            {
                printf("%s\n", buffer);
                if (isDir(fileStat))
                {
                    printDir(buffer);
                }
            }
        }
    }

    closedir(dir);
}

void searchFile(char *path)
{
    struct dirent *contents;
    DIR *dir = opendir(path);
    struct stat fileStat;

    if (dir == NULL)
    {
        return;
    }

    while ((contents = readdir(dir)) != NULL)
    {
        char buffer[MAX_SIZE];
        strcpy(buffer, path);
        strcat(buffer, "/");

        if (strcmp(contents->d_name, ".") != 0 && strcmp(contents->d_name, "..") != 0)
        {
            strcat(buffer, contents->d_name);
            stat(buffer, &fileStat);

            if (d_flag && f_flag || !d_flag && !f_flag)
            {
                if (strcmp(contents->d_name, search_file_name) == 0)
                {
                    strcpy(search_file_path, buffer);
                    found = true;
                    return;
                }
                if (isDir(fileStat))
                {
                    searchFile(buffer);
                }
            }
            else if (d_flag)
            {
                if (isDir(fileStat))
                {
                    searchFile(buffer);
                }
            }
            else if (f_flag)
            {
                if (!isDir(fileStat))
                {
                    if (strcmp(contents->d_name, search_file_name) == 0)
                    {
                        strcpy(search_file_path, buffer);
                        found = true;
                        return;
                    }
                }
                else
                {
                    searchFile(buffer);
                }
            }
        }
    }

    closedir(dir);
}

void discover()
{
    found = false;
    search = false;
    d_flag = false;
    f_flag = false;

    if (argc > 4)
    {
        error("Error: discover: too many arguments\n");
        return;
    }

    char target_dir[MAX_SIZE];
    strcpy(target_dir, ".");

    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "-d") == 0)
        {
            d_flag = true;
        }
        else if (strcmp(argv[i], "-f") == 0)
        {
            f_flag = true;
        }
        else if (strcmp(argv[i], "~") == 0)
        {
            strcpy(target_dir, HOME);
        }
        else if (strncmp(&argv[i][0], "~", 1) == 0)
        {
            strcpy(target_dir, HOME);
            strcat(target_dir, &argv[i][1]);

            struct stat fileStat;
            if (stat(target_dir, &fileStat) == -1)
            {
                red();
                perror("Error: discover");
                reset_error();
                return;
            }
        }
        else
        {
            if (argv[i][0] == '"')
            {
                for (int j = 1; j < strlen(argv[i]) - 1; j++)
                {
                    search_file_name[j - 1] = argv[i][j];
                }
                search = true;
            }
            else
            {
                struct stat fileStat;
                if (stat(argv[i], &fileStat) == -1)
                {
                    red();
                    fflush(stdout);
                    perror("Error");
                    reset_error();
                    return;
                }

                if (isDir(fileStat))
                {
                    strcpy(target_dir, argv[i]);
                }
                else
                {
                    red();
                    fprintf(stderr, "Error: discover: No such target directory found\n");
                    reset_error();
                    return;
                }
            }
        }
    }

    if (search)
    {
        searchFile(target_dir);
        if (found)
        {
            printf("%s\n", search_file_path);
            return;
        }
        else
        {
            red();
            fprintf(stderr, "Error: discover: No such file found\n");
            reset_error();
            return;
        }
    }
    else
    {
        if (!f_flag)
        {
            printf("%s\n", target_dir);
        }
        printDir(target_dir);
    }
}