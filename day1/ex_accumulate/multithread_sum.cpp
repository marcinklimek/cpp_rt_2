#include <iostream>
#include <pthread.h>
#include <chrono>

using namespace std;

const unsigned long N =  90000000;

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
void multithread_sum(long* begin, long* end)
{
    auto tstart = chrono::high_resolution_clock::now();

    // TODO: SUM

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
