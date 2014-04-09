#include "internet_socket.h"

#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <cerrno>
#include <cstring>

using namespace net;

internet_socket::internet_socket( int type, int protocol)
    : base_socket( INTERNET, type, protocol)
{}

internet_socket::internet_socket( int sockfd)
    : base_socket(sockfd) {}

void internet_socket::connect( const std::string& foreignAddress, unsigned short foreignPort)
{
    sockaddr_in addr;
    fillAddress( foreignAddress, foreignPort, addr);

    if( ::connect( _socket, (sockaddr*) &addr, sizeof(addr)) < 0)
        throw socket_exception("Connect failed (connect)");

    _peerDisconnected = false;
}

void internet_socket::bind( unsigned short localPort /* = 0 */)
{
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(localPort);

    if( ::bind( _socket, (sockaddr*) &addr, sizeof(addr)) < 0)
        throw socket_exception("Set of local port failed (bind)");
}

void internet_socket::bind( const std::string& localAddress, unsigned short localPort /* = 0 */)
{
    sockaddr_in addr;
    fillAddress( localAddress, localPort, addr);

    if( ::bind( _socket, (sockaddr*) &addr, sizeof(addr)) < 0)
        throw socket_exception("Set of local address and port failed (bind)");
}

std::string internet_socket::getLocalAddress() const
{
    sockaddr_in addr;
    socklen_t addr_len = sizeof(addr);

    if( getsockname( _socket, (sockaddr*) &addr, &addr_len) < 0)
        throw socket_exception("Fetch of local address failed (getsockname)");

    return inet_ntoa( addr.sin_addr);
}

unsigned short internet_socket::getLocalPort() const
{
    sockaddr_in addr;
    socklen_t addr_len = sizeof(addr);

    if( getsockname( _socket, (sockaddr*) &addr, &addr_len) < 0)
        throw socket_exception("Fetch of local port failed (getsockname)");

    return ntohs( addr.sin_port);
}

std::string internet_socket::getForeignAddress() const
{
    sockaddr_in addr;
    socklen_t addr_len = sizeof(addr);

    if( getpeername( _socket, (sockaddr*) &addr, &addr_len) < 0)
        throw socket_exception("Fetch of foreign address failed (getpeername)");

    return inet_ntoa( addr.sin_addr);
}

unsigned short internet_socket::getForeignPort() const
{
    sockaddr_in addr;
    socklen_t addr_len = sizeof(addr);

    if( getpeername( _socket, (sockaddr*) &addr, &addr_len) < 0)
        throw socket_exception("Fetch of foreign port failed (getpeername)");

    return ntohs( addr.sin_port);
}

void internet_socket::fillAddress( const std::string& address, unsigned short port, sockaddr_in& addr)
{
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);

    // Assume we have a simple ipv4 address
    if( inet_aton( address.c_str(), &addr.sin_addr)) return;

    // We need to resolve the address
    hostent* host = gethostbyname( address.c_str());
    if( host == 0)
    {
        // strerror() will not work for gethostbyname()
        throw socket_exception("Failed to resolve address (gethostbyname)");
    }
    addr.sin_addr.s_addr = *reinterpret_cast<uint32_t*>( host->h_addr);
}
