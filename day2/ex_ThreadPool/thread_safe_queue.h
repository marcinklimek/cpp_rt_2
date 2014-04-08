#include <pthread.h>
#include <queue>
#include "lock_guard.h"

template<typename T>
class thread_safe_queue
{
    std::queue<T> q;
    pthread_mutex_t mtx;
    pthread_cond_t cond;

public:
    thread_safe_queue()
    {
        pthread_mutexattr_t mutexattr;
        pthread_mutexattr_init(&mutexattr);
        pthread_mutexattr_settype(&mutexattr, PTHREAD_MUTEX_RECURSIVE);
        pthread_mutex_init(&mtx, &mutexattr);
        pthread_mutexattr_destroy(&mutexattr);

        pthread_cond_init(&cond, NULL);
    }

    ~thread_safe_queue()
    {
        pthread_mutex_destroy(&mtx);
        pthread_cond_destroy(&cond);
    }

    void pop(T& item)
    {
        lock_guard l(mtx);

        while (is_empty())
        {
            pthread_cond_wait(&cond, &mtx);
        }
        item = q.front();
        q.pop();
    }

    bool try_pop(T& item)
    {
        lock_guard l(mtx);

        if (is_empty())
            return false;

        item = q.front();
        q.pop();

        return true;
    }

    void push(T item)
    {
        {
        lock_guard l(mtx);
        q.push(item);
        }

        pthread_cond_signal(&cond);
    }

    bool is_empty()
    {
        lock_guard l(mtx);

        return q.empty();
    }
};
