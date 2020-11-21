/* Program that runs for some number of seconds and then exits */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
    int d = 5;
    if (argc > 1)
	d = atoi(argv[1]);
    printf("Sleeping for %d seconds\n", d);
    sleep(d);
    printf("Finished sleeping\n");
}
