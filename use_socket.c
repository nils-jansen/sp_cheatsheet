#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>

#define MY_SOCKET 1

void use_socket()
{
    while (1)
    {
        int conn = accept(MY_SOCKET, NULL, NULL);
        if (conn == -1)
        {
            // error
        }
        // USE CONNECTION
        close(conn);
    }
    close(MY_SOCKET);
}