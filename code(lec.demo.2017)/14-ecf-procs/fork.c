#include "csapp.h"

/* $begin fork */
/* $begin wasidefork */
int main(int argc, char *argv[]) 
{
    pid_t pid;
    int x = 1;

    pid = Fork(); //line:ecf:forkreturn
    if (pid == 0) {  /* Child */
	printf("child : x=%d\n", ++x); //line:ecf:childprint
	fflush(stdout);
	return 0;
    }

    /* Parent */
    printf("parent: x=%d\n", --x); //line:ecf:parentprint
    fflush(stdout);
    return 0;
}
/* $end fork */
/* $end wasidefork */

