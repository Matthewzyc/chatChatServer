#ifndef _THREAD_POOL_H_
#define _THREAD_POOL_H_

#include <vector>
#include <thread>

#include "ThreadWorker.hpp"

class ThreadPool
{
public:
    ThreadPool() {}
    ~ThreadPool() {}

    void setThreadNum(int num);
    ThreadWorker *getNextReactor();
private:
    int _thread_num;
    int _thread_index;
    std::vector<ThreadWorker> _thread_pool;
};

#endif