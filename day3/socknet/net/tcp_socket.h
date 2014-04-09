#pragma once

#include "socket_handle.h"
#include "internet_socket.h"

namespace net
{
class tcp_socket : public internet_socket
{
public:

    typedef socket_handle sockhandle;

    tcp_socket();
    tcp_socket(sockhandle handle);

    int sendAll( const void* buffer, size_t len);
    void listen( int backlog = 5);

    sockhandle accept() const;
};

}
