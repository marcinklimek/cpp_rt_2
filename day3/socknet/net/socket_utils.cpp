#include "socket_utils.h"
#include "base_socket.h"

#include <sys/socket.h>
#include <sys/ioctl.h>
#include <arpa/inet.h>
#include <unistd.h>

#include <cstring>
#include <cstdlib>
#include <sstream>
#include <iomanip>
#include <netdb.h>

using namespace net;

namespace
{
class sockaddr_ptr
{
public:
    sockaddr_ptr( const sockaddr* sa) : msa(*sa) {}
    const sockaddr_in& operator*() const
    {
        return msi;
    }
    const sockaddr_in* operator->() const
    {
        return &msi;
    }

private:
    union
    {
        sockaddr msa;
        sockaddr_in msi;
    };
};
}

std::string net::resolveHostname( const std::string& hostname)
{
    hostent* host = gethostbyname( hostname.c_str());

    if( host == 0)
    {
        // strerror() will not work for gethostbyname()
        throw socket_exception("Failed to resolve address (gethostbyname)", false);
    }

    return std::string(host->h_addr);
}

uint16_t net::resolveService( const std::string& service, const std::string& protocol)
{
    struct servent* serv;
    if(protocol == "")
        serv = getservbyname( service.c_str(), 0);
    else
        serv = getservbyname( service.c_str(), protocol.c_str());

    if(serv)
        return ntohs( static_cast<uint16_t>(serv->s_port));
    return 0;
}
