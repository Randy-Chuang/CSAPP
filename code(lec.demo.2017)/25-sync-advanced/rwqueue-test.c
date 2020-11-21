#define NITERS 100

/* Sample code demonstrating reader-writers using queue based
 * synchronization..  Create NITERS agents, numbered from 1 to NITERS.
 * Each agent is randomly chosen to be a reader (probability 80%) or a
 * writer (probability 20%).  Writers assign their ID to the global
 * value.  Readers read the global value.  Also insert random delays
 * in agent generation so that writers can catch up to the readers.
 */
#include "rwqueue.h"

/* Uniform random number in [0.0,1.0] */
static double uniform() {
    return (double) random() / RAND_MAX;
}

bool choose_with_probability(double prob) {
    return uniform() <=  prob;
}


/* Global variables */
rw_queue_t q;

void init() {
    rw_queue_init(&q);
}

/* Get read access to data and read */
int ireader(int *buf) 
{
    rw_token_t tok;
    rw_queue_request_read(&q, &tok);

    /* Critical section */
    int v = *buf;

    rw_queue_release(&q);
    return v;
}

/* Get write access to data and write */
void iwriter(int *buf, int v) 
{
    rw_token_t tok;
    rw_queue_request_write(&q, &tok);
    /* Critical section */
    *buf = v;
    /* Writing happens  */ 
    rw_queue_release(&q);
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
				
