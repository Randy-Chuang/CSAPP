#include <stdio.h>
#include <stdlib.h>

int sq(int x) {
    return x*x;
}

int main(int argc, char *argv[]) {
    int i;
    for (i = 1; i < argc; i++) {
	    int x = atoi(argv[i]);
	    int sx = sq(x);
	    printf("sq(%d) = %d\n", x, sx);
    }
    return 0;
}

