#include "udp_socket.h"
#include <unistd.h>
#include <errno.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <poll.h>

using namespace net;

namespace
{

void setBroadcast( int socket)
{
    int broadcastPermission = 1;
    setsockopt( socket,
                SOL_SOCKET,
                SO_BROADCAST,
                (raw_type*)&broadcastPermission,
                sizeof(broadcastPermission));
}

int groupAction( int socket, const std::string& multicastGroup, int action)
{
    struct ip_mreq multicastRequest;
    multicastRequest.imr_multiaddr.s_addr = inet_addr( multicastGroup.c_str());
    multicastRequest.imr_interface.s_addr = htonl(INADDR_ANY);

    return( setsockopt( socket,
                        IPPROTO_IP,
                        action,
                        (raw_type*)&multicastRequest,
                        sizeof(multicastRequest)));
}

} // namespace

udp_socket::udp_socket()
    : internet_socket( DATAGRAM, IPPROTO_UDP)
{
    setBroadcast(_socket);
}

void udp_socket::sendTo( const void* buffer, size_t len, const std::string& foreignAddress, unsigned short foreignPort)
{
    sockaddr_in destAddr;
    fillAddress( foreignAddress, foreignPort, destAddr);

    int sent = TEMP_FAILURE_RETRY (::sendto( _socket, (const raw_type*)buffer, len, 0, (sockaddr*)&destAddr, sizeof(destAddr)));

    // Write out the whole buffer as a single message
    if( sent != (int)len)
        throw socket_exception("Send failed (sendto)");
}

int udp_socket::receiveFrom( void* buffer, size_t len, std::string& sourceAddress, unsigned short& sourcePort)
{
    sockaddr_in clientAddr;
    socklen_t addrLen = sizeof(clientAddr);

    int ret = TEMP_FAILURE_RETRY (::recvfrom( _socket, (raw_type*)buffer, len, 0, (sockaddr*)&clientAddr, &addrLen));
    if( ret < 0)
        throw socket_exception("Receive failed (recvfrom)");

    sourceAddress = inet_ntoa( clientAddr.sin_addr);
    sourcePort = ntohs( clientAddr.sin_port);

    return ret;
}

void udp_socket::setMulticastTTL( unsigned char multicastTTL)
{
    if( setsockopt( _socket,
                    IPPROTO_IP,
                    IP_MULTICAST_TTL,
                    (raw_type*)&multicastTTL,
                    sizeof(multicastTTL)) < 0)
        throw socket_exception("Multicast set TTL failed (setsockopt)");
}

void udp_socket::joinGroup( const std::string& multicastGroup)
{
    if( groupAction( _socket, multicastGroup, IP_ADD_MEMBERSHIP) < 0)
        throw socket_exception("Multicast group join failed (setsockopt)");
}

void udp_socket::leaveGroup( const std::string& multicastGroup)
{
    if( groupAction( _socket, multicastGroup, IP_DROP_MEMBERSHIP) < 0)
        throw socket_exception("Multicast group leave failed (setsockopt)");
}
