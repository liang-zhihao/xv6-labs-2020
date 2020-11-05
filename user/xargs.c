#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

/*

Use fork and exec to invoke the command on each line of input. Use wait in the parent to wait for the child to complete the command.

To read individual lines of input, read a character at a time until a newline ('\n') appears.

kernel/param.h declares MAXARG, which may be useful if you need to declare an argv array.

Add the program to UPROGS in Makefile.

Changes to the file system persist across runs of qemu; to get a clean file system run make clean and then make qemu.

*/
char **readFromLine()
{
    static char *args[32];
    char str[512];
    char t;
    int length = 0;
    int size = 0;
    while (read(0, &t, sizeof(t)) != 0)
    {
        // printf("%c", t);
        if (t == '\n')
        {
            // printf("34: %s\n", str);
            // allocate mem to pointer ! important
            args[size] = (char *)malloc(sizeof(str));
            strcpy(args[size++], str);
            // printf("36: %s\n", args[size - 1]);
            memset(str, 0, sizeof(str));
            length = 0;
        }
        else
        {
            str[length++] = t;
        }
    }

    return args;
}
int main(int argc, char *argv[])
{
    char **argsFromLines = readFromLine();
    char *args[32];
    int ind = 0;
    for (; ind < argc - 1; ind++)
    {
        args[ind] = argv[ind + 1];
    }
    for (int i = 0; argsFromLines && argsFromLines[i]; i++)
    {
        // printf("59: %s\n", argsFromLines[i]);
        args[ind++] = argsFromLines[i];
    }

    // for (int i = 0; i < ind; i++)
    // {
    //     printf("64: %s\n", args[i]);
    // }

    if (argc >= 2)
    {
        if (fork() == 0)
        {
            exec(argv[1], args);
        }
    }
    wait((int *)0);
    exit(0);
}
