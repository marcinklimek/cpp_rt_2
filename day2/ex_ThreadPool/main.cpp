#include <iostream>
#include <pthread.h>
#include <functional>
#include <memory>

#include "threadpool.h"


int main()
{
    threadpool tp(2);
    tp.add_task([](){std::cout << "Hello from first task" << std::endl;});
    tp.add_task([](){std::cout << "Hello from second task" << std::endl;});

    return 0;
}

