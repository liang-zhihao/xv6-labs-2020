#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

/*

Look at user/ls.c to see how to read directories.

Use recursion to allow find to descend into sub-directories.

Don't recurse into "." and "..".

Changes to the file system persist across runs of qemu; to get a clean file system run make clean and then make qemu.

You'll need to use C strings. Have a look at K&R (the C book), for example Section 5.5.

Note that == does not compare strings like in Python. Use strcmp() instead.

*/

char *
fmtname(char *path)
{
    static char buf[DIRSIZ + 1];
    char *p;

    // Find first character after last slash.
    for (p = path + strlen(path); p >= path && *p != '/'; p--)
        ;
    p++;

    // Return blank-padded name.
    if (strlen(p) >= DIRSIZ)
        return p;
    memmove(buf, p, strlen(p));
    memset(buf + strlen(p), ' ', DIRSIZ - strlen(p));
    return buf;
}
void fun(char *path, char *fileName)
{
    char buf[512], *p;
    int fd;
    struct dirent de;
    struct stat st;
    memset(fileName + strlen(fileName), ' ', DIRSIZ - strlen(fileName));
    // open the file
    if ((fd = open(path, 0)) < 0)
    {
        fprintf(2, "ls: cannot open %s\n", path);
        return;
    }
    // get status of the file
    if (fstat(fd, &st) < 0)
    {
        fprintf(2, "ls: cannot stat %s\n", path);
        close(fd);
        return;
    }

    // if file is a dir, print the files of the dir.
    if (strlen(path) + 1 + DIRSIZ + 1 > sizeof buf)
    {
        printf("ls: path too long\n");
        return;
    }
    // copy the path to buf
    strcpy(buf, path);
    p = buf + strlen(buf);

    *p++ = '/';
    // printf("buf: %s\n", buf);
    // printf("strlen(buf): %d\n", strlen(buf));
    while (read(fd, &de, sizeof(de)) == sizeof(de))
    {
        if (de.inum == 0)
            continue;
        memmove(p, de.name, DIRSIZ);
        p[DIRSIZ] = 0;
        if (stat(buf, &st) < 0)
        {
            printf("ls: cannot stat %s\n", buf);
            continue;
        }

        char *currentFileName = fmtname(buf);
        // printf("strlen: %d\n", strlen(fileName));
        // printf("%d\n", strcmp(fileName, currentFileName));
        // printf("fileName: %s,buf: %s1\n", fileName, currentFileName);
        if (st.type == T_DIR && currentFileName[0] != '.')
        {
            // printf("Dir: %s\n", buf);
            fun(buf, fileName);
        }

        if (strcmp(fileName, currentFileName) == 0 && st.type == T_FILE)
        {
            printf("%s\n", buf);
        }
    }

    close(fd);
}

int main(int argc, char *argv[])
{

    if (argc >= 3)
    {
        fun(argv[1], argv[2]);
    }
    else
    {
        printf("args error\n");
    }

    exit(0);
}
/*
#define T_DIR 1 // Directory
#define T_FILE 2 // File
#define T_DEVICE 3 // Device
struct stat {
int dev; // File system’s disk device
uint ino; // Inode number
short type; // Type of file
short nlink; // Number of links to file
uint64 size; // Size of file in bytes
};
struct dirent {
  ushort inum;
  char name[DIRSIZ];
};

*/