#include "csapp.h"

volatile sig_atomic_t ccount = 0;
volatile sig_atomic_t hcount = 0;

void handler(int sig) {
    hcount++;
    while (wait(NULL) > 0)
        ccount--;
    return;
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
    while (ccount > 0)
        ;
    printf("hcount = %d\n", hcount);
    return 0;
}
