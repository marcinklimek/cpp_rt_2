#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <errno.h>

#define test_errno(msg) do{if (errno) {perror(msg); exit(EXIT_FAILURE);}} while(0)

void thread_cleanup(void* num)
{
    printf("cleanup thread #%d\n", (int)(size_t)num);
}
//------------------------------------------------------------------------

void* thread_01(void* num)
{
    int i, n;

    pthread_cleanup_push(thread_cleanup, num);

    errno = pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    test_errno("pthread_setcancelstate");

    errno = pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
    test_errno("pthread_setcanceltype");

    printf("thread #%d (PTHREAD_CANCEL_ASYNCHRONOUS)\n", (int)(size_t)num);
    while (1)
    {
        n = 1000000;
        for (i=0; i < n; i++);
    }

    pthread_cleanup_pop(1);

    return NULL;
}
//------------------------------------------------------------------------

void* thread_02(void* num)
{
    int i, n;
    pthread_cleanup_push(thread_cleanup, num);

    errno = pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    test_errno("pthread_setcancelstate");

    errno = pthread_setcanceltype(PTHREAD_CANCEL_DEFERRED, NULL);
    test_errno("pthread_setcanceltype");

    printf("thread #%d (PTHREAD_CANCEL_DEFERRED)\n", (int)(size_t)num);
    while (1)
    {
        n = 1000000;
        for (i=0; i < n; i++);

        pthread_testcancel();   // interruption point
    }

    pthread_cleanup_pop(1);
    return NULL;
}
//------------------------------------------------------------------------

void* thread_03(void* num)
{
    pthread_cleanup_push(thread_cleanup, num);

    errno = pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
    test_errno("pthread_setcancelstate");

    printf("thread #%d - cancel disabled for 2s\n", (int)(size_t)num);

    sleep(2);

    printf("thread #%d - cancel enabled\n", (int)(size_t)num);
    errno = pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    test_errno("pthread_setcancelstate");
    pause();

    pthread_cleanup_pop(1);
    return NULL;
}
//------------------------------------------------------------------------

void interrupt(pthread_t id, const char* napi)
{
    struct timespec tim;
    tim.tv_sec = 0;
    tim.tv_nsec = 5000000;

    nanosleep(&tim, NULL);

    printf("%s: interrupt start\n", napis);
    errno = pthread_cancel(id);
    test_errno("pthread_cancel");

    printf("%s: wating for thread \n", napis);
    errno = pthread_join(id, NULL);
    test_errno("pthread_join");

    printf("%s: interrupt end\n", napis);
}
//------------------------------------------------------------------------

int main(int argc, char* argv[])
{
    pthread_t id;

    if (argc < 1)
        exit(1);


    switch( atoi(argv[1]) )
    {
    case 1:
        // async
        errno = pthread_create(&id, NULL, thread_01, (void*)(size_t)(0));
        test_errno("pthread_create (1)");

        interrupt(id, "#0");
        break;

    case 2:
        // deffered
        errno = pthread_create(&id, NULL, thread_02, (void*)(size_t)(1));
        test_errno("pthread_create (2)");

        interrupt(id, "#1");
        break;

    default:
        // disable enable
        errno = pthread_create(&id, NULL, thread_03, (void*)(size_t)(2));
        test_errno("pthread_create (3)");

        interrupt(id, "#2");
    }

    return EXIT_SUCCESS;
}
//------------------------------------------------------------------------
