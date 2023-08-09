#include "headers.h"

void discovery(char tokens[][200], int count, char *buf)
{
    int d_come = 0;
    int f_come = 0;
    int file_come = 0;
    int dir_come = 0;
    char file[1000];
    char dir[1000];
    for (int bhians = 1; bhians < count; bhians++)
    {
        if (strcmp(tokens[bhians], "-d") == 0)
        {
            d_come = 1;
        }
        else if (strcmp(tokens[bhians], "-f") == 0)
        {
            f_come = 1;
        }
        else if (strcmp(tokens[bhians], "-df") == 0 || strcmp(tokens[bhians], "-fd") == 0)
        {
            d_come = 1;
            f_come = 1;
        }
        else if (tokens[bhians][0] == '"')
        {
            file_come = 1;
            strncpy(file, tokens[bhians] + 1, strlen(tokens[bhians]) - 2);
        }
        else
        {
            dir_come = 1;
            if (tokens[bhians][0] == '~')
            {
                if (strlen(tokens[bhians]) == 1)
                {
                    strcpy(dir, buf);
                }
                else
                {
                    char file_path[200];
                    strcpy(file_path, buf);
                    strcat(file_path, "/");
                    int copy = strlen(file_path);
                    for (int cat = 2; tokens[bhians][cat] != '\0'; cat++)
                    {
                        file_path[copy++] = tokens[bhians][cat];
                    }
                    file_path[copy] = '\0';
                    strcpy(dir, file_path);
                }
            }
            else
            {
                strcpy(dir, tokens[bhians]);
            }
        }
    }
    if (!file_come)
    {
        if (dir_come)
        {
            if (d_come)
            {
                if (f_come)
                {
                    printf("\033[1;34m");
                    printf("%s\n", dir);
                    printf("\033[0m");
                    discover(dir, 0);
                }
                else
                {
                    printf("\033[1;34m");
                    printf("%s\n", dir);
                    printf("\033[0m");
                    discover(dir, 1);
                }
            }
            else
            {
                if (f_come)
                {
                    discover(dir, 2);
                }
                else
                {
                    printf("\033[1;34m");
                    printf("%s\n", dir);
                    printf("\033[0m");
                    discover(dir, 0);
                }
            }
        }
        else
        {
            strcpy(dir, ".");
            if (d_come)
            {
                if (f_come)
                {
                    printf("\033[1;34m");
                    printf("%s\n", dir);
                    printf("\033[0m");
                    discover(dir, 0);
                }
                else
                {
                    printf("\033[1;34m");
                    printf("%s\n", dir);
                    printf("\033[0m");
                    discover(dir, 1);
                }
            }
            else
            {
                if (f_come)
                {
                    discover(dir, 2);
                }
                else
                {
                    printf("\033[1;34m");
                    printf("%s\n", dir);
                    printf("\033[0m");
                    discover(dir, 0);
                }
            }
        }
    }
    else
    {
        if (dir_come)
        {
            if (d_come)
            {
                if (f_come)
                {
                    find(dir, 1, file);
                }
                else
                {
                    // printf("kutta\n");
                    find(dir, 2, file);
                }
            }
            else
            {
                if (f_come)
                {
                    // printf("kfd");
                    find(dir, 0, file);
                }
                else
                {
                    find(dir, 1, file);
                }
            }
        }
        else
        {
            strcpy(dir, ".");
            if (d_come)
            {
                if (f_come)
                {
                    find(dir, 1, file);
                }
                else
                {
                    // printf("kutta\n");
                    find(dir, 2, file);
                }
            }
            else
            {
                if (f_come)
                {
                    // printf("kfd");
                    find(dir, 0, file);
                }
                else
                {
                    find(dir, 1, file);
                }
            }
        }
    }
}

void discover(char *path, int hidden)
{
    struct dirent **arrfile;
    int no_files = scandir(path, &arrfile, NULL, alphasort);

    int blockcount = 0;
    if (no_files == -1)
    {
        perror("\033[1;31mError\033[0m");
        return;
    }
    else
    {
        for (int i = 0; i < no_files; i++)
        {
            if (hidden == 0)
            {
                if (arrfile[i]->d_name[0] != '.')
                {
                    struct stat file_stat;
                    char file_path[1000];
                    strcpy(file_path, path);
                    strcat(file_path, "/");
                    strcat(file_path, arrfile[i]->d_name);
                    stat(file_path, &file_stat);
                    if (S_ISDIR(file_stat.st_mode))
                    {
                        printf("\033[1;34m");
                        printf("%s\n", file_path);
                        printf("\033[0m");
                        discover(file_path, 0);
                    }
                    else if ((file_stat.st_mode & S_IXUSR))
                    {
                        printf("\033[1;32m");
                        printf("%s\n", file_path);
                        printf("\033[0m");
                    }
                    else
                    {
                        printf("%s\n", file_path);
                    }
                }
            }
            else if (hidden == 1)
            {
                if (arrfile[i]->d_name[0] != '.')
                {
                    struct stat file_stat;
                    char file_path[1000];
                    strcpy(file_path, path);
                    strcat(file_path, "/");
                    strcat(file_path, arrfile[i]->d_name);
                    stat(file_path, &file_stat);
                    if (S_ISDIR(file_stat.st_mode))
                    {
                        printf("\033[1;34m");
                        printf("%s\n", file_path);
                        printf("\033[0m");
                        discover(file_path, 1);
                    }
                }
            }
            else if (hidden == 2)
            {
                if (arrfile[i]->d_name[0] != '.')
                {
                    struct stat file_stat;
                    char file_path[1000];
                    strcpy(file_path, path);
                    strcat(file_path, "/");
                    strcat(file_path, arrfile[i]->d_name);
                    stat(file_path, &file_stat);
                    if (S_ISDIR(file_stat.st_mode))
                    {
                        discover(file_path, 2);
                    }
                    else if ((file_stat.st_mode & S_IXUSR))
                    {
                        printf("\033[1;32m");
                        printf("%s\n", file_path);
                        printf("\033[0m");
                    }
                    else
                    {
                        printf("%s\n", file_path);
                    }
                }
            }
            free(arrfile[i]);
        }
    }
    free(arrfile);
}

void find(char *path, int hidden, char *file)
{
    struct dirent **arrfile;
    int no_files = scandir(path, &arrfile, NULL, alphasort);

    int blockcount = 0;
    if (no_files == -1)
    {
        perror("\033[1;31mError\033[0m");
        return;
    }
    else
    {
        for (int i = 0; i < no_files; i++)
        {
            if (hidden == 0)
            {
                if (arrfile[i]->d_name[0] != '.')
                {
                    struct stat file_stat;
                    char file_path[1000];
                    strcpy(file_path, path);
                    strcat(file_path, "/");
                    strcat(file_path, arrfile[i]->d_name);
                    stat(file_path, &file_stat);
                    if (S_ISDIR(file_stat.st_mode))
                    {
                        find(file_path, hidden, file);
                    }
                    else if ((file_stat.st_mode & S_IXUSR))
                    {
                        printf("\033[1;32m");
                        if (strcmp(arrfile[i]->d_name, file) == 0)
                            printf("%s\n", file_path);
                        printf("\033[0m");
                    }
                    else
                    {
                        if (strcmp(arrfile[i]->d_name, file) == 0)
                            printf("%s\n", file_path);
                    }
                }
            }
            else if (hidden == 1)
            {
                if (arrfile[i]->d_name[0] != '.')
                {
                    struct stat file_stat;
                    char file_path[1000];
                    strcpy(file_path, path);
                    strcat(file_path, "/");
                    strcat(file_path, arrfile[i]->d_name);
                    stat(file_path, &file_stat);
                    if (S_ISDIR(file_stat.st_mode))
                    {
                        if (strcmp(arrfile[i]->d_name, file) == 0)
                        {
                            printf("\033[1;34m");
                            printf("%s\n", file_path);
                            printf("\033[0m");
                        }
                        find(file_path, hidden, file);
                    }
                    else if ((file_stat.st_mode & S_IXUSR))
                    {
                        printf("\033[1;32m");
                        if (strcmp(arrfile[i]->d_name, file) == 0)
                            printf("%s\n", file_path);
                        printf("\033[0m");
                    }
                    else
                    {
                        if (strcmp(arrfile[i]->d_name, file) == 0)
                            printf("%s\n", file_path);
                    }
                }
            }
            else if (hidden == 2)
            {
                if (arrfile[i]->d_name[0] != '.')
                {
                    struct stat file_stat;
                    char file_path[1000];
                    strcpy(file_path, path);
                    strcat(file_path, "/");
                    strcat(file_path, arrfile[i]->d_name);
                    stat(file_path, &file_stat);
                    if (S_ISDIR(file_stat.st_mode))
                    {
                        if (strcmp(arrfile[i]->d_name, file) == 0)
                        {
                            printf("\033[1;34m");
                            printf("%s\n", file_path);
                            printf("\033[0m");
                        }
                        find(file_path, hidden, file);
                    }
                }
            }
            free(arrfile[i]);
        }
    }
    free(arrfile);
}