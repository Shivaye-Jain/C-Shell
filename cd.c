#include "headers.h"

void cd(char tokens[][200], char **currdir, char *buf, char *buf2, char *buf3)
{
    if (tokens[1][0] != '~')
    {
        char *ret = getcwd(buf3, 1000);
        if (ret == NULL)
        {
            while ((ret = getcwd(buf3, 1000)) == NULL)
                ;
        }
        if (chdir(tokens[1]) != 0)
        {
            perror("\033[1;31mError\033[0m");
            return;
        }
        ret = getcwd(buf2, 1000);
        if (ret == NULL)
        {
            while ((ret = getcwd(buf2, 1000)) == NULL)
                ;
        }
        int yes = 1;
        if (strlen(buf2) < strlen(buf))
        {
            strcpy(*currdir, buf2);
        }
        else
        {
            int yes = 1;
            int i = 0;
            for (i = 0; i < strlen(buf); i++)
            {
                if (buf2[i] != buf[i])
                {
                    yes = 0;
                }
            }
            if (yes == 0)
            {
                strcpy(*currdir, buf2);
            }
            else if (buf2[i] == '/' || buf2[i] == '\0')
            {
                strcpy(*currdir, "~");
                if (strlen(buf) == strlen(buf2))
                {
                    return;
                }
                int j = strlen(*currdir);
                for (; i < strlen(buf2); i++)
                {
                    (*currdir)[j++] = buf2[i];
                }
                (*currdir)[j] = '\0';
            }
            else
            {
                strcpy(*currdir, buf2);
            }
        }
    }
    else
    {
        char stri2[1000];
        strcpy(stri2, *currdir);
        strcpy(*currdir, tokens[1]);
        char stri[1000];
        strcpy(stri, buf);
        strcpy(buf + strlen(buf), tokens[1] + 1);
        int ret = chdir(buf);
        if (ret != 0)
        {
            strcpy(buf, stri);
            strcpy(*currdir, stri2);
            perror("\033[1;31mError\033[0m");
            return;
        }
        strcpy(buf, stri);
    }
}