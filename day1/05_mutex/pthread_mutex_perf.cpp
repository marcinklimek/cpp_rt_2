#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <errno.h>
#include <sched.h>

#define test_errno(msg) do{if (errno) {perror(msg); exit(EXIT_FAILURE);}} while(0)


#define	N 10	/* num threads */
#define K 10000	/* num iterations */

long licznik = 0;		// global counter

pthread_mutex_t blokada;


void* worker(void* numer)
{
    int i;
    int j;

    for (i=0; i < K; i++)
    {
        pthread_mutex_lock(&blokada);
        licznik = licznik + 1;
        pthread_mutex_unlock(&blokada);

        sched_yield();
    }

    return NULL;
}
//------------------------------------------------------------------------

int main()
{
    pthread_t id[N];
    long i;
    
    errno = pthread_mutex_init( &blokada, NULL );
    test_errno("pthread mutex init");



    for (i=0; i < N; i++)
    {
        errno = pthread_create(&id[i], NULL, worker, (void*)i);
        test_errno("pthread_create");
    }

    /* wait for finish */
    for (i=0; i < N; i++)
    {
        errno = pthread_join(id[i], NULL);
        test_errno("pthread_join");
    }

    printf("counter = %li, expected = %d %s\n",
           licznik,
           N*K,
           (licznik != N*K ? "error!" : "ok")
          );

    return EXIT_SUCCESS;
}
//------------------------------------------------------------------------
