#include <iostream> 
#include <cstdlib> 
#include <iomanip>
#include <pthread.h>
#include <chrono>
#include <functional>
#include <memory>
#include <vector>
#include <algorithm>
#include <random>

using namespace std;

class RandGen
{
    int range_;
    random_device* rd;
    mt19937 *gen;
public:

    RandGen(int range) : range_(range)
    {
        rd = new random_device;
        gen = new mt19937((*rd)());
    }

    int operator()() const
    {
        uniform_int_distribution<> dis(0, range_);
        return dis(*gen);
    }
};

void *thread_routine( void * ptr_to_functor)
{
    std::unique_ptr<std::function<void()>> task(static_cast<std::function<void()>*>(ptr_to_functor));
    (*task)();
    pthread_exit(NULL);
}

void launch_thread(pthread_t *id, std::function<void()> f)
{
    std::function<void()> *f_ptr = new std::function<void()>(f); // copy constructor
    int err = pthread_create(id, NULL, thread_routine, f_ptr);
    if (err == -1)
    {
        delete f_ptr;
        throw std::exception();
    }
}

//-------------

void darts(int num_darts, int& result, RandGen& rg)
{ 
    int i, p=0;

    for (i = 1; i <= num_darts; i++)
    {
        double x, y;
        x = static_cast< double >( rg() )/RAND_MAX;
        y = static_cast< double >( rg() )/RAND_MAX;
        if ( (x*x + y*y) <= 1.0)
            p++;
    }

    result = p;
}


void calculate_serial(int num_darts, int seed)
{
    RandGen rg(RAND_MAX);

    auto tstart = chrono::high_resolution_clock::now();


    int result;
    darts(num_darts, result, rg);
    double pi =  4.0*static_cast< double >( result )/num_darts;

    auto tend = chrono::high_resolution_clock::now();
    float milis = chrono::duration_cast<chrono::milliseconds>(tend-tstart).count();
    std::cout << "Serial total time: " << milis << " ms" << std::endl;
    cout << "For " << num_darts << " darts thrown the value of PI is " << pi << std::endl;
}

void calculate_threaded(int num_darts, int seed)
{
    int num_threads = 2;

    RandGen rg(RAND_MAX);

    vector<int> results( num_threads );
    vector<pthread_t> ids(num_threads);

    auto tstart = chrono::high_resolution_clock::now();

    for(int i=0; i<num_threads; i++)
        launch_thread(&ids[i], std::bind(darts, num_darts/num_threads, std::ref(results[i]), std::ref(rg) ) );

    for(int i=0; i<num_threads; i++)
        pthread_join(ids[i], NULL);

    int result = std::accumulate(results.begin(), results.end(), 0);

    double pi = 4.0*static_cast< double >( result )/num_darts;

    auto tend = chrono::high_resolution_clock::now();

    float milis = chrono::duration_cast<chrono::milliseconds>(tend-tstart).count();
    std::cout << "Threaded total time: " << milis << " ms" << std::endl;
    cout << "For " << num_darts << " darts thrown the value of PI is " << pi << std::endl;
}


int main() 
{ 
    int num_darts = 100000000;
    int seed = time(NULL);

    calculate_threaded(num_darts, seed);
    calculate_serial(num_darts, seed);
}
