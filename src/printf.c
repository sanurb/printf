#include "../include/vfprintf.h"
#include "../include/printf.h"
#include "../include/format_parser.h"

// Wrapper for my_vfprintf that provides printf-like behavior.
// By handling a variable argument list, my_printf imitates the functionality of printf
// for standard output (stdout), serving as the entry point for formatted printing.
int my_printf(const char *format, ...) {
    va_list args;
    va_start(args, format);

    // Redirects to my_vfprintf with stdout as the target stream.
    // This redirection allows modularity, making my_vfprintf reusable for other streams.
    // In standard printf, stdout is the default, but custom implementations allow output redirection,
    // which is particularly useful in contexts like logging or file output.
    int result = my_vfprintf(stdout, format, args);

    va_end(args);
    return result;
}

// Initializes resources required for custom printf, including format specifiers.
// This function centralizes setup, allowing control over all supported specifiers.
// Custom printf implementations often need such initialization to ensure all
// necessary specifiers are registered, enhancing extensibility.
void initialize_printf() {
    initialize_format_specifiers();
}

// Cleans up global resources such as the hashmap used for format specifiers.
// Essential for preventing memory leaks, especially in persistent or embedded systems
// where memory management is critical. Mimics standard library conventions where cleanup is handled implicitly.
void cleanup_printf() {
    cleanup_format_specifiers();
}