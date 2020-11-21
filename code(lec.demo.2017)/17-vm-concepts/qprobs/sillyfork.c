#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>


int main(int argc, char *argv[]) 
{
    int x = 1;

    fork();

    if ((fork() & 0x1) != 0) {
	exit(0);
    }

    printf("Process %d Done\n", getpid());
    return 0;
}


