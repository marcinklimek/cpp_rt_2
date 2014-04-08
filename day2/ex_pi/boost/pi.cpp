#include <iostream> 
#include <cstdlib> 
#include <ctime> 
#include <iomanip>
#include "boost/thread.hpp"
#include <omp.h>

using namespace std;


double darts(int num_darts) 
{ 
    int i, p=0;
    for (i = 1; i <= num_darts; i++)
    {
        double x, y;
        x = static_cast< double >( rand() )/RAND_MAX;
        y = static_cast< double >( rand() )/RAND_MAX;
        if ( (x*x + y*y) <= 1.0)
            p++;
    }
    return 4.0*static_cast< double >( p )/num_darts;
}

void darts_threaded(int num_darts, int& result) 
{ 
    int i, p=0;

    for (i = 1; i <= num_darts; i++)
    {
        double x, y;
        x = static_cast< double >( rand() )/RAND_MAX;
        y = static_cast< double >( rand() )/RAND_MAX;
        if ( (x*x + y*y) <= 1.0)
            p++;
    }

    result = p;
}


void calculate_serial(int num_darts, int seed)
{
    srand(seed);

    boost::posix_time::ptime start = boost::posix_time::microsec_clock::local_time();

    double PI = darts(num_darts);
    boost::posix_time::ptime stop = boost::posix_time::microsec_clock::local_time();

    cout << "For " << num_darts << " darts thrown the value of PI is " << PI << " time " << (stop - start).total_milliseconds() << endl;
}

void calculate_threaded(int num_darts, int seed)
{

    int num_threads = boost::thread::hardware_concurrency();

    srand(seed);
    std::vector<int> results( num_threads );
    boost::posix_time::ptime start = boost::posix_time::microsec_clock::local_time();

    boost::thread_group grp;

    for(int i=0; i<num_threads; i++)
        grp.create_thread(boost::bind(darts_threaded, num_darts/num_threads, boost::ref(results[i])));

    grp.join_all();

    int result = std::accumulate(results.begin(), results.end(), 0);

    double PI = 4.0*static_cast< double >( result )/num_darts;

    //stop  = omp_get_wtime();
    boost::posix_time::ptime stop = boost::posix_time::microsec_clock::local_time();
    cout << "For " << num_darts << " darts thrown the value of PI is " << PI << " time " << (stop - start).total_milliseconds() << endl;
}


int main() 
{ 
    int num_darts = 10000000;
    int seed = time(NULL);

    calculate_serial(num_darts, seed);
    calculate_threaded(num_darts, seed);
}
