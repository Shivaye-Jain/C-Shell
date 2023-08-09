#include "headers.h"

extern process process_arr[200];
extern int count_for_background_processes;

void fg(char tokens[][200], int count)
{
    int no = atoi(tokens[1]);
    if (no > count_for_background_processes || no < 1)
    {
        printf("Error: No job with the given number exists\n");
        return;
    }
    pid_t pid = process_arr[no].pid;
    kill(pid, SIGCONT);
    for (int i = 1; i <= count_for_background_processes; i++)
    {
        if (process_arr[i].pid == pid)
        {
            for (int j = i; j < count_for_background_processes; j++)
            {
                strcpy(process_arr[j].cmd, process_arr[j + 1].cmd);
                process_arr[j].pid = process_arr[j + 1].pid;
            }
            count_for_background_processes--;
            fflush(stdout);
            break;
        }
    }
    waitpid(-1, NULL, WUNTRACED);
}