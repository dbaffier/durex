#include "Durex.h"

void createDurex(void)
{
    int fd;
    int fdParent;
    int lock;
    struct stat sb;

    lock = open("/var/lock/durex.lock", O_RDONLY);
    if (lock > 1)
    {
        syslog(LOG_ERR, "/var/lock/durex.lock already on");
        syslog(LOG_INFO, "Quitting.");
        exit(1);
    }
    close(lock);
    // if (flock(lock, LOCK_EX | LOCK_NB) < 0)
    // {
    //     syslog(LOG_ERR, "Error file locked");
    //     syslog(LOG_INFO, "Quitting.");
    //     close(lock);
    //     exit(1);
    // }
    fd = open("/bin/Durex", O_CREAT | O_RDWR, 0777);    
    if (fd == -1)
    {
        syslog(LOG_ERR, "Cannot create /bin/Durex.");
        exit(1);
    }
    fdParent = open("./Durex", O_RDONLY);
    if (fdParent == -1)
    {
        syslog(LOG_ERR, "Cannot create /bin/Durex.");
        exit(1);
    }
    if (fstat(fdParent, &sb) == -1)
    {
        syslog(LOG_ERR, "Cannot stat fd : [%d]", fd);
        syslog(LOG_INFO, "Quitting.");
        exit(1);
    }
    for (int i = 0; i < sb.st_size; i++)
    {
        char c;

        read(fdParent, &c, 1);
        write(fd, &c, 1);
    }
    close(fdParent);
    close(fd);
}