#include "net/tcp_socket.h"

#include <iostream>

static char recv_msg[50];

int main()
{
    try
    {
        net::tcp_socket client_socket;
        client_socket.connect( "127.0.0.1", 12345);

        int bytes_sent = client_socket.send( recv_msg, 50);
        std::cout << "Sent data to server: " << bytes_sent  << std::endl;
        int bytes_recv = client_socket.receive( recv_msg, 50);

        // we got a message!
        if(bytes_recv > 0)
            std::cout << "Received data from server: " << recv_msg << std::endl;

        // this is optional, we could simply do nothing here
        client_socket.disconnect();
    }
    catch( const net::socket_exception& e)
    {
        std::cerr << e.what() << std::endl;

        return 1;
    }

    return 0;
}
