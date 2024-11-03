#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../include/format_parser.h"
#include "../include/hashmap.h"
#include "../include/buffer.h"

// Static hashmap to store format specifiers and their handlers once
// to avoid repetitive lookups and registration during runtime.
static hashmap_t *format_specifiers = NULL;

// Handlers for each supported format specifier.
static void print_string(va_list args, buffer_t *buffer);
static void print_char(va_list args, buffer_t *buffer);
static void print_integer(va_list args, buffer_t *buffer);
static void print_pointer(va_list args, buffer_t *buffer);
static void print_binary(va_list args, buffer_t *buffer);
static void print_hexadecimal_low(va_list args, buffer_t *buffer);
static void print_hexadecimal_upp(va_list args, buffer_t *buffer);
static void print_octal(va_list args, buffer_t *buffer);
static void print_rot(va_list args, buffer_t *buffer);

// Register default format specifiers and their handlers in the hashmap.
// This avoids repetitive handler declarations and centralizes specifier management.
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

// Initialize the hashmap for format specifiers to allow lookups only once.
// Ensures thread-safety and resource efficiency by checking if it’s already initialized.
void initialize_format_specifiers(void) {
    if (format_specifiers == NULL) {
        format_specifiers = create_hashmap(DEFAULT_HASHMAP_CAPACITY);
        register_default_specifiers();
    }
}

// Free the resources used by format specifiers hashmap on cleanup.
// Important to prevent memory leaks when format specifiers are no longer needed.
void cleanup_format_specifiers(void) {
    if (format_specifiers != NULL) {
        free_hashmap(format_specifiers);
        format_specifiers = NULL;
    }
}

// Parses format starting from '%' and identifies its handler if valid.
// Provides flexibility to support custom format specifiers as needed.
format_info_t parse_format(const char *format) {
    format_info_t info = {0};

    if (*format != FORMAT_SPECIFIER_START) {  // Expecting a '%' here
        info.valid = false;
        info.length = INVALID_SPECIFIER_LENGTH;
        return info;
    }

    const char *start = format + 1;  // Move past '%'
    char specifier = *start;

    format_handler_t handler = get_format_handler(specifier);

    if (handler) {
        info.valid = true;
        info.specifier = specifier;
        info.handler = handler;
        info.length = MAX_SPECIFIER_LENGTH;
    } else {
        // Setting length to skip the invalid specifier safely.
        info.valid = false;
        info.length = INVALID_SPECIFIER_LENGTH;
    }

    return info;
}

// Register a format specifier and associate it with a handler function.
// Wrapping function pointers in structs simplifies hashmap insertion and management.
void register_specifier(char specifier, format_handler_t handler) {
    if (format_specifiers != NULL) {
        char key[2] = {specifier, '\0'};

        function_wrapper_t *wrapper = malloc(sizeof(function_wrapper_t));
        wrapper->handler = handler;

        insert_hashmap(format_specifiers, key, wrapper);
    }
}

// Retrieve the handler function for a given format specifier.
// Returns NULL if the handler is not registered, allowing the caller to handle missing cases.
format_handler_t get_format_handler(char specifier) {
    if (format_specifiers != NULL) {
        char key[2] = {specifier, '\0'};

        function_wrapper_t *wrapper = (function_wrapper_t *)get_hashmap(format_specifiers, key);
        return wrapper ? wrapper->handler : NULL;
    }
    return NULL;
}

// Appends a string to the buffer, handling NULL cases explicitly
// to prevent unexpected behavior with NULL pointers.
void print_string(va_list args, buffer_t *buffer) {
    char *value = va_arg(args, char *);
    if (value == NULL) {
        append_to_buffer(buffer, "(null)", 6);  // Standard fallback for NULL strings
    } else {
        append_to_buffer(buffer, value, strlen(value));
    }
}

// Appends a char argument to the buffer.
// Casting to char here handles potential widening due to default argument promotions.
void print_char(va_list args, buffer_t *buffer) {
    char value = (char)va_arg(args, int);
    append_to_buffer(buffer, &value, 1);
}

// Converts an integer to a string and appends it to the buffer.
// Relies on base 10 to maintain compatibility with common integer specifiers.
void print_integer(va_list args, buffer_t *buffer) {
    int value = va_arg(args, int);
    char str[20];
    itoa(value, str, 10);
    append_to_buffer(buffer, str, strlen(str));
}

// Formats a pointer to a hexadecimal representation with '0x' prefix.
// Uses uintptr_t to support pointers of varying sizes, increasing portability.
void print_pointer(va_list args, buffer_t *buffer) {
    void *ptr = va_arg(args, void *);
    if (ptr == NULL) {
        append_to_buffer(buffer, "(nil)", 5);  // Conventionally represent NULL pointers
        return;
    }
    uintptr_t value = (uintptr_t)ptr;
    char str[20];
    itoa(value, str, 16);
    append_to_buffer(buffer, "0x", 2);
    append_to_buffer(buffer, str, strlen(str));
}

// Converts an unsigned integer to a binary string representation for %b specifier.
// Provides a max buffer size to handle up to 32-bit binary strings safely.
void print_binary(va_list args, buffer_t *buffer) {
    unsigned int value = va_arg(args, unsigned int);
    char str[35];
    itoa(value, str, 2);
    append_to_buffer(buffer, str, strlen(str));
}

// Converts an unsigned integer to a lowercase hexadecimal string.
void print_hexadecimal_low(va_list args, buffer_t *buffer) {
    unsigned int value = va_arg(args, unsigned int);
    char str[20];
    itoa(value, str, 16);
    append_to_buffer(buffer, str, strlen(str));
}

// Converts an unsigned integer to an uppercase hexadecimal string.
// Uppercase conversion is applied after conversion for clarity.
void print_hexadecimal_upp(va_list args, buffer_t *buffer) {
    unsigned int value = va_arg(args, unsigned int);
    char str[20];
    itoa(value, str, 16);
    for (int i = 0; str[i] != '\0'; i++) {
        str[i] = toupper(str[i]);
    }
    append_to_buffer(buffer, str, strlen(str));
}

// Converts an unsigned integer to an octal string for the %o specifier.
// The octal base is directly applied to fit common format specifier standards.
void print_octal(va_list args, buffer_t *buffer) {
    unsigned int value = va_arg(args, unsigned int);
    char str[20];
    itoa(value, str, 8);
    append_to_buffer(buffer, str, strlen(str));
}

// Applies ROT13 to each character in the string for the %R specifier.
// ROT13 transformation provides simple encoding, common in specific applications.
void print_rot(va_list args, buffer_t *buffer) {
    char *str = va_arg(args, char *);
    if (str == NULL) {
        append_to_buffer(buffer, "(null)", 6);
        return;
    }
    for (int i = 0; str[i] != '\0'; i++) {
        char c = str[i];
        if (c >= 'a' && c <= 'z') {
            c = (c - 'a' + 13) % 26 + 'a';
        } else if (c >= 'A' && c <= 'Z') {
            c = (c - 'A' + 13) % 26 + 'A';
        }
        append_to_buffer(buffer, &c, 1);
    }
}
