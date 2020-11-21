#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    int fd1, fd2;
    char x, y, z;
    char *fname = argv[1];
    fd1 = open(fname, O_RDONLY, 0);
    fd2 = open(fname, O_RDONLY, 0);

    read(fd1, &x, 1);
    dup2(fd2, fd1);
    read(fd1, &y, 1);
    read(fd2, &z, 1);
    printf("x = %c, y = %c, z = %c\n", x, y, z);

    close(fd1);
    close(fd2);
    return 0;
}
