#include <iostream>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>

using namespace std;

struct thrad_data
{

};


void* worker(void* arg)
{
    int* value = (int*)arg;
    std::cout << "worker - argument " << *value << std::endl;

    *value = 42;

    pthread_exit((void*)42);

    //return (void*)42;
}


int main()
{
    pthread_t tid;
    int value = 0;

    int err = pthread_create(&tid, NULL, worker, (void*)&value );

    if ( err < 0 )
    {
        std::cout << strerror( err ) << std::endl;
        exit(1);
    }


    int return_value;
    pthread_join(tid, (void**)&return_value);

    std::cout << "main - ret value " << value << " " << return_value << std::endl;

    return 0;
}

