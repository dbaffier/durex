#include "Durex.h"

int     _lock;

static void sig_handler(int sig)
{
    syslog(LOG_CRIT, "Caught signal [ %s ]", strsignal(sig));
    syslog(LOG_INFO, "Quitting.");
    close(_lock);
    remove("/var/lock/durex.lock");
    exit(128 + sig);
}

void    daemon_durex(void)
{
    pid_t   pid;

    pid = fork();
	if (pid < 0)
		exit(1);
	if (pid > 0)
		exit(0);
	if (setsid() < 0)
		exit(1);

	pid = fork();
	if (pid < 0)
        exit(1);
    if (pid > 0)
        exit(0);
	/* Set new file permissions */
    umask(0);

    chdir("/");
    _lock = open("/var/lock/durex.lock", O_CREAT, 0644);
    if (_lock < 0)
    {
        syslog(LOG_ERR, "Can't open :/var/lock/durex.lock");
        syslog(LOG_INFO, "Quitting.");
        exit(1);
    }
    if (flock(_lock, LOCK_EX | LOCK_NB) < 0)
    {
        syslog(LOG_ERR, "Error file locked");
        syslog(LOG_INFO, "Quitting.");
        close(_lock);
        exit(1);
    }
    close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);
    int i = 0;
    while (++i < 32)
    {
        signal(i, &sig_handler);
    }
    signal(SIGPIPE, SIG_IGN);
}