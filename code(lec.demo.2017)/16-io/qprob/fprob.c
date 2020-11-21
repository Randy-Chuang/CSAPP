#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    int fd1, fd2;
    char c1a, c1b, c2a;
    char *fname = argv[1];
    fd1 = open(fname, O_RDONLY, 0);
    fd2 = open(fname, O_RDONLY, 0);

    read(fd1, &c1a, 1);
    dup2(fd2, fd1);
    read(fd1, &c1b, 1);
    read(fd2, &c2a, 1);
    printf("c1a = %c, c1b = %c, c2a = %c\n", c1a, c1b, c2a);

    close(fd1);
    close(fd2);
    return 0;
}
