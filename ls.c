#include "headers.h"
#include "ls.h"
#include "misc.h"

extern char HOME[MAX_SIZE];
extern char CWD[MAX_SIZE];
extern int argc;
extern char *argv[MAX_SIZE];

bool a_flag = false;
bool l_flag = false;
bool multiple = false;
bool single = false;

extern int alphasort();

bool isHidden(struct dirent *contests)
{
    return (contests->d_name[0] == '.') && (!a_flag);
}

bool isDir(struct stat fileStat)
{
    return S_ISDIR(fileStat.st_mode);
}

bool isExecutable(struct stat fileStat)
{
    return fileStat.st_mode & S_IXUSR || fileStat.st_mode & S_IXGRP || fileStat.st_mode & S_IXOTH;
}

void printName(struct dirent *contents)
{
    struct stat fileStat;
    stat(contents->d_name, &fileStat);

    if (!isHidden(contents))
    {
        if (isDir(fileStat))
        {
            blue();
            printf("%s\n", contents->d_name);
            reset();
        }
        else if (isExecutable(fileStat))
        {
            green();
            printf("%s\n", contents->d_name);
            reset();
        }
        else
        {
            printf("%s\n", contents->d_name);
        }
    }
}

void printInformation(struct stat fileStat, struct dirent *contents)
{
    if (!isHidden(contents))
    {
        if (S_ISREG(fileStat.st_mode))
        {
            printf("-");
        }
        else if (S_ISDIR(fileStat.st_mode))
        {
            printf("d");
        }
        else if (S_ISCHR(fileStat.st_mode))
        {
            printf("c");
        }
        else if (S_ISBLK(fileStat.st_mode))
        {
            printf("b");
        }
        else if (S_ISFIFO(fileStat.st_mode))
        {
            printf("p");
        }
        else if (S_ISLNK(fileStat.st_mode))
        {
            printf("l");
        }
        else if (S_ISSOCK(fileStat.st_mode))
        {
            printf("s");
        }

        if (fileStat.st_mode & S_IRUSR)
        {
            printf("r");
        }
        else
        {
            printf("-");
        }

        if (fileStat.st_mode & S_IWUSR)
        {
            printf("w");
        }
        else
        {
            printf("-");
        }

        if (fileStat.st_mode & S_IXUSR)
        {
            printf("x");
        }
        else
        {
            printf("-");
        }
        if (fileStat.st_mode & S_IRGRP)
        {
            printf("r");
        }
        else
        {
            printf("-");
        }

        if (fileStat.st_mode & S_IWGRP)
        {
            printf("w");
        }
        else
        {
            printf("-");
        }

        if (fileStat.st_mode & S_IXGRP)
        {
            printf("x");
        }
        else
        {
            printf("-");
        }
        if (fileStat.st_mode & S_IROTH)
        {
            printf("r");
        }
        else
        {
            printf("-");
        }

        if (fileStat.st_mode & S_IWOTH)
        {
            printf("w");
        }
        else
        {
            printf("-");
        }

        if (fileStat.st_mode & S_IXOTH)
        {
            printf("x");
        }
        else
        {
            printf("-");
        }

        printf(". ");
        printf("%d ", fileStat.st_nlink);

        struct passwd *pw = getpwuid(fileStat.st_uid);
        struct group *gr = getgrgid(fileStat.st_gid);

        printf("%s ", pw->pw_name);
        printf("%s ", gr->gr_name);

        printf("%7d ", fileStat.st_size);

        struct tm *m_time = localtime(&(fileStat.st_mtime));
        char months[12][5];
        strcpy(&months[0][0], "Jan");
        strcpy(&months[1][0], "Feb");
        strcpy(&months[2][0], "Mar");
        strcpy(&months[3][0], "Apr");
        strcpy(&months[4][0], "May");
        strcpy(&months[5][0], "Jun");
        strcpy(&months[6][0], "Jul");
        strcpy(&months[7][0], "Aug");
        strcpy(&months[8][0], "Sept");
        strcpy(&months[9][0], "Oct");
        strcpy(&months[10][0], "Nov");
        strcpy(&months[11][0], "Dec");

        printf("%4s ", months[m_time->tm_mon]);
        printf("%2d ", m_time->tm_mday);
        printf("%2d:", m_time->tm_hour);
        printf("%2d ", m_time->tm_min);

        printName(contents);
    }
}

void print()
{
    struct dirent **contents;
    int count_contents = scandir("./", &contents, NULL, alphasort);

    if (count_contents == -1)
    {
        red();
        perror("Error: ls");
        reset_error();
        chdir(CWD);
        return;
    }

    int i = 0;
    struct stat fileStat;

    int total = 0;
    if (l_flag)
    {
        while (i < count_contents)
        {
            stat(contents[i]->d_name, &fileStat);
            total += fileStat.st_blocks;
            i++;
        }

        printf("total %d\n", total / 2);
    }

    i = 0;

    while (i < count_contents)
    {
        stat(contents[i]->d_name, &fileStat);

        if (l_flag)
        {
            printInformation(fileStat, contents[i]);
        }
        else
        {
            printName(contents[i]);
        }

        i++;
    }
}

void ls()
{
    a_flag = false;
    l_flag = false;
    multiple = false;
    single = false;

    int arguments = argc - 1;

    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "-a") == 0)
        {
            a_flag = true;
        }
        else if (strcmp(argv[i], "-l") == 0)
        {
            l_flag = true;
        }
        else if (strcmp(argv[i], "-al") == 0 || strcmp(argv[i], "-la") == 0)
        {
            a_flag = true;
            l_flag = true;
        }
    }

    if (a_flag)
    {
        arguments--;
    }
    if (l_flag)
    {
        arguments--;
    }

    if (arguments > 1)
    {
        multiple = true;
    }

    if (arguments == 0)
    {
        single = true;
    }

    char target_dir[MAX_SIZE];
    strcpy(target_dir, CWD);

    if (single)
    {
        print();
        return;
    }

    for (int i = 1; i < argc; i++)
    {
        if (strcmp(argv[i], "-a") != 0 && strcmp(argv[i], "-l") != 0 && strcmp(argv[i], "-al") != 0 && strcmp(argv[i], "-la") != 0)
        {
            if (strcmp(argv[i], "~") == 0)
            {
                strcpy(target_dir, HOME);
                chdir(target_dir);
                print();
            }
            else if (strncmp(&argv[i][0], "~", 1) == 0)
            {
                strcpy(target_dir, HOME);
                strcat(target_dir, &argv[i][1]);

                struct stat fileStat;
                if (stat(target_dir, &fileStat) == -1)
                {
                    red();
                    fflush(stdout);
                    perror("Error: ");
                    reset();
                    return;
                }

                chdir(target_dir);
                print();
            }
            else
            {
                struct stat fileStat;

                if (stat(argv[i], &fileStat) == -1)
                {
                    red();
                    fflush(stdout);
                    fprintf(stderr, "Error: ls: cannot access %s: No such file or directory\n", argv[i]);
                    reset();
                }
                else
                {
                    if (isDir(fileStat))
                    {
                        if (multiple)
                        {
                            printf("%s:\n", argv[i]);
                        }

                        strcpy(target_dir, argv[i]);
                        chdir(target_dir);
                        print();
                    }
                    else
                    {
                        printf("%s\n", argv[i]);
                    }
                }
            }
        }
    }

    chdir(CWD);
}