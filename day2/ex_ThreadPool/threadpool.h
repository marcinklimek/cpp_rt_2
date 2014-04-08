#pragma once
#include <iostream>
#include <pthread.h>
#include <functional>
#include "thread_safe_queue.h"

typedef std::function<void()> task_t;

class threadpool
{
public:
    threadpool(unsigned int pool_size);
    ~threadpool();
    void add_task( task_t f);
};
