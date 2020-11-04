#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[])
{
    int p2c[2];
    int c2p[2];

    pipe(p2c);
    pipe(c2p);

    if (fork() == 0)
    { // child process
        int pid = getpid();
        char rBuf[1];
        memset(rBuf, 0, sizeof(rBuf));
        close(p2c[1]);
        close(c2p[0]);
        // sleep(3);
        read(p2c[0], rBuf, 1);
        printf("%d: received ping\n", pid);
        write(c2p[1], rBuf, 1);
        close(p2c[0]);
        close(c2p[1]);
    }
    else
    { //  parent process
        char rBuf[1];
        memset(rBuf, 0, sizeof(rBuf));
        int pid = getpid();
        write(p2c[1], "1", 1);
        // wait child process
        // wait((int *)0);
        read(c2p[0], rBuf, 1);
        printf("%d: received pong\n", pid);
    }

    exit(0);
}
