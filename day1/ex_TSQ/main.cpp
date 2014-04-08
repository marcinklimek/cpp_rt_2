#include <iostream>
#include <vector>
#include "pthread.h"
#include <functional>

#include "launch_thread.h"
#include "thread_safe_queue.hpp"

const int NUM_ITERATIONS = 20000;
const int NUM_CONSUMERS = 5;

void producer(thread_safe_queue<int> &q, unsigned int num_iterations, unsigned int num_consumers)
{
    std::cout << "producer" << std::endl;

    for(int i = 0; i < num_iterations; ++i)
        q.push(i);

    for(int i = 0; i < num_consumers; ++i)
        q.push(-1);
}

void consumer(thread_safe_queue<int> &q)
{
    int value;
    while(true)
    {
        q.pop(value);
        if (value == -1)
        {
            std::cout << "consumer quits" << std::endl;
            break;
        }
        else
        {
            std::cout << "pop: " << value << std::endl;
        }
    }
}

/*

    TODO: 
          1. zaimplementować wielowątkowo bezpieczną kolejkę ( interface w thread_safe_queue.hpp)
          2. uruchomić producenta i konsumentow

*/

int main(int argc, char **argv) 
{
    thread_safe_queue<int> q;

    q.push(10);
    int i;
    q.pop(i);
    std::cout << "val: " << i << std::endl;

    pthread_t tid;
    //  producer( q, NUM_ITERATIONS, NUM_CONSUMERS);
    launch_thread( &tid, std::bind( producer, std::ref(q), NUM_ITERATIONS, NUM_CONSUMERS  )  );

    for(int i = 0; i < NUM_CONSUMERS; ++i)
        consumer( q );

}
