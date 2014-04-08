#include <iostream>
#include <pthread.h>
#include "futures.h"
#include <memory>
#include <unistd.h>

using namespace std;

int answer()
{
    sleep(1);
    return 42;
}

int main()
{
    cout << "Hello World!" << endl;
    pthread_t thd;
    launch_thread(&thd, [](){cout << "hello from thread" << endl;});
    pthread_join(thd, NULL);
    //[]() -> int {return 42;}
    future<int> res = async<int>(&answer );
    future<int> res2 = async<int>( []() -> int {return 666;} );
    cout << res.get() << endl;
    cout << res2.get() << endl;
    return 0;
}

