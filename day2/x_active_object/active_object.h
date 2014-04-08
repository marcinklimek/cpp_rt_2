#ifndef ACTIVE_OBJECT_H
#define ACTIVE_OBJECT_H
#include <string>
#include <iostream>
#include <functional>
#include <chrono>
#include <ctime>
#include "thread_safe_queue.h"
#include <pthread.h>
#include <unistd.h>
#include "launch_thread.h"

class Logger
{
public:
    Logger() {};
    void message(std::string msg)
    {
        std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        std::string time = std::ctime(&now);
        time.erase(time.find('\n', 0),1);
        sleep(1);
        std::cout << time;
        std::cout << " >> " << msg << std::endl;
    }
};

// ActiveObject - implementation

typedef std::function<void()> task_t;

class ActiveObject
{
    thread_safe_queue<task_t> mq;
    pthread_t ao_worker;

    void worker()
    {
        while(true)
        {
            task_t f;
            mq.pop(f);

            if (f)
                f();
            else
                break;
        }
    }

public:
    ActiveObject()
    {
        launch_thread(&ao_worker, std::bind(&ActiveObject::worker, this) );
    }

    ~ActiveObject(){
        mq.push(nullptr);
        pthread_join(ao_worker, NULL);
    }

    void Send(task_t t)
    {
        mq.push(t);
    }

    ActiveObject(const ActiveObject &) = delete;  // no copying
    void operator=(const ActiveObject&) = delete; // no copying

};

class LoggerAsync
{
    ActiveObject ao;
public:
    LoggerAsync() {};
    void message(std::string msg)
    {
        std::time_t now = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
        std::string time = std::ctime(&now);
        time.erase(time.find('\n', 0),1);
        ao.Send( [time, msg]() {
            sleep(1);
            std::cout << time;
            std::cout << " >> " << msg << std::endl;
        } );
    }

};

#endif // ACTIVE_OBJECT_H
