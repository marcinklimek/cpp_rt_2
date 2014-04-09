#include "net/tcp_socket.h"
#include "thread/thread_safe_queue.h"
#include "thread/launch_thread.h"
#include <iostream>
#include <vector>
#include <pthread.h>

static const char send_msg[] = "Shut up and take my money!";


int main()
{
    try
    {
        net::tcp_socket server_socket;

        server_socket.bind( "127.0.0.1", 12345);
        server_socket.listen();

        std::cout << server_socket.getfd() << std::endl;

        net::tcp_socket::sockhandle handle = server_socket.accept();
        if ( !handle )
            return 1;

        std::cout << "client " <<  handle << std::endl;

        net::tcp_socket session_socket(handle);
        session_socket.send( send_msg, sizeof(send_msg) );

        if( session_socket.peerDisconnected())
            return 1;

        std::cout << "TCP server sucessfully sent data to the client." << std::endl;

        session_socket.disconnect();
    }
    catch( const net::socket_exception& e)
    {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}
