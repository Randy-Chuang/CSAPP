#include "csapp.h"

volatile sig_atomic_t ccount = 0;

void handler(int sig) {
    while (wait(NULL) > 0)
        ccount--;
}

int main(int argc, char *argv[]) {
    int i;
    Signal(SIGCHLD, handler);
    for (i = 0 ; i < 5; i++) {
        if (Fork() == 0)
            exit(0);  // Child
        else
            ccount++; // Parent
    }
    printf("ccount = %d\n", ccount);
    while (ccount > 0)
        ;
    return 0;
}
