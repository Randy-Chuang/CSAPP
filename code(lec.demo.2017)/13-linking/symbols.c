#include <stdio.h>

int time;

int foo(int a) {
    int b = a + 1;
    return b;
}

int main(int argc, char *argv[])
{
    printf("%d\n", foo(5));
    return 0;
}
