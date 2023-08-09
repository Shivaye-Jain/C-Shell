#include "headers.h"

extern int curr_pid2;
extern char foreground_pr[1000];

void foreground(char tokens[][200], int count)
{
    int no_parts = count;

    char *inst[no_parts + 1];

    for (int i = 0; i < no_parts; i++)
    {
        inst[i] = (char *)malloc(sizeof(char) * 1000);
        strcpy(inst[i], tokens[i]);
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
        
        // setpgid(0, 0);
        int ret = execvp(inst[0], inst);
        if (ret == -1){
            printf("\033[1;31m");
            printf("Error: No such command\n");
            printf("\033[0m");
            exit(1);
        }
        exit(0);
    }
    else
    {
        int st;
        curr_pid2 = pid;
        waitpid(pid, &st, 2);
        // printf("[%d] %d\n", count_for_background_processes, pid);
    }
}