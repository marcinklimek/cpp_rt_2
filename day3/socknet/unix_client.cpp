#include "net/unix_datagram_socket.h"
#include <unistd.h>
#include <iostream>

static char recv_msg[50];

int main()
{
    try
    {
        net::unix_datagram_socket client_socket;

        client_socket.bind("/tmp/ux_sock_"+std::to_string( getpid() ));

        client_socket.sendTo(recv_msg, 1, "/tmp/ux_sock" );
        int bytes_recv = client_socket.receive(recv_msg, 50);

        if(bytes_recv > 0)
            std::cout << "Received data from server: " << recv_msg << std::endl;

        client_socket.disconnect();
    }
    catch( const net::socket_exception& e)
    {
        std::cerr << e.what() << std::endl;

        return 1;
    }

    return 0;
}
