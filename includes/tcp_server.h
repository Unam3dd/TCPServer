#pragma once
#include <stdint.h>
#include <arpa/inet.h>

#define TCP_SERVER_EPOLL_EVENTS_MAX 0x10

///////////////////////////////////////////
//            STRUCT
//////////////////////////////////////////


typedef struct tcp_server_t tcp_server_t;
typedef struct tcp_server_callbacks_t tcp_server_callbacks_t;
typedef struct tcp_server_functions_t tcp_server_functions_t;
typedef struct tcp_client_t tcp_client_t;

struct tcp_client_t 
{
    struct sockaddr_in s;
    int fd;
};

struct tcp_server_callbacks_t
{
    void * (*on_accept)(tcp_server_t *, tcp_client_t *);
    void * (*on_handle)(tcp_server_t *, tcp_client_t *);
    void * (*on_close)(tcp_server_t *, tcp_client_t *);
};

struct tcp_server_t
{
    struct sockaddr_in s;
    tcp_server_callbacks_t callbacks;
    
    uint8_t (*init)(tcp_server_t *);
    void (*set)(tcp_server_t *, const char *, uint16_t , uint16_t, uint16_t);
    void (*set_callbacks)(tcp_server_callbacks_t *, void *on_accept, void *on_handle, void *on_close);
    void (*close)(tcp_server_t *);
    void (*wait)(tcp_server_t *);
    void (*signal_handler)(int);

    int server_fd;
    int epoll_fd;
    uint16_t backlog;
};

// STATIC VARIABLES
static volatile int run = 1;


// utils.c
uint8_t fd_is_readable(tcp_server_t *server, tcp_client_t *client);
int handle_client_file_descriptor(tcp_client_t *client);

// server.c
uint8_t init_tcp_server(tcp_server_t *server);
void set_tcp_server(tcp_server_t *server, const char *addr, uint16_t port, uint16_t family, uint16_t backlog);
void set_tcp_callbacks(tcp_server_callbacks_t *server_callbacks, void *on_accept, void *on_handle, void *on_close);
void close_tcp_server(tcp_server_t *server);
void wait_tcp_server(tcp_server_t *server);
void handle_signal(int sig);
void create_tcp_server(tcp_server_t *server);
void delete_tcp_server(tcp_server_t *server);



//////////////////////////////////////////////////////
//      CALLBACKS EXAMPLES
//////////////////////////////////////////////////////

// callbacks.c
void *callback_accept(tcp_server_t *server, tcp_client_t *client);
void *callback_handle(tcp_server_t *server, tcp_client_t *client);
void *callback_close(tcp_server_t *server, tcp_client_t *client);