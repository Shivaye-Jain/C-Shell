#include "headers.h"

void redirection(char tokens[][200], int redirect, char *input_files, char *output_files, char *buf, char **history, int his_count, int count, int append, int is_output)
{
    char *arr[redirect + 2];
    int set = 0;
    int is_input = 0;
    int original;
    for (int i = 0; i < redirect; i++)
    {
        arr[i] = (char *)malloc(sizeof(char) * 200);
        strcpy(arr[i], tokens[i]);
    }
    arr[redirect] = NULL;
    int fd = 0;
    int fd2 = 0;
    if (strcmp(output_files, "") != 0)
    {
        if (append)
            fd = open(output_files, O_APPEND | O_WRONLY | O_CREAT, 0644);
        else if (is_output)
        {
            fd = open(output_files, O_WRONLY | O_CREAT | O_TRUNC, 0644);
        }
        if (fd == -1)
        {
            perror("Error");
            return;
        }
        else
        {
            set = 1;
            if (append)
                lseek(fd, 0, SEEK_END);
            if (dup2(STDOUT_FILENO, original) < 0)
            {
                perror("Error");
                return;
            }
            if (dup2(fd, STDOUT_FILENO) < 0)
            {
                perror("Error");
                return;
            }
        }
    }
    if (strcmp(input_files, "") != 0)
    {
        is_input = 1;
    }

    arr[redirect] = NULL;

    if (strcmp(tokens[0], "echo") == 0)
    {
        if (strcmp(input_files, "") != 0)
        {
            printf("Error\n");
        }
        else
            echo(tokens, redirect);
    }
    else if (strcmp(tokens[0], "pwd") == 0)
    {
        if (strcmp(input_files, "") != 0)
        {
            printf("Error\n");
        }
        else
            abspath();
    }
    else if (strcmp(tokens[0], "ls") == 0)
    {
        ls(tokens, redirect, buf);
    }
    else if (strcmp(tokens[0], "pinfo") == 0)
    {
        if (strcmp(input_files, "") != 0)
        {
            printf("Error\n");
        }
        else
            pinfo(tokens, buf, redirect);
    }
    else if (strcmp(tokens[0], "history") == 0)
    {
        if (strcmp(input_files, "") != 0)
        {
            printf("Error\n");
        }
        else
            history2(history, his_count, redirect);
    }
    else if (strcmp(tokens[0], "discover") == 0)
    {
        if (strcmp(input_files, "") != 0)
        {
            printf("Error\n");
        }
        else
            discovery(tokens, redirect, buf);
    }
    else
    {
        pid_t pid = fork();

        if (pid == -1)
        {
            perror("\033[1;31mError\033[0m");
            return;
        }
        else if (pid == 0)
        {
            if (is_input)
            {
                arr[redirect] = input_files;
                arr[redirect + 1] = NULL;
            }
            int ret;
            // setpgid(0, 0);
            ret = execvp(arr[0], arr);
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
            int st;
            waitpid(pid, &st, 2);
            // printf("[%d] %d\n", count_for_background_processes, pid);
        }
    }
    if (set == 1)
    {
        if (dup2(original, STDOUT_FILENO) < 0)
        {
            perror("Error");
            return;
        }
    }
}

// void redirection2(char tokens[][200], int save, int redirect, char *input_files, char *output_files, char *buf, char **history, int his_count, int *count)
// {
//     // count = (int *)malloc(sizeof(int) * 2);
//     // // printf("laalchi kutta\n");
//     // if (pipe(count) < 0)
//     // {
//     //     perror("Error");
//     //     return;
//     // }
//     // STDIN_FILENO
//     char *arr[redirect + 2];
//     int set = 0;
//     int chipkali = dup(0);
//     int is_input = 0;
//     int original;
//     for (int i = save; i < redirect; i++)
//     {
//         arr[i - save] = (char *)malloc(sizeof(char) * 200);
//         strcpy(arr[i - save], tokens[i]);
//     }
//     arr[redirect - save] = NULL;
//     int fd2 = 0;
//     if (strcmp(output_files, "") != 0)
//     {
//         if (dup2(STDOUT_FILENO, original) < 0)
//         {
//             perror("Error");
//             return;
//         }
//         if (dup2(count[1], STDOUT_FILENO) < 0)
//         {
//             perror("Error");
//             return;
//         }
//     }
//     if (strcmp(input_files, "") != 0)
//     {
//         is_input = 1;
//     }

//     arr[redirect] = NULL;
//     // printf("thathajaraha\n");

//     if (strcmp(arr[0], "echo") == 0)
//     {
//         echo(arr, redirect - save);
//     }
//     else if (strcmp(arr[0], "pwd") == 0)
//     {
//         abspath();
//     }
//     else if (strcmp(arr[0], "ls") == 0)
//     {
//         ls(arr, redirect - save, buf);
//     }
//     else if (strcmp(arr[0], "pinfo") == 0)
//     {
//         pinfo(arr, buf, redirect - save);
//     }
//     else if (strcmp(arr[0], "history") == 0)
//     {
//         history2(history, his_count, redirect - save);
//     }
//     else if (strcmp(arr[0], "discover") == 0)
//     {
//         discovery(arr, redirect - save, buf);
//     }
//     else
//     {
//         pid_t pid = fork();

//         if (pid == -1)
//         {
//             perror("\033[1;31mError\033[0m");
//             return;
//         }
//         else if (pid == 0)
//         {
//             if (is_input)
//             {
//                 arr[redirect] = input_files;
//                 arr[redirect + 1] = NULL;
//             }
//             int ret;
//             // printf("jaraha\n");
//             // setpgid(0, 0);
//             // char lis[1100]; // char~ lis2[1200];
//             // printf("JK\n");
//             // read(count[0], lis, sizeof(lis));
//             // printf("JK\n");
//             // printf("This is cat - %s\n", lis);
//             // write(chipkali, lis2, strlen(lis2));
//             close(count[0]);
//             printf("Before execvp\n");
//             ret = execvp(arr[0], arr);
//             if (ret == -1)
//             {
//                 printf("\033[1;31m");
//                 printf("Error: No such command\n");
//                 printf("\033[0m");
//                 exit(1);
//             }
//             exit(0);
//         }
//         else
//         {
//             printf("hejdhe\n");
//             int st;
//             waitpid(pid, &st, 2);
//             // printf("[%d] %d\n", count_for_background_processes, pid);
//         }
//     }
//     if (set == 1)
//     {
//         if (dup2(original, STDOUT_FILENO) < 0)
//         {
//             perror("Error");
//             return;
//         }
//     }
// }

// void redirection3(char tokens[][200], int save, int redirect, char *input_files, char *output_files, char *buf, char **history, int his_count, int *save2, int *count)
// {
//     // count = (int *)malloc(sizeof(int) * 2);
//     // // printf("laalchi kutta\n");
//     // if (pipe(count) < 0)
//     // {
//     //     perror("Error");
//     //     return;
//     // }
//     char *arr[redirect + 2];
//     int set = 0;
//     int is_input = 0;
//     int original;
//     int original2;
//     for (int i = save; i < redirect; i++)
//     {
//         arr[i - save] = (char *)malloc(sizeof(char) * 200);
//         strcpy(arr[i - save], tokens[i]);
//     }
//     arr[redirect - save] = NULL;
//     // int fd = dup(count);
//     // int fd2 = dup(frog);
//     if (strcmp(output_files, "") != 0)
//     {
//         if (dup2(STDOUT_FILENO, original) < 0)
//         {
//             perror("Error");
//             return;
//         }
//         if (dup2(count[1], STDOUT_FILENO) < 0)
//         {
//             perror("Error");
//             return;
//         }
//     }
//     close(save2[1]);
//     if (strcmp(input_files, "") != 0)
//     {
//         if (dup2(STDIN_FILENO, original2) < 0)
//         {
//             perror("Error");
//             return;
//         }
//         if (dup2(save2[0], STDOUT_FILENO) < 0)
//         {
//             perror("Error");
//             return;
//         }
//     }
//     if (strcmp(input_files, "") != 0)
//     {
//         is_input = 1;
//     }

//     arr[redirect] = NULL;

//     if (strcmp(arr[0], "echo") == 0)
//     {
//         echo(arr, redirect - save);
//     }
//     else if (strcmp(arr[0], "pwd") == 0)
//     {
//         abspath();
//     }
//     else if (strcmp(arr[0], "ls") == 0)
//     {
//         ls(arr, redirect - save, buf);
//     }
//     else if (strcmp(arr[0], "pinfo") == 0)
//     {
//         pinfo(arr, buf, redirect - save);
//     }
//     else if (strcmp(arr[0], "history") == 0)
//     {
//         history2(history, his_count, redirect - save);
//     }
//     else if (strcmp(arr[0], "discover") == 0)
//     {
//         discovery(tokens, redirect - save, buf);
//     }
//     else
//     {
//         pid_t pid = fork();

//         if (pid == -1)
//         {
//             perror("\033[1;31mError\033[0m");
//             return;
//         }
//         else if (pid == 0)
//         { // WRITE END
//             if (is_input)
//             {
//                 // close(count[1]);
//                 // close(save2[1]);
//                 // char *arr2 = (char *)malloc(sizeof(char) * 1000);
//                 char lis[1100]; // char lis2[1200];
//                 // read(count[0], lis, sizeof(lis));
//                 // printf("This is cat - %s\n", lis);
//                 close(save2[1]);
//                 // read(save2[0], arr2, 1000);
//                 // printf("File contents -%ss\n", arr2);
//                 // arr[redirect - save] = arr2;
//                 // arr[redirect - save + 1] = NULL;
//                 if (strcmp(input_files, "") != 0)
//                 {
//                     if (dup2(STDIN_FILENO, original) < 0)
//                     {
//                         perror("Error");
//                         return;
//                     }
//                     if (dup2(save2[0], STDOUT_FILENO) < 0)
//                     {
//                         perror("Error");
//                         return;
//                     }
//                 }
//             }
//             int ret;
//             close(count[0]);
//             // setpgid(0, 0);
//             ret = execvp(arr[0], arr);
//             if (ret == -1)
//             {
//                 printf("\033[1;31m");
//                 printf("Error: No such command\n");
//                 printf("\033[0m");
//                 exit(1);
//             }
//             exit(0);
//         }
//         else
//         {
//             int st;
//             waitpid(pid, &st, 2);
//             // printf("[%d] %d\n", count_for_background_processes, pid);
//         }
//     }
//     if (set == 1)
//     {
//         if (dup2(original, STDOUT_FILENO) < 0)
//         {
//             perror("Error");
//             return;
//         }
//     }
//     if (is_input)
//     {
//         if (dup2(original2, STDOUT_FILENO) < 0)
//         {
//             perror("Error");
//             return;
//         }
//     }
// }