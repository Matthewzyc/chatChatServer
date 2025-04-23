#ifndef _INET_ADDRESS_H_
#define _INET_ADDRESS_H_

#include <netinet/in.h>
#include <string>

class InetAddress
{
public:
    InetAddress(std::string ip = "127.0.0.1", uint16_t port = 8080);
    void setAddress(const sockaddr_in &address);
    const sockaddr_in *getAddress() const;
    uint16_t getPort() const;
    std::string getIp() const;
    std::string getIpPort() const;
    
private:
    sockaddr_in _address;
    uint16_t _port;
    std::string _ip;
};

#endif