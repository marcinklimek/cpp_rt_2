#include <iostream>
#include "inet_sockets.h"
#define BUF_SIZE 128

void client()
{
    int client_fd = sock_connect("localhost", "12345", SOCK_STREAM);

    if ( client_fd == -1 )
    {
        std::cout << "sock_connect " << errno << " " << strerror( errno ) << std::endl;
        exit(1);
    }

    char buf[BUF_SIZE];
    memset(buf, 0, sizeof(buf) );
    strncpy(buf, "foo bar", BUF_SIZE);

    ssize_t num_sent = send(client_fd, buf, strlen(buf)+1, 0);
    std::cout << "num_sent " << num_sent << std::endl;

    memset(buf, 0, sizeof(buf) );
    ssize_t num_recv = recv(client_fd, buf, BUF_SIZE, 0);
    std::cout << "num_recv " << num_recv << std::endl;


    close( client_fd );
}

void server()
{

    std::cout << "Server" << std::endl;

    int server_fd = sock_listen("12345", 10, NULL);

    if ( server_fd == -1 )
    {
        std::cout << "sock_listen error " << errno << " " << strerror( errno ) << std::endl;
        exit(1);
    }


    threadpool tp(10);

    while (true)
    {
        sockaddr_storage client_addr;
        socklen_t len = sizeof(client_addr);

        int client_fd = accept(server_fd, (sockaddr*)&client_addr, &len);

        tp.add_task( [client_fd] () {

            print_client_info("New client - ", client_addr);
            std::cout<<std::endl;

            char buf[BUF_SIZE];
            memset(buf, 0, sizeof(buf) );

            ssize_t num_recv = recv(client_fd, buf, BUF_SIZE, 0);
            std::cout << "num_recv " << num_recv << " " << buf << std::endl;

            sleep(2);

            strncpy(buf, "abcdef", BUF_SIZE);
            ssize_t num_sent = send(client_fd, buf, strlen(buf)+1, 0);
            std::cout << "num_sent " << num_sent << std::endl;

            close( client_fd );
        });
    }
}



int main(int argc, char* argv[] )
{

    if ( argc > 1 )
    {
        client();
    }
    else
    {
        server();
    }

    return 0;
}

