#include "Durex.h"

extern int  _lock;

void    launch_server(t_shell *shell)
{
    if ((shell->ss = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        syslog(LOG_ALERT, "Cannot create socket");
        close(_lock);
        remove("/var/lock/durex.lock");
        exit(1);
    }
    if (setsockopt(shell->ss, SOL_SOCKET, SO_REUSEADDR, &shell->opt, sizeof(shell->opt)) < 0)
    {
        syslog(LOG_ALERT, "setsockopt error");
        close(_lock);
        remove("/var/lock/durex.lock");
        exit(1);
    }
    shell->server_addr.sin_family = AF_INET;
    shell->server_addr.sin_addr.s_addr = INADDR_ANY; 
    shell->server_addr.sin_port = htons(shell->port);
    if (bind(shell->ss, (struct sockaddr *)&shell->server_addr, sizeof(shell->server_addr)) < 0)
    {
        // Log.Log(LOG_ERR, "Cannot bind socket.");
        close(_lock);
        remove("/var/lock/matt_daemon.lock");
        exit(1);
    }
    if (listen(shell->ss, 3) < 0)
    {
        // Log.Log(LOG_ERR, "Cannot listen (3).");
        close(_lock);
        remove("/var/lock/matt_daemon.lock");
        exit(1);
    }
    int new_socket;
    socklen_t   length = sizeof(shell->server_addr);
    if ((new_socket = accept(shell->ss, (struct sockaddr *)&shell->server_addr, 
                       &length)) < 0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    char buffer[1024];
    memset(buffer, 0, 1024);

    new_socket = read(new_socket , buffer, 1024);
    printf("%s\n",buffer );
    send(new_socket , "Hello" , 6, 0);
    printf("Hello message sent\n");
    while (1)
        ;
}

void shell()
{
    t_shell     *shell;

    shell = malloc(sizeof(t_shell));
    if (!shell)
        exit(1);
    memset(shell, 0, sizeof(t_shell));

    shell->opt = 1;
    shell->port = 4242;
    launch_server(shell);
}