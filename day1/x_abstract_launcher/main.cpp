#include <iostream>
#include <pthread.h>
#include <functional>
#include <memory>

class abstract_launcher
{
public:
    virtual ~abstract_launcher() {}
    virtual void run() = 0;
};

template<class F>
class launcher : public abstract_launcher
{
    F f_;
public:
    launcher(F f) : f_(f)  {}
    void run()
    {
        f_();
    }
};

void * thread_routine( void * ptr_to_functor)
{
    std::auto_ptr<abstract_launcher> l(static_cast<abstract_launcher* >(ptr_to_functor));
    l->run();
    pthread_exit(NULL);
}

template<class F>
int launch_thread( pthread_t &id, F f )
{
    std::auto_ptr<abstract_launcher> p( new launcher<F>(f));
    int err = pthread_create(&id, NULL, &thread_routine, p.get());
    if (err == 0)
    {
        p.release();
    }
}

using namespace std;

int main()
{
    cout << "Hello World!" << endl;
    pthread_t id;
    launch_thread(id, []() { cout << "Hello from thread" << endl;});
    pthread_join(id, NULL);
    return 0;
}

