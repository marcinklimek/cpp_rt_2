#ifndef _LAUNCH_THREAD_H
#define _LAUNCH_THREAD_H

#include <iostream>
#include <functional>
#include <memory>

void launch_thread(pthread_t *id, std::function<void()> f);

#endif