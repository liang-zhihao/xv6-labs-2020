#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#define INTSIZ 4
/*

Be careful to close file descriptors that a process doesn't need, because otherwise your program will run xv6 out of resources before the first process reaches 35.

Once the first process reaches 35, it should wait until the entire pipeline terminates, including all children, grandchildren, &c. Thus the main primes process should only exit after all the output has been printed, and after all the other primes processes have exited.

Hint: read returns zero when the write-side of a pipe is closed.

It's simplest to directly write 32-bit (4-byte) ints to the pipes, rather than using formatted ASCII I/O.

You should create the processes in the pipeline only as they are needed.
Add the program to UPROGS in Makefile.


*/
void fun(int pParent[])
{

    int rBuf;
    int toNext[2];
    if (read(pParent[0], &rBuf, INTSIZ) == 0)
    {
        return;
    }
    printf("prime %d\n", rBuf);
    int q = rBuf;
    pipe(toNext);
    while (read(pParent[0], &rBuf, INTSIZ) != 0)
    {

        int n = rBuf;
        // printf("%d/%d\n", n, q);
        if (n % q != 0)
        {
            // printf("n/q %d\n", n);
            write(toNext[1], &n, INTSIZ);
        }
    }
    close(toNext[1]);
    close(pParent[0]);
    if (fork() == 0)
    {
        fun(toNext);
    }
    wait((int *)0);
    exit(0);
}
int main(int argc, char *argv[])
{
    int pParent[2];
    pipe(pParent);
    for (int i = 2; i <= 35; i++)
    {
        write(pParent[1], &i, INTSIZ);
    }
    close(pParent[1]);
    fun(pParent);

    exit(0);
}
