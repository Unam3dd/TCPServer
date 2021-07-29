#include "log.h"
#include <errno.h>
#include <time.h>
#include <unistd.h>
#include <string.h>

char *get_time(char *buffer)
{
    time_t t = time(NULL);
    struct tm *tinfo = localtime(&t);

    strftime(buffer, 0x20, "%d/%m/%Y %H:%M:%S", tinfo);

    return (buffer);
}

void ft_putchar(char c)
{
    write(STDOUT_FILENO, &c, 1);
}

void ft_print_str(char *str)
{
    while (*str)
        ft_putchar(*str++);
}

char *int_to_str(uint32_t number, char *output)
{
    char *tmp = output;
    uint32_t r = 0, c = 0;

    while (number) {
        r *= 10;
        r += (number % 10);
        number /= 10;
        c++;
    }

    while (c--) {
        *tmp++ = (r % 10) + '0';
        r /= 10;
    }
    
    *tmp = 0;

    return (output);
}

void print_log(const char *format, ...)
{
    char *tmp = (char *)format;
    char buffer[0x20];
    va_list parameters;

    va_start(parameters, format);

    while (*tmp) {
        
        if (*tmp != '%') {
            ft_putchar(*tmp++);
            continue;
        }

        tmp++;

        switch (*tmp) {

            case 's': {
                ft_print_str(va_arg(parameters, char *));
                break;
            }
            
            case 'd': {
                ft_print_str(int_to_str(va_arg(parameters, uint32_t), buffer));
                break;
            }

            case 't': {
                ft_print_str(get_time(buffer));
                break;
            }

            default:
                break;
        }

        tmp++;
    }

    va_end(parameters);
}

void print_error(void)
{
    print_log("[%t] error : %s\n", strerror(errno));
}