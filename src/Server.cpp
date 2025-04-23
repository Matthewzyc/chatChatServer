#include "Server.hpp"

#include <unistd.h>

Server::Server() : _server_fd(socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK, 0)), _operator(_server_fd)
{
    _epoll_fd = epoll_create1(0);
    _operator.bind(InetAddress("127.0.0.1", 8080));
}

Server::~Server()
{
    close(_server_fd);
}

void Server::setSubReactorNum(int num)
{
    _pool.setPoolSize(num);
}

void Server::start()
{
    _pool.init();
    _pool.start();
    epoll_event ev;
    
    ev.events = EPOLLIN;
    ev.data.fd = _server_fd;
    epoll_ctl(_epoll_fd, EPOLL_CTL_ADD, _server_fd, &ev);

    _operator.listen();
    size_t current = 0;
    while (true)
    {
        int n = epoll_wait(_epoll_fd, _events, 10, -1);
        for (int i = 0; i < n; ++i)
        {
            if (_events[i].data.fd == _server_fd)
            {
                // 接受新连接
                sockaddr_in client_addr;
                socklen_t addr_len = sizeof(client_addr);
                int client_fd;
                while ((client_fd = accept4(_server_fd, (sockaddr *)&client_addr,
                                            &addr_len, SOCK_NONBLOCK)) > 0)
                {
                    // 轮询分发到子Reactor
                    auto sub_reactor = _pool.getNextReactor();
                    sub_reactor->pushNewClient(client_fd);
                    uint64_t u = 1;
                    int ret = write(sub_reactor->getReactorFd(), &u, sizeof(u));
                    if (ret == -1)  
                    {
                        perror("write error");
                    }
                }
                if (client_fd < 0 && errno != EAGAIN)
                {
                    perror("accept failed");
                }
            }
        }
    }
}