#include "csapp.h"

void catch_child(int signo) {
    printf("Child exited!\n"); // this call may reenter printf/puts! BAD!  DEADLOCK!
    while (waitpid(-1, NULL, WNOHANG) > 0) continue; // reap all children
}

int main(int argc, char** argv) {
    char buf[MAXLINE];
    int i;
    signal(SIGCHLD, catch_child);
    for (i = 0; i < 1000000; i++) {
	if (fork() == 0) {
	    // in child, exit immediately
	    exit(0);
	}
	// in parent
	sprintf(buf, "Child #%d started\n", i);
	printf("%s", buf);
    }
    return 0;
}
