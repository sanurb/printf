#ifndef BUFFER_H
#define BUFFER_H

#include <stddef.h>
#include <stdio.h>

// Structure to represent a dynamic buffer.
typedef struct {
    char *data;    // Pointer to the buffer's data.
    size_t size;   // Current allocated size of the buffer.
    size_t used;   // Number of bytes currently used in the buffer.
} buffer_t;

// Initializes a buffer with the given initial size.
// Returns a pointer to the buffer_t structure or NULL on failure.
buffer_t *init_buffer(size_t initial_size);

// Appends a string of given length to the buffer, expanding it if necessary.
void append_to_buffer(buffer_t *buffer, const char *str, size_t len);

// Expands the buffer by a given length.
// This is called automatically when the buffer runs out of space.
void expand_buffer(buffer_t *buffer, size_t extra_len);

// Flushes the buffer's content to the given stream (e.g., stdout or a file).
void flush_buffer(buffer_t *buffer, FILE *stream);

// Frees the memory associated with the buffer.
void free_buffer(buffer_t *buffer);

#endif // BUFFER_H
