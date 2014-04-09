#include "tcp_socket.h"
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <poll.h>

using namespace net;

tcp_socket::tcp_socket()
    : internet_socket( STREAM, IPPROTO_TCP)
{}

tcp_socket::tcp_socket( sockhandle handle)
    : internet_socket( handle )
{}

int tcp_socket::sendAll( const void* buffer, size_t len)
{
    size_t sent = 0;
    while( sent != len)
    {
        const char* buf = static_cast<const char*>(buffer) + sent;
        int ret = send( buf, len - sent);
        if( ret < 0) return ret;
        sent += static_cast<unsigned>(ret);
    }
    return sent;
}

void tcp_socket::listen( int backlog /* = 5 */)
{
    int ret = ::listen( _socket, backlog);
    if( ret < 0)
        throw socket_exception("listen failed, most likely another socket is already listening on the same port");
}

tcp_socket::sockhandle tcp_socket::accept() const
{
    int ret = ::accept( _socket, 0, 0);
    if( ret < 0)
        throw socket_exception("TCPSocket::accept failed");
    return sockhandle(ret);
}
