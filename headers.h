#ifndef __HEADERS_H
#define __HEADERS_H

#include <stdio.h>
#include <stdlib.h>
#include <pwd.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <grp.h>
#include <time.h>
#include <sys/wait.h>
#include <signal.h>
#include <assert.h>
#include <termios.h>
#include <ctype.h>

typedef struct process
{
    pid_t pid;
    char cmd[1000];
} process;

void abspath();
void cd(char tokens[][200], char **, char *, char *, char *);
void echo(char tkens[][200], int);
void pinfo(char tokens[][200], char *buf, int);
void prompt(char *currdir, long time);
void background(char tokens[][200], int start, int end, int);
void ls(char tokens[][200], int, char *);
void foreground(char tokens[][200], int count);
void discover(char *path, int hidden);
void find(char *path, int hidden, char *file);
void redirection(char tokens[][200], int redirect, char *input_files, char *output_files, char *buf, char **history, int his_count, int count, int append, int is_output);
void history2(char **history, int his_count, int count);
void discovery(char tokens[][200], int count, char *buf);
void pipe_(char tokens[][200], int count, char *buf, char **history, int his_count);
void redirection2(char tokens[][200], int save, int redirect, char *input_files, char *output_files, char *buf, char **history, int his_count, int *count);
// void redirection3(char tokens[][200], int save, int redirect, char *input_files, char *output_files, char *buf, char **history, int his_count, int *count);
void redirection3(char tokens[][200], int save, int redirect, char *input_files, char *output_files, char *buf, char **history, int his_count, int *save2, int *count);
void jobs(char tokens[][200], int count);
void sig(char tokens[][200], int count);
void fg(char tokens[][200], int count);
void bg(char tokens[][200], int count);

#endif