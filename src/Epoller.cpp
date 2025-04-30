#include "Epoller.hpp"

#include <cstdio>
#include <sys/eventfd.h>
#include <unistd.h>

// for test begin
#include <sys/timerfd.h>
#include <sys/socket.h>
#include <thread>
// for test end

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
    _mysql.connect();
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
        // for test begin
        forTest(event_fd);
        // for test end
    }
}

void Epoller::setMessageHandler(const std::function<void(const int &event_fd)> &handler)
{
    _messageHandler = std::move(handler);
}

// for test begin
void Epoller::forTest(const int &client_fd)
{
    // 读取请求（ET模式必须循环读取）
    char buffer[4096];
    ssize_t n;
    bool close_conn = false;

    while ((n = recv(client_fd, buffer, sizeof(buffer), 0)) > 0)
    {
        // 简单处理HTTP请求（实际需要解析）
        // std::cout << "Received " << n << " bytes from fd " << client_fd << std::endl;
    }

    if (n == 0 || (n < 0 && errno != EAGAIN))
    {
        close_conn = true;
    }

    //_mysql.insert("INSERT INTO user(name, age) VALUES('Dlice', 40)");
    // _mysql.remove("DELETE FROM user WHERE age = 30");
    // _mysql.update("UPDATE user SET age = 50 WHERE name = 'Alice'");
    MYSQL_RES *res = _mysql.query("SELECT age FROM user WHERE name = 'Dlice'");

    // 发送响应
    if (!close_conn)
    {
        // std::this_thread::sleep_for(std::chrono::milliseconds(13));
        const char response[] =
            "HTTP/1.1 200 OK\r\n"
            "Content-Length: 12\r\n"
            "Connection: close\r\n"
            "\r\n"
            "Hello World!";
        send(client_fd, response, sizeof(response) - 1, MSG_NOSIGNAL);
    }

    // 关闭连接
    close(client_fd);
}
// for test end