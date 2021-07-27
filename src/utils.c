#include "tcp_server.h"
#include <sys/ioctl.h>
#include <unistd.h>


uint8_t fd_is_readable(tcp_server_t *server, tcp_client_t *client)
{
    size_t len = 0;

    ioctl(client->fd, FIONREAD, &len);

    return (len);
}