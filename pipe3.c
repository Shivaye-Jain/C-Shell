#include "headers.h"

void pipe_(char tokens[][200], int count, char *buf, char **history, int his_count)
{
    int last_pipe_index = -1;
    int save2;
    int prev = 0;
    int arr_p[2];
    int original2 = 0; // fd_in
    int original;
    for (int i = count - 1; i >= 0; i--)
    {
        if (strcmp(tokens[i], "|") == 0)
        {
            last_pipe_index = i;
            break;
        }
    }
    // if (dup2(STDOUT_FILENO, original) < 0)
    // {
    //     perror("Error");
    //     return;
    // }
    // if (dup2(STDIN_FILENO, original2) < 0)
    // {
    //     perror("Error");
    //     return;
    // }
    int studout = dup(STDOUT_FILENO);
    for (int i = 0; i < count; i++)
    {
        if (strcmp(tokens[i], "|") == 0)
        {
            if (pipe(arr_p) < 0)
            {
                perror("Error");
                return;
            }
            // if (dup2(arr_p[1], STDOUT_FILENO) < 0)
            // {
            //     perror("Error");
            //     return;
            // }
            if (prev == 0)
            {
                char arr[i - prev + 1][200];
                char input_files[1000] = "";
                char output_files[1000] = "";
                int redirect = -1;
                int is_input = 0;
                int is_output = 0;
                int append = 0;
                int errorn = 0;
                for (int st = prev; st < i; st++)
                {
                    if (strcmp(tokens[st], "<") == 0)
                    {
                        if (redirect == -1)
                        {
                            redirect = st - prev;
                        }
                        is_input = 1;
                        if (st + 1 == i)
                        {
                            errorn = 1;
                        }
                        else
                        {
                            strcpy(input_files, tokens[st + 1]);
                        }
                    }
                    else if (strcmp(tokens[st], ">") == 0)
                    {
                        is_output = 1;
                        if (redirect == -1)
                        {
                            redirect = st - prev;
                        }
                        if (st + 1 == i)
                        {
                            errorn = 1;
                        }
                        else
                        {
                            strcpy(output_files, tokens[st + 1]);
                        }
                    }
                    else if (strcmp(tokens[st], ">>") == 0)
                    {
                        if (redirect == -1)
                        {
                            redirect = st - prev;
                        }
                        append = 1;
                        if (st + 1 == i)
                        {
                            errorn = 1;
                        }
                        else
                        {
                            strcpy(output_files, tokens[st + 1]);
                        }
                    }
                }
                if (errorn)
                {
                    printf("bash: syntax error near unexpected token `newline'\n");
                    return;
                }
                for (int j = prev; j < i; j++)
                {
                    strcpy(arr[j - prev], tokens[j]);
                    // printf("tokenx - %s\n", tokens[j]);
                }
                // f or (int j = prev; j < i; j++)
                // {
                //     printf("arr - %s\n", arr[j - prev]);
                // }
                pid_t pid = fork();
                char *arr2[i - prev + 1];
                for (int counter = 0; counter < i - prev; counter++)
                {
                    arr2[counter] = (char *)malloc(sizeof(char) * 200);
                    strcpy(arr2[counter], arr[counter]);
                }
                // for (int counter = 0; counter < i - prev; counter++)
                // {
                //     printf("bug - %s\n", arr2[counter]);
                // }
                arr2[i - prev] = NULL;
                if (pid == -1)
                {
                    dup2(studout, 1);
                    perror("\033[1;31mError\033[0m");
                    return;
                }
                else if (pid == 0)
                {
                    dup2(original2, 0);
                    dup2(arr_p[1], 1);
                    close(arr_p[0]);
                    int ret;
                    if (append || is_input || is_output)
                    {
                        redirection(arr, redirect, input_files, output_files, buf, history, his_count, count, append, is_output);
                    }
                    else
                    {
                        if (strcmp(arr[0], "echo") == 0)
                        {
                            echo(arr, i - prev);
                        }
                        else if (strcmp(arr[0], "pwd") == 0)
                        {
                            abspath();
                        }
                        else if (strcmp(arr[0], "ls") == 0)
                        {
                            ls(arr, i - prev, buf);
                        }
                        else if (strcmp(arr[0], "pinfo") == 0)
                        {
                            pinfo(arr, buf, i - prev);
                        }
                        else if (strcmp(arr[0], "history") == 0)
                        {
                            history2(history, his_count, i - prev);
                        }
                        else if (strcmp(arr[0], "discover") == 0)
                        {
                            discovery(arr, i - prev, buf);
                        }
                        else
                        {
                            ret = execvp(arr2[0], arr2);
                            if (ret == -1)
                            {
                                dup2(studout, 1);
                                printf("\033[1;31m");
                                printf("Error: No such command\n");
                                printf("\033[0m");
                                exit(1);
                            }
                        }
                    }
                    exit(0);
                }
                else
                {
                    // printf("hejdhe\n");
                    int st;
                    waitpid(pid, &st, 2);
                    close(arr_p[1]);
                    original2 = arr_p[0];
                }
                // if (dup2(original, STDOUT_FILENO) < 0)
                // {
                //     perror("Error");
                //     return;
                // }
                // save2 = dup(arr_p[0]);
            }
            else
            {
                // if (dup2(save2, STDIN_FILENO) < 0)
                // {
                //     perror("Error");
                //     return;
                // }
                char arr[i - prev + 1][200];
                char input_files[1000] = "";
                char output_files[1000] = "";
                int redirect = -1;
                int is_input = 0;
                int is_output = 0;
                int append = 0;
                int errorn = 0;
                for (int st = prev; st < i; st++)
                {
                    if (strcmp(tokens[st], "<") == 0)
                    {
                        if (redirect == -1)
                        {
                            redirect = st - prev;
                        }
                        is_input = 1;
                        if (st + 1 == i)
                        {
                            errorn = 1;
                        }
                        else
                        {
                            strcpy(input_files, tokens[st + 1]);
                        }
                    }
                    else if (strcmp(tokens[st], ">") == 0)
                    {
                        is_output = 1;
                        if (redirect == -1)
                        {
                            redirect = st - prev;
                        }
                        if (st + 1 == i)
                        {
                            errorn = 1;
                        }
                        else
                        {
                            strcpy(output_files, tokens[st + 1]);
                        }
                    }
                    else if (strcmp(tokens[st], ">>") == 0)
                    {
                        if (redirect == -1)
                        {
                            redirect = st - prev;
                        }
                        append = 1;
                        if (st + 1 == i)
                        {
                            errorn = 1;
                        }
                        else
                        {
                            strcpy(output_files, tokens[st + 1]);
                        }
                    }
                }
                if (errorn)
                {
                    printf("bash: syntax error near unexpected token `newline'\n");
                    return;
                }
                for (int j = prev; j < i; j++)
                {
                    strcpy(arr[j - prev], tokens[j]);
                    // printf("token2 - %s\n", tokens[j]);
                }
                // for (int j = prev; j < i; j++)
                // {
                //     printf("arr - %s\n", arr[j - prev]);
                // }
                pid_t pid = fork();
                char *arr2[i - prev + 1];
                for (int counter = 0; counter < i - prev; counter++)
                {
                    arr2[counter] = (char *)malloc(sizeof(char) * 200);
                    strcpy(arr2[counter], arr[counter]);
                }
                // for (int counter = 0; counter < i - prev; counter++)
                // {
                //     printf("bug - %s\n", arr2[counter]);
                // }
                arr2[i - prev] = NULL;
                if (pid == -1)
                {
                    dup2(studout, 1);
                    perror("\033[1;31mError\033[0m");
                    return;
                }
                else if (pid == 0)
                {
                    dup2(original2, 0);
                    dup2(arr_p[1], 1);
                    close(arr_p[0]);
                    int ret;
                    if (append || is_input || is_output)
                    {
                        redirection(arr, redirect, input_files, output_files, buf, history, his_count, count, append, is_output);
                    }
                    else
                    {
                        if (strcmp(arr[0], "echo") == 0)
                        {
                            echo(arr, i - prev);
                        }
                        else if (strcmp(arr[0], "pwd") == 0)
                        {
                            abspath();
                        }
                        else if (strcmp(arr[0], "ls") == 0)
                        {
                            ls(arr, i - prev, buf);
                        }
                        else if (strcmp(arr[0], "pinfo") == 0)
                        {
                            pinfo(arr, buf, i - prev);
                        }
                        else if (strcmp(arr[0], "history") == 0)
                        {
                            history2(history, his_count, i - prev);
                        }
                        else if (strcmp(arr[0], "discover") == 0)
                        {
                            discovery(arr, i - prev, buf);
                        }
                        else
                        {
                            ret = execvp(arr2[0], arr2);
                            if (ret == -1)
                            {
                                dup2(studout, 1);
                                printf("\033[1;31m");
                                printf("Error: No such command\n");
                                printf("\033[0m");
                                exit(1);
                            }
                        }
                    }
                    exit(0);
                }
                else
                {
                    int st;
                    waitpid(pid, &st, 2);
                    close(arr_p[1]);
                    original2 = arr_p[0];
                }
                // if (dup2(original, STDOUT_FILENO) < 0)
                // {
                //     perror("Error6");
                //     return;
                // }
                // save2 = dup(arr_p[0]);
            }
            prev = i + 1;
        }
        else if (i == last_pipe_index + 1)
        {
            // if (dup2(save2, STDIN_FILENO) < 0)
            // {
            //     perror("Error23");
            //     return;
            // }
            char arr[count - prev + 1][200];
            char input_files[1000] = "";
            char output_files[1000] = "";
            int redirect = -1;
            int is_input = 0;
            int is_output = 0;
            int append = 0;
            int errorn = 0;
            for (int st = prev; st < count; st++)
            {
                if (strcmp(tokens[st], "<") == 0)
                {
                    if (redirect == -1)
                    {
                        redirect = st - prev;
                    }
                    is_input = 1;
                    if (st + 1 == count)
                    {
                        errorn = 1;
                    }
                    else
                    {
                        strcpy(input_files, tokens[st + 1]);
                    }
                }
                else if (strcmp(tokens[st], ">") == 0)
                {
                    is_output = 1;
                    if (redirect == -1)
                    {
                        redirect = st - prev;
                    }
                    if (st + 1 == count)
                    {
                        errorn = 1;
                    }
                    else
                    {
                        strcpy(output_files, tokens[st + 1]);
                    }
                }
                else if (strcmp(tokens[st], ">>") == 0)
                {
                    if (redirect == -1)
                    {
                        redirect = st - prev;
                    }
                    append = 1;
                    if (st + 1 == count)
                    {
                        errorn = 1;
                    }
                    else
                    {
                        strcpy(output_files, tokens[st + 1]);
                    }
                }
            }
            if (errorn)
            {
                printf("bash: syntax error near unexpected token `newline'\n");
                return;
            }
            for (int j = prev; j < count; j++)
            {
                strcpy(arr[j - prev], tokens[j]);
                // printf("token3 - %s\n", tokens[j]);
            }
            // for (int j = prev; j < i; j++)
            // {
            //     printf("arr - %s\n", arr[j - prev]);
            // }
            pid_t pid = fork();
            char *arr2[count - prev + 1];
            for (int counter = 0; counter < count - prev; counter++)
            {
                arr2[counter] = (char *)malloc(sizeof(char) * 200);
                strcpy(arr2[counter], arr[counter]);
            }
            // for (int counter = 0; counter < count - prev; counter++)
            //     {
            //         printf("bug - %s\n", arr2[counter]);
            //     }
            arr2[count - prev] = NULL;
            if (pid == -1)
            {
                dup2(studout, 1);
                perror("\033[1;31mError\033[0m");
                return;
            }
            else if (pid == 0)
            {
                dup2(original2, 0);
                // dup2(arr_p[1], 1);
                close(arr_p[0]);
                int ret;
                // close(count[0]);
                if (append || is_input || is_output)
                {
                    redirection(arr, redirect, input_files, output_files, buf, history, his_count, count, append, is_output);
                }
                else
                {
                    if (strcmp(arr[0], "echo") == 0)
                    {
                        echo(arr, count - prev);
                    }
                    else if (strcmp(arr[0], "pwd") == 0)
                    {
                        abspath();
                    }
                    else if (strcmp(arr[0], "ls") == 0)
                    {
                        ls(arr, count - prev, buf);
                    }
                    else if (strcmp(arr[0], "pinfo") == 0)
                    {
                        pinfo(arr, buf, count - prev);
                    }
                    else if (strcmp(arr[0], "history") == 0)
                    {
                        history2(history, his_count, count - prev);
                    }
                    else if (strcmp(arr[0], "discover") == 0)
                    {
                        discovery(arr, count - prev, buf);
                    }
                    // printf("Before execvp\n");
                    else
                    {
                        ret = execvp(arr2[0], arr2);
                        if (ret == -1)
                        {
                            dup2(studout, 1);
                            printf("\033[1;31m");
                            printf("Error: No such command\n");
                            printf("\033[0m");
                            exit(1);
                        }
                    }
                }
                exit(0);
            }
            else
            {
                int st;
                waitpid(pid, &st, 2);
                close(arr_p[1]);
                original2 = arr_p[0];
            }
            // if (dup2(original2, STDIN_FILENO) < 0)
            // {
            //     perror("Error234");
            //     return;
            // }
        }
    }
    // if (dup2(save2, STDIN_FILENO) < 0)
    // {
    //     perror("Error23");
    //     return;
    // }
    // char arr[count - prev + 1][200];

    // for (int j = prev; j < count; j++)
    // {
    //     strcpy(arr[j - prev], tokens[j]);
    // }

    // if (strcmp(arr[0], "echo") == 0)
    // {
    //     echo(arr, count - prev);
    // }
    // else if (strcmp(arr[0], "pwd") == 0)
    // {
    //     abspath();
    // }
    // else if (strcmp(arr[0], "ls") == 0)
    // {
    //     ls(arr, count - prev, buf);
    // }
    // else if (strcmp(arr[0], "pinfo") == 0)
    // {
    //     pinfo(arr, buf, count - prev);
    // }
    // else if (strcmp(arr[0], "history") == 0)
    // {
    //     history2(history, his_count, count - prev);
    // }
    // else if (strcmp(arr[0], "discover") == 0)
    // {
    //     discovery(arr, count - prev, buf);
    // }
    // else
    // {
    //     pid_t pid = fork();
    //     char *arr2[count - prev + 1];
    //     for (int counter = 0; counter < count - prev; counter++)
    //     {
    //         arr2[counter] = (char *)malloc(sizeof(char) * 200);
    //         strcpy(arr2[counter], arr[counter]);
    //     }
    //     arr2[count - prev] = NULL;
    //     if (pid == -1)
    //     {
    //         perror("\033[1;31mError\033[0m");
    //         return;
    //     }
    //     else if (pid == 0)
    //     {
    //         int ret;
    //         // close(count[0]);
    //         // printf("Before execvp\n");
    //         ret = execvp(arr2[0], arr2);
    //         if (ret == -1)
    //         {
    //             printf("\033[1;31m");
    //             printf("Error: No such command\n");
    //             printf("\033[0m");
    //             exit(1);
    //         }
    //         exit(0);
    //     }
    //     else
    //     {
    //         int st;
    //         waitpid(pid, &st, 2);
    //     }
    // }
    // if (dup2(original2, STDIN_FILENO) < 0)
    // {
    //     perror("Error234");
    //     return;
    // }
}