#include <stdlib.h>
#include <string.h>
#include "../include/format_parser.h"
#include <ctype.h>
#include "hashmap.h"
#include "buffer.h"

// Static hashmap for storing the format specifiers and their handlers.
static hashmap_t *format_specifiers = NULL;

// Handler declarations for each format specifier.
static void print_string(va_list args, buffer_t *buffer);
static void print_char(va_list args, buffer_t *buffer);
static void print_integer(va_list args, buffer_t *buffer);
static void print_pointer(va_list args, buffer_t *buffer);
static void print_binary(va_list args, buffer_t *buffer);
static void print_hexadecimal_low(va_list args, buffer_t *buffer);
static void print_hexadecimal_upp(va_list args, buffer_t *buffer);
static void print_octal(va_list args, buffer_t *buffer);
static void print_rot(va_list args, buffer_t *buffer);

// Registers the default set of format specifiers and their handlers.
static void register_default_specifiers(void) {
    register_specifier('s', print_string);
    register_specifier('c', print_char);
    register_specifier('i', print_integer);
    register_specifier('d', print_integer);
    register_specifier('p', print_pointer);
    register_specifier('b', print_binary);
    register_specifier('x', print_hexadecimal_low);
    register_specifier('X', print_hexadecimal_upp);
    register_specifier('o', print_octal);
    register_specifier('R', print_rot);
}

// Initializes the hashmap of format specifiers.
void initialize_format_specifiers(void) {
    if (format_specifiers == NULL) {
        format_specifiers = create_hashmap(DEFAULT_HASHMAP_CAPACITY);
        register_default_specifiers();
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
        info.valid = true;
        info.specifier = specifier;
        info.handler = handler;
        info.length = MAX_SPECIFIER_LENGTH;  // Total length of the format specifier (including '%').
    } else {
        info.valid = false;
        info.length = INVALID_SPECIFIER_LENGTH;  // Skip just the '%' character to avoid infinite loops.
    }

    return info;
}

// Registers a custom format specifier and its handler function in the hashmap.
void register_specifier(char specifier, format_handler_t handler) {
    if (format_specifiers != NULL) {
        char key[2] = {specifier, '\0'};  // Create a key for the hashmap (specifier + null terminator).

        // Create a wrapper structure to hold the function pointer.
        function_wrapper_t *wrapper = malloc(sizeof(function_wrapper_t));
        wrapper->handler = handler;

        // Insert the wrapper into the hashmap instead of the function pointer directly.
        insert_hashmap(format_specifiers, key, wrapper);
    }
}

// Retrieves the handler function for the specified format character from the hashmap.
// Returns NULL if the format specifier is not found.
format_handler_t get_format_handler(char specifier) {
    if (format_specifiers != NULL) {
        char key[2] = {specifier, '\0'};  // Create the key for the hashmap lookup.

        // Retrieve the wrapper from the hashmap.
        function_wrapper_t *wrapper = (function_wrapper_t *)get_hashmap(format_specifiers, key);

        // Return the handler from the wrapper.
        if (wrapper != NULL) {
            return wrapper->handler;
        }
    }
    return NULL;  // Return NULL if no handler is found.
}

// Handles string specifier (%s)
void print_string(va_list args, buffer_t *buffer) {
    // Retrieve the argument as a string
    char *value = va_arg(args, char *);

    // Handle null pointers safely by printing "(null)"
    if (value == NULL) {
        append_to_buffer(buffer, "(null)", 6);
        return;
    }

    // Handle empty strings
    if (value[0] == '\0') {
        append_to_buffer(buffer, "", 0);
        return;
    }

    // Append the valid string to the buffer
    append_to_buffer(buffer, value, strlen(value));
}

// Handles char specifier (%c)
void print_char(va_list args, buffer_t *buffer) {
    char value = (char)va_arg(args, int);
    append_to_buffer(buffer, &value, 1);
}

// Handles integer specifier (%i, %d)
void print_integer(va_list args, buffer_t *buffer) {
    int value = va_arg(args, int);
    char str[20];
    itoa(value, str, 10);  // Convert integer to string (base 10).
    append_to_buffer(buffer, str, strlen(str));
}

// Handles pointer specifier (%p)
void print_pointer(va_list args, buffer_t *buffer) {
    void *ptr = va_arg(args, void *);
    if (ptr == NULL) {
        append_to_buffer(buffer, "(nil)", 5);
        return;
    }
    uintptr_t value = (uintptr_t)ptr;  // Use uintptr_t for portability.
    char str[20];
    itoa(value, str, 16);
    append_to_buffer(buffer, "0x", 2); // Add '0x' prefix for pointers.
    append_to_buffer(buffer, str, strlen(str));
}

// Handles binary specifier (%b)
void print_binary(va_list args, buffer_t *buffer) {
    unsigned int value = va_arg(args, unsigned int);
    char str[35];  // 32 bits + '\0'.
    itoa(value, str, 2);  // Convert to binary string (base 2).
    append_to_buffer(buffer, str, strlen(str));
}

// Handles hexadecimal lowercase specifier (%x)
void print_hexadecimal_low(va_list args, buffer_t *buffer) {
    unsigned int value = va_arg(args, unsigned int);
    char str[20];
    itoa(value, str, 16);  // Convert to hex string (base 16).
    append_to_buffer(buffer, str, strlen(str));
}

// Handles hexadecimal uppercase specifier (%X)
void print_hexadecimal_upp(va_list args, buffer_t *buffer) {
    unsigned int value = va_arg(args, unsigned int);
    char str[20];
    itoa(value, str, 16);  // Convert to hex string (base 16).
    // Convert the string to uppercase.
    for (int i = 0; str[i] != '\0'; i++) {
        str[i] = toupper(str[i]);
    }
    append_to_buffer(buffer, str, strlen(str));
}

// Handles octal specifier (%o)
void print_octal(va_list args, buffer_t *buffer) {
    unsigned int value = va_arg(args, unsigned int);
    char str[20];
    itoa(value, str, 8);  // Convert to octal string (base 8).
    append_to_buffer(buffer, str, strlen(str));
}

// Handles rot13 string specifier (%R)
void print_rot(va_list args, buffer_t *buffer) {
    char *str = va_arg(args, char *);
    if (str == NULL) {
        append_to_buffer(buffer, "(null)", 6);
        return;
    }
    for (int i = 0; str[i] != '\0'; i++) {
        char c = str[i];
        if ((c >= 'a' && c <= 'z')) {
            c = (c - 'a' + 13) % 26 + 'a';
        } else if (c >= 'A' && c <= 'Z') {
            c = (c - 'A' + 13) % 26 + 'A';
        }
        append_to_buffer(buffer, &c, 1);
    }
}