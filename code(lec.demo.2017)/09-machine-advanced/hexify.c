/* Convert sequence of hex digits on command line into a string, terminated by \n */
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    int i;
    for (i = 1; i < argc; i++) {
	    unsigned long dig = strtoul(argv[i], NULL, 16);
	    putchar((char) dig);
    }
    putchar('\n');
    return 0;
}
