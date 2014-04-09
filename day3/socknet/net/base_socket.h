#pragma once

#include <sys/socket.h>
#include <string>
#include <exception>

namespace net
{

typedef void raw_type;

class socket_exception : public std::exception
{
public:
    socket_exception(const std::string& message , bool use_strerror=true) throw();
    ~socket_exception() throw() {}

    const char* what() const throw()
    {
        return _message.c_str();
    }

    int errorCode() const throw()
    {
        return _errorcode;
    }

private:
    std::string _message;
    int _errorcode;
};


class base_socket
{
public:

    virtual ~base_socket();

    int send( const void* buffer, size_t len);
    int receive( void* buffer, size_t len);
    void disconnect();

    void shutdown();
    bool peerDisconnected() const;

    int getfd() { return _socket; }

protected:

    enum socket_domain
    {
        INTERNET = PF_INET,
        UNIX = PF_UNIX,
    };

    enum socket_type
    {
        STREAM = SOCK_STREAM,
        DATAGRAM = SOCK_DGRAM
    };

    base_socket( int sockfd);
    base_socket( int domain, int type, int protocol);

    // socket descriptor
    int _socket;
    bool _peerDisconnected;

private:

    base_socket( const base_socket&);
    const base_socket& operator=( const base_socket&);
};

}
