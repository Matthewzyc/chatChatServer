#ifndef _EPOLLER_H_
#define _EPOLLER_H_

#include <sys/epoll.h>
#include <vector>
#include <mutex>
#include <functional>

class Epoller
{
public:
    explicit Epoller();
    ~Epoller();

    void init();

    void poll();
    void add(epoll_event &event);
    void mod(epoll_event &event);
    void del(epoll_event &event);
    void registerNewFd();

    void distributor(const int &event_fd);
    void setMessageHandler(const std::function<void(const int &event_fd)> &handler);
    int getEventFd() const { return _event_fd; }
    void pushNewFd(int fd);

private:
    void update(int operation, epoll_event &event);
private:
    const int _epoll_fd;
    int _event_fd;
    std::vector<epoll_event> _events;
    std::vector<int> _new_fd;
    std::mutex _mtx;
    std::function<void(const int &event_fd, const int &reactor_fd)> _distributor;
    std::function<void(const int &event_fd)> _messageHandler;
};

#endif