#include "net/unix_datagram_socket.h"
#include <iostream>
#include <unistd.h>

static const char send_msg[] = "The quick brown fox jumps over the lazy dog";
static char recv_msg[50];

int main()
{
    try
    {
        net::unix_datagram_socket server_socket;

        server_socket.bind("/tmp/ux_sock");

        while( true )
        {
            std::string clientPath;
            server_socket.receiveFrom( recv_msg, sizeof(recv_msg), clientPath);
            std::cout << "recv from " << clientPath << std::endl;

            server_socket.sendTo( send_msg, sizeof(send_msg), clientPath);
        }
    }
    catch( const net::socket_exception& e)
    {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}
