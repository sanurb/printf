#ifndef ERROR_HANDLING_H
#define ERROR_HANDLING_H

// Error codes.
#define MEMORY_ALLOCATION_ERROR 1
#define INVALID_FORMAT 2

// Function prototype for handling errors.
void handle_error(int error_code, const char *message);

// Function to register custom error handlers (optional).
void register_error_handler(void (*handler)(int, const char *));

#endif // ERROR_HANDLING_H
