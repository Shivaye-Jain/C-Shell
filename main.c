#include "headers.h"

process process_arr[200];
time_t start, end;
char *history[20];
int curr_pid2 = -1;
int his_count = 0;
pid_t curr_pid;
char foreground_pr[1000];

char *currdir;

int count_for_background_processes = 0;

void handler(int t)
{
    int child_status;
    pid_t pid = waitpid(WAIT_ANY, &child_status, WNOHANG);
    for (int i = 1; i <= count_for_background_processes; i++)
    {
        if (process_arr[i].pid == pid)
        {
            int exit_status = WEXITSTATUS(child_status);
            if (exit_status == 0)
            {
                fprintf(stderr, "\n%s with pid %d exited normally\n", process_arr[i].cmd, process_arr[i].pid);
            }
            else
            {
                fprintf(stderr, "\n%s with pid %d exited abnormally\n", process_arr[i].cmd, process_arr[i].pid);
            }
            for (int j = i; j < count_for_background_processes; j++)
            {
                strcpy(process_arr[j].cmd, process_arr[j + 1].cmd);
                process_arr[j].pid = process_arr[j + 1].pid;
            }
            count_for_background_processes--;
            prompt(currdir, -1);
            fflush(stdout);
            break;
        }
    }
}

void ctrl_c(int t)
{
}

void ctrl_z(int t)
{
    pid_t pt = getpid();
    if (pt != curr_pid)
        return;
    // print();
    if (curr_pid2 != -1)
    {
        kill(curr_pid2, SIGTTIN);
        kill(curr_pid2, SIGTSTP);
        count_for_background_processes++;
        process_arr[count_for_background_processes].pid = curr_pid2;
        strcpy(process_arr[count_for_background_processes].cmd, foreground_pr);
    }
}

void die(const char *s)
{
    perror(s);
    exit(1);
}

struct termios orig_termios;

void disableRawMode()
{
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1)
    {
    }
    // die("tcsetattr");
}

/**
 * Enable row mode for the terminal
 * The ECHO feature causes each key you type to be printed to the terminal, so you can see what you're typing.
 * Terminal attributes can be read into a termios struct by tcgetattr().
 * After modifying them, you can then apply them to the terminal using tcsetattr().
 * The TCSAFLUSH argument specifies when to apply the change: in this case, it waits for all pending output to be written to the terminal, and also discards any input that hasn't been read.
 * The c_lflag field is for "local flags"
 */
void enableRawMode()
{
    if (tcgetattr(STDIN_FILENO, &orig_termios) == -1)
    {
    }
    // die("tcgetattr");
    atexit(disableRawMode);
    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ICANON | ECHO);
    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1)
    {
    }
    // die("tcsetattr");
}

void swap2(struct dirent **arrfile1, struct dirent **arrfile2)
{
    struct dirent *temp = *arrfile1;
    *arrfile1 = *arrfile2;
    *arrfile2 = temp;
}

void sort_(struct dirent **arrfile, int no_files)
{
    for (int i = no_files - 1; i >= 0; i--)
    {
        for (int j = 0; j < i; j++)
        {
            if (strcasecmp(arrfile[j]->d_name, arrfile[j + 1]->d_name) > 0)
            {
                swap2(&(arrfile[j]), &(arrfile[j + 1]));
            }
        }
    }
}

char *main_func()
{
    char *inp = malloc(sizeof(char) * 1000);
    char c;
    setbuf(stdout, NULL);
    enableRawMode();
    memset(inp, '\0', 100);
    int pt = 0;
    while (read(STDIN_FILENO, &c, 1) == 1)
    {
        if (iscntrl(c))
        {
            if (c == 10)
                break;
            else if (c == 27)
            {
                char buf[3];
                buf[2] = 0;
                if (read(STDIN_FILENO, buf, 2) == 2)
                { // length of escape code
                    printf("\rarrow key: %s", buf);
                }
            }
            else if (c == 127)
            { // backspace
                if (pt > 0)
                {
                    if (inp[pt - 1] == 9)
                    {
                        for (int i = 0; i < 7; i++)
                        {
                            printf("\b");
                        }
                    }
                    inp[--pt] = '\0';
                    printf("\b \b");
                }
            }
            else if (c == 9)
            {
                // TAB character
                inp[pt++] = c;
                for (int i = 0; i < 8; i++)
                {
                    // TABS should be 8 spaces

                    printf(" ");
                }
                int slash = 0;
                int bhians;
                int another_counter;
                int yet_another_counter = 0;
                char search_string[1000];
                char all_poss[500][200];
                for (bhians = pt - 1; inp[bhians] != ' '; bhians--)
                {
                    if (inp[bhians] == '/')
                    {
                        slash = 1;
                        break;
                    }
                }
                for (another_counter = bhians + 1; another_counter < pt; another_counter++)
                {
                    search_string[yet_another_counter++] = inp[another_counter];
                }
                search_string[yet_another_counter] = '\0';
                char dir[1000];
                if (slash == 1)
                {
                    int save_ir = bhians;
                    for (bhians = bhians - 1; inp[bhians] != ' '; bhians--)
                    {
                    }
                    yet_another_counter = 0;
                    for (int bichu = bhians + 1; bichu < save_ir; bichu++)
                    {
                        dir[yet_another_counter++] = inp[bichu];
                    }
                    dir[yet_another_counter] = '\0';
                }
                else
                {
                    strcpy(dir, ".");
                }
                struct dirent **arrfile;
                int no_files = scandir(dir, &arrfile, NULL, alphasort);

                int blockcount = 0;
                if (no_files == -1)
                {
                }
                else
                {
                    int no_2 = 0;
                    sort_(arrfile, no_files);
                    for (int i = 0; i < no_files; i++)
                    {
                        if (arrfile[i]->d_name[0] != '.')
                        {
                            int yes = 1;
                            int length = strlen(search_string);
                            for (int an_counter = 0; an_counter < length; an_counter++)
                            {
                                if (arrfile[i]->d_name[an_counter] != search_string[an_counter])
                                {
                                    yes = 0;
                                }
                            }
                            if (yes == 1)
                            {
                                strcpy(all_poss[no_2++], arrfile[i]->d_name);
                            }
                        }
                    }
                    if (no_2 == 1)
                    {
                        struct stat file_stat;
                        char file_path[1000];
                        strcpy(file_path, dir);
                        strcat(file_path, "/");
                        strcat(file_path, all_poss[no_2 - 1]);
                        stat(file_path, &file_stat);
                        if (!S_ISDIR(file_stat.st_mode))
                        {
                            printf("%s \n", file_path);
                        }
                        else
                        {
                            printf("%s/\n", file_path);
                        }
                    }
                    else if (no_2 > 1)
                    {
                    }
                }
            }
            else if (c == 4)
            {
                exit(0);
            }
            else
            {
                printf("%d\n", c);
            }
        }
        else
        {
            inp[pt++] = c;
            printf("%c", c);
        }
    }
    disableRawMode();
    printf("\n");
    // printf("\nInput Read: [%s]\n", inp);
    // printf("jdhsjd\n");

    // printf("khfdfk\n");
    return inp;
}

int main()
{
    // main_func();
    pid_t pid_s = getpid();
    curr_pid = pid_s;
    printf("\033[1;35m");
    printf("\t\t\t\t Welcome to Shivaye Jain's terminal!!\n\n");
    printf("\033[1;36m");
    printf("Enter 'quit' to exit the terminal\n\n");
    printf("\033[0m");
    int cd_come = 0;
    currdir = malloc(sizeof(char) * 1000);
    strcpy(currdir, "~");
    prompt(currdir, -1);

    char buf[1000];
    char buf2[1000];
    char buf3[1000];
    char buf4[1000];

    for (int counter = 0; counter < 20; counter++)
    {
        history[counter] = (char *)malloc(sizeof(char) * 1000);
        assert(history[counter] != NULL);
    }

    FILE *f, *f2;
    f = fopen("history.txt", "r");
    if (f == NULL)
    {
        printf("\033[1;31m");
        printf("\nError: history.txt is not present\n");
        printf("\033[0m");
        exit(0);
    }
    char string[1000];
    while (fgets(string, 1000, f) != NULL)
    {
        strcpy(history[his_count++], string);
    }
    f2 = fopen("history.txt", "w");
    if (f2 == NULL)
    {
        while ((f2 = fopen("history.txt", "w")) == NULL)
            ;
    }

    char *ret = getcwd(buf, 1000);
    if (ret == NULL)
    {
        while ((ret = getcwd(buf, 1000)) == NULL)
            ;
    }
    strcpy(buf3, buf);
    signal(SIGCHLD, handler);
    signal(SIGINT, ctrl_c);
    signal(SIGTSTP, ctrl_z);
    while (1)
    {
        curr_pid2 = -1;
        int foreground_come = 0;
        char cmd_arr[20][200]; // Array for storing commands separated by ';'
        char cmd[2000];        // String storing the command entered
        char cmd_dup[2000];
        char tokens[20][200]; // Array for storing the tokens of elements of cmd_arr
        char *token;
        fgets(cmd, 2000, stdin); // Reading the command from the terminal

        cmd[strlen(cmd) - 1] = '\0'; // Null - termination
        if (strcmp(cmd, "") == 0)
        {
            prompt(currdir, -1);
            continue;
        }
        if (his_count == 0)
        {
            strcpy(history[his_count++], cmd_dup);
        }
        else if (strcmp(history[his_count - 1], cmd_dup) != 0)
        {
            if (his_count == 20)
            {
                for (int hiskacount = 0; hiskacount < his_count - 1; hiskacount++)
                {
                    strcpy(history[hiskacount], history[hiskacount + 1]);
                }
                strcpy(history[his_count - 1], cmd_dup);
            }
            else
            {
                strcpy(history[his_count++], cmd_dup);
            }
        }

        int i = 0;
        // Getting the array of commands separated by ';'
        if (token = strtok(cmd, ";"))
        {
            strcpy(cmd_arr[i++], token);
            while ((token = strtok(NULL, ";")) != NULL)
            {
                strcpy(cmd_arr[i++], token);
            }
        }
        else
        {
            strcpy(cmd_arr[i++], "");
        }
        start = time(NULL);
        for (int j = 0; j < i; j++)
        {
            int count = 0;
            if (strcmp(cmd_arr[j], "") == 0)
            {
                continue;
            }
            if ((token = strtok(cmd_arr[j], "\t\n\r ")))
            {
                strcpy(tokens[count++], token);
            }
            else
            {
                continue;
            }
            while ((token = strtok(NULL, "\t\n\r ")) != NULL)
            {
                strcpy(tokens[count++], token);
            }
            char input_files[1000] = "";
            char output_files[1000] = "";
            int redirect = -1;
            int is_input = 0;
            int is_output = 0;
            int append = 0;
            int errorn = 0;
            int errorn2 = 0;
            int pipe = 0;
            for (int st2 = 0; st2 < count; st2++)
            {
                if (strcmp(tokens[st2], "|") == 0)
                {
                    pipe = 1;
                    if (st2 == 0 || st2 == count - 1)
                    {
                        errorn2 = 1;
                    }
                    else
                    {
                        if (strcmp(tokens[st2 + 1], "|") == 0)
                        {
                            errorn2 = 1;
                        }
                    }
                }
            }
            for (int st = 0; st < count; st++)
            {
                if (strcmp(tokens[st], "<") == 0)
                {
                    if (redirect == -1)
                    {
                        redirect = st;
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
                        redirect = st;
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
                        redirect = st;
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
            if (errorn2)
            {
                printf("Error: Invalid command\n");
            }
            else if (pipe)
            {
                pipe_(tokens, count, buf, history, his_count);
            }
            else if (errorn)
            {
                printf("bash: syntax error near unexpected token `newline'\n");
            }
            else if (append || is_input || is_output)
            {
                redirection(tokens, redirect, input_files, output_files, buf, history, his_count, count, append, is_output);
            }
            else if (strcmp(tokens[0], "echo") == 0)
            {
                echo(tokens, count);
            }
            else if (strcmp(tokens[0], "pwd") == 0)
            {
                abspath();
            }
            else if (strcmp(tokens[0], "cd") == 0)
            {
                if (count == 2)
                {
                    if (strcmp(tokens[1], "-") == 0)
                    {
                        if (cd_come == 0)
                        {
                            printf("\033[1;31m");
                            printf("bash: cd: OLDPWD not set\n");
                            printf("\033[0m");
                        }
                        else
                        {
                            printf("%s\n", buf3);
                            strcpy(tokens[1], buf3);
                            cd(tokens, &currdir, buf, buf2, buf3);
                        }
                    }
                    else if (strcmp(tokens[1], ".") == 0)
                    {
                        cd_come = 1;
                        strcpy(buf3, getcwd(buf4, 1000));
                    }
                    else if (strcmp(tokens[1], "..") == 0)
                    {
                        cd_come = 1;
                        getcwd(buf4, 1000);
                        int o;
                        for (o = strlen(buf4) - 1; o >= 0; o--)
                        {
                            if (buf4[o] == '/')
                            {
                                break;
                            }
                        }
                        strncpy(tokens[1], buf4, o + 1);
                        tokens[1][o + 1] = '\0';
                        cd(tokens, &currdir, buf, buf2, buf3);
                    }
                    else if (strcmp(tokens[1], "~") == 0)
                    {
                        cd_come = 1;
                        strcpy(tokens[1], buf);
                        cd(tokens, &currdir, buf, buf2, buf3);
                    }
                    else
                    {
                        cd_come = 1;
                        cd(tokens, &currdir, buf, buf2, buf3);
                    }
                }
                else if (count > 2)
                {
                    printf("\033[1;31m");
                    printf("bash: cd: too many arguments\n");
                    printf("\033[0m");
                }
                else if (count == 1)
                {
                    cd_come = 1;
                    strcpy(tokens[1], buf);
                    cd(tokens, &currdir, buf, buf2, buf3);
                }
            }
            else if (strcmp(tokens[0], "ls") == 0)
            {
                ls(tokens, count, buf);
            }
            else if (strcmp(tokens[0], "pinfo") == 0)
            {
                pinfo(tokens, buf, count);
            }
            else if (strcmp(tokens[0], "quit") == 0)
            {
                for (int hiskacount = 0; hiskacount < his_count; hiskacount++)
                {
                    fprintf(f2, "%s", history[hiskacount]);
                }
                exit(0);
            }
            else if (strcmp(tokens[0], "history") == 0)
            {
                history2(history, his_count, count);
            }
            else if (strcmp(tokens[0], "discover") == 0)
            {
                discovery(tokens, count, buf);
            }
            else if (strcmp(tokens[0], "jobs") == 0)
            {
                jobs(tokens, count);
            }
            else if (strcmp(tokens[0], "sig") == 0)
            {
                sig(tokens, count);
            }
            else if (strcmp(tokens[0], "fg") == 0)
            {
                fg(tokens, count);
            }
            else if (strcmp(tokens[0], "bg") == 0)
            {
                bg(tokens, count);
            }
            else
            {
                int yes23 = 0;
                int temp = 0;
                int save = -1;
                int z = 0;
                for (z = 0; z < count; z++)
                {
                    if (strcmp(tokens[z], "&") == 0 && temp == 0 || ((tokens[z][strlen(tokens[z]) - 1] == '&') && temp == 0))
                    {
                        if (strcmp(tokens[z], "&") == 0 && temp == 0)
                        {
                            count_for_background_processes++;
                            temp = 1;
                            save = z;
                            // Consider commands from 0 to z - 1
                            background(tokens, 0, z - 1, count_for_background_processes);
                        }
                        else
                        {
                            tokens[z][strlen(tokens[z]) - 1] = '\0';
                            count_for_background_processes++;
                            temp = 1;
                            save = z;
                            // Consider commands from 0 to z - 1
                            background(tokens, 0, z, count_for_background_processes);
                        }
                    }
                    else if ((strcmp(tokens[z], "&") == 0 && temp == 1) || ((tokens[z][strlen(tokens[z]) - 1] == '&') && temp == 1))
                    {
                        if (strcmp(tokens[z], "&") == 0 && temp == 0)
                        {
                            count_for_background_processes++;
                            background(tokens, save + 1, z - 1, count_for_background_processes);
                            save = z;
                        }
                        else
                        {
                            count_for_background_processes++;
                            tokens[z][strlen(tokens[z]) - 1] = '\0';
                            background(tokens, save + 1, z, count_for_background_processes);
                            save = z;
                        }
                    }
                }
                if (save != count - 1)
                {
                    char arr[20][200];
                    int ct = 0;
                    for (int q = save + 1; q < count; q++)
                    {
                        strcpy(arr[ct++], tokens[q]);
                    }
                    if (ct != 0)
                    {
                        count = ct;
                        for (int kutta = 0; kutta < ct; kutta++)
                        {
                            strcpy(tokens[kutta], arr[kutta]);
                        }
                        if (strcmp(tokens[0], "echo") == 0 || strcmp(tokens[0], "pwd") == 0 || strcmp(tokens[0], "cd") == 0 || strcmp(tokens[0], "ls") == 0 || strcmp(tokens[0], "pinfo") == 0 || strcmp(tokens[0], "quit") == 0 || strcmp(tokens[0], "history") == 0 || strcmp(tokens[0], "discover") == 0)
                        {
                            strcpy(cmd_arr[j], "");
                            for (int thatha = 0; thatha < count - 1; thatha++)
                            {
                                strcat(cmd_arr[j], tokens[thatha]);
                                strcat(cmd_arr[j], " ");
                            }
                            strcat(cmd_arr[j], tokens[count - 1]);
                            j--;
                        }
                        else
                        {
                            strcpy(foreground_pr, cmd);

                            foreground_come = 1;
                            foreground(tokens, count);
                        }
                    }
                }
                // call any function for the left - over command
            }
        }
        if (foreground_come)
        {
            end = time(NULL);
            long time_taken = (end - start);
            prompt(currdir, time_taken);
        }
        else
        {
            prompt(currdir, -1);
        }
    }

    return 0;
}