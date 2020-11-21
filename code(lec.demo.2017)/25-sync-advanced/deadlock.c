/* Illustration of deadlock */

/*
 * If invoke program with no arguments, then uses "risky" behavior,
 * where locks acquired in different orders by different threads.
 * When invoke with some argument, follows safe behavior.
 */

#define NITERS 100

#include "csapp.h"
#include <stdbool.h>

sem_t mutex[2];

bool risky = true;

void *lock2(void *vargp) {
    int id = *(int *) vargp;
    int lock = id % 2;
    Free(vargp);
    if (risky) {
	P(&mutex[lock]); P(&mutex[1-lock]);
    } else {
	P(&mutex[0]); P(&mutex[1]);
    }
    printf("Got locks for id %d\n", id);
    V(&mutex[lock]);
    V(&mutex[1-lock]);
    return NULL;
}

int main(int argc, char *argv[])
{
    pthread_t tid[NITERS];
    Sem_init(&mutex[0], 0, 1);
    Sem_init(&mutex[1], 0, 1);
    int id;
    risky = argc == 1;
    for (id = 0; id < NITERS; id++) {
	void *vargp = malloc(sizeof(int));
	*(int *) vargp = id;
	Pthread_create(&tid[id], NULL, lock2, vargp);
    }
    for (id = 0; id < NITERS; id++) {
	Pthread_join(tid[id], NULL);
    }
    return 0;
}
