#ifndef _SERVER_H_
#define _SERVER_H_

#include "ThreadPool.hpp"

class Server
{
public:
    Server() {}
    ~Server() {}

    void setSubReactorNum(int num);
    void start();
private:
    ThreadPool _pool;
};

#endif