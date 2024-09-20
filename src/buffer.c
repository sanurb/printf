#include <stdlib.h>
#include <string.h>
#include "../include/buffer.h"
#include "../include/error_handling.h"

// Initializes a buffer with the specified initial size.
// Returns a pointer to the buffer, or NULL if allocation fails.
buffer_t *init_buffer(size_t initial_size) {
    buffer_t *buffer = (buffer_t *)malloc(sizeof(buffer_t));
    if (!buffer) {
        handle_error(MEMORY_ALLOCATION_ERROR, "Failed to allocate buffer structure");
        return NULL;
    }

    buffer->data = (char *)malloc(initial_size);
    if (!buffer->data) {
        free(buffer);  // Free the buffer structure if data allocation fails.
        handle_error(MEMORY_ALLOCATION_ERROR, "Failed to allocate buffer data");
        return NULL;
    }

    buffer->size = initial_size;
    buffer->used = 0;

    return buffer;
}

// Appends the given string (or data) to the buffer.
// Expands the buffer automatically if there is not enough space.
void append_to_buffer(buffer_t *buffer, const char *str, size_t len) {
    // Check if the buffer has enough space, and expand it if necessary.
    if (buffer->used + len > buffer->size) {
        expand_buffer(buffer, len);
    }

    // Copy the data to the buffer and update the used size.
    memcpy(buffer->data + buffer->used, str, len);
    buffer->used += len;
}

// Expands the buffer to accommodate at least extra_len more bytes.
// The buffer size is usually doubled to minimize reallocations.
void expand_buffer(buffer_t *buffer, size_t extra_len) {
    size_t new_size = buffer->size * 2 + extra_len;
    char *new_data = (char *)realloc(buffer->data, new_size);
    if (!new_data) {
        handle_error(MEMORY_ALLOCATION_ERROR, "Failed to expand buffer");
        return;  // Graceful handling, but future operations may fail if expansion is critical.
    }

    buffer->data = new_data;
    buffer->size = new_size;
}

// Flushes the buffer to the specified stream (e.g., stdout or a file).
// After flushing, the buffer is cleared (used size is reset to 0).
void flush_buffer(buffer_t *buffer, FILE *stream) {
    // Write the buffer contents to the stream.
    fwrite(buffer->data, 1, buffer->used, stream);
    buffer->used = 0;  // Reset the used size after flushing.
}

// Frees the memory associated with the buffer, including its data.
void free_buffer(buffer_t *buffer) {
    if (buffer) {
        free(buffer->data);  // Free the data inside the buffer.
        free(buffer);        // Free the buffer structure itself.
    }
}
