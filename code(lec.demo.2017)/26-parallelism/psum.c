/* 
 * psum.c - A simple parallel sum program
 */
/* $begin psum */
#include "csapp.h"
#include "realtimer.h"
#include <getopt.h>

#define MAXTHREADS 32    
#define MAXSPACING 16

/* Underlying data type */
typedef unsigned long data_t;
/* Thread type */
typedef void * (*thread_fun_t)(void *);

/* Thread routines */
void *sum_global(void *vargp);
void *sum_local(void *vargp);
void *sum_race(void *vargp);
void *sum_sem(void *vargp);
void *sum_mutex(void *vargp);


/* Global shared variables */
/* Multiple accumulators */
/* Partial sum computed by each thread */
volatile data_t psum[MAXTHREADS*MAXSPACING];
size_t spacing = 1;
/* Single accumulator */
volatile data_t global_sum;
/* Mutex semaphore for global sum */
sem_t semaphore;
pthread_mutex_t mutex;


size_t nelems_per_thread; /* Number of elements summed by each thread */

/* Interface declaration */
#define NMODE 5
/* Different summation modes */
typedef enum { SUM_GLOBAL, SUM_LOCAL, SUM_RACE, SUM_SEM, SUM_MUTEX } mode_t;
/* String mode selectors */
char *mode_string[NMODE] = {"global", "local", "race", "semaphore", "mutex" };
/* Thread functions */
thread_fun_t mode_fun[NMODE] = { sum_global, sum_local, sum_race, sum_sem, sum_mutex };

void run(size_t nelems, int nthreads, thread_fun_t fun) {
    data_t result = 0;
    data_t e;
    int i;
    pthread_t tid[MAXTHREADS];
    int myid[MAXTHREADS];
    start_timer();

    nelems_per_thread = nelems / nthreads;
    /* Set global values */
    global_sum = 0;
    sem_init(&semaphore, 0, 1);
    pthread_mutex_init(&mutex, NULL);

    /* Create peer threads and wait for them to finish */
    for (i = 0; i < nthreads; i++) {
	myid[i] = i;
	psum[i*spacing] = 0;
	Pthread_create(&tid[i], NULL, fun, &myid[i]);
    }                                                
    for (i = 0; i < nthreads; i++)                   
	Pthread_join(tid[i], NULL);                  
    
    /* Add up the partial sums computed by each thread */
    for (i = 0; i < nthreads; i++)                   
	result += psum[i*spacing];                           
    /* Add leftover elements */
    for (e = nthreads * nelems_per_thread; e < nelems; e++)
        result += e;
    /* Add global value */
    result += global_sum;

    /* Check final answer */
    data_t answer = nelems % 2 ?
      nelems * ((nelems-1)/2) :
      (nelems/2) * (nelems-1);
    if (result != answer)    
        printf("Error: result=%lu.  Should be %lu\n", result, answer);

    printf("%d\t%.2f\n", nthreads, elapsed_time());
}

/* Thread routines */
/* Accumulate in separate global memory locations */
void *sum_global(void *vargp) 
{
    int myid = *((int *)vargp);            /* Extract the thread ID */
    size_t start = myid * nelems_per_thread; /* Start element index */
    size_t end = start + nelems_per_thread;  /* End element index */
    size_t i;
    size_t index = myid*spacing;

    psum[index] = 0;
    for (i = start; i < end; i++) {
	psum[index] += i;                  
    }	                           
    return NULL;
}


/* Accumulate in separate local memory locations */
void *sum_local(void *vargp) 
{
    int myid = *((int *)vargp);            /* Extract the thread ID */
    size_t start = myid * nelems_per_thread; /* Start element index */
    size_t end = start + nelems_per_thread;  /* End element index */ 
    size_t i;
    size_t index = myid*spacing;

    data_t sum = 0;
    for (i = start; i < end; i++) {
	sum += i;                  
    }	                           
    psum[index] = sum; 
    return NULL;
}

/* Accumulate in single, unprotected location */
void *sum_race(void *vargp) 
{
    int myid = *((int *)vargp);            /* Extract the thread ID */
    size_t start = myid * nelems_per_thread; /* Start element index */
    size_t end = start + nelems_per_thread;  /* End element index */ 
    size_t i;

    for (i = start; i < end; i++) {
	global_sum += i;                  
    }	                           
    return NULL;
}

/* Accumulate in single location protected by semaphore */
void *sum_sem(void *vargp) 
{
    int myid = *((int *)vargp);            /* Extract the thread ID */
    size_t start = myid * nelems_per_thread; /* Start element index */
    size_t end = start + nelems_per_thread;  /* End element index */ 
    size_t i;

    for (i = start; i < end; i++) {
        sem_wait(&semaphore);
	global_sum += i;
	sem_post(&semaphore);
    }	                           
    return NULL;
}

/* Accumulate in single location protected by semaphore */
void *sum_mutex(void *vargp) 
{
    int myid = *((int *)vargp);            /* Extract the thread ID */
    size_t start = myid * nelems_per_thread; /* Start element index */
    size_t end = start + nelems_per_thread;  /* End element index */ 
    size_t i;

    for (i = start; i < end; i++) {
        pthread_mutex_lock(&mutex);
	global_sum += i;
	pthread_mutex_unlock(&mutex);
    }	                           
    return NULL;
}

void usage(char *cmd) {
  printf("Usage: %s [-h] [-t minthreads] [-x maxthreads] [-l logelems] [-m (i|r|s|m)]\n", cmd);
  printf("\t-h Print this message\n");
  printf("\t-t minthreads\tMinimum thread count\n");
  printf("\t-x maxthreads\tMaximum thread count\n");
  printf("\t-l logelems\tLog of number of elements\n");
  printf("\t-s spacing\tSpacing between global variables\n");
  printf("\t-m Select summation mode\n");
  printf("\t\tg\tAccumulate in separate global variables\n");
  printf("\t\tl\tAccumulate in separate local variables\n");
  printf("\t\tr\tAccumulate globally with race\n");
  printf("\t\ts\tAccumulate globally with semaphore\n");
  printf("\t\tm\tAccumulate globally with mutex\n");
  exit(0);
}

int main(int argc, char *argv[]) {
    int minthreads = 1;
    int maxthreads = 1;
    int log_nelems = 30;
    int mode = 0;

  int c;
  while ((c = getopt(argc, argv, "ht:x:l:s:m:")) != -1) {
    switch(c) {
    case 'h':
      usage(argv[0]);
      break;
    case 't':
      minthreads = maxthreads = atoi(optarg);
      break;
    case 'x':
      maxthreads = atoi(optarg);
      break;
    case 'l':
      log_nelems = atoi(optarg);
      break;
    case 's':
      spacing = atoi(optarg);
      break;
    case 'm':
      for (mode = 0; mode < NMODE; mode++)
	if (optarg[0] == mode_string[mode][0]) {
	  break;
	}
      if (mode >= NMODE) {
	printf("Unrecognized mode '%s'\n", optarg);
	usage(argv[0]);
      }
      break;
    default:
      printf("Unrecognized option '%c'\n", c);
      usage(argv[0]);
      break;
    }
  }
  data_t nelems = 1ULL << log_nelems;
  int i;
  printf("%lu elements\n", nelems);
  printf("Mode %s\n", mode_string[mode]);
  if (mode == SUM_GLOBAL)
    printf("Spacing %d\n", (int) spacing);
  printf("Threads\tSeconds\n");
  for (i = minthreads; i <= maxthreads; i++)
    run(nelems, i, mode_fun[mode]);
  return 0;
}


