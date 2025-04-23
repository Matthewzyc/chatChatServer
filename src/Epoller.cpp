#include "Epoller.hpp"

#include <cstdio>

#define INIT_SIZE 20

Epoller::Epoller() : _epoll_fd(epoll_create1(0)), _events(INIT_SIZE)
{
    if (_epoll_fd == -1)
    {
        perror("epoll_create error");
    }
}

void Epoller::poll()
{
    int events_num = epoll_wait(_epoll_fd, &*_events.begin(), _events.size(), -1);

    if (events_num > 0)
    {
        for (int i = 0; i < events_num; i++)
        {
            // handler;
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

void Epoller::update(int operation, epoll_event &event)
{
    if (epoll_ctl(_epoll_fd, operation, event.data.fd, &event) == -1)
    {
        perror("epoll_ctl error");
    }
}