#pragma once
#include <pthread.h>
#include <queue>

template<typename T>
class thread_safe_queue
{
    std::queue<T> q;
    pthread_mutex_t mtx;
    pthread_cond_t cond;

public:
    thread_safe_queue()
    {
        pthread_mutex_init(&mtx, NULL);
        pthread_cond_init(&cond, NULL);
    }

    void pop(T& item)
    {
        pthread_mutex_lock(&mtx);
        while (is_empty())
        {
            pthread_cond_wait(&cond, &mtx);
        }
        item = q.front();
        q.pop();
        pthread_mutex_unlock(&mtx);
    }

    bool try_pop(T& item)
    {
        pthread_mutex_lock(&mtx);
        if (is_empty())
        {
            return false;
        }
        item = q.front();
        q.pop();
        pthread_mutex_unlock(&mtx);
        return true;
    }

    void push(T item)
    {
        pthread_mutex_lock(&mtx);
        q.push(item);
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mtx);
    }

    bool is_empty()
    {
        return q.empty();
    }
};
