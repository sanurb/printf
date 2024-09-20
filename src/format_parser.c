#include <stdlib.h>
#include <string.h>
#include "../include/format_parser.h"
#include "../include/hashmap.h"
#include "../include/buffer.h"

// Static variable to store the hashmap of format specifiers.
static hashmap_t *format_specifiers = NULL;

// Handler functions for format specifiers
static void handle_integer(va_list args, buffer_t *buffer);
static void handle_string(va_list args, buffer_t *buffer);
static void handle_hex(va_list args, buffer_t *buffer);

// Registers the default set of format specifiers and their handlers.
static void register_default_specifiers(void) {
    register_specifier('d', handle_integer);  // Handle integer (%d)
    register_specifier('s', handle_string);   // Handle string (%s)
    register_specifier('x', handle_hex);      // Handle hex (%x)
    // TODO: Add more specifiers
}

// Initializes the hashmap of format specifiers.
void initialize_format_specifiers(void) {
    if (format_specifiers == NULL) {
        format_specifiers = create_hashmap(DEFAULT_HASHMAP_CAPACITY);
        register_default_specifiers();  // Register default specifiers.
    }
}

// Cleans up the hashmap and any resources associated with the format specifiers.
void cleanup_format_specifiers(void) {
    if (format_specifiers != NULL) {
        free_hashmap(format_specifiers);
        format_specifiers = NULL;
    }
}

// Parses the format string and returns information about the format specifier.
// Starts parsing from the '%' character.
format_info_t parse_format(const char *format) {
    format_info_t info = {0};  // Initialize the structure.

    // Ensure we're starting at a valid format specifier.
    if (*format != FORMAT_SPECIFIER_START) {
        info.valid = false;
        info.length = INVALID_SPECIFIER_LENGTH;
        return info;
    }

    const char *start = format + 1;  // Skip the '%' character.
    char specifier = *start;

    // Look up the handler for the format specifier in the hashmap.
    format_handler_t handler = get_format_handler(specifier);

    if (handler) {
        // If a valid handler is found, populate the format_info_t structure.
        info.valid = true;
        info.specifier = specifier;
        info.handler = handler;
        info.length = start - format + 1;  // Total length of the format specifier (including '%').
    } else {
        // If no valid handler is found, mark the specifier as invalid.
        info.valid = false;
        info.length = INVALID_SPECIFIER_LENGTH;  // Skip just the '%' character to avoid infinite loops.
    }

    return info;
}

// Registers a custom format specifier and its handler function in the hashmap.
void register_specifier(char specifier, format_handler_t handler) {
    if (format_specifiers != NULL) {
        char key[2] = {specifier, '\0'};  // Create a key for the hashmap (specifier + null terminator).
        insert_hashmap(format_specifiers, key, handler);
    }
}

// Retrieves the handler function for the specified format character from the hashmap.
// Returns NULL if the format specifier is not found.
format_handler_t get_format_handler(char specifier) {
    if (format_specifiers != NULL) {
        char key[2] = {specifier, '\0'};  // Create a key for the hashmap lookup.
        return (format_handler_t)get_hashmap(format_specifiers, key);
    }
    return NULL;  // If format_specifiers is NULL or handler not found.
}

// Handles integer specifier (%d). Writes the integer value to the buffer.
static void handle_integer(va_list args, buffer_t *buffer) {
    int value = va_arg(args, int);
    char str[20];  // Buffer to hold the string representation of the integer.
    snprintf(str, sizeof(str), "%d", value);  // Convert integer to string.
    append_to_buffer(buffer, str, strlen(str));  // Append the string to the buffer.
}

// Handles string specifier (%s). Writes the string to the buffer.
static void handle_string(va_list args, buffer_t *buffer) {
    char *value = va_arg(args, char *);
    if (value == NULL) {
        // Handle null strings safely.
        append_to_buffer(buffer, "(null)", 6);
    } else {
        append_to_buffer(buffer, value, strlen(value));
    }
}

// Handles hexadecimal specifier (%x). Writes the hexadecimal value to the buffer.
static void handle_hex(va_list args, buffer_t *buffer) {
    unsigned int value = va_arg(args, unsigned int);
    char str[20];  // Buffer to hold the string representation of the hex value.
    snprintf(str, sizeof(str), "%x", value);  // Convert integer to hexadecimal string.
    append_to_buffer(buffer, str, strlen(str));  // Append the string to the buffer.
}
