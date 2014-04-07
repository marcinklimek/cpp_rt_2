#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include <pthread.h>

#define test_errno(msg) do{if (errno) {perror(msg); exit(EXIT_FAILURE);}} while(0)

pthread_t id;
pthread_mutex_t mutex;
pthread_mutexattr_t mutexattr;

void* watek(void* _arg)
{
    int errno;

    // 1
    puts("before pthread_mutex_lock (1)");
    errno = pthread_mutex_lock(&mutex);
    test_errno("pthread_mutex_lock (1)");
    puts("after pthread_mutex_lock (1)");

    // 2
    puts("before pthread_mutex_lock (2)");
    errno = pthread_mutex_lock(&mutex);
    test_errno("pthread_mutex_lock (2)");
    puts("after pthread_mutex_lock (2)");

    // 3
    puts("before pthread_mutex_unlock (2)");
    errno = pthread_mutex_unlock(&mutex);
    test_errno("pthread_mutex_unlock (2)");
    puts("after pthread_mutex_unlock (2)");

    // 4
    puts("before pthread_mutex_unlock (1)");
    errno = pthread_mutex_unlock(&mutex);
    test_errno("pthread_mutex_unlock (1)");
    puts("after pthread_mutex_unlock (1)");

    return NULL;
}
//------------------------------------------------------------------------

int main(int argc, char* argv[])
{
    int errno;

    pthread_mutexattr_init(&mutexattr);

    if (argc > 1)
    {
        switch (atoi(argv[1]))
        {
        case 1:
            puts("mutex typu PTHREAD_MUTEX_ERRORCHECK");
            pthread_mutexattr_settype(&mutexattr, PTHREAD_MUTEX_ERRORCHECK);
            break;
        case 2:
            puts("mutex typu PTHREAD_MUTEX_RECURSIVE");
            pthread_mutexattr_settype(&mutexattr, PTHREAD_MUTEX_RECURSIVE);
            break;
        default:
            puts("mutex typu PTHREAD_MUTEX_NORMAL");
            pthread_mutexattr_settype(&mutexattr, PTHREAD_MUTEX_NORMAL);
            break;
        }
    }
    else
    {
        printf("usage: program [0 - normal | 1 - errorcheck | 2 - recursive ]");
        return EXIT_FAILURE;
    }

    /* init */
    errno = pthread_mutex_init(&mutex, &mutexattr);
    test_errno("pthread_mutex_init");

    /* create */
    errno = pthread_create(&id, NULL, watek, NULL);
    test_errno("pthread_create");

    /* wait */
    pthread_join(id, NULL);
    test_errno("pthread_join");
}