#pragma once

#include "internet_socket.h"

namespace net
{
class udp_socket : public internet_socket
{
public:

    udp_socket();

    void sendTo( const void* buffer, size_t len, const std::string& foreignAddress, unsigned short foreignPort);
    int receiveFrom( void* buffer, size_t len, std::string& sourceAddress, unsigned short& sourcePort);

    void setMulticastTTL( unsigned char multicastTTL);
    void joinGroup( const std::string& multicastGroup);
    void leaveGroup( const std::string& multicastGroup);
};

}
