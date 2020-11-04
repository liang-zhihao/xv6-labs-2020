#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"


int main(int argc, char *argv[])
{
    if (argc <= 1)
    {
        printf("need an int\n");
        exit(0);
    }
    int i = atoi(argv[1]);
    sleep(i);
    exit(0);
}
