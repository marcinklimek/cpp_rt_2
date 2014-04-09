#include <iostream>
#include <stdlib.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

using namespace std;


#define SOCK_PATH "/tmp/ux_stream_srv"

int server()
{
    struct sockaddr_un addr;
    int sfd;
    int cfd;

    sfd = socket( AF_UNIX, SOCK_STREAM, 0);
    if (sfd == -1)
    {
        std::cout << "socket error" << endl;
        exit(1);
    }

    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy( addr.sun_path, SOCK_PATH, sizeof(addr.sun_path)-1 );

    if ( bind(sfd, (sockaddr*)&addr, sizeof(addr) ) == -1)
    {
        std::cout << "bind error" << endl;
        exit(1);
    }

    if ( listen(sfd, 5) == -1 )
    {
        std::cout << "listen error" << endl;
        exit(1);
    }

    char msg[] = "foo\n";

    while( true )
    {
        cfd = accept( sfd, NULL, 0);
        if ( cfd == -1)
        {
            std::cout << "accept error" << endl;
            exit(1);
        }

        std::cout << "new client " << cfd << std::endl;

        write(cfd, msg, strlen(msg) + 1 );

        close( cfd );
    }


    return 0;
}

#define BUF_SIZE 1024
//#define SOCK_PATH_CLIENT "/tmp/ux_stream_cl"

void client()
{

     struct sockaddr_un addr;
     int fd;

     char buf[BUF_SIZE];


     fd = socket(AF_UNIX, SOCK_STREAM, 0);

     memset(&addr, 0, sizeof(addr));
     addr.sun_family = AF_UNIX;
     strncpy( addr.sun_path, SOCK_PATH, sizeof(addr.sun_path)-1 );

     connect( fd, (sockaddr*)&addr, sizeof(addr));

     int num_read = read(fd, buf, sizeof(buf));

     if ( num_read < 1 )
     {
         std::cout << "Read error " << num_read << std::endl;

         close(fd);

         exit(1);
     }

     std::cout << "Rcv " << buf << std::endl;

     close( fd );
}


int main(int argc, char*argv[])
{

    if (argc > 1)
    {
        client();
        exit(0);
    }

    server();
}


