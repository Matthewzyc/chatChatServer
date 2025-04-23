#include "SubReactor.hpp"

#include <iostream>

SubReactor::SubReactor()
{
    _reactor_fd = _epoller.getEventFd();
    _stop.store(false);
}

SubReactor::~SubReactor()
{
    if (_thread.joinable())
    {
        _thread.join();
    }
}

void SubReactor::start()
{
    _thread = std::thread([this]() {
        while (!_stop.load())
        {
            _epoller.poll();
        }
    });
    std::cout << "SubReactor start" << std::endl;
}

void SubReactor::pushNewClient(int client_fd)
{
    _epoller.pushNewFd(client_fd);
}
