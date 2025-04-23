#include "Epoller.hpp"

#include <cstdio>
#include <sys/eventfd.h>
#include <unistd.h>

#define INIT_SIZE 20

Epoller::Epoller() : _epoll_fd(epoll_create1(0)), _event_fd(eventfd(0, EFD_NONBLOCK)), _events(INIT_SIZE)
{
    if (_epoll_fd == -1)
    {
        perror("epoll_create1 error");
    }

    if (_event_fd == -1)
    {
        perror("eventfd error");
    }

    init();
}

Epoller::~Epoller()
{
    close(_epoll_fd);
    close(_event_fd);
}

void Epoller::init()
{
    _event_fd = eventfd(0, EFD_NONBLOCK);
    if (_event_fd == -1)
    {
        perror("eventfd error");
    }
    epoll_event ev;
    ev.events = EPOLLIN | EPOLLET;
    ev.data.fd = _event_fd;
    add(ev);
}

void Epoller::poll()
{
    int events_num = epoll_wait(_epoll_fd, &*_events.begin(), _events.size(), -1);

    if (events_num > 0)
    {
        for (int i = 0; i < events_num; i++)
        {
            distributor(_events[i].data.fd);
        }
    }
    else if (events_num == -1)
    {
        perror("epoll_wait error");
    }
}

void Epoller::add(epoll_event &event)
{
    update(EPOLL_CTL_ADD, event);
}

void Epoller::mod(epoll_event &event)
{
    update(EPOLL_CTL_MOD, event);
}

void Epoller::del(epoll_event &event)
{
    update(EPOLL_CTL_DEL, event);
}

void Epoller::registerNewFd()
{
    std::vector<int> tmp_fd;
    {
        std::lock_guard<std::mutex> lock(_mtx);
        tmp_fd.swap(_new_fd);
        _new_fd.clear();
    }

    for (int fd : tmp_fd)
    {
        epoll_event ev;
        ev.events = EPOLLIN | EPOLLET | EPOLLRDHUP;
        ev.data.fd = fd;
        add(ev);
    }
}

void Epoller::pushNewFd(int fd)
{
    std::lock_guard<std::mutex> lock(_mtx);
    _new_fd.push_back(fd);
}  

void Epoller::update(int operation, epoll_event &event)
{
    if (epoll_ctl(_epoll_fd, operation, event.data.fd, &event) == -1)
    {
        perror("epoll_ctl error");
    }
    else
    {
        printf("epoll_ctl success\n");
    }
}

void Epoller::distributor(const int &event_fd)
{
    if (event_fd == _event_fd)
    {
        registerNewFd();
    }
    else
    {
        _messageHandler(event_fd);
    }
}

void Epoller::setMessageHandler(const std::function<void(const int &event_fd)> &handler)
{
    _messageHandler = std::move(handler);
}