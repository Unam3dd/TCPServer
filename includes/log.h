#pragma once
#include <stdint.h>
#include <stdarg.h>

char *get_time(char *buffer);
char *int_to_str(uint32_t number, char *output);
void ft_putchar(char c);
void ft_print_str(char *str);
void print_log(const char *format, ...);
void print_error(void);