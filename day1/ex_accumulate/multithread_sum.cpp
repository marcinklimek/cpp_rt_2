#include <iostream>
#include <pthread.h>
#include <chrono>

using namespace std;

const unsigned long N =  900000;

void one_thread_sum(long* begin, long* end)
{
    auto tstart = chrono::high_resolution_clock::now();
    unsigned long sum = 0;

    for(; begin < end; ++begin)
        sum += *begin;

    auto tend = chrono::high_resolution_clock::now();
    float micros = chrono::duration_cast<chrono::microseconds>(tend-tstart).count();
    std::cout << "Total time: " << micros << " us" << std::endl;
    std::cout << "Sum: " << sum << std::endl;
}

// multithreaded version

struct range_t
{
    long* start;
    long* end;
    unsigned long partial_sum;
};

void* partial_sum(void *rng)
{
    unsigned long sum;
    range_t *range = (range_t*)rng;
    for(; range->start < range->end; ++range->start)
        range->partial_sum += *range->start;
    pthread_exit(NULL);
}

void multithread_sum(long* begin, long* end)
{
    auto tstart = chrono::high_resolution_clock::now();
    unsigned long sum = 0;

    const unsigned int N = 4;
    pthread_t threads[N];
    range_t ranges[N];

    size_t block_size = (end-begin)/4;
    long* block_start = begin;
    for (unsigned int i = 0 ; i < N-1 ; ++i)
    {
        long* block_end = block_start;
        block_end += block_size;
        ranges[i].start = block_start;
        ranges[i].end = block_end;
        ranges[i].partial_sum = 0;
        block_start = block_end;
    }
    ranges[N-1].start = block_start;
    ranges[N-1].end = end;
    ranges[N-1].partial_sum = 0;

    for (int i = 0 ; i < N ; ++i)
    {
        pthread_create(&threads[i], NULL, partial_sum, (void*)&ranges[i]);
    }
    for (int i = 0 ; i < N ; ++i)
    {
        pthread_join(threads[i], NULL);
        sum += ranges[i].partial_sum;
    }

    auto tend = chrono::high_resolution_clock::now();
    float micros = chrono::duration_cast<chrono::microseconds>(tend-tstart).count();
    std::cout << "Total time: " << micros << " us" << std::endl;
    std::cout << "Sum: " << sum << std::endl;
}

int main()
{
    long *table;
    table = new long[N];
    for (size_t i = 0 ; i < N ; ++i)
        table[i] = i;

    std::cout << N << std::endl;
    one_thread_sum(table, table+N);
    std::cout << "\n-----------------------------\n" << std::endl;
    multithread_sum(table, table+N);
    std::cout << "\n-----------------------------\n" << std::endl;
}
