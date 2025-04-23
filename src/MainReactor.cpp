#include "MainReactor.hpp"

MainReactor::MainReactor()
{
    _reactor_fd = _epoller.getEventFd();
    _stop.store(false);
}

MainReactor::~MainReactor()
{
    if (_thread.joinable())
    {
        _thread.join();
    }
}

void MainReactor::start()
{
    _thread = std::thread([this]() {
        while (!_stop.load())
        {
            _epoller.poll();
        }
    });
}

void MainReactor::pushNewClient(int client_fd)
{
    std::lock_guard<std::mutex> lock(_mutex);
    _epoller.pushNewFd(client_fd);
}
