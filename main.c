#include "tcp_server.h"
#include "log.h"
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char **argv)
{
    tcp_server_t server;

    create_tcp_server(&server);

    server.set_callbacks(&server.callbacks, &callback_accept, &callback_handle, &callback_close);
    server.set(&server, argv[1], atoi(argv[2]), AF_INET, SOMAXCONN);
    
    if (server.init(&server)) {
        print_error();
        delete_tcp_server(&server);
        return (1);
    }
    
    server.wait(&server);

    server.close(&server);

    delete_tcp_server(&server);

    return (0);
}