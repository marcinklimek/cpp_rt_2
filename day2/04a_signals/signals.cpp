#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>

#define test_errno(msg) do{if (errno) {perror(msg); exit(EXIT_FAILURE);}} while(0)

pthread_t main_id;  // id głównego wątek

void bt_sighandler(int sig, siginfo_t *info, void *context)
{
    printf("bt_sighandler 0x%lx\n", pthread_self());
}


void* worker(void* args)
{
    printf("thread start\n");
    printf("thread 0x%lx\n", pthread_self());
    sleep(1);

    printf("send SIGUSR1 to main\n");
    errno = pthread_kill(main_id, SIGUSR1);
    test_errno("pthread_kill");

    return NULL;
}

int main()
{
    pthread_t id;
    int signum;
    sigset_t mask;

    struct sigaction sa;
    
    sa.sa_sigaction = bt_sighandler;
    sigemptyset (&sa.sa_mask);
    sa.sa_flags = SA_RESTART | SA_SIGINFO;
    
    sigaction(SIGRTMIN, &sa, NULL);

    printf("main 0x%lx\n", pthread_self());

    // SIGUSR1, SIGRTMIN
    sigemptyset(&mask);
    sigaddset(&mask, SIGUSR1);
    sigaddset(&mask, SIGRTMIN);

    errno = pthread_sigmask(SIG_BLOCK, &mask, NULL);
    test_errno("pthread_kill");

    // thread_id
    main_id = pthread_self();

    errno = pthread_create(&id, NULL, worker, NULL);
    test_errno("pthread_create");

    printf("wait for signal\n");
    sigwait(&mask, &signum);
    test_errno("sigwait");
    
    if (signum == SIGUSR1)
        printf("received SIGUSR1\n");
    else
        printf("received %d\n", signum);

  
    pthread_join(id, NULL);

    return EXIT_SUCCESS;
}
//------------------------------------------------------------------------