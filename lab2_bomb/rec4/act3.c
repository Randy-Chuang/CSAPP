#include <string.h>
#include <stdlib.h>
#include <stdio.h>

/*
 * Record: 
 *    After inspecting the assembly code and debugging with gdb, 
 *    we could find that our input arguments should make this formula (a + b + 5 = 15213)
 *    hold. However, there is a subtle problem between the source code and source assembly code.
 *    For the function - compare(), the type of arguments defined in the source code is integer, 
 *    but the turely implementation in source assembly code is written by 64 bits instruction. 
 *    Therefore, unless we modify either of the source codes to match the length of arguments, 
 *    we won't be able to input negative numbers for testing.
 */

int compare(int a, int b);

int main(int argc, char** argv)
{
    int a,b;
    if (argc!=3) {
       printf("usage: %s num1 num2\n",argv[0]);
       exit(-1);
    }

    a=atoi(argv[1]);
    b=atoi(argv[2]);
    if (compare(a,b)==1) {
       printf("good args!\n");
    }
    else {
       printf("bad numbers, try harder!\n");
    }
    return 0;
}
