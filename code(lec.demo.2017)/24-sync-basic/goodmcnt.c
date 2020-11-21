/* 
 * goodcnt.c - A correctly synchronized counter program 
 * using pthread mutex
 */
/* $begin goodcnt */
#include "csapp.h"

void *thread(void *vargp); /* Thread routine prototype */

/* Global shared variables */
/* $begin goodcntmutexdef */
    volatile long cnt = 0; /* Counter */
    pthread_mutex_t mutex;           /* Mutex that protects counter */
/* $end goodcntmutexdef */

int main(int argc, char **argv) 
{
    long niters;
    pthread_t tid1, tid2;

    /* Check input argument */
    if (argc != 2) {
	printf("usage: %s <niters>\n", argv[0]);
	exit(0);
    }
    niters = atoi(argv[1]);

    /* Create threads and wait for them to finish */
/* $begin goodcntseminit */
    pthread_mutex_init(&mutex, NULL);  /* mutex = 1 */
/* $end goodcntseminit */
    Pthread_create(&tid1, NULL, thread, &niters);
    Pthread_create(&tid2, NULL, thread, &niters);
    Pthread_join(tid1, NULL);
    Pthread_join(tid2, NULL);

    /* Check result */
    if (cnt != (2 * niters))
	printf("BOOM! cnt=%ld\n", cnt);
    else
	printf("OK cnt=%ld\n", cnt);
    return 0;
}

/* Thread routine */
void *thread(void *vargp) 
{
    long i;
    long niters = *((int *)vargp);

/* $begin goodcntthread */
    for (i = 0; i < niters; i++) {
	pthread_mutex_lock(&mutex);
	cnt++;
	pthread_mutex_unlock(&mutex);
    }
/* $end goodcntthread */
    return NULL;
}
/* $end goodcnt */

