#include "csapp.h"

int main(int argc, char *argv[]) {
    char *myargv[3] = {"/bin/echo", "hi!", NULL };
    execve(myargv[0], myargv, environ);
    printf("Done\n");
    return 0;
}
