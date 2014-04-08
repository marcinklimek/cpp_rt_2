#pragma once

#include <functional>

void launch_thread(pthread_t *id, std::function<void()> f);
void launch_thread(std::function<void()> f);
