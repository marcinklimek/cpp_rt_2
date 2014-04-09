#include "threadpool.h"
#include "launch_thread.h"
#include "functional"

threadpool::threadpool(unsigned int pool_size)
    : pool_size_(pool_size)
{
    threads = new pthread_t[pool_size];

    for (unsigned int i = 0 ; i < pool_size_ ; ++i)
        launch_thread(&threads[i], std::bind(&threadpool::worker, this) );
}

threadpool::~threadpool()
{
    std::cout << "Destructor called" << std::endl;
    for (unsigned int i = 0 ; i < pool_size_ ; ++i)
    {
        queue_.push(nullptr);
    }
    for (unsigned int i = 0 ; i < pool_size_ ; ++i)
    {
        pthread_join(threads[i], NULL);
    }

    delete[] threads;
}

void threadpool::add_task(task_t f)
{
    queue_.push(f);
}

void threadpool::worker()
{

    while(true)
    {
        task_t f;
        std::cout << "waiting for queue" << std::endl;
        queue_.pop(f);
        std::cout << "got smthing from queue" << std::endl;
        if (f)
        {
            f();
        }
        else
        {
            std::cout << "got empty task, finishing" << std::endl;
            break;
        }
    }
    pthread_exit(NULL);
}
