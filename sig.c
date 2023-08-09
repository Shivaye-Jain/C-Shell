#include "headers.h"

extern process process_arr[200];
extern int count_for_background_processes;

void sig(char tokens[][200], int count)
{
    int i1;
    int i2;
    i1 = atoi(tokens[1]);
    i2 = atoi(tokens[2]);
    if (i1 > count_for_background_processes || i1 < 1)
    {
        printf("Error: No job with the given number exists\n");
        return;
    }
    kill(process_arr[i1].pid, i2);
}