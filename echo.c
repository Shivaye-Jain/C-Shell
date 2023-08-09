#include "headers.h"

void echo(char tokens[][200], int count)
{
    for (int i = 1; i < count - 1; i++)
    {
        printf("%s ", tokens[i]);
    }
    if (count != 1)
        printf("%s", tokens[count - 1]);
    printf("\n");
}