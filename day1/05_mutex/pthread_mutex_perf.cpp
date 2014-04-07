#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <errno.h>
#include <sched.h>

#define test_errno(msg) do{if (errno) {perror(msg); exit(EXIT_FAILURE);}} while(0)


#define	N 10	/* num threads */
#define K 10000	/* num iterations */

long licznik = 0;		// global counter

pthread_mutex_t blokada = PTHREAD_MUTEX_INITIALIZER;


class lock_guard
{
    pthread_mutex_t &lock_;

public:

    lock_guard(pthread_mutex_t &lock) : lock_(lock)
    {
        std::cout << "lock" << std::endl;
        pthread_mutex_lock( &lock_ );
    }


    ~lock_guard()
    {
        std::cout << "unlock" << std::endl;
        pthread_mutex_unlock( &lock_ );
    }
};


void* worker(void* numer)
{
    int i;
    int j;

    for (i=0; i < K; i++)
    {
        lock_guard lk(blokada);

        licznik = licznik + 1;

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

    pthread_mutex_destroy( &blokada );

    return EXIT_SUCCESS;
}
//------------------------------------------------------------------------
