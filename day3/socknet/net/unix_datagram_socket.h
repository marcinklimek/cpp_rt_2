#pragma once

#include "unix_socket.h"

namespace net
{
class unix_datagram_socket : public unix_socket
{
public:

    unix_datagram_socket();

    void sendTo( const void* buffer, size_t len, const std::string& foreignPath);
    int receiveFrom( void* buffer, size_t len, std::string& sourcePath);
};

}