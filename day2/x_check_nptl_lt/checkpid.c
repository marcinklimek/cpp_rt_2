#include <pthread.h>
#include <unistd.h>   /* for getpid() */
#include <stdio.h>
#include <malloc.h>

void* _obtain_thread_pid(void* arg)
{
    long *exit_status = malloc(sizeof(long));
    (void) arg; /* keep compiler quiet. */
    *exit_status = getpid();
    return exit_status;
}
int main()
{
    pthread_t thread;
    void *status_ptr;
    long *thread_pid;
    long process_pid = getpid();
    /* a well behaved application tests the return value of
    ** pthread_create to ensure there is no error. */
    pthread_create(&thread, NULL, &_obtain_thread_pid, NULL);
    pthread_join(thread, &status_ptr);
    thread_pid = (long*)status_ptr;
    if (*thread_pid != process_pid)
        printf("This is linked to the LinuxThreads library.\n");
    else
        printf("This is linked to the NPTL library.\n");
    return 0;
}