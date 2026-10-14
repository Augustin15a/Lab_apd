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

pthread_mutex_t count_mutex;


/* thread function */
void *inc_count_in_for(void *t)
{
    int my_id = *(int *)t;

    for(int i = 0; i < 1000000; i++)
    {
        pthread_mutex_lock(&count_mutex);
        count++;
        pthread_mutex_unlock(&count_mutex);
    }
    //printf("Thread %d incremented count to %d \n", my_id, count);
    return NULL;
}
void *inc_count_outside_for(void *t)
{
    int my_id = *(int *)t;

    pthread_mutex_lock(&count_mutex);
    for(int i = 0; i < 1000000; i++)
    {
        count++;
    }
    pthread_mutex_unlock(&count_mutex);
    //printf("Thread %d incremented count to %d \n", my_id, count);
    return NULL;
}

int main(int argc, char *argv[])
{

    pthread_t threads1[NUM_THREADS];
    pthread_t threads2[NUM_THREADS];
    int ids[NUM_THREADS];
    struct timespec start, finish;

    double sol1,sol2;
    /* Initialize mutex */
    pthread_mutex_init(&count_mutex, NULL);

    /* Create incrementer threads */
    //Wall-clock time inc_count_in_for
        clock_gettime(CLOCK_MONOTONIC,&start);

    for (int i = 0; i < NUM_THREADS; i++)
    {
        ids[i] = i;
        pthread_create(&threads1[i], NULL, inc_count_in_for, (void *)&ids[i]);
    }
    clock_gettime(CLOCK_MONOTONIC,&finish);

    sol2 = (finish.tv_sec - start.tv_sec);
    sol2 += (double)(finish.tv_nsec - start.tv_nsec) / 1000000000.0;

    //Wall-clock time inc_count_outside_for
    clock_gettime(CLOCK_MONOTONIC,&start);
    for (int i = 0; i < NUM_THREADS; i++)
    {
        ids[i] = i;
        pthread_create(&threads2[i], NULL, inc_count_outside_for, (void *)&ids[i]);
    }
    clock_gettime(CLOCK_MONOTONIC,&finish);
    sol1 = (finish.tv_sec - start.tv_sec);
    sol1 += (double)(finish.tv_nsec - start.tv_nsec) / 1000000000.0;

    printf("in_for : %f \noutside_for : %f\n",sol2,sol1);

    /* Wait for all threads to complete */
    for (int i = 0; i < NUM_THREADS; i++)
    {
        pthread_join(threads1[i], NULL);
        pthread_join(threads2[i], NULL);
    }

    printf("Final value of count = %d \n", count);
}
// min 973715
// max 1655700