#ifndef _MAIN_REACTOR_H_
#define _MAIN_REACTOR_H_

#include "Epoller.hpp"

#include <thread>
#include <mutex>
#include <atomic>
#include <vector>
#include <functional>

class MainReactor
{
public:
    MainReactor();
    ~MainReactor();
    void start();
    void stop() { _stop.store(true); }
    int getReactorFd() const { return _reactor_fd; }
    void pushNewClient(int client_fd);

private:
    std::thread _thread;
    Epoller _epoller;
    int _reactor_fd;
    std::mutex _mutex;
    std::atomic<bool> _stop;
};

#endif