#include "headers.h"

extern process process_arr[200];

void background(char tokens[][200], int start, int end, int count_for_background_processes)
{
    int no_parts = end - start + 1;

    char *inst[no_parts + 1];

    for (int i = start; i <= end; i++)
    {
        inst[i - start] = (char *)malloc(sizeof(char) * 200);
        strcpy(inst[i - start], tokens[i]);
    }

    inst[no_parts] = NULL;

    pid_t pid = fork();

    if (pid == -1)
    {
        perror("\033[1;31mError\033[0m");
        exit(1);
    }
    else if (pid == 0)
    {
        int ret = execvp(inst[0], inst);
        if (ret == -1)
        {
            printf("\033[1;31m");
            printf("Error: No such command\n");
            printf("\033[0m");
            exit(1);
        }
        exit(0);
    }
    else
    {

        printf("[%d] %d\n", count_for_background_processes, pid);
    }

    strcpy(process_arr[count_for_background_processes].cmd, "");

    for (int i = 0; i < no_parts - 1; i++)
    {
        strcat(process_arr[count_for_background_processes].cmd, inst[i]);
        strcat(process_arr[count_for_background_processes].cmd, " ");
    }

    strcat(process_arr[count_for_background_processes].cmd, inst[no_parts - 1]);
    process_arr[count_for_background_processes].pid = pid;
}