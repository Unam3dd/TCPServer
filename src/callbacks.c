#include "tcp_server.h"
#include "log.h"
#include <unistd.h>
#include <sys/epoll.h>

void *callback_accept(tcp_server_t *server, tcp_client_t *client)
{
    struct epoll_event ev;

    socklen_t size_sin_client = sizeof(client->s);

    if ((client->fd = accept(server->server_fd, (struct sockaddr *)&client->s, &size_sin_client)) < 0)
        return ((int *)1);
    
    ev.events = EPOLLIN;
    ev.data.fd = client->fd;
    ev.data.ptr = (tcp_client_t *) client;

    if (epoll_ctl(server->epoll_fd, EPOLL_CTL_ADD, client->fd, &ev) < 0)
        return ((void *) 1);
    
    return ((void *)0);
}

void *callback_handle(tcp_server_t *server, tcp_client_t *client)
{
    if (fd_is_readable(server, client) == 0) {
        server->callbacks.on_close(server, client);
        return ((void *) 0);
    }

    print_log("[%t] Opening the shell prompt on %s:%d\n", inet_ntoa(client->s.sin_addr), ntohs(client->s.sin_port));

    handle_client_file_descriptor(client);

    print_log("[%t] Waiting.. closing client connection -> %s:%d\n", inet_ntoa(client->s.sin_addr), ntohs(client->s.sin_port));

    server->callbacks.on_close(server, client);

    return ((void *) 0);
}

void *callback_close(tcp_server_t *server, tcp_client_t *client)
{
    if (client->fd)
        close(client->fd);
    
    return ((void *) 0);
}