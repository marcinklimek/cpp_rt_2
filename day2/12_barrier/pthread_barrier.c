#define NUM_THREADS 10

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#define test_errno(msg) do{if (errno) {perror(msg); exit(EXIT_FAILURE);}} while(0)

pthread_barrier_t	barrier;

void* worker(void* numer)
{
    int s, status;

    s = rand() % 4 + 1;
    printf("thread #%d start, sleep for %d s\n", (int)(size_t)numer, s);

    sleep(s);

    printf("thread #%d reached barrier\n", (int)(size_t)numer);
    status = pthread_barrier_wait(&barrier);
    switch (status)
    {
    case 0: // ok
        break;

    case PTHREAD_BARRIER_SERIAL_THREAD:
        printf( "(PTHREAD_BARRIER_SERIAL_THREAD, thread #%d)\n", (int)(size_t)numer );
        break;

    default:
        fprintf(stderr, "pthread_barrier_wait: %s\n", strerror(status));
        break;
    }

    printf( "thread #%d quit \n", (int)(size_t)numer );

    return NULL;
}


int main()
{
    int i;
    pthread_t	id[NUM_THREADS];

    srand(time(NULL));

    /* init barrier for NUM_THREADS */
    errno = pthread_barrier_init(&barrier, NULL, NUM_THREADS );
    test_errno("pthread_barrier_init");

    /* create */
    for (i=0; i < NUM_THREADS ; i++)
    {
        errno = pthread_create(&id[i], NULL, worker, (void*)(size_t)i);
        test_errno("pthread_create");
    }

    /* wait */
    for (i=0; i < NUM_THREADS ; i++)
    {
        errno = pthread_join(id[i], NULL);
        test_errno("pthread_join");
    }

    printf("threads in sync\n" );

    /* destroy */
    errno = pthread_barrier_destroy(&barrier);
    test_errno("pthread_barrier_destroy");

    return EXIT_SUCCESS;
}