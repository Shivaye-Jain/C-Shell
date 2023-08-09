#include "headers.h"

void pinfo(char tokens[][200], char *buf, int count)
{
    char p1[2000];
    char p2[2000];
    char file_path[3000];
    pid_t pid;
    if (count == 1)
    {
        pid = getpid();
    }
    else if (count == 2)
    {
        pid = atoi(tokens[1]);
    }
    else
    {
        printf("\033[1;31m");
        printf("Error: Too many arguments\n");
        printf("\033[0m");
        return;
    }
    sprintf(p1, "/proc/%d/stat", pid);

    FILE *filedescriptor = fopen(p1, "r");
    if (filedescriptor == NULL)
    {
        printf("\033[1;31m");
        printf("Error: Such a process doesn't exist\n");
        printf("\033[0m");
    }
    else
    {
        sprintf(p2, "/proc/%d/exe", pid);

        char status;
        long long memory;
        int a, b;
        fscanf(filedescriptor, "%d %*s %c %d %*d %d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %lld", &pid, &status, &a, &b, &memory);
        printf("pid : %d\n", pid);
        if (a == b)
            printf("process status : %c+\n", status);
        else
        {
            printf("process status : %c\n", status);
        }
        printf("memory : %lld {Virtual Memory}\n", memory);

        // printf("%s\n", file_path);
        int ret = readlink(p2, file_path, 1000);
        // printf("%s\n", file_path);
        file_path[ret++] = '\0';

        if (strlen(file_path) >= strlen(buf))
        {
            int yes = 1;
            int i;
            for (i = 0; i < strlen(buf); i++)
            {
                if (buf[i] != file_path[i])
                {
                    yes = 0;
                }
            }
            if (yes == 0)
            {
                printf("executable path : %s\n", file_path);
            }
            else if (file_path[i] == '/' || file_path[i] == '\0')
            {
                char file_path2[3000];
                strcpy(file_path2, "~");
                int count = 1;
                for (int j = strlen(buf); j < strlen(file_path); j++)
                {
                    file_path2[count++] = file_path[j];
                }
                file_path2[count] = '\0';
                printf("executable path : %s\n", file_path2);
            }
        }
        else
        {
            printf("executable path : %s\n", file_path);
        }
    }
}