#include <iostream>
#include <pthread.h>
#include <queue>
#include <unistd.h>

using namespace std;


pthread_cond_t  cond = PTHREAD_COND_INITIALIZER;
pthread_mutex_t cond_mutex = PTHREAD_MUTEX_INITIALIZER;

std::queue<int> q;

void* th_producent(void* arg)
{
    int i=0;

    while( true )
    {
        pthread_mutex_lock( &cond_mutex );
        q.push( i++ );

        std::cout << "Producer " << std:: endl;

        pthread_mutex_unlock( &cond_mutex );

        pthread_cond_signal( &cond );
        sleep(1);
    }
}

void* th_consumer(void* arg)
{

    while( true )
    {
        pthread_mutex_lock( &cond_mutex );

        while ( q.size() == 0 )
        {
            std::cout << "Queue empty - wait" << std::endl;

            pthread_cond_wait( &cond, &cond_mutex );
            //
        }

        int message = q.front();
        q.pop();

        std::cout << "Consumer - " << message << std::endl;

        pthread_mutex_unlock( &cond_mutex );
        sleep(2);
    }
}



int main()
{

    pthread_t tid_prod;
    pthread_t tid_cons;

    pthread_create(&tid_prod, NULL, th_producent, NULL);
    pthread_create(&tid_cons, NULL, th_consumer, NULL);


    pthread_join(tid_prod, NULL);
    pthread_join(tid_cons, NULL);

    return 0;
}

