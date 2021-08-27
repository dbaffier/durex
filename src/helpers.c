#include "Durex.h"

int isCopy(void)
{
    char buff[1024];

    memset(buff, 0, sizeof(buff));

    readlink("/proc/self/exe", buff, 1024);
    printf("Buff : %s\n", buff);
    if (strcmp(buff, "/usr/bin/Durex") == 0)
        return (1);
    return (0);
}