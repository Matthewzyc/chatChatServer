#include "Server.hpp"

int main()
{
    Server server;
    server.setSubReactorNum(4);
    server.start();
    return 0;
}