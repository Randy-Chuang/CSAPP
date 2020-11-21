/* Illustration of producer/consumer code using sbuf */

/*
 * The idea is to have NAGENTS producer agents, each of which
 * generates a range of NELEMENTS integers, yielding values from 0 to
 * NAGENTS*NELEMENTS-1.  There are also NAGENTS consumer agents, each
 * consuming NELEMENTS values.  In the end, each value should be
 * produced and consumed.  Tweak random delays to get some buildup in
 * the buffer.
 */

#define NAGENTS 5
#define NELEMENTS 20
#define BUFSIZE 10

#include <stdbool.h>
#include "csapp.h"
#include "sbuf.h"

sbuf_t sbuf;

int item_count[NAGENTS * NELEMENTS] = { 0 };

void *produce_thread(void *vargp) {
    int id = *(int *) vargp;
    Free(vargp);
    int ele;
    for (ele = id * NELEMENTS; ele < (id+1) * NELEMENTS; ele++) {
	sbuf_insert(&sbuf, ele);
	printf("Producer agent %d produced value %d\n", id, ele);
    }
    return NULL;
}

void *consume_thread(void *vargp) {
    int id = *(int *) vargp;
    Free(vargp);
    int i;
    for (i = 0; i < NELEMENTS; i++) {
	int ele = sbuf_remove(&sbuf);
	printf("Consumer agent %d consumed value %d\n", id, ele);
	item_count[ele]++;
    }
    return NULL;
}

int main(int argc, char *argv[]) {
    sbuf_init(&sbuf, BUFSIZE);
    pthread_t ptid[NAGENTS];
    pthread_t ctid[NAGENTS];
    int id;
    for (id = 0; id < NAGENTS; id++) {
	void *vargp = malloc(sizeof(int));
	*(int *) vargp = id;
	Pthread_create(&ptid[id], NULL, produce_thread, vargp);
	vargp = malloc(sizeof(int));
	*(int *) vargp = id;
	Pthread_create(&ctid[id], NULL, consume_thread, vargp);
    }
    for (id = 0; id < NAGENTS; id++) {
	Pthread_join(ptid[id], NULL);
	Pthread_join(ctid[id], NULL);
    }
    int ele;
    bool ok = true;
    for (ele = 0; ele < NAGENTS * NELEMENTS; ele++) {
	if (item_count[ele] != 1) {
	    printf("Error: Element %d has invalid count %d\n", ele, item_count[ele]);
	    ok = false;
	}
    }
    if (ok)
	printf("All %d values produced and consumed\n", NAGENTS * NELEMENTS);
    return 0;
    
}
