#include "headers.h"

void abspath()
{
    char buf[1000];
    char *ret = getcwd(buf, 1000);
    if (ret == NULL)
    {
        while ((ret = getcwd(buf, 1000)) == NULL)
                ;
    }
    printf("%s\n", buf);
}