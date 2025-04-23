#ifndef _SOCKET_OPS_H_
#define _SOCKET_OPS_H_

#include "InetAddress.hpp"

class SocketOps
{
public:
    explicit SocketOps(int server_fd = -1);
    ~SocketOps();
    bool bind(const InetAddress &server_addr);
    bool listen();
    int accept(const InetAddress &client_addr);

private:
    const int _server_fd;
};

#endif