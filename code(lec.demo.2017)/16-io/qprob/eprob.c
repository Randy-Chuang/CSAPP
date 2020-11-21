#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char** argv)
{
    if (argc == 1) return 0;
    int i = atoi(argv[1]);  
    if (i == 0) {printf("All done.\n"); return 0;}

    printf("%s %d - %d\n", argv[0] , i, getpid());  
    argv[1][0]--;
    execve(argv[0], argv, NULL);

    return 0;
}
