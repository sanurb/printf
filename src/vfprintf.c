#include <stdio.h>
#include "../include/vfprintf.h"
#include "../include/format_parser.h"
#include "../include/buffer.h"
#include "../include/error_handling.h"

// Implementation of my_vfprintf, which handles formatted output to the provided stream.
int my_vfprintf(FILE *stream, const char *format, va_list args) {
    buffer_t *buffer = init_buffer(1024);  // Initialize the buffer with an arbitrary size.
    if (!buffer) {
        handle_error(MEMORY_ALLOCATION_ERROR, "Failed to initialize buffer");
        return -1;
    }

    const char *ptr = format;
    while (*ptr) {
        if (*ptr == '%') {
            // Parse the format specifier.
            format_info_t info = parse_format(ptr);
            if (!info.valid) {
                handle_error(INVALID_FORMAT, "Invalid format specifier");
                free_buffer(buffer);
                return -1;
            }

            // Process the format specifier.
            info.handler(args, buffer);

            // Skip the length of the parsed format specifier.
            ptr += info.length;
        } else {
            // Append normal characters to the buffer.
            append_to_buffer(buffer, ptr, 1);
            ptr++;
        }
    }

    // Flush the buffer to the output stream.
    flush_buffer(buffer, stream);

    // Free the buffer resources.
    free_buffer(buffer);

    return 0;  // Return the number of characters written (you can implement this in future versions).
}
