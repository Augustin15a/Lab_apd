/* File:
 *    wrong_counter.c
 * It illustrates the absence of Mutexes for threads synchronization
 * Main creates a number of NUM_THREADS=2 threads
 * There is a shared variable count, initialized with 0
 * The two threads both increment count without any form of mutual exclusion
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define NUM_THREADS 2

/* shared variable */
int count = 0;

/* thread function */
void *inc_count(void *t)
{
    int i;
    int my_id = *(int *)t;

    for(int i = 0; i < 1000000; i++)
        count++;
    printf("Thread %d incremented count to %d \n", my_id, count);
    return NULL;
}

int main(int argc, char *argv[])
{

    pthread_t threads[NUM_THREADS];
    int ids[NUM_THREADS];

    /* Create incrementer threads */
    for (int i = 0; i < NUM_THREADS; i++)
    {
        ids[i] = i;
        pthread_create(&threads[i], NULL, inc_count, (void *)&ids[i]);
    }

    /* Wait for all threads to complete */
    for (int i = 0; i < NUM_THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }

    printf("Final value of count = %d \n", count);
}