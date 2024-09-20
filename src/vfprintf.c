#include <stdio.h>
#include <stdbool.h>
#include "../include/vfprintf.h"
#include "../include/format_parser.h"
#include "../include/buffer.h"
#include "../include/error_handling.h"

// Helper function to process literal '%' characters in the format string.
static bool is_escaped_percent(const char *ptr) {
    return (*ptr == '%' && *(ptr + 1) == '%');
}

// Helper function to handle invalid format specifiers, like "%z".
static void handle_invalid_specifier(buffer_t *buffer, const char **ptr) {
    append_to_buffer(buffer, *ptr, 1);  // Add the '%' to the buffer.
    (*ptr)++;  // Move the pointer forward to the next character.
}

// Implementation of my_vfprintf, which handles formatted output to the provided stream.
int my_vfprintf(FILE *stream, const char *format, va_list args) {
    buffer_t *buffer = init_buffer(1024);  // Initialize the buffer with a reasonable size.
    if (!buffer) {
        handle_error(MEMORY_ALLOCATION_ERROR, "Failed to initialize buffer");
        return -1;
    }

    const char *ptr = format;
    int total_written = 0;

    // Iterate over the format string.
    while (*ptr) {
        if (*ptr == FORMAT_SPECIFIER_START) {  // Detect the '%' character.

            if (is_escaped_percent(ptr)) {
                // Handle escaped '%%' by appending a single '%' to the buffer.
                append_to_buffer(buffer, "%", 1);
                ptr += 2;  // Move past both '%' characters.
                continue;
            }

            // Parse the format specifier.
            const format_info_t info = parse_format(ptr);

            if (!info.valid) {
                // Handle invalid format specifiers (e.g., "%z").
                handle_invalid_specifier(buffer, &ptr);
                continue;
            }

            // Call the appropriate handler for the format specifier.
            info.handler(args, buffer);

            // Move the pointer forward based on the length of the format specifier.
            ptr += info.length;

        } else {
            // Append normal characters to the buffer.
            append_to_buffer(buffer, ptr, 1);
            ptr++;
        }
    }

    // Flush the buffer to the output stream.
    total_written = buffer->used;
    flush_buffer(buffer, stream);

    // Free the buffer resources.
    free_buffer(buffer);

    return total_written;
}
