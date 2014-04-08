#ifndef _THREAD_SAFE_Q_H
#define _THREAD_SAFE_Q_H

#include <pthread.h>
#include <queue>

template<typename T>
class thread_safe_queue
{
    std::queue<T> q;
    pthread_mutex_t mutex_;


public:
    thread_safe_queue()
    {
        pthread_mutexattr_t mutex_attr;
        pthread_mutexattr_init( &mutex_attr );

        pthread_mutexattr_settype( &mutex_attr, PTHREAD_MUTEX_RECURSIVE );

        pthread_mutex_init( &mutex_, &mutex_attr);

        pthread_mutexattr_destroy(&mutex_attr);
    }

    void pop(T& item)
    {
        item = q.front();
        q.pop();
    }

    bool try_pop(T& item)
    {
        if (is_empty())
            return false;

        item = q.front();
        q.pop();

        return true;
    }

    void push(T item)
    {
        q.push(item);
    }

    bool is_empty()
    {
        return q.empty();
    }
};

#endif