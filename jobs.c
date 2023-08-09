#include "headers.h"
extern process process_arr[200];
extern int count_for_background_processes;
void sort2()
{
    for (int i = count_for_background_processes; i >= 1; i--)
    {
        for (int j = 1; j < i; j++)
        {
            if (strcasecmp(process_arr[j].cmd, process_arr[j + 1].cmd) > 0)
            {
                char temp[1000];
                pid_t pid, temp2;
                strcpy(temp, process_arr[j].cmd);
                strcpy(process_arr[j].cmd, process_arr[j + 1].cmd);
                strcpy(process_arr[j + 1].cmd, temp);
                temp2 = process_arr[j].pid;
                process_arr[j].pid = process_arr[j + 1].pid;
                process_arr[j + 1].pid = temp2;
            }
        }
    }
}

void jobs(char tokens[][200], int count)
{
    sort2();
    int r_come = 0;
    int s_come = 0;
    int other_come = 0;
    for (int i = 1; i < count; i++)
    {
        if (strcmp(tokens[i], "-r") == 0)
        {
            r_come = 1;
        }
        else if (strcmp(tokens[i], "-s") == 0)
        {
            s_come = 1;
        }
        else if (strcmp(tokens[i], "-rs") == 0 || strcmp(tokens[i], "-sr") == 0)
        {
            r_come = s_come = 1;
        }
        else
        {
            other_come = 1;
        }
    }
    if (r_come == 0 && s_come == 0)
    {
        r_come = s_come = 1;
    }
    if (other_come == 1)
    {
        printf("Error: No such command\n");
        return;
    }
    for (int i = 1; i <= count_for_background_processes; i++)
    {
        char p_id[30];
        int x;
        char file_path[200];
        char pro_stat;
        pid_t pro_id;
        long long int bhians;
        sprintf(p_id, "%d", process_arr[i].pid);

        strcpy(file_path, "/proc/");
        strcat(file_path, p_id);
        strcat(file_path, "/stat");
        FILE *fd;
        fd = fopen(file_path, "r");
        fscanf(fd, "%d %*s %c %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %lld", &pro_id, &pro_stat, &bhians);
        if (pro_stat == 'T')
        {
            if (s_come)
                printf("[%d] %s %s [%d]\n", i, "Stopped", process_arr[i].cmd, process_arr[i].pid);
        }
        else
        {
            if (r_come)
                printf("[%d] %s %s [%d]\n", i, "Running", process_arr[i].cmd, process_arr[i].pid);
        }
    }
}