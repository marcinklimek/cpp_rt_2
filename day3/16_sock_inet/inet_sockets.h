#ifndef _SOCKETS_HELPERS_H
#define _SOCKETS_HELPERS_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <errno.h>

#define TRUE 1
#define FALSE 0
#define IS_ADDR_STR_LEN 4096

int sock_connect(const char *host, const char *service, int type);
int sock_listen(const char *service, int backlog, socklen_t *addrlen);
int sock_bind(const char *service, int type, socklen_t *addrlen);

char* sock_address_str(const struct sockaddr *addr, socklen_t addrlen, char *addrStr, int addrStrLen);
void print_client_info(const char* info, sockaddr_storage &claddr);


#endif
