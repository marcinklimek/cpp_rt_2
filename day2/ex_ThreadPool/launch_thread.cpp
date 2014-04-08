#include <iostream>
#include <functional>
#include <memory>


void* thread_routine( void* ptr )
{
    std::unique_ptr< std::function<void()> > task( static_cast<std::function<void()>*>(ptr));
    (*task)();
    pthread_exit(NULL);
}


void launch_thread(pthread_t *id, std::function<void()> f)
{
    std::function<void()>* f_ptr = new std::function<void()>(f);
    int err = pthread_create(id, NULL, thread_routine, f_ptr);
    if( err == -1)
    {
        delete f_ptr;
        throw std::exception();
    }
}
