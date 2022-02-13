#include <sys/socket.h>
#include <netinet/in.h>

#define MY_PORT 8080

void create_socket()
{
    int sock = socket(AF_INET6, SOCK_STREAM, 0);
    if (sock == -1)
    {
        // error
    }

    struct sockaddr_in6 sin = {
        .sin6_family = AF_INET6,
        .sin6_port = htons(MY_PORT),
        .sin6_addr = in6addr_any,
    };
    if (bind(sock, (struct sockaddr *)&sin, sizeof(sin)) == -1)
    {
        // error
    }

    if (listen(sock, SOMAXCONN) == -1)
    {
        // error
    }
}