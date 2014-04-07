#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <errno.h>

#define test_errno(msg) do{if (errno) {perror(msg); exit(EXIT_FAILURE);}} while(0)


void recursive_worker(int counter, int limit)
{
    int i;

    for (i=0; i < counter; i++) 
    	putchar(' ');

    printf("counter = %d, limit = %d\n", counter, limit);

    if (counter == limit)
        pthread_exit(NULL); /* terminate thread */
    else
        recursive_worker(counter+1, limit);

    printf("finished\n");
}
//------------------------------------------------------------------------

void* thread(void* arg)
{
    recursive_worker(0, 5);
    return NULL;
}
//------------------------------------------------------------------------

int main()
{
    pthread_t id;

    /* create */
    errno = pthread_create(&id, NULL, thread, NULL);
    test_errno("pthread_create");

    /* wait */
    errno = pthread_join(id, NULL);
    test_errno("pthread_join");

    return EXIT_SUCCESS;
}
//------------------------------------------------------------------------
