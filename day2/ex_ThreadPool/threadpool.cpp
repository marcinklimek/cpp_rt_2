#include "threadpool.h"

threadpool::threadpool(unsigned int pool_size)
    : pool_size_(pool_size)
{
}

threadpool::~threadpool()
{
}

void threadpool::add_task(task_t f)
{
}

void *worker(void *ptr)
{
}
