#ifndef FORMAT_PARSER_H
#define FORMAT_PARSER_H

#include <stdarg.h>
#include <stdbool.h>
#include "buffer.h"

// Constants for format specifier lengths and other important values.
#define FORMAT_SPECIFIER_START '%'
#define DEFAULT_HASHMAP_CAPACITY 16  // Initial hashmap capacity.
#define INVALID_SPECIFIER_LENGTH 1  // Default length for invalid specifiers.
#define MAX_SPECIFIER_LENGTH 2  // Maximum length of a valid format specifier, including '%'.

// Structure to hold information about a parsed format specifier.
typedef struct {
    bool valid;  // Indicates if the format specifier is valid.
    char specifier;  // The format specifier character (e.g., 'd', 's').
    int length;  // The length of the parsed format specifier (e.g., '%d' is 2 characters long).
    void (*handler)(va_list args, buffer_t *buffer);  // Function to handle the format specifier.
} format_info_t;

// Typedef for a function pointer that handles a specific format specifier.
typedef void (*format_handler_t)(va_list args, buffer_t *buffer);

// Wrapper structure to store format handlers in the hashmap.
typedef struct {
    format_handler_t handler;  // Directly store the function pointer here.
} function_wrapper_t;

// Initializes the hashmap of format specifiers. Should be called before using the parser.
void initialize_format_specifiers(void);

// Cleans up the hashmap and any other resources used by the parser.
void cleanup_format_specifiers(void);

// Parses the format string starting at a '%' character and returns information about the specifier.
format_info_t parse_format(const char *format);

// Registers a format specifier and its corresponding handler function in the hashmap.
void register_specifier(char specifier, format_handler_t handler);

// Retrieves the handler function for a specific format specifier from the hashmap.
format_handler_t get_format_handler(char specifier);

#endif // FORMAT_PARSER_H
