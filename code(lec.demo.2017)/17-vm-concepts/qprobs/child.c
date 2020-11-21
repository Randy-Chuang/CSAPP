#include "csapp.h"

volatile sig_atomic_t children = 0;
volatile sig_atomic_t handles = 0;

void handler(int sig) {
    handles++;
    while (wait(NULL) > 0)
        children++;
    return;
}

int main(int argc, char *argv[]) {
    int i;
    pid_t parent = getpid();
    signal(SIGUSR1, handler);
    for (i = 0 ; i < 5; i++) {
        if (fork() == 0) {
	    // Child
	    kill(parent, SIGUSR1); 
            exit(0);
	}
    }
    while (children < 5)
        ;
    printf("handles = %d\n", handles);
    return 0;
}
