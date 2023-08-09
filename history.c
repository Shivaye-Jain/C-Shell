#include "headers.h"

void history2(char **history, int his_count, int count)
{
    // printf("here\n");
    if (count > 1)
    {
        printf("\033[1;31m");
        printf("Error: Too many arguments\n");
        printf("\033[0m");
    }
    else
    {
        if (his_count <= 10)
        {
            for (int hiskacount = 0; hiskacount < his_count; hiskacount++)
            {
                printf("%s", history[hiskacount]);
            }
        }
        else
        {
            for (int hiskacount = his_count - 10; hiskacount < his_count; hiskacount++)
            {
                printf("%s", history[hiskacount]);
            }
        }
    }
}