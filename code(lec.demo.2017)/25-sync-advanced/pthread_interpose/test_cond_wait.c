#include <stdio.h>
#include <pthread.h>
#include "csapp.h"

//#define DEBUG

#ifdef DEBUG
#define dbg_printf(...) {printf(__VA_ARGS__); fflush(stdout);}
#else
#define dbg_printf(...)
#endif

int item_ready = 0;
pthread_mutex_t producer_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  item_ready_cond = PTHREAD_COND_INITIALIZER;

int free_space = 1;
pthread_mutex_t consumer_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t  free_space_cond = PTHREAD_COND_INITIALIZER;

void *worker_producer(void *param)
{
    long my_id = (long) param;
    while(1)
    {
        dbg_printf("Producer trying producer_lock\n");
        pthread_mutex_lock(&producer_lock);
        dbg_printf("Producer got producer_lock\n");

            //Give up the lock and wait until there's a free space
            while(!free_space)
            {
                dbg_printf("Producer cond waiting for free space\n");
                pthread_cond_wait(&free_space_cond, &producer_lock);
                dbg_printf("Producer completed cond wait for free space\n");
            }

            //Do some production
            printf("#%ld.  Production Done! Item ready\n", my_id);
            free_space = 0;                
        dbg_printf("Producer releasing producer_lock\n");
        pthread_mutex_unlock(&producer_lock);
        dbg_printf("Producer gave up producer_lock\n");

        dbg_printf("Producer trying consumer_lock\n");
        pthread_mutex_lock(&consumer_lock); 
        dbg_printf("Producer got consumer_lock\n");
            item_ready = 1;
            dbg_printf("Producer signalling Consumer\n");
            pthread_cond_signal(&item_ready_cond);
            dbg_printf("Producer signalled Consumer\n");
        pthread_mutex_unlock(&consumer_lock);        
        dbg_printf("Producer gave up consumer_lock\n"); 
    }
    return NULL;
}

void *worker_consumer(void *param)
{        
    long my_id = (long) param;
    while(1)
    {
        dbg_printf("Consumer trying consumer_lock\n");
        pthread_mutex_lock(&consumer_lock);    
        dbg_printf("Consumer got consumer_lock\n");
            //Give up the lock and wait until there's something to be consumed
            while(!item_ready)
            {
                dbg_printf("Consumer cond waiting for item_ready\n");
                pthread_cond_wait(&item_ready_cond, &consumer_lock);
                dbg_printf("Consumer done cond waiting for item_ready\n");
            }
            //Consume what was produced
            printf("#%ld. Consumed Item! Free space left\n", my_id);
            item_ready = 0;
        dbg_printf("Consumer releasing consumer_lock\n");
        pthread_mutex_unlock(&consumer_lock); 
        dbg_printf("Consumer gave up consumer_lock\n");


        dbg_printf("Consumer trying producer_lock\n");
        pthread_mutex_lock(&producer_lock);
        dbg_printf("Consumer got producer_lock\n");
	free_space = 1;  
	dbg_printf("Consumer signalling producer\n");
	pthread_cond_signal(&free_space_cond);
	dbg_printf("Consumer signalled Producer\n");
        pthread_mutex_unlock(&producer_lock);
        dbg_printf("Consumer gave up producer_lock\n");
    }
    return NULL;
}


int main(int argc, char **argv)
{
    int m = 5;
    int n = 7;
    pthread_t producers[m], consumers[n];

    long i;

    printf("Creating producer threads\n");
    for (i = 0; i < m; i++)
	if(pthread_create(&producers[i], NULL, &worker_producer, (void *) i))
	 {
	     printf("Error creating producer thread\n");        
	 }  
    printf("Creating consumer threads\n");
    for (i = 0; i < n; i++)
	if(pthread_create(&consumers[i], NULL, &worker_consumer, (void *) i))
        {
	    printf("Error creating threadB\n");
	} 

    for (i = 0; i < m; i++)
	pthread_join(producers[i], NULL);
    for (i = 0; i < n; i++)
	pthread_join(consumers[i], NULL);    
}
