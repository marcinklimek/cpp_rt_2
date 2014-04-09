#include "unix_socket.h"

#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <cstring>

using namespace net;

unix_socket::unix_socket( int type, int protocol)
    : base_socket( UNIX, type, protocol)
{}

unix_socket::~unix_socket()
{
    sockaddr_un addr;
    fillAddress( getLocalPath(), addr);
    ::unlink( addr.sun_path);
}

void unix_socket::connect( const std::string& foreignPath)
{
    sockaddr_un addr;
    fillAddress( foreignPath, addr);

    if( ::connect( _socket, (sockaddr*) &addr, sizeof(addr)) < 0)
        throw socket_exception("Connect failed (connect)");
}

void unix_socket::bind( const std::string& localPath)
{
    sockaddr_un addr;
    fillAddress( localPath, addr);
    ::unlink( addr.sun_path);

    if( ::bind( _socket, (sockaddr*) &addr, sizeof(addr)) < 0)
        throw socket_exception("Set of local path failed (bind)");
}

std::string unix_socket::getLocalPath() const
{
    sockaddr_un addr;
    socklen_t addr_len = sizeof(addr);

    if( getsockname( _socket, (sockaddr*) &addr, &addr_len) < 0)
        throw socket_exception("Fetch of local path failed (getsockname)");

    return extractPath( addr, addr_len);
}

std::string unix_socket::getForeignPath() const
{
    sockaddr_un addr;
    socklen_t addr_len = sizeof(addr);

    if( getpeername( _socket, (sockaddr*) &addr, &addr_len) < 0)
        throw socket_exception("Fetch of foreign path failed (getpeername)");

    return extractPath( addr, addr_len);
}

void unix_socket::fillAddress( const std::string& path, sockaddr_un& addr)
{
    // needed space is size plus null character
    if( path.size() >= sizeof(sockaddr_un::sun_path))
        throw socket_exception("Path to socket file is too long");

    addr.sun_family = AF_UNIX;
    std::strcpy( addr.sun_path, path.c_str());
}

std::string unix_socket::extractPath( const sockaddr_un& addr, socklen_t len)
{
    return std::string( addr.sun_path, len - sizeof(sa_family_t) - 1);
}
