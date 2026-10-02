 /**
 * Note the difference between measuring wall-clock time and CPU time!
 * 
 * clock_gettime() - measures wallclock time or CPUtime, depending on first argument used
 * clock_gettime(CLOCK_MONOTONIC, &t)
 * clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &t); 
 * 
 * clock() - not a portable solution! measures CPU time on unix and wall-clock on windows 
 */
#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>

/* size of dummy workload for threads*/
#define MAXWORK 20000

/* number of working threads */
#define NTHREADS 4

/* thread function 1 - does some dummy counting */
void *pth_fct1(void *dummy)
{
    int count = 0;
    for (int i = 0; i < MAXWORK; i++)
        for (int j = 0; j < MAXWORK; j++)
            count++;
    return NULL;
} /* pth_fct1 */

/* solution 1 - spawns threads executing pth_fct1 */
void solution1(void)
{
    long thread;
    pthread_t *thread_handles;

    thread_handles = malloc(NTHREADS * sizeof(pthread_t));

    for (thread = 0; thread < NTHREADS; thread++)
        pthread_create(&thread_handles[thread], NULL,
                       pth_fct1, NULL);

    for (thread = 0; thread < NTHREADS; thread++)
        pthread_join(thread_handles[thread], NULL);

    free(thread_handles);
}

/* thread function 1 - does dummy counting  AND sleeps in between*/
void *pth_fct2(void *dummy)
{
    int count = 0;

    for (int i = 0; i < MAXWORK; i++)
    {
        if (i % 2000 == 0)
            sleep(1);
        for (int j = 0; j < MAXWORK; j++)
            count++;
    }

    return NULL;
} /* pth_fct2 */

/* solution 2 - spawns threads executing pth_fct1 */
void solution2(void)
{
    long thread;
    pthread_t *thread_handles;

    thread_handles = malloc(NTHREADS * sizeof(pthread_t));

    for (thread = 0; thread < NTHREADS; thread++)
        pthread_create(&thread_handles[thread], NULL,
                       pth_fct2, NULL);

    for (thread = 0; thread < NTHREADS; thread++)
        pthread_join(thread_handles[thread], NULL);

    free(thread_handles);
}

int main(void)
{
    struct timespec start, finish;

    double TIME1, TIME2,TIME3,TIME4,TIME5,TIME6;
    // Experiment1: Measuring Wall-clock time uses clock_gettime  CLOCK_MONOTONIC
        //Wall-clock time solution 1
            clock_gettime(CLOCK_MONOTONIC,&start);
            solution1();
            clock_gettime(CLOCK_MONOTONIC,&finish);

            TIME1 = (finish.tv_sec - start.tv_sec);
            TIME1 += (double)(finish.tv_nsec - start.tv_nsec) / 1000000000.0;
        //Wall-clock time solution 2
            clock_gettime(CLOCK_MONOTONIC,&start);
            solution2();
            clock_gettime(CLOCK_MONOTONIC,&finish);

            TIME2 = (finish.tv_sec - start.tv_sec);
            TIME2 += (double)(finish.tv_nsec - start.tv_nsec) / 1000000000.0;

        printf("Experiment1: Measuring Wall-clock time uses clock_gettime  CLOCK_MONOTONIC\n");
        printf("        Wall-clock time - Solution 1: %fs\n", TIME1);
        printf("        Wall-clock time - Solution 2: %fs\n", TIME2);
        
    //Experiment2: Measuring CPU time PER PROCESS (all threads) uses clock_gettime CLOCK_PROCESS_CPUTIME_ID
        //CPU time solution 1
                clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&start);
                solution1();
                clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&finish);

                TIME3 = (finish.tv_sec - start.tv_sec);
                TIME3 += (double)(finish.tv_nsec - start.tv_nsec) / 1000000000.0;
        //CPU time solution 2
                clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&start);
                solution2();
                clock_gettime(CLOCK_PROCESS_CPUTIME_ID,&finish);

                TIME4 = (finish.tv_sec - start.tv_sec);
                TIME4 += (double)(finish.tv_nsec - start.tv_nsec) / 1000000000.0;

        printf("Experiment2: Measuring CPU time PER PROCESS (all threads) uses clock_gettime CLOCK_PROCESS_CPUTIME_ID\n");
        printf("        CPU time - Solution 1: %fs\n", TIME3);
        printf("        CPU time - Solution 2: %fs\n", TIME4);
    
    //Experiment3: using clock() CPU time on unix/linuxwall-clock time on windows
        clock_t start_clock, finish_clock;  
         //CPU time solution 1
                start_clock = clock();
                solution1();
                finish_clock = clock(); 

                TIME5 = (finish_clock - start_clock) / (double)CLOCKS_PER_SEC;
        //CPU time solution 2
                start_clock = clock();
                solution2();
                finish_clock = clock(); 

                TIME6 = (finish_clock - start_clock) / (double)CLOCKS_PER_SEC;

        printf("Experiment3: using clock() CPU time on unix/linuxwall-clock time on windows\n");
        printf("        CPU time using clock() - Solution 1: %fs\n", TIME5);
        printf("        CPU time using clock() - Solution 2: %fs\n", TIME6);

    return 0;
}