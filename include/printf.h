#ifndef PRINTF_H
#define PRINTF_H

#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>

// Public function prototype for my_printf, mimicking the behavior of printf.
int my_printf(const char *format, ...);

// Public function prototype for initializing resources (if necessary).
void initialize_printf(void);

// Public function prototype for cleaning up resources (if necessary).
void cleanup_printf(void);

#endif // PRINTF_H
