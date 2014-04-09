#include "inet_sockets.h"

int sock_connect(const char *host, const char *service, int type)
{
    addrinfo hints;
    addrinfo *result, *rp;
    int sfd, s;

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_canonname = NULL;
    hints.ai_addr = NULL;
    hints.ai_next = NULL;
    hints.ai_family = AF_INET;
    hints.ai_socktype = type;

    s = getaddrinfo(host, service, &hints, &result);
    if (s != 0)
    {
        errno = ENOSYS;
        return -1;
    }

    // Walk through returned list until we find an address structure
    //   that can be used to successfully connect a socket
    for (rp = result; rp != NULL; rp = rp->ai_next)
    {
        sfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (sfd == -1)
            continue;                   /* On error, try next address */

        if (connect(sfd, rp->ai_addr, rp->ai_addrlen) != -1)
            break;                      /* Success */

        /* Connect failed: close this socket and try next address */
        close(sfd);
    }

    freeaddrinfo(result);

    return (rp == NULL) ? -1 : sfd;
}

int sock_create_passive(const char *service, int type, socklen_t *addrlen, int doListen, int backlog)
{
    struct addrinfo hints;
    struct addrinfo *result, *rp;
    int sfd, optval, s;

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_canonname = NULL;
    hints.ai_addr = NULL;
    hints.ai_next = NULL;
    hints.ai_socktype = type;
    hints.ai_family = AF_INET;
    hints.ai_flags = AI_PASSIVE;

    s = getaddrinfo(NULL, service, &hints, &result);
    if (s != 0)
        return -1;

    // find free addr we can use
    optval = 1;
    for (rp = result; rp != NULL; rp = rp->ai_next)
    {
        sfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);

        if (sfd == -1)
            continue; // try next address

        if (doListen)
        {
            if (setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) == -1)
            {
                close(sfd);
                freeaddrinfo(result);
                return -1;
            }
        }

        if (bind(sfd, rp->ai_addr, rp->ai_addrlen) == 0)
            break;  // ok

        // failed: close and try next address
        close(sfd);
    }

    if (rp != NULL && doListen)
    {
        if (listen(sfd, backlog) == -1)
        {
            freeaddrinfo(result);
            return -1;
        }
    }

    if (rp != NULL && addrlen != NULL)
        *addrlen = rp->ai_addrlen;      // return address structure size

    freeaddrinfo(result);

    return (rp == NULL) ? -1 : sfd;
}

int sock_listen(const char *service, int backlog, socklen_t *addrlen)
{
    return sock_create_passive(service, SOCK_STREAM, addrlen, TRUE, backlog);
}

int sock_bind(const char *service, int type, socklen_t *addrlen)
{
    return sock_create_passive(service, type, addrlen, FALSE, 0);
}

char* sock_address_str(const struct sockaddr *addr, socklen_t addrlen, char *addrStr, int addrStrLen)
{
    char host[NI_MAXHOST], service[NI_MAXSERV];

    if (getnameinfo(addr, addrlen, host, NI_MAXHOST,
                    service, NI_MAXSERV, NI_NUMERICSERV) == 0)
        snprintf(addrStr, addrStrLen, "(%s, %s)", host, service);
    else
        snprintf(addrStr, addrStrLen, "(?UNKNOWN?)");

    return addrStr;
}

void print_client_info(const char* info, sockaddr_storage &claddr)
{
    char host[NI_MAXHOST];
    char service[NI_MAXSERV];
    char addr_str[IS_ADDR_STR_LEN];

    if (getnameinfo((sockaddr *) &claddr, IS_ADDR_STR_LEN-1,
                    host, NI_MAXHOST, service, IS_ADDR_STR_LEN, 0) == 0)
        snprintf(addr_str, IS_ADDR_STR_LEN, "(%s, %s)", host, service);
    else
        snprintf(addr_str, IS_ADDR_STR_LEN, "(?UNKNOWN?)");

    printf("%s %s", info, addr_str);
}
