#define NITERS 100

/* Sample code demonstrating reader-writers.  Create NITERS agents,
 * numbered from 1 to NITERS.  Each agent is randomly chosen to bea
 * reader (probability 80%) or a writer (probability 20%).  Writers
 * assign their ID to the global value.  Readers read the global
 * value.  Also insert random delays in agent generation so that
 * writers can catch up to the readers.
 */
#include "csapp.h"
#include <stdbool.h>

/* Uniform random number in [0.0,1.0] */
static double uniform() {
    return (double) random() / RAND_MAX;
}

bool choose_with_probability(double prob) {
    return uniform() <=  prob;
}


/* $begin reader1 */
/* Global variables */
int readcnt;    /* Initially = 0 */
sem_t mutex, w; /* Both initially = 1 */

void init() {
    readcnt = 0;
    Sem_init(&mutex, 0, 1);
    Sem_init(&w, 0, 1);
}

/* Get read access to data and read */
int ireader(int *buf) 
{
    P(&mutex);
    readcnt++;
    if (readcnt == 1) /* First in */
	P(&w);          
    V(&mutex);          

    /* Critical section */
    int v = *buf;

    P(&mutex);
    readcnt--;
    if (readcnt == 0) /* Last out */
	V(&w);
    V(&mutex);
    return v;
}

/* Get write access to data and write */
void iwriter(int *buf, int v) 
{
    P(&w);

    /* Critical section */
    *buf = v;
    /* Writing happens  */ 

    V(&w);
}

int global_value = 0;

void *rthread(void *vargp) {
    int id = *(int *) vargp;
    Free(vargp);
    int v = ireader(&global_value);
    printf("Reader %d read value %d\n", id, v);
    return NULL;
}

void *wthread(void *vargp) {
    int id = *(int *) vargp;
    Free(vargp);
    iwriter(&global_value, id);
    printf("Writer %d wrote value %d\n", id, id);
    return NULL;
}


int main(int argc, char *argv[]) {
    int niters = NITERS;
    if (argc > 1)
	niters = atoi(argv[1]);
    pthread_t tid[niters];
    init();
    int id;
    for (id = 1; id <= niters; id++) {
	bool doread = choose_with_probability(0.8);
	void *vargp = Malloc(sizeof(int));
	*(int *) vargp = id;
	Pthread_create(&tid[id-1], NULL,
		       doread ? rthread : wthread,
		       vargp);
    }
    for (id = 1; id <= niters; id++) {
	Pthread_join(tid[id-1], NULL);
    }
    return 0;
}
				
