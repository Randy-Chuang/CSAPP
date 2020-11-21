#include <unistd.h>
#include <stdio.h>

int main(int argc, char** argv)
{
    if (argc == 1) return 0;
    char c = (*argv[1])--;
    if (c < 'a') {printf("All done.\n"); return 0;}

    printf("%s %c %d\n", argv[0] , c, getpid());  
    execve(argv[0], argv, NULL);

    return 0;
}
