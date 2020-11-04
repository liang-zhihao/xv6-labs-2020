#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    int p[2];
    pipe(p);
    if (fork() == 0)
    {
    }
    else
    {
        for (int i = 2; i <= 35; i++)
        {
            write(p[1], i, 2);
        }
    }
    write(1);
        exit(0);
}
