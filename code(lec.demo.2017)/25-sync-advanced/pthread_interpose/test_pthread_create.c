#include <stdio.h>
#include <pthread.h>
#include "csapp.h"

#define DEFAULT_THREAD_COUNT    20

#define DEBUG 

/* this function is run by the second thread */
void *worker_func(void *param)
{
#ifdef DEBUG    
    printf("Starting work on thread %zu\n", (uintptr_t)param);    
#endif    
    return NULL;
}


int main(int argc, char **argv)
{
    int i, thread_count = DEFAULT_THREAD_COUNT;
    pthread_t *threads;

    if(argc > 2)
    {
        //Usage
        printf("Usage: %s thread_count", argv[0]);
        return -1;
    }
    if(argc == 2)
    {
        //Get no of threads from command line
        sscanf(argv[1], "%d", &thread_count);
    }    

    threads = malloc(sizeof(pthread_t) * thread_count);

    for(i = 0; i < thread_count; i++)
    {
        if(pthread_create(&threads[i], NULL, &worker_func, 
                                        (void*)((unsigned long)i))) 
        {
            printf("Error creating threads\n");
            goto error;
        }
    }

    for(i = 0; i < thread_count; i++)
    {
        if(pthread_join(threads[i], NULL)) 
        {
            printf("Error joining threads\n");
            goto error;
        }
    }    

    free(threads);
    return 0;

error:
    free(threads);
    return -1;    
}
