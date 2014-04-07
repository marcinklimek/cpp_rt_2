#ifndef _THREAD_SAFE_Q_H
#define _THREAD_SAFE_Q_H

#include <pthread.h>
#include <queue>

template<typename T>
class thread_safe_queue
{
    std::queue<T> q;

public:
    thread_safe_queue()
    {
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