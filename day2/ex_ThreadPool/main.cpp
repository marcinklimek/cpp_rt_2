#include <iostream>
#include <pthread.h>
#include <functional>
#include <memory>

#include "threadpool.h"

using namespace std;

/*
   TODO:   zaimplementować wzorzec thread pool przy uzyciu thread_safe_queue i ew. launch_thread i bind
*/

int main()
{
    threadpool tp(2);
    tp.add_task([](){cout << "Hello from first task" << endl;});
    tp.add_task([](){cout << "Hello from second task" << endl;});

    return 0;
}

