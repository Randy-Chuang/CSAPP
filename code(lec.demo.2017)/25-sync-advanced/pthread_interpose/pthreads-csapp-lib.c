/*
 * pthreads-csapp-lib.c - Wrapper for functions frequetly used with pthread, 
 * that introduces non-determinism in the order that the threads are executed
 */
#define _GNU_SOURCE
#include <sys/time.h>
#include <sys/types.h>
#include <semaphore.h>
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>

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

void *wrapped_worker_func(void *param);


typedef struct
{   
    void *(*worker_func)(void *param);
    void *param;
}pthread_param_t;

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


void *wrapped_worker_func(void *param)
{
    pthread_param_t real_thread_params;    
    useconds_t thread_delay = get_random_sleep_time();

    //Make a local stack copy of the data param holds
    memcpy(&real_thread_params, param, sizeof(pthread_param_t));

    //free the area malloc'd  for the wrapper thread's parameters
    free(param);

    dbg_printf("Wrapped worker thread called. Going to sleep for %u\n", 
                 thread_delay);    

    /* Sleep for a random time before calling the actual worker thread
     function */
    UDELAY(thread_delay);

    //Jump to the actual thread and return what the actual thread returned
    return real_thread_params.worker_func(real_thread_params.param);
}


/*
 * pthread_create - Run-time interposed wrapper for pthread_create() that 
 * introduces non-determinism in the order that resulting threads are executed.
 * After creating a thread, it runs a wrapped worker thread which sleeps for 
 * a random period before finally switching the control to the actual worker
 * thread.
 */
int pthread_create(pthread_t *thread, const pthread_attr_t *attr,
                          void *(*start_routine) (void *), void *arg)
{
    char *error;
    int (*real_pthread_create)(pthread_t *thread, const pthread_attr_t *attr,
                          void *(*start_routine) (void *), void *arg);
    
    real_pthread_create = dlsym(RTLD_NEXT, "pthread_create");
    if( (error = dlerror()) != NULL)
    {
        fputs(error, stderr);
        exit(1);
    }


    pthread_param_t *worker_params;

    worker_params = malloc(sizeof(pthread_param_t));

    worker_params->worker_func  = start_routine;
    worker_params->param        = arg;

    /* Call the real pthread_create function and return the value like a normal
        call to pthread_create*/        
    return real_pthread_create(thread, attr, &wrapped_worker_func,
     worker_params);
}


/*
 * pthread_cond_wait - Run-time interposed wrapper for pthread_cond_wait()  
 * that introduces non-determinism in waiting for the condition variable.
 * We unlock the mutex and re-lock it to introduce non-determinism. 
 * We then call the real pthread_cond_wait function. After the function call, 
 * we sleep for a random period before returning the control to the caller.
 */
int pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex)
{
    char *error;
    int (*real_pthread_cond_wait)(pthread_cond_t *cond, 
                                                pthread_mutex_t *mutex);    
    int retval;
    useconds_t thread_delay;

    dbg_printf("Inside the run-time interposed wrapper of pthread_cond_wait. Intercepted the call to pthread_cond_wait\n");  
    real_pthread_cond_wait = dlsym(RTLD_NEXT, "pthread_cond_wait");
    if( (error = dlerror()) != NULL)
    {
        fputs(error, stderr);
        exit(1);
    }

    retval = real_pthread_cond_wait(cond, mutex);
    dbg_printf("Completed the real cond_wait\n");    

    //Release the mutex and lock it again
    if(pthread_mutex_unlock(mutex))
    {
        perror("");
    }

    //Sleep for a random time after releasing the mutex
    thread_delay = get_random_sleep_time();
    UDELAY(thread_delay);

    dbg_printf("Attempting to lock the mutex again\n");    
    if(pthread_mutex_lock(mutex))
    {
        perror("");
    }    

    dbg_printf("Locked the mutex again\n");    

    dbg_printf("Returned from real_pthread_cond_wait\n");

    return retval;    
}

int pthread_cond_signal(pthread_cond_t *cond) {
    char *error;
    int (*real_pthread_cond_signal)(pthread_cond_t *cond);
    int retval;
    useconds_t thread_delay;

    dbg_printf("Inside the run-time interposed wrapper of pthread_cond_signal. Intercepted the call to pthread_cond_signal\n");  
    real_pthread_cond_signal = dlsym(RTLD_NEXT, "pthread_cond_signal");
    if( (error = dlerror()) != NULL)
    {
        fputs(error, stderr);
        exit(1);
    }

    retval = real_pthread_cond_signal(cond);
    dbg_printf("Completed the real cond_signal\n");    

    //Sleep for a random time after releasing the mutex
    thread_delay = get_random_sleep_time();
    UDELAY(thread_delay);

    return retval;

}

int pthread_mutex_lock(pthread_mutex_t *mutex)
{
    char *error;
    int (*real_pthread_mutex_lock)(pthread_mutex_t *mutex);
    useconds_t thread_delay;

    real_pthread_mutex_lock = dlsym(RTLD_NEXT, "pthread_mutex_lock");
    dbg_printf("Intercepted the call to pthread_mutex_lock\n");  
    if( (error = dlerror()) != NULL)
    {
        fputs(error, stderr);
        exit(1);
    }

    /* Sleep for a random time before calling the actual function */
    thread_delay = get_random_sleep_time();
    UDELAY(thread_delay);

    return real_pthread_mutex_lock(mutex);
}

int pthread_mutex_trylock(pthread_mutex_t *mutex)
{
    char *error;
    int (*real_pthread_mutex_trylock)(pthread_mutex_t *mutex);
    useconds_t thread_delay;

    real_pthread_mutex_trylock = dlsym(RTLD_NEXT, "pthread_mutex_trylock");
    dbg_printf("Intercepted the call to pthread_mutex_trylock\n");  
    if( (error = dlerror()) != NULL)
    {
        fputs(error, stderr);
        exit(1);
    }

    /* Sleep for a random time before calling the actual function */
    thread_delay = get_random_sleep_time();
    UDELAY(thread_delay);
    
    return real_pthread_mutex_trylock(mutex);
}

int pthread_rwlock_rdlock(pthread_rwlock_t *rwlock)
{
    char *error;
    int (*real_pthread_rwlock_rdlock)(pthread_rwlock_t *rwlock);
    useconds_t thread_delay;

    real_pthread_rwlock_rdlock = dlsym(RTLD_NEXT, "pthread_rwlock_rdlock");
    dbg_printf("Intercepted the call to pthread_rwlock_rdlock\n");  
    if( (error = dlerror()) != NULL)
    {
        fputs(error, stderr);
        exit(1);
    }

    /* Sleep for a random time before calling the actual function */
    thread_delay = get_random_sleep_time();
    UDELAY(thread_delay);
    
    return real_pthread_rwlock_rdlock(rwlock);
}
int pthread_rwlock_tryrdlock(pthread_rwlock_t *rwlock)
{
    char *error;
    int (*real_pthread_rwlock_tryrdlock)(pthread_rwlock_t *rwlock);
    useconds_t thread_delay;

    real_pthread_rwlock_tryrdlock=dlsym(RTLD_NEXT, "pthread_rwlock_tryrdlock");
    dbg_printf("Intercepted the call to pthread_rwlock_tryrdlock\n");  
    if( (error = dlerror()) != NULL)
    {
        fputs(error, stderr);
        exit(1);
    }

    /* Sleep for a random time before calling the actual function */
    thread_delay = get_random_sleep_time();
    UDELAY(thread_delay);
    
    return real_pthread_rwlock_tryrdlock(rwlock);    
}

int pthread_rwlock_trywrlock(pthread_rwlock_t *rwlock)
{
    char *error;
    int (*real_pthread_rwlock_trywrlock)(pthread_rwlock_t *rwlock);
    useconds_t thread_delay;

    real_pthread_rwlock_trywrlock=dlsym(RTLD_NEXT, "pthread_rwlock_trywrlock");
    dbg_printf("Intercepted the call to pthread_rwlock_trywrlock\n");  
    if( (error = dlerror()) != NULL)
    {
        fputs(error, stderr);
        exit(1);
    }

    /* Sleep for a random time before calling the actual function */
    thread_delay = get_random_sleep_time();
    UDELAY(thread_delay);
    
    return real_pthread_rwlock_trywrlock(rwlock); 

}

int pthread_rwlock_wrlock(pthread_rwlock_t *rwlock)
{
    char *error;
    int (*real_pthread_rwlock_wrlock)(pthread_rwlock_t *rwlock);
    useconds_t thread_delay;

    real_pthread_rwlock_wrlock=dlsym(RTLD_NEXT, "pthread_rwlock_wrlock");
    dbg_printf("Intercepted the call to pthread_rwlock_wrlock\n");  
    if( (error = dlerror()) != NULL)
    {
        fputs(error, stderr);
        exit(1);
    }

    /* Sleep for a random time before calling the actual function */
    thread_delay = get_random_sleep_time();
    UDELAY(thread_delay);
    
    return real_pthread_rwlock_wrlock(rwlock); 
}

int pthread_rwlock_timedwrlock(pthread_rwlock_t *rwlock,
const struct timespec *abs_timeout)
{
    char *error;
    int (*real_pthread_rwlock_timedwrlock)(pthread_rwlock_t *rwlock,
                    const struct timespec *abs_timeout);
    useconds_t thread_delay;

    real_pthread_rwlock_timedwrlock = dlsym(RTLD_NEXT,
                                        "pthread_rwlock_timedwrlock");

    dbg_printf("Intercepted the call to pthread_rwlock_timedwrlock\n");  
    if( (error = dlerror()) != NULL)
    {
        fputs(error, stderr);
        exit(1);
    }

    /* Sleep for a random time before calling the actual function */
    thread_delay = get_random_sleep_time();
    UDELAY(thread_delay);
    
    return real_pthread_rwlock_timedwrlock(rwlock, abs_timeout);     
}


int pthread_once(pthread_once_t *once_control, void (*init_routine)(void))
{
    char *error;
    int (*real_pthread_once)(pthread_once_t *once_control, 
                                            void (*init_routine)(void));
    useconds_t thread_delay;

    real_pthread_once = dlsym(RTLD_NEXT, "pthread_once");
    dbg_printf("Intercepted the call to pthread_once\n");
    if( (error = dlerror()) != NULL)
    {
        fputs(error, stderr);
        exit(1);
    }

    /* Sleep for a random time before calling the actual function */
    thread_delay = get_random_sleep_time();
    UDELAY(thread_delay);
    
    return real_pthread_once(once_control, init_routine);
}


int sem_getvalue(sem_t *sem, int *sval)
{
    char *error;
    int (*real_sem_getvalue)(sem_t *sem, int *sval);
    useconds_t thread_delay;

    real_sem_getvalue = dlsym(RTLD_NEXT, "sem_getvalue");
    dbg_printf("Intercepted the call to sem_getvalue\n");
    if( (error = dlerror()) != NULL)
    {
        fputs(error, stderr);
        exit(1);
    }

    /* Sleep for a random time before calling the actual function */
    thread_delay = get_random_sleep_time();
    UDELAY(thread_delay);
    
    return real_sem_getvalue(sem, sval);
}

int sem_wait(sem_t *sem)
{
    char *error;
    int (*real_sem_wait)(sem_t *sem);
    useconds_t thread_delay;

    real_sem_wait = dlsym(RTLD_NEXT, "sem_wait");
    dbg_printf("Intercepted the call to sem_wait\n");
    if( (error = dlerror()) != NULL)
    {
        fputs(error, stderr);
        exit(1);
    }

    /* Sleep for a random time before calling the actual function */
    thread_delay = get_random_sleep_time();
    UDELAY(thread_delay);
    
    return real_sem_wait(sem);
}
