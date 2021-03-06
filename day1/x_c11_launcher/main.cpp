#include <iostream>
#include <functional>
#include <memory>

#include "launch_thread.h"

void run(int x, int &ret)
{
    std::cout << "from thread " << x << std::endl;

    ret = 42;
}

class Foo
{
    int x_;
public:

    Foo(int x) : x_(x)
    {

    }

    void operator() ()
    {
        std::cout << "from foo" << std::endl;
    }
};


int main()
{
    int value = 10;
    pthread_t id;

    launch_thread(&id, std::bind(run, 10, std::ref(value) ));
    pthread_join(id, NULL);
    std::cout << value << std::endl;

    pthread_t id2;

    Foo f(10);
    launch_thread(&id2, f);
    
    pthread_join(id2, NULL);

    pthread_t id3;

    auto func = []() { std::cout << "from lambda" <<std::endl; };

    launch_thread(&id3, func );
    pthread_join(id3, NULL);

    return 0;
}

