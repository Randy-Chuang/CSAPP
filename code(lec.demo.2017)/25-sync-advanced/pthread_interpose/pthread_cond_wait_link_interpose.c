#include <sys/time.h>
#include <sys/types.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DEBUG

#ifdef DEBUG
#define dbg_printf(...) printf(__VA_ARGS__)
#else
#define dbg_printf(...)
#endif


/* Choose how want to delay */
#define UDELAY uspin

/* Sleep for a random period between 0 and MAX_SLEEP microseconds */
#define MAX_SLEEP 100000

#define CONVERT(val) (((double)val)/(double)RAND_MAX)
struct timeval p_time;

/*
 * Implement microsecond-scale delay that spins rather than sleeps.
 * Unlike usleep, this function will not be terminated when a signal
 * is received.
 */
static void uspin(useconds_t usec)
{
    if (usec == 0)
        return;
    unsigned long ustart;
    unsigned long ucurr;
    gettimeofday(&p_time, NULL);
    ustart = 1000000 * p_time.tv_sec + p_time.tv_usec;
    ucurr = ustart;
    while (ucurr - ustart < usec)
    {
        gettimeofday(&p_time, NULL);
        ucurr = 1000000 * p_time.tv_sec + p_time.tv_usec;
    }
}

static useconds_t get_random_sleep_time()
{
    gettimeofday(&p_time, NULL);
    srand(p_time.tv_usec);
    
    useconds_t thread_delay = (useconds_t)(CONVERT(rand()) * MAX_SLEEP);


    return thread_delay;
}


int __real_pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex);

int __wrap_pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex)
{
    dbg_printf("Inside __wrap_pthread_cond_wait. Intercepted the call to pthread_cond_wait\n");    
  
    int retval;
    useconds_t thread_delay;

    retval = __real_pthread_cond_wait(cond, mutex);

    //Release the mutex and lock it again
    if(pthread_mutex_unlock(mutex))
    {
        perror("");
    }

    //Sleep for a random time between releasing the mutex and locking it again
    thread_delay = get_random_sleep_time();
    UDELAY(thread_delay);

    if(pthread_mutex_lock(mutex))
    {
        perror("");
    }    

    dbg_printf("Locked the mutex again\n");    

    dbg_printf("Returned from __real_pthread_cond_wait\n");

    return retval;  

}
