#include <stdio.h>
#include <stdlib.h>
#include "../include/error_handling.h"

// Pointer to the custom error handler, if any.
static void (*global_error_handler)(int, const char *) = NULL;

// Default error handler that prints the error message and exits the program.
void handle_error(int error_code, const char *message) {
    if (global_error_handler) {
        // If a custom error handler is registered, invoke it.
        global_error_handler(error_code, message);
    } else {
        // Default behavior: print the error and exit.
        fprintf(stderr, "Error [%d]: %s\n", error_code, message);
        exit(error_code);
    }
}

// Allows registering a custom error handler.
void register_error_handler(void (*handler)(int, const char *)) {
    global_error_handler = handler;
}
