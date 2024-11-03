#include <stdio.h>
#include <stdlib.h>
#include "../include/error_handling.h"

// Global pointer to a user-defined error handler function.
// Allows for flexibility in error handling by enabling custom behaviors.
static void (*global_error_handler)(int, const char *) = NULL;

// Default error handler that either invokes a custom handler, if one is set, or
// prints an error message to stderr and terminates the program. This design ensures
// that critical errors are addressed without risk of the program continuing in an invalid state.
void handle_error(int error_code, const char *message) {
    if (global_error_handler) {
        // Use the custom handler if registered, allowing the user to control error response.
        global_error_handler(error_code, message);
    } else {
        // Fallback: log the error message and exit to prevent undefined behavior.
        fprintf(stderr, "Error [%d]: %s\n", error_code, message);
        exit(error_code);
    }
}

// Allows external code to register a custom error handler.
// This design provides flexibility in managing errors, making the printf implementation
// adaptable for different use cases where default error handling may not be ideal.
void register_error_handler(void (*handler)(int, const char *)) {
    global_error_handler = handler;
}
