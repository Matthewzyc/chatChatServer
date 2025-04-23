#include "SocketOps.hpp"

#include <sys/socket.h>
#include <unistd.h>
#include <cstdio>

#define MAX_QUEUE_SIZE 1024

SocketOps::SocketOps(int server_fd) : _server_fd(server_fd) {}

SocketOps::~SocketOps()
{
    close(_server_fd);
}

bool SocketOps::bind(const InetAddress &server_addr)
{
    int ret = ::bind(_server_fd, (sockaddr *)server_addr.getAddress(), sizeof(sockaddr_in));
    if (ret == -1)
    {
        perror("bind error");
        return false;
    }
    return true;
}

bool SocketOps::listen()
{
    int ret = ::listen(_server_fd, MAX_QUEUE_SIZE);
    if (ret == -1)
    {
        perror("listen error");
        return false;
    }
    return true;
}

int SocketOps::accept(const InetAddress &client_addr)
{
    sockaddr_in addr;
    socklen_t len = sizeof(addr);
    int client_fd = ::accept4(_server_fd, (sockaddr *)&addr, &len, SOCK_NONBLOCK);
    if (client_fd == -1)
    {
        perror("accept error");
        return -1;
    }
    return client_fd;
}