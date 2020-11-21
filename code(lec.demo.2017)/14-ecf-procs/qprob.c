#include "csapp.h"


int main(int argc, char *argv[]) 
{
    pid_t pid;
    int x = 1;

    pid = fork(); //line:ecf:forkreturn
    if (pid == 0) {  /* Child */
	printf("Child\n");
    }

    printf("Done\n");
    return 0;
}


