#include "base_socket.h"

#include <poll.h>
#include <unistd.h>
#include <cstring>
#include <cerrno>
#include <unistd.h>
#include <errno.h>

using namespace net;

socket_exception::socket_exception( const std::string& message, bool use_strerror) throw()
    : _message(message)
    , _errorcode(0)
{
    _errorcode = errno;
    _message += ": ";

    if ( use_strerror )
        _message += strerror(_errorcode);
}

base_socket::base_socket( int domain, int type, int protocol)
    : _peerDisconnected(false)
{
    if( (_socket = ::socket( domain, type, protocol)) < 0)
        throw socket_exception("Socket creation failed (socket)");
}

base_socket::base_socket( int sockfd)
    : _socket(sockfd)
    , _peerDisconnected(false)
{
    if(sockfd < 0)
        throw socket_exception("Tried to initialize Socket with invalid Handle");
}

base_socket::~base_socket()
{
    TEMP_FAILURE_RETRY (::close(_socket));
}

int base_socket::send( const void* buffer, size_t len)
{
    int sent = TEMP_FAILURE_RETRY (::send( _socket, (const raw_type*) buffer, len, 0));
    if( sent < 0)
    {
        switch(errno)
        {
        case ECONNRESET:
        case ECONNREFUSED:
            _peerDisconnected = true;
            break;
        default:
            throw socket_exception("Send failed (send)");
        }
    }
    return sent;
}

int base_socket::receive( void* buffer, size_t len)
{
    int ret = TEMP_FAILURE_RETRY (::recv( _socket, (raw_type*) buffer, len, 0));
    if( ret < 0) throw socket_exception("Received failed (receive)");
    return ret;
}

void base_socket::disconnect()
{
    sockaddr addr;
    std::memset( &addr, 0, sizeof(addr));
    addr.sa_family = AF_UNSPEC;

    if( ::connect( _socket, &addr, sizeof(addr)) < 0)
    {
        if( errno != ECONNRESET)
            throw socket_exception("Disconnect failed (connect)");

        _peerDisconnected = false;
    }
}

void base_socket::shutdown()
{
    if( ::shutdown( _socket, SHUT_RDWR) < 0)
        throw socket_exception("Shutdown failed (shutdown)");
}

bool base_socket::peerDisconnected() const
{
    return _peerDisconnected;
}
