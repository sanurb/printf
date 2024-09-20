#include "../include/vfprintf.h"
#include "../include/printf.h"
#include "../include/format_parser.h"

// Wrapper function for my_printf, which mimics the behavior of printf using a variable argument list.
int my_printf(const char *format, ...) {
    va_list args;
    va_start(args, format);

    // Call my_vfprintf with stdout as the default output stream.
    int result = my_vfprintf(stdout, format, args);

    va_end(args);
    return result;
}


void initialize_printf() {
    // Inicializar el hashmap de especificadores
    initialize_format_specifiers();
}

void cleanup_printf() {
    // Limpiar recursos globales como el hashmap
    cleanup_format_specifiers();
}