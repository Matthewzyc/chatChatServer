#include "Server.hpp"

void Server::setSubReactorNum(int num)
{
    _pool.setThreadNum(num);
}