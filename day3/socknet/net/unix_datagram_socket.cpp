#include "unix_datagram_socket.h"

#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <poll.h>

using namespace net;

unix_datagram_socket::unix_datagram_socket() : unix_socket( DATAGRAM, 0)
{
}

void unix_datagram_socket::sendTo( const void* buffer, size_t len, const std::string& foreignPath)
{
    sockaddr_un destAddr;
    fillAddress( foreignPath, destAddr);

    int sent = TEMP_FAILURE_RETRY (::sendto( _socket, (const raw_type*)buffer, len, 0, (sockaddr*)&destAddr, sizeof(destAddr)));

    // Write out the whole buffer as a single message
    if( sent != (int)len)
        throw socket_exception("Send failed (sendto)");
}

int unix_datagram_socket::receiveFrom( void* buffer, size_t len, std::string& sourcePath)
{
    sockaddr_un clientAddr;
    socklen_t addr_len = sizeof(clientAddr);

    int ret = TEMP_FAILURE_RETRY (::recvfrom( _socket, (raw_type*)buffer, len, 0, (sockaddr*)&clientAddr, &addr_len));
    if( ret < 0)
        throw socket_exception("Receive failed (recvfrom)");

    sourcePath = extractPath( clientAddr, addr_len);
    return ret;
}
