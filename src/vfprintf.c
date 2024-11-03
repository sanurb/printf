#include <stdio.h>
#include <stdbool.h>
#include "../include/vfprintf.h"
#include "../include/format_parser.h"
#include "../include/buffer.h"
#include "../include/error_handling.h"

// Detects escaped '%' characters (e.g., "%%") to output a literal '%'.
// This helps manage cases where '%' is not meant to be a format specifier.
static bool is_escaped_percent(const char *ptr) {
    return (*ptr == '%' && *(ptr + 1) == '%');
}

// Handles invalid format specifiers by appending '%' and moving past it.
// This keeps invalid sequences visible in the output, rather than silently failing.
static void handle_invalid_specifier(buffer_t *buffer, const char **ptr) {
    append_to_buffer(buffer, *ptr, 1);  // Append '%' to make the issue clear in output.
    (*ptr)++;  // Advance past the invalid specifier.
}

// Custom implementation of vfprintf to handle formatted output to a stream.
// Inspired by standard printf's logic: parsing the format string, identifying format specifiers,
// and calling appropriate handlers to build the output.
// Unlike printf, this version isolates buffer management to handle larger outputs and improve flexibility.
int my_vfprintf(FILE *stream, const char *format, va_list args) {
    // Initialize the output buffer with an initial size; enables efficient
    // appending without repeated reallocations. Custom printf often uses buffers to manage
    // formatted data before writing, optimizing for both speed and memory.
    buffer_t *buffer = init_buffer(1024);
    if (!buffer) {
        handle_error(MEMORY_ALLOCATION_ERROR, "Failed to initialize buffer");
        return -1;
    }

    const char *ptr = format;  // Pointer to traverse the format string.
    int total_written = 0;

    // Iterate through the format string, parsing and handling specifiers as they appear.
    while (*ptr) {
        if (*ptr == FORMAT_SPECIFIER_START) {  // Detect '%', marking a format specifier.

            if (is_escaped_percent(ptr)) {
                // Handle '%%' by appending a single '%'. In standard printf, this is
                // a common escape sequence to print '%' without triggering formatting logic.
                append_to_buffer(buffer, "%", 1);
                ptr += 2;  // Move past both '%' characters.
                continue;
            }

            // Parse the format specifier and determine its validity and handler.
            // This step mimics printf's specifier parsing, allowing flexible handling
            // of different data types. Custom parsers improve modularity, allowing for
            // easy addition or modification of specifiers.
            const format_info_t info = parse_format(ptr);

            if (!info.valid) {
                // When encountering an invalid specifier (e.g., "%z"), output the '%'
                // to indicate an issue in the format string, but skip additional handling.
                handle_invalid_specifier(buffer, &ptr);
                continue;
            }

            // Call the handler associated with the format specifier. Each handler
            // processes its respective argument type, converts it to a string, and appends it
            // to the buffer. This design enables modularity and separates parsing from processing.
            info.handler(args, buffer);

            // Move the pointer forward by the length of the parsed specifier,
            // ready to process the next portion of the format string.
            ptr += info.length;

        } else {
            // Append literal characters (not part of a specifier) to the buffer directly.
            // This logic mirrors printf's approach of copying non-specifier characters verbatim.
            append_to_buffer(buffer, ptr, 1);
            ptr++;
        }
    }

    // Write the buffer contents to the output stream in one operation.
    // Unlike printf, which writes directly, this buffered approach consolidates output,
    // minimizing I/O calls, which are relatively slow.
    total_written = buffer->used;
    flush_buffer(buffer, stream);

    // Clean up the buffer after flushing to prevent memory leaks.
    free_buffer(buffer);

    return total_written;
}