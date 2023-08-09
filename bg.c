#include "headers.h"

extern process process_arr[200];
extern int count_for_background_processes;

void bg(char tokens[][200], int count)
{
    int no = atoi(tokens[1]);
    if (no > count_for_background_processes || no < 1)
    {
        printf("Error: No job with the given number exists\n");
        return;
    }
    pid_t pid = process_arr[no].pid;
    kill(pid, SIGTTIN);
    kill(pid, SIGCONT);
}