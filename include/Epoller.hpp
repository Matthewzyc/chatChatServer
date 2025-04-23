#ifndef _EPOLLER_H_
#define _EPOLLER_H_

#include <sys/epoll.h>
#include <vector>

class Epoller
{
public:
    Epoller();
    ~Epoller();
    void poll();
    void add(epoll_event &event);
    void mod(epoll_event &event);
    void del(epoll_event &event);

private:
    void update(int operation, epoll_event &event);
private:
    int _epoll_fd;
    std::vector<epoll_event> _events;
};

#endif