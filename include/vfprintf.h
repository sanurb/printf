#ifndef VPRINTF_H
#define VPRINTF_H

#include <stdarg.h>
#include <stdio.h>
#include "buffer.h"

// Public function prototype for my_vfprintf, which handles formatted output to a FILE stream.
int my_vfprintf(FILE *stream, const char *format, va_list args);

#endif // VPRINTF_H
