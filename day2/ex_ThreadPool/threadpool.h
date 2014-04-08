#pragma once
#include <iostream>
#include <pthread.h>
#include <functional>
#include "thread_safe_queue.h"

void * worker(void * ptr);
typedef std::function<void()> task_t;

class threadpool
{
    unsigned int pool_size_;
    pthread_t *threads;
    thread_safe_queue<task_t> queue_;

    void worker();

public:

    threadpool(unsigned int pool_size);
    ~threadpool();

    void add_task( task_t f);
};
