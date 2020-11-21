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
#include <stdbool.h>
#include <dlfcn.h>

#define DEBUG

#ifdef DEBUG
#define dbg_printf(...) printf(__VA_ARGS__)
#else
#define dbg_printf(...)
#endif

/*** Support Code ***/

/* Choose how want to delay */
#define UDELAY uspin

/* Sleep for a random period between 0 and MAX_SLEEP microseconds */
#define MAX_SLEEP 100000

/* Probability that a call to pthread_cond_wait will return spuriously */
#define SPURIOUS_PROB 0.33

/* Convert random value to float in range [0,1) */
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

/* Flip weighted coin and return yes/no */
static bool make_choice(double prob) 
{
    gettimeofday(&p_time, NULL);
    srand(p_time.tv_usec);
    double cval = CONVERT(rand());
    return cval < prob;
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

/*** Functions requiring special consideration ***/

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
 * pthread_cond_wait - Run-time interposed wrapper for
 * pthread_cond_wait() With some probability, it returns spuriously.
 * Otherwise, we call the real pthread_cond_wait function. After the
 * function call, we unlock the mutex, sleep for a random period, and
 * re-lock it to introduce non-determinism.
 */

int pthread_cond_wait(pthread_cond_t *cond, pthread_mutex_t *mutex)
{
    char *error;
    int (*real_pthread_cond_wait)(pthread_cond_t *cond, 
                                                pthread_mutex_t *mutex);    
    int retval;
    useconds_t thread_delay;

    dbg_printf("Call to pthread_cond_wait\n");  

    if (make_choice(SPURIOUS_PROB)) {
	dbg_printf("Spurious return from cond_wait\n");
	return 0;
    } else {
	dbg_printf("Proceeding with pthread_cond_wait\n");	
    }


    real_pthread_cond_wait = dlsym(RTLD_NEXT, "pthread_cond_wait");
    if( (error = dlerror()) != NULL)
    {
        fputs(error, stderr);
        exit(1);
    }

    retval = real_pthread_cond_wait(cond, mutex);

    //Release the mutex and lock it again
    if(pthread_mutex_unlock(mutex))
    {
        perror("");
    }
    dbg_printf("Unlocked the mutex\n");

    //Sleep for a random time after releasing the mutex
    thread_delay = get_random_sleep_time();
    UDELAY(thread_delay);

    if(pthread_mutex_lock(mutex))
    {
        perror("");
    }    

    dbg_printf("Locked the mutex again after a random delay\n");

    return retval;    
}

/*** Functions for which an initial delay is inserted ***/

int pthread_cancel(pthread_t thread)
{
    char *error;
    int (*real_pthread_cancel)(pthread_t thread);
    useconds_t thread_delay;

    real_pthread_cancel = dlsym(RTLD_NEXT, "pthread_cancel");
    dbg_printf("Call to pthread_cancel\n");  
    if( (error = dlerror()) != NULL)
    {
        fputs(error, stderr);
        exit(1);
    }

    /* Sleep for a random time before calling the actual function */
    thread_delay = get_random_sleep_time();
    UDELAY(thread_delay);

    return real_pthread_cancel(thread);
}

int pthread_mutex_lock(pthread_mutex_t *mutex)
{
    char *error;
    int (*real_pthread_mutex_lock)(pthread_mutex_t *mutex);
    useconds_t thread_delay;

    real_pthread_mutex_lock = dlsym(RTLD_NEXT, "pthread_mutex_lock");
    dbg_printf("Call to pthread_mutex_lock\n");  
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

int pthread_mutex_timedlock(pthread_mutex_t *mutex,
			    const struct timespec *abs_timeout)
{
    char *error;
    int (*real_pthread_mutex_timedlock)(pthread_mutex_t *mutex,
					const struct timespec *abs_timeout);
    useconds_t thread_delay;

    real_pthread_mutex_timedlock = dlsym(RTLD_NEXT, "pthread_mutex_timedlock");
    dbg_printf("Call to pthread_mutex_timedlock\n");  
    if( (error = dlerror()) != NULL)
    {
        fputs(error, stderr);
        exit(1);
    }

    /* Sleep for a random time before calling the actual function */
    thread_delay = get_random_sleep_time();
    UDELAY(thread_delay);
    
    return real_pthread_mutex_timedlock(mutex, abs_timeout);
}


int pthread_mutex_trylock(pthread_mutex_t *mutex)
{
    char *error;
    int (*real_pthread_mutex_trylock)(pthread_mutex_t *mutex);
    useconds_t thread_delay;

    real_pthread_mutex_trylock = dlsym(RTLD_NEXT, "pthread_mutex_trylock");
    dbg_printf("Call to pthread_mutex_trylock\n");  
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

int pthread_once(pthread_once_t *once_control, void (*init_routine)(void))
{
    char *error;
    int (*real_pthread_once)(pthread_once_t *once_control, 
                                            void (*init_routine)(void));
    useconds_t thread_delay;

    real_pthread_once = dlsym(RTLD_NEXT, "pthread_once");
    dbg_printf("Call to pthread_once\n");
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

int pthread_rwlock_rdlock(pthread_rwlock_t *rwlock)
{
    char *error;
    int (*real_pthread_rwlock_rdlock)(pthread_rwlock_t *rwlock);
    useconds_t thread_delay;

    real_pthread_rwlock_rdlock = dlsym(RTLD_NEXT, "pthread_rwlock_rdlock");
    dbg_printf("Call to pthread_rwlock_rdlock\n");  
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

int pthread_rwlock_timedwrlock(pthread_rwlock_t *rwlock,
const struct timespec *abs_timeout)
{
    char *error;
    int (*real_pthread_rwlock_timedwrlock)(pthread_rwlock_t *rwlock,
                    const struct timespec *abs_timeout);
    useconds_t thread_delay;

    real_pthread_rwlock_timedwrlock = dlsym(RTLD_NEXT,
                                        "pthread_rwlock_timedwrlock");

    dbg_printf("Call to pthread_rwlock_timedwrlock\n");  
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

int pthread_rwlock_tryrdlock(pthread_rwlock_t *rwlock)
{
    char *error;
    int (*real_pthread_rwlock_tryrdlock)(pthread_rwlock_t *rwlock);
    useconds_t thread_delay;

    real_pthread_rwlock_tryrdlock=dlsym(RTLD_NEXT, "pthread_rwlock_tryrdlock");
    dbg_printf("Call to pthread_rwlock_tryrdlock\n");  
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
    dbg_printf("Call to pthread_rwlock_trywrlock\n");  
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
    dbg_printf("Call to pthread_rwlock_wrlock\n");  
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

int sem_getvalue(sem_t *sem, int *sval)
{
    char *error;
    int (*real_sem_getvalue)(sem_t *sem, int *sval);
    useconds_t thread_delay;

    real_sem_getvalue = dlsym(RTLD_NEXT, "sem_getvalue");
    dbg_printf("Call to sem_getvalue\n");
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
    dbg_printf("Call to sem_wait\n");
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

/*** Functions that are used as-is.  Must be included for completeness ***/

int pthread_attr_init(pthread_attr_t *attr)
{
    char *error;
    int (*real_pthread_attr_init)(pthread_attr_t *attr);    
    dbg_printf("Call to pthread_attr_init\n");  

    real_pthread_attr_init = dlsym(RTLD_NEXT, "pthread_attr_init");
    if( (error = dlerror()) != NULL)
    {
        fputs(error, stderr);
        exit(1);
    }
    return real_pthread_attr_init(attr);
}

int pthread_attr_destroy(pthread_attr_t *attr)
{
    char *error;
    int (*real_pthread_attr_destroy)(pthread_attr_t *attr);    
    dbg_printf("Call to pthread_attr_destroy\n");  

    real_pthread_attr_destroy = dlsym(RTLD_NEXT, "pthread_attr_destroy");
    if( (error = dlerror()) != NULL)
    {
        fputs(error, stderr);
        exit(1);
    }
    return real_pthread_attr_destroy(attr);
}

int pthread_cond_broadcast(pthread_cond_t *cond)
{
    char *error;
    int (*real_pthread_cond_broadcast)(pthread_cond_t *cond);    
    dbg_printf("Call to pthread_cond_broadcast\n");  

    real_pthread_cond_broadcast = dlsym(RTLD_NEXT, "pthread_cond_broadcast");
    if( (error = dlerror()) != NULL)
    {
        fputs(error, stderr);
        exit(1);
    }

    return real_pthread_cond_broadcast(cond);
}

int pthread_cond_destroy(pthread_cond_t *cond)
{
    char *error;
    int (*real_pthread_cond_destroy)(pthread_cond_t *cond);    
    dbg_printf("Call to pthread_cond_destroy\n");  

    real_pthread_cond_destroy = dlsym(RTLD_NEXT, "pthread_cond_destroy");
    if( (error = dlerror()) != NULL)
    {
        fputs(error, stderr);
        exit(1);
    }

    return real_pthread_cond_destroy(cond);
}

int pthread_cond_init(pthread_cond_t *cond, const pthread_condattr_t *attr)
{
    char *error;
    int (*real_pthread_cond_init)(pthread_cond_t *cond, 
                                        const pthread_condattr_t *attr);    
    dbg_printf("Call to pthread_cond_init\n");  

    real_pthread_cond_init = dlsym(RTLD_NEXT, "pthread_cond_init");
    if( (error = dlerror()) != NULL)
    {
        fputs(error, stderr);
        exit(1);
    }

    return real_pthread_cond_init(cond, attr);    
}

int pthread_cond_signal(pthread_cond_t *cond)
{
    char *error;
    int (*real_pthread_cond_signal)(pthread_cond_t *cond);    
    dbg_printf("Call to pthread_cond_signal\n");  

    real_pthread_cond_signal = dlsym(RTLD_NEXT, "pthread_cond_signal");
    if( (error = dlerror()) != NULL)
    {
        fputs(error, stderr);
        exit(1);
    }

    return real_pthread_cond_signal(cond);
}

int pthread_detach(pthread_t thread)
{
    char *error;
    int (*real_pthread_detach)(pthread_t thread);    
    dbg_printf("Call to pthread_detach\n");  

    real_pthread_detach = dlsym(RTLD_NEXT, "pthread_detach");
    if( (error = dlerror()) != NULL)
    {
        fputs(error, stderr);
        exit(1);
    }

    return real_pthread_detach(thread);   
}

int pthread_equal(pthread_t t1, pthread_t t2)
{
    char *error;
    int (*real_pthread_equal)(pthread_t t1, pthread_t t2);    
    dbg_printf("Call to pthread_equal\n");  

    real_pthread_equal = dlsym(RTLD_NEXT, "pthread_equal");
    if( (error = dlerror()) != NULL)
    {
        fputs(error, stderr);
        exit(1);
    }

    return real_pthread_equal(t1, t2);   
}


void pthread_exit(void *retval)
{
    char *error;
    void (*real_pthread_exit)(void *retval);    
    dbg_printf("Call to pthread_exit\n");  

    real_pthread_exit = dlsym(RTLD_NEXT, "pthread_exit");
    if( (error = dlerror()) != NULL)
    {
        fputs(error, stderr);
        exit(1);
    }

    real_pthread_exit(retval);
    while (1); /* Assure compiler that function will not return */
}

int pthread_join(pthread_t thread, void **retval)
{
    char *error;
    int (*real_pthread_join)(pthread_t thread, void **retval);    
    dbg_printf("Call to pthread_join\n");  

    real_pthread_join = dlsym(RTLD_NEXT, "pthread_join");
    if( (error = dlerror()) != NULL)
    {
        fputs(error, stderr);
        exit(1);
    }

    return real_pthread_join(thread, retval);   
}

int pthread_mutex_destroy(pthread_mutex_t *mutex)
{
    char *error;
    int (*real_pthread_mutex_destroy)(pthread_mutex_t *mutex);    
    dbg_printf("Call to pthread_mutex_destroy\n");  

    real_pthread_mutex_destroy = dlsym(RTLD_NEXT, "pthread_mutex_destroy");
    if( (error = dlerror()) != NULL)
    {
        fputs(error, stderr);
        exit(1);
    }

    return real_pthread_mutex_destroy(mutex);   
}

int pthread_mutex_init(pthread_mutex_t *mutex, const pthread_mutexattr_t *attr)
{
    char *error;
    int (*real_pthread_mutex_init)(pthread_mutex_t *mutex, 
                                    const pthread_mutexattr_t *attr);    
    dbg_printf("Call to pthread_mutex_init\n");  

    real_pthread_mutex_init = dlsym(RTLD_NEXT, "pthread_mutex_init");
    if( (error = dlerror()) != NULL)
    {
        fputs(error, stderr);
        exit(1);
    }

    return real_pthread_mutex_init(mutex, attr);   
}

int pthread_mutex_unlock(pthread_mutex_t *mutex)
{
    char *error;
    int (*real_pthread_mutex_unlock)(pthread_mutex_t *mutex);    
    dbg_printf("Call to pthread_mutex_unlock\n");  

    real_pthread_mutex_unlock = dlsym(RTLD_NEXT, "pthread_mutex_unlock");
    if( (error = dlerror()) != NULL)
    {
        fputs(error, stderr);
        exit(1);
    }

    return real_pthread_mutex_unlock(mutex);   
}

int pthread_rwlock_destroy(pthread_rwlock_t *rwlock)
{
    char *error;
    int (*real_pthread_rwlock_destroy)(pthread_rwlock_t *rwlock);  
    dbg_printf("Call to pthread_rwlock_destroy\n");  

    real_pthread_rwlock_destroy = dlsym(RTLD_NEXT, "pthread_rwlock_destroy");
    if( (error = dlerror()) != NULL)
    {
        fputs(error, stderr);
        exit(1);
    }

    return real_pthread_rwlock_destroy(rwlock); 
}

int pthread_rwlock_init(pthread_rwlock_t *rwlock, const pthread_rwlockattr_t *attr)
{
    char *error;
    int (*real_pthread_rwlock_init)(pthread_rwlock_t *rwlock, 
                                    const pthread_rwlockattr_t *attr);  
    dbg_printf("Call to pthread_rwlock_init\n");  

    real_pthread_rwlock_init = dlsym(RTLD_NEXT, "pthread_rwlock_init");
    if( (error = dlerror()) != NULL)
    {
        fputs(error, stderr);
        exit(1);
    }

    return real_pthread_rwlock_init(rwlock, attr); 
}

int pthread_rwlock_unlock(pthread_rwlock_t *rwlock)
{
    char *error;
    int (*real_pthread_rwlock_unlock)(pthread_rwlock_t *rwlock);  
    dbg_printf("Call to pthread_rwlock_unlock\n");  

    real_pthread_rwlock_unlock = dlsym(RTLD_NEXT, "pthread_rwlock_unlock");
    if( (error = dlerror()) != NULL)
    {
        fputs(error, stderr);
        exit(1);
    }

    return real_pthread_rwlock_unlock(rwlock); 
}

int pthread_rwlockattr_getpshared(const pthread_rwlockattr_t *attr, 
                                                        int *pshared)
{
    char *error;
    int (*real_pthread_rwlockattr_getpshared)(const pthread_rwlockattr_t *attr,
                                                            int *pshared);  
    dbg_printf("Call to pthread_rwlockattr_getpshared\n");  

    real_pthread_rwlockattr_getpshared = dlsym(RTLD_NEXT, 
                                        "pthread_rwlockattr_getpshared");
    if( (error = dlerror()) != NULL)
    {
        fputs(error, stderr);
        exit(1);
    }

    return real_pthread_rwlockattr_getpshared(attr, pshared); 
}

int pthread_rwlockattr_destroy(pthread_rwlockattr_t *attr)
{
    char *error;
    int (*real_pthread_rwlockattr_destroy)(pthread_rwlockattr_t *attr);
    dbg_printf("Call to pthread_rwlockattr_destroy\n");  

    real_pthread_rwlockattr_destroy = dlsym(RTLD_NEXT, 
                                        "pthread_rwlockattr_destroy");
    if( (error = dlerror()) != NULL)
    {
        fputs(error, stderr);
        exit(1);
    }

    return real_pthread_rwlockattr_destroy(attr); 
}

int pthread_rwlockattr_init(pthread_rwlockattr_t *attr)
{
    char *error;
    int (*real_pthread_rwlockattr_init)(pthread_rwlockattr_t *attr);
    dbg_printf("Call to pthread_rwlockattr_init\n");  

    real_pthread_rwlockattr_init = dlsym(RTLD_NEXT, 
                                        "pthread_rwlockattr_init");
    if( (error = dlerror()) != NULL)
    {
        fputs(error, stderr);
        exit(1);
    }

    return real_pthread_rwlockattr_init(attr); 
}

int pthread_rwlockattr_setpshared(pthread_rwlockattr_t *attr, int pshared)
{
    char *error;
    int (*real_pthread_rwlockattr_setpshared)(pthread_rwlockattr_t *attr, 
                                                            int pshared);  
    dbg_printf("Call to pthread_rwlockattr_setpshared\n");  

    real_pthread_rwlockattr_setpshared = dlsym(RTLD_NEXT, 
                                        "pthread_rwlockattr_setpshared");
    if( (error = dlerror()) != NULL)
    {
        fputs(error, stderr);
        exit(1);
    }

    return real_pthread_rwlockattr_setpshared(attr, pshared); 
}

pthread_t pthread_self(void)
{
    char *error;
    pthread_t (*real_pthread_self)(void);  
    dbg_printf("Call to pthread_self\n");  

    real_pthread_self = dlsym(RTLD_NEXT, "pthread_self");
    if( (error = dlerror()) != NULL)
    {
        fputs(error, stderr);
        exit(1);
    }

    return real_pthread_self(); 
}

int sem_destroy(sem_t *sem)
{
    char *error;
    int (*real_sem_destroy)(sem_t *sem);  
    dbg_printf("Call to sem_destroy\n");  

    real_sem_destroy = dlsym(RTLD_NEXT, "sem_destroy");
    if( (error = dlerror()) != NULL)
    {
        fputs(error, stderr);
        exit(1);
    }

    return real_sem_destroy(sem); 
}


int sem_init(sem_t *sem, int pshared, unsigned int value)
{
    char *error;
    int (*real_sem_init)(sem_t *sem, int pshared, unsigned int value);  
    dbg_printf("Call to sem_init\n");  

    real_sem_init = dlsym(RTLD_NEXT, "sem_init");
    if( (error = dlerror()) != NULL)
    {
        fputs(error, stderr);
        exit(1);
    }

    return real_sem_init(sem, pshared, value); 
}

sem_t *sem_open(const char *name, int oflag, ...)
{
    char *error;
    sem_t *(*real_sem_open)(const char *name, int oflag);
    dbg_printf("Call to sem_open\n");  

    real_sem_open = dlsym(RTLD_NEXT, "sem_open");
    if( (error = dlerror()) != NULL)
    {
        fputs(error, stderr);
        exit(1);
    }

    return real_sem_open(name, oflag); 
}

int sem_post(sem_t *sem)
{
    char *error;
    int (*real_sem_post)(sem_t *sem);  
    dbg_printf("Call to sem_post\n");  

    real_sem_post = dlsym(RTLD_NEXT, "sem_post");
    if( (error = dlerror()) != NULL)
    {
        fputs(error, stderr);
        exit(1);
    }

    return real_sem_post(sem); 
}

int sem_unlink(const char *name)
{
    char *error;
    int (*real_sem_unlink)(const char *name);  
    dbg_printf("Call to sem_unlink\n");  

    real_sem_unlink = dlsym(RTLD_NEXT, "sem_unlink");
    if( (error = dlerror()) != NULL)
    {
        fputs(error, stderr);
        exit(1);
    }

    return real_sem_unlink(name); 
}

