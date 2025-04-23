#ifndef _SERVER_H_
#define _SERVER_H_

#include "SocketOps.hpp"
#include "ReactorPool.hpp"

#include <sys/epoll.h>

class Server
{
public:
    Server();
    ~Server();

    void setSubReactorNum(int num);
    void start();

private:
    int _epoll_fd;
    int _server_fd;
    epoll_event _events[1024];
    SocketOps _operator;
    ReactorPool _pool;
};

#endif