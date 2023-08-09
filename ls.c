#include "headers.h"

void swap(struct dirent **arrfile1, struct dirent **arrfile2){
    struct dirent *temp = *arrfile1;
    *arrfile1 = *arrfile2;
    *arrfile2 = temp;
}

void sort(struct dirent **arrfile, int no_files){
    for (int i = no_files - 1; i >= 0; i--){
        for (int j = 0; j < i; j++){
            if (strcasecmp(arrfile[j]->d_name, arrfile[j + 1]->d_name) > 0){
                swap(&(arrfile[j]), &(arrfile[j + 1]));
            }
        }
    }
}

void info(char *path, char *name)
{
    struct stat file_stat;
    char file_path[1000];
    strcpy(file_path, path);
    strcat(file_path, "/");
    strcat(file_path, name);
    struct group *groups;
    struct passwd *passwds;

    if (stat(file_path, &file_stat) < 0)
    {
        return;
    }
    groups = getgrgid(file_stat.st_gid);
    passwds = getpwuid(file_stat.st_uid);
    printf((S_ISDIR(file_stat.st_mode)) ? "d" : "-");
    printf((file_stat.st_mode & S_IRUSR) ? "r" : "-");
    printf((file_stat.st_mode & S_IWUSR) ? "w" : "-");
    printf((file_stat.st_mode & S_IXUSR) ? "x" : "-");
    printf((file_stat.st_mode & S_IRGRP) ? "r" : "-");
    printf((file_stat.st_mode & S_IWGRP) ? "w" : "-");
    printf((file_stat.st_mode & S_IXGRP) ? "x" : "-");
    printf((file_stat.st_mode & S_IROTH) ? "r" : "-");
    printf((file_stat.st_mode & S_IWOTH) ? "w" : "-");
    printf((file_stat.st_mode & S_IXOTH) ? "x " : "- ");
    printf("%3ld ", file_stat.st_nlink);
    printf("%s ", passwds->pw_name);
    printf("%s ", groups->gr_name);
    printf(" %6ld\t", file_stat.st_size);
    printf(" %.20s ", ctime(&file_stat.st_mtime));
    if (S_ISDIR(file_stat.st_mode))
    {
        printf("\033[1;34m");
    }
    else if ((file_stat.st_mode & S_IXUSR))
    {
        printf("\033[1;32m");
    }
    printf("%s\n", name);
    printf("\033[0m");
}

void ls2(char *path, int hidden)
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
        sort(arrfile, no_files);
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
                    blockcount += (file_stat.st_blocks);
                }
            }
            else if (hidden == 1)
            {
                struct stat file_stat;
                char file_path[1000];
                strcpy(file_path, path);
                strcat(file_path, "/");
                strcat(file_path, arrfile[i]->d_name);
                stat(file_path, &file_stat);
                blockcount += (file_stat.st_blocks);
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
                    blockcount += (file_stat.st_blocks);
                }
            }
            else if (hidden == 3)
            {
                struct stat file_stat;
                char file_path[1000];
                strcpy(file_path, path);
                strcat(file_path, "/");
                strcat(file_path, arrfile[i]->d_name);
                stat(file_path, &file_stat);
                blockcount += (file_stat.st_blocks);
            }
        }
    }
    if (hidden == 3 || hidden == 2)
        printf("total %d\n", blockcount / 2);

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
                }
                else if ((file_stat.st_mode & S_IXUSR))
                {
                    printf("\033[1;32m");
                }
                printf("%s\n", arrfile[i]->d_name);
                printf("\033[0m");
            }
        }
        else if (hidden == 1)
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
            }
            else if ((file_stat.st_mode & S_IXUSR))
            {
                printf("\033[1;32m");
            }
            printf("%s\n", arrfile[i]->d_name);
            printf("\033[0m");
        }
        else if (hidden == 2)
        {
            if (arrfile[i]->d_name[0] != '.')
                info(path, arrfile[i]->d_name);
        }
        else if (hidden == 3)
        {
            info(path, arrfile[i]->d_name);
        }
        free(arrfile[i]);
    }
    free(arrfile);
}

void ls(char tokens[][200], int count, char *buf)
{
    if (tokens[1][0] == '~')
    {
        char a2[200];
        strcpy(a2, buf);
        int i;
        for (i = 1; tokens[1][i] != '\0'; i++)
        {
            a2[i + strlen(buf) - 1] = tokens[1][i];
        }
        a2[i + strlen(buf) - 1] = '\0';
        strcpy(tokens[1], a2);
    }
    struct dirent *de;
    if (count == 1)
    {
        ls2(".", 0);
    }
    else if (count == 2)
    {
        if (strcmp(tokens[1], "-a") == 0)
        {
            ls2(".", 1);
        }
        else if (strcmp(tokens[1], "-l") == 0)
        {
            ls2(".", 2);
        }
        else if (strcmp(tokens[1], "-al") == 0 || strcmp(tokens[1], "-la") == 0)
        {
            ls2(".", 3);
        }
        else if (strcmp(tokens[1], "~") == 0)
        {
            ls2(buf, 0);
        }
        else if (strcmp(tokens[1], "..") == 0)
        {
            char buf4[1000];
            getcwd(buf4, 1000);
            int o;
            for (o = strlen(buf4) - 1; o >= 0; o--)
            {
                if (buf4[o] == '/')
                {
                    break;
                }
            }
            char buf5[1000];
            strncpy(buf5, buf4, o + 1);
            buf5[o + 1] = '\0';
            ls2(buf5, 0);
        }
        else if (strcmp(tokens[1], ".") == 0)
        {
            ls2(".", 0);
        }
        else
        {
            struct stat file_stat;
            if (stat(tokens[1], &file_stat) < 0)
            {
                printf("\033[1;31mls: cannot access '%s': No such file or directory\n\033[0m", tokens[1]);
            }
            else
            {
                if (S_ISDIR(file_stat.st_mode))
                {
                    ls2(tokens[1], 0);
                }
                else
                {
                    if ((file_stat.st_mode & S_IXUSR))
                    {
                        printf("\033[1;32m");
                    }
                    printf("%s\n", tokens[1]);
                    printf("\033[0m");
                }
            }
        }
    }
    else
    {
        int count2 = 0;
        char dir[20][100];
        int direc = 0;
        int a_come = 0;
        int l_come = 0;
        for (int i = 1; i < count; i++)
        {
            if (strcmp(tokens[i], "-a") == 0)
            {
                a_come = 1;
            }
            else if (strcmp(tokens[i], "-l") == 0)
            {
                l_come = 1;
            }
            else if (strcmp(tokens[i], "-al") == 0 || strcmp(tokens[i], "-la") == 0)
            {
                a_come = l_come = 1;
            }
            else
            {
                direc = 1;
                if (tokens[i][0] == '~')
                {
                    char a2[200];
                    strcpy(a2, buf);
                    int x;
                    for (x = 1; tokens[i][x] != '\0'; x++)
                    {
                        a2[x + strlen(buf) - 1] = tokens[i][x];
                    }
                    a2[x + strlen(buf) - 1] = '\0';
                    strcpy(tokens[i], a2);
                }
                strcpy(dir[count2++], tokens[i]);
            }
        }
        if (direc != 1)
        {
            if (a_come)
            {
                if (l_come)
                {
                    ls2(".", 3);
                }
                else
                {
                    ls2(".", 1);
                }
            }
            else
            {
                if (l_come)
                {
                    ls2(".", 2);
                }
                else
                {
                    printf("\033[1;31mError: No such file or directory\n\033[0m");
                }
            }
        }
        else
        {
            if (a_come)
            {
                if (l_come)
                {
                    for (int i = 0; i < count2; i++)
                    {
                        struct stat file_stat;
                        if (stat(dir[i], &file_stat) < 0)
                        {
                            printf("\033[1;31mls: cannot access '%s': No such file or directory\n\033[0m", dir[i]);
                            printf("\n");
                        }
                        else
                        {
                            if (S_ISDIR(file_stat.st_mode))
                            {
                                printf("%s:\n", dir[i]);
                                ls2(dir[i], 3);
                                printf("\n");
                            }
                            else
                            {
                                struct group *groups;
                                struct passwd *passwds;
                                groups = getgrgid(file_stat.st_gid);
                                passwds = getpwuid(file_stat.st_uid);
                                printf((S_ISDIR(file_stat.st_mode)) ? "d" : "-");
                                printf((file_stat.st_mode & S_IRUSR) ? "r" : "-");
                                printf((file_stat.st_mode & S_IWUSR) ? "w" : "-");
                                printf((file_stat.st_mode & S_IXUSR) ? "x" : "-");
                                printf((file_stat.st_mode & S_IRGRP) ? "r" : "-");
                                printf((file_stat.st_mode & S_IWGRP) ? "w" : "-");
                                printf((file_stat.st_mode & S_IXGRP) ? "x" : "-");
                                printf((file_stat.st_mode & S_IROTH) ? "r" : "-");
                                printf((file_stat.st_mode & S_IWOTH) ? "w" : "-");
                                printf((file_stat.st_mode & S_IXOTH) ? "x " : "- ");
                                printf("%3ld ", file_stat.st_nlink);
                                printf("%s ", passwds->pw_name);
                                printf("%s ", groups->gr_name);
                                printf(" %6ld\t", file_stat.st_size);
                                printf(" %.20s ", ctime(&file_stat.st_mtime));
                                if ((file_stat.st_mode & S_IXUSR))
                                {
                                    printf("\033[1;32m");
                                }
                                printf("%s\n", dir[i]);
                                printf("\033[0m");
                                printf("\n");
                            }
                        }
                    }
                }
                else
                {
                    for (int i = 0; i < count2; i++)
                    {
                        struct stat file_stat;
                        if (stat(dir[i], &file_stat) < 0)
                        {
                            printf("\033[1;31mls: cannot access '%s': No such file or directory\n\033[0m", dir[i]);
                        }
                        else
                        {
                            if (S_ISDIR(file_stat.st_mode))
                            {
                                printf("%s:\n", dir[i]);
                                ls2(dir[i], 1);
                                printf("\n");
                            }
                            else
                            {
                                if ((file_stat.st_mode & S_IXUSR))
                                {
                                    printf("\033[1;32m");
                                }
                                printf("%s\n\n", dir[i]);
                                printf("\033[0m");
                            }
                        }
                    }
                }
            }
            else
            {
                if (l_come)
                {
                    for (int i = 0; i < count2; i++)
                    {
                        struct stat file_stat;
                        if (stat(dir[i], &file_stat) < 0)
                        {
                            printf("\033[1;31mls: cannot access '%s': No such file or directory\n\033[0m", dir[i]);
                        }
                        else
                        {
                            if (S_ISDIR(file_stat.st_mode))
                            {
                                printf("%s:\n", dir[i]);
                                ls2(dir[i], 2);
                                printf("\n");
                            }
                            else
                            {
                                struct group *groups;
                                struct passwd *passwds;
                                groups = getgrgid(file_stat.st_gid);
                                passwds = getpwuid(file_stat.st_uid);
                                printf((S_ISDIR(file_stat.st_mode)) ? "d" : "-");
                                printf((file_stat.st_mode & S_IRUSR) ? "r" : "-");
                                printf((file_stat.st_mode & S_IWUSR) ? "w" : "-");
                                printf((file_stat.st_mode & S_IXUSR) ? "x" : "-");
                                printf((file_stat.st_mode & S_IRGRP) ? "r" : "-");
                                printf((file_stat.st_mode & S_IWGRP) ? "w" : "-");
                                printf((file_stat.st_mode & S_IXGRP) ? "x" : "-");
                                printf((file_stat.st_mode & S_IROTH) ? "r" : "-");
                                printf((file_stat.st_mode & S_IWOTH) ? "w" : "-");
                                printf((file_stat.st_mode & S_IXOTH) ? "x " : "- ");
                                printf("%3ld ", file_stat.st_nlink);
                                printf("%s ", passwds->pw_name);
                                printf("%s ", groups->gr_name);
                                printf(" %6ld\t", file_stat.st_size);
                                printf(" %.20s ", ctime(&file_stat.st_mtime));
                                if ((file_stat.st_mode & S_IXUSR))
                                {
                                    printf("\033[1;32m");
                                }
                                printf("%s\n", dir[i]);
                                printf("\033[0m");
                                printf("\n");
                            }
                        }
                    }
                }
                else
                {
                    for (int i = 0; i < count2; i++)
                    {
                        struct stat file_stat;
                        if (stat(dir[i], &file_stat) < 0)
                        {
                            printf("\033[1;31mls: cannot access '%s': No such file or directory\n\033[0m", dir[i]);
                        }
                        else
                        {
                            if (S_ISDIR(file_stat.st_mode))
                            {
                                printf("%s:\n", dir[i]);
                                ls2(dir[i], 0);
                                printf("\n");
                            }
                            else
                            {
                                if ((file_stat.st_mode & S_IXUSR))
                                {
                                    printf("\033[1;32m");
                                }
                                printf("%s\n\n", dir[i]);
                                printf("\033[0m");
                            }
                        }
                    }
                }
            }
        }
    }
}