#pragma once

#include "base_socket.h"

struct sockaddr_in;

namespace net
{
class internet_socket : public base_socket
{
public:

    void connect( const std::string& foreignAddress, unsigned short foreignPort);
    void bind( unsigned short localPort = 0);
    void bind( const std::string& localAddress, unsigned short localPort = 0);

    std::string getLocalAddress() const;
    unsigned short getLocalPort() const;

    std::string getForeignAddress() const;
    unsigned short getForeignPort() const;

protected:
    internet_socket(int sockfd);

    internet_socket(int type, int protocol);

    static void fillAddress( const std::string& address, unsigned short port, sockaddr_in& addr);
};

}