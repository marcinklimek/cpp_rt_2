#pragma once

#include <pthread.h>

class lock_guard
{
    pthread_mutex_t &lock_;

public:

    lock_guard(pthread_mutex_t &lock) : lock_(lock)
    {
        pthread_mutex_lock( &lock_ );
    }

    ~lock_guard()
    {
        pthread_mutex_unlock( &lock_ );
    }
};