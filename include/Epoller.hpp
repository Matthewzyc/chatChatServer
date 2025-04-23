#ifndef _EPOLLER_H_
#define _EPOLLER_H_

#include <sys/epoll.h>
#include <vector>
#include <functional>

class Epoller
{
public:
    Epoller();
    ~Epoller();
    void poll();
    void add(epoll_event &event);
    void mod(epoll_event &event);
    void del(epoll_event &event);
    void setEventHandler(const std::function<void(int event_fd)> &handler);

private:
    void update(int operation, epoll_event &event);
private:
    int _epoll_fd;
    std::vector<epoll_event> _events;
    std::function<void(int event_fd)> _eventHandler;
};

#endif