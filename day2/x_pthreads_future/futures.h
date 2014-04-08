#ifndef FUTURES_H
#define FUTURES_H
#include <iostream>
#include <functional>
#include <memory>

void *thread_routine( void * ptr_to_functor);
void launch_thread(pthread_t *id, std::function<void()> f);

template <typename T>
class ptask
{
    T& result;
    bool done;
    std::function<T()> task;
public:
    ptask(std::function<T()> task, T& result)
        : result(result), task(task), done(false)
    {
        std::cout << "ptask: constr" << std::endl;
    }

    void operator()()
    {
        std::cout << "ptask: calling " << std::endl;
        result = task();
        done = true;
        std::cout << "ptask: after calling " << std::endl;
    }
};

template <typename T>
class future
{
    pthread_t id;
    T result;
    ptask<T> task_;
public:
    future(std::function<T()> task) : task_(ptask<T>(task, result))
    {
        launch_thread(&id, std::move(task_));
    }

    ~future()
    {
        std::cout << "future: destructor" << std::endl;
        pthread_join(id, NULL);
    }

    T get()
    {
        pthread_join(id, NULL);
        return result;
    }
};

template <typename T>
future<T> async( std::function<T()> f)
{
    return future<T>(f);
}


#endif // FUTURES_H
