#include <stdio.h>
#include <stdlib.h>

/*
 * Record: 
 *      For a non-negative integer input.
 *      The simplified C-like pseudo-code transformed back from assembly code is listed below:
 *          Compute: 
 *                  int (edx) a *= 3;
 *                  int (eax) st = a & 0xf;
 *          L0:	    if (st > 0x4) goto L1;
 *                  jump according to st;
 *          st=3	a &= 0x01;
 *                  st = a;
 *                  goto L0;
 *          st=2 	a >>= 2;
 *                  st = a;
 *          L1:	    st = a & 0xf;
 *                  if(a >= 0) goto L0;
 *          st=1 	a *= 2;
 *                  goto L1;
 *          st=2	a--;
 *                  goto L1;
 *          st=0	calling printf with formatted string;
 *                  return; 
 *      
 *          In order to have the expected output string, the input number should satisfy the following requirements: 
 *          1. Non-negative integer
 *          2. At any looping stage, the lower 4-bits of integer argument shouldn't exceed 0x04.
 *              (Or the execution would be stuck in the loop forever)
 */

void compute(int inArg)
{
    int st;
    inArg *= 3;
    st = inArg & 0xf;
    do {
        switch(st)
        {
            case 0: printf("Finish\n"); inArg = -1; break;
            case 1: inArg *= 2; break;
            case 2: inArg >>= 2; break;
            case 3: inArg &= 1; break;
            case 4: inArg--; break;
            default:  break;
        }
        st = inArg & 0xf;
    } while (inArg >= 0);
}

int main(int argc, char** argv)
{
    int inArg;
    if (argc == 1) {fprintf(stderr, "Please rerun with a positive number argument\n"); return 1;}
    
    inArg = atoi(argv[1]);
    if (inArg < 0) {fprintf(stderr, "Argument was not a positive integer\n"); return 1;}
    compute(inArg);
    
    return 0;
}