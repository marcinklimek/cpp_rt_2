#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <sched.h>
#include <errno.h>
#include <unistd.h>
#include <limits.h>


#define test_errno(msg) do{if (errno) {perror(msg); exit(EXIT_FAILURE);}} while(0)
#define N 4 /* num threads */



typedef struct
{
    int  counter;
    char interrupt;
    int  priority;
} Arg;

char stack[8192];

void display_attr()
{
    int x;
    int policy;
    size_t size;
    void* addr;
    struct sched_param param;

    pthread_attr_t* attr = new pthread_attr_t;
    //pthread_getattr_np(pthread_self(), attr);
    pthread_attr_init(attr);

    printf("* detach state: ");
    errno = pthread_attr_getdetachstate(attr, &x);
    test_errno("pthread_attr_getdetachstate");

    switch (x)
    {
    case PTHREAD_CREATE_JOINABLE:
        printf("joinable\n");
        break;
    case PTHREAD_CREATE_DETACHED:
        printf("detached\n");
        break;
    default:
        printf("???\n");
    }

    errno = pthread_attr_getstacksize(attr, &size);
    test_errno("pthread attr getstacksize");
    printf("* stack size: %zu (min %d)\n", size, PTHREAD_STACK_MIN);

    errno = pthread_attr_getguardsize(attr, &size);
    test_errno("pthread_attr_getguardsize");
    printf("* guard size: %zu\n", size);

    errno = pthread_getschedparam(pthread_self(), &policy, &param);
    test_errno("pthread_attr_getschedpolicy");

    printf("* policy: ");
    switch (policy)
    {
    case SCHED_OTHER:
        printf(" SCHED_OTHER\n");
        break;
    case SCHED_RR:
        printf(" SCHED_RR\n");
        break;
    case SCHED_FIFO:
        printf(" SCHED_FIFO\n");
        break;
    default:
        printf(" ???\n");
    }

    printf("* priority: %d\n", param.sched_priority);

    errno = pthread_attr_getscope(attr, &x);
    test_errno("pthread_attr_getscope");

    printf("* scope: ");
    switch (x)
    {
    case PTHREAD_SCOPE_PROCESS:
        printf("proces\n");
        break;
    case PTHREAD_SCOPE_SYSTEM:
        printf("system\n");
        break;
    default:
        printf("???\n");
    }

    pthread_attr_destroy(attr);
    delete attr;
}

void* worker_thread(void* _arg)
{
    Arg *arg = (Arg*)_arg;

    display_attr();

    sleep(3);

    arg->counter = 0;
    while (!arg->interrupt)
    {
        arg->counter += 1;

        printf("worker_thread priority: %d, counter: %d\n", arg->priority, arg->counter);

        sleep(1);
    }

    return NULL;
}

int main(int argc, char* argv[])
{
    Arg arg[N];
    pthread_t id[N];
    pthread_attr_t attr;
    
    int pmin, pmax;
    int i, sched_policy;
    struct sched_param sp;

    sched_policy = SCHED_FIFO; // SCHED_RR SCHED_FIFO

    pmin = sched_get_priority_min(sched_policy);
    pmax = sched_get_priority_max(sched_policy);

    printf("priority %d ... %d\n", pmin, pmax);

    errno = pthread_attr_init(&attr);
    test_errno("pthread_attr_init");

    errno = pthread_attr_setinheritsched(&attr, PTHREAD_EXPLICIT_SCHED);
    test_errno("pthread_attr_setinheritsched");

    /* wybór podanego algorytmu szeregowania */
    errno = pthread_attr_setschedpolicy(&attr, sched_policy);
    test_errno("pthread_attr_setschedpolicy");

    for (i=0; i < N; i++)
    {
        /* kolejne wątki mają coraz wyższe priorityy */
        pthread_attr_getschedparam(&attr, &sp);
        sp.sched_priority = sched_get_priority_min(sched_policy)+i;
        errno = pthread_attr_setschedparam(&attr, &sp);
        test_errno("pthread_attr_setschedparam");

        arg[i].interrupt = 0;
        arg[i].counter   = 0;
        arg[i].priority  = sp.sched_priority;

        /* uruchomienie wątku */
        errno = pthread_create(&id[i], &attr, worker_thread, &arg[i]);
        test_errno("pthread_create");

        printf("thread  #%d o priorytecie %d\n", i, arg[i].priority);
    }

    errno = pthread_attr_destroy(&attr);
    test_errno("pthread_attr_destroy");

    /* oczekiwanie */
    sleep(10);

    /* ustawienie flagi zakończenia pracy, którą testują funkcje wątków
       oraz odczyt ich bieżących counterów */
    for (i=0; i < N; i++)
    {
        arg[i].interrupt = 1;
        printf("wątek #%d (priority %3d): counter = %d\n",
               i,
               arg[i].priority,
               arg[i].counter
              );
    }

    /* teraz oczekiwanie na ich zakończenie */
    for (i=0; i < N; i++)
    {
        errno = pthread_join(id[i], NULL);
        test_errno("pthread_join");
    }

    return EXIT_SUCCESS;
}
//---------------------------------------------------------
