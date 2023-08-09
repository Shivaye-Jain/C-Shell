#include "headers.h"

void prompt(char *currdir, long time)
{
    char *username;
    uid_t uid = geteuid();
    struct passwd *pw = getpwuid(uid);
    username = pw->pw_name;

    char hostname[100];

    gethostname(hostname, sizeof(hostname));
    if (time < 1)
    {
        printf("\033[1;34m%s@%s\033[0m:\033[1;32m%s\033[0m ", username, hostname, currdir);
    }
    else
    {
        printf("\033[1;34m%s@%s\033[0m:\033[1;32m%s took %llds\033[0m ", username, hostname, currdir, (long long)time);
    }
}