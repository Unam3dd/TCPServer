#include <stdio.h>
#include <stdlib.h>
#include "tcp_server.h"

int main(int argc, char **argv)
{
    tcp_server_t server;

    create_tcp_server(&server);

    server.set_callbacks(&server.callbacks, &callback_accept, &callback_handle, &callback_close);
    server.set(&server, argv[1], atoi(argv[2]), AF_INET, SOMAXCONN);
    
    if (server.init(&server))
        fprintf(stderr, "[-] Error intialize TCP server !\n");
    
    server.wait(&server);

    server.close(&server);

    delete_tcp_server(&server);

    return (0);
}