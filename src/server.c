#include "tcp_server.h"

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/epoll.h>
#include <unistd.h>
#include <signal.h>

void handle_signal(int sig)
{
    signal(sig, SIG_IGN);
    usleep(2000);
    run = 0;
}

uint8_t init_tcp_server(tcp_server_t *server)
{
    struct epoll_event ev;

    server->server_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (server->server_fd < 0)
        return (1);
    
    if (setsockopt(server->server_fd, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int)) < 0)
        return (1);

    if (bind(server->server_fd, (struct sockaddr *)&server->s, sizeof(server->s)) < 0)
        return (1);
    
    if (listen(server->server_fd, server->backlog) < 0)
        return (1);
    
    if ((server->epoll_fd = epoll_create1(0)) < 0)
        return (1);
    
    ev.events = EPOLLIN;
    ev.data.fd = server->server_fd;

    if (epoll_ctl(server->epoll_fd, EPOLL_CTL_ADD, server->server_fd, &ev) < 0)
        return (1);

    return (0);
}

void set_tcp_server(tcp_server_t *server, const char *addr, uint16_t port, uint16_t family, uint16_t backlog)
{
    // Convert Host address to network byte order
    server->s.sin_addr.s_addr = inet_addr(addr);

    // Convert Host port to network byte order
    server->s.sin_port = htons(port);
    
    // Set family type
    server->s.sin_family = family;

    server->backlog = backlog;
}

uint8_t close_tcp_server(tcp_server_t *server)
{
    if (server->epoll_fd)
        close(server->epoll_fd);

    if (server->server_fd)
        close(server->server_fd);
    
    return (0);
}

void wait_tcp_server(tcp_server_t *server)
{
    struct epoll_event evs[TCP_SERVER_EPOLL_EVENTS_MAX];

    int w_fd = 0, i_fd = 0;

    signal(SIGINT, handle_signal);

    while (run) {
        w_fd = epoll_wait(server->epoll_fd, evs, TCP_SERVER_EPOLL_EVENTS_MAX, -1);

        for (i_fd = 0; i_fd < w_fd; i_fd++) {
            if (evs[i_fd].data.fd == server->server_fd) {
                
                tcp_client_t client = {0};

                server->callbacks.on_accept(server, &client);

            } else 
                server->callbacks.on_handle(server, (tcp_client_t *) evs[i_fd].data.ptr);
        }
    }
}


void set_tcp_callbacks(tcp_server_callbacks_t *server_callbacks, void *on_accept, void *on_handle, void *on_close)
{
    server_callbacks->on_accept = on_accept;
    server_callbacks->on_close = on_close;
    server_callbacks->on_handle = on_handle;
}

void create_tcp_server(tcp_server_t *server)
{
    server->init = &init_tcp_server;
    server->wait = &wait_tcp_server;
    server->close = &close_tcp_server;
    server->set_callbacks = &set_tcp_callbacks;
    server->set = &set_tcp_server;
}

void delete_tcp_server(tcp_server_t *server)
{
    server->init = NULL;
    server->wait = NULL;
    server->close = NULL;
    server->set_callbacks = NULL;
    server->set = NULL;
}