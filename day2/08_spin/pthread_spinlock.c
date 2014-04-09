#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <errno.h>

#define test_errno(msg) {if (errno) {perror(msg); exit(EXIT_FAILURE);}}

#define	NUM_THREADS 25
#define NUM_ITER 100000	

pthread_mutex_t 	mutex;
pthread_spinlock_t	spinlock;
int counter = 0;

void ms_sleep(unsigned ms)
{
    struct timespec req;
    req.tv_sec  = (ms / 1000);
    req.tv_nsec = (ms % 1000 * 1000000);
    nanosleep(&req, NULL);
}
//------------------------------------------------------------------------

void* thread(void* numer)
{
    int i;
    for (i=0; i < NUM_ITER; i++)
    {
#ifdef MUTEX
        pthread_mutex_lock(&mutex);
#else
        pthread_spin_lock(&spinlock);
#endif
        
        counter = counter + 1;

#ifdef MUTEX
        pthread_mutex_unlock(&mutex);
#else
        pthread_spin_unlock(&spinlock);
#endif

    }

    return NULL;
}
//------------------------------------------------------------------------

int main()
{
    pthread_t id[NUM_THREADS];
    int i;

    printf("counter = %d\n", counter);

    errno = pthread_mutex_init(&mutex, NULL);
    test_errno("pthread_mutex_init");

    errno = pthread_spin_init(&spinlock, PTHREAD_PROCESS_PRIVATE);
    test_errno("pthread_spin_init");

   
    for (i=0; i < NUM_THREADS; i++)
    {
        errno = pthread_create(&id[i], NULL, thread, NULL);
        test_errno("pthread_create");
    }


    for (i=0; i < NUM_THREADS; i++)
    {
        errno = pthread_join(id[i], NULL);
        test_errno("pthread_join");
    }

    printf("counter = %d, expected value = %d %s\n",
           counter,
           NUM_THREADS*NUM_ITER,
           (counter != NUM_THREADS*NUM_ITER ? "error!" : "ok")
          );

    return EXIT_SUCCESS;
}
//------------------------------------------------------------------------
