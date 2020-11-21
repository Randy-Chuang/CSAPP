#include "csapp.h"
#define N 2
void *thread(void *vargp);

long *pointers[N];

int main(int argc, char *argv[]) 
{
    long i;  
    pthread_t tids[N];

    for (i = 0; i < N; i++) 
        Pthread_create(&tids[i], NULL, thread, (void *) i);
    sleep(1);    // Sleep #1
    for (i = 0; i < N; i++)
	printf("Thread id %u has local value %ld\n",
	       (int) tids[i], *pointers[i]);
    for (i = 0; i < N; i++)
	Pthread_join(tids[i], NULL);
    return 0;
}

void *thread(void *vargp) 
{
    long myid = (long) vargp;
    pointers[myid] = &myid;
    sleep(2);    // Sleep #2
    return NULL;
}
