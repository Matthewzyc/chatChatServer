#include "InetAddress.hpp"

#include <arpa/inet.h>
#include <string.h>

InetAddress::InetAddress(std::string ip, uint16_t port) : _ip(ip), _port(port)
{
    bzero(&_address, sizeof(_address));
    _address.sin_family = AF_INET;
    _address.sin_port = htons(port);
    _address.sin_addr.s_addr = inet_addr(ip.c_str());
}

void InetAddress::setAddress(const sockaddr_in &address)
{
    _address = address;
    _ip = inet_ntoa(_address.sin_addr);
    _port = ntohs(_address.sin_port);
}

const sockaddr_in *InetAddress::getAddress() const
{
    return &_address;
}

uint16_t InetAddress::getPort() const
{
    return _port;
}

std::string InetAddress::getIp() const
{
    return _ip;
}

std::string InetAddress::getIpPort() const
{
    return _ip + ":" + std::to_string(_port);
}

