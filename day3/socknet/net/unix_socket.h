#pragma once

#include "base_socket.h"

struct sockaddr_un;

namespace net
{
class unix_socket : public base_socket
{
public:

    ~unix_socket();

    void connect( const std::string& foreignPath);
    void bind( const std::string& localPath);

    std::string getLocalPath() const;
    std::string getForeignPath() const;

protected:
    unix_socket( int type, int protocol);

    static void fillAddress( const std::string& path, sockaddr_un& addr);
    static std::string extractPath( const sockaddr_un& addr, socklen_t len);
};

}
