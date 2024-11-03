#include <stdlib.h>
#include <string.h>
#include "../include/buffer.h"
#include "../include/error_handling.h"

// Initializes a buffer with a specified initial size, handling memory allocation for buffered output.
// Buffered output is essential for a custom printf to efficiently manage intermediate data
// before writing it in bulk, which is faster than writing byte-by-byte to the output stream.
buffer_t *init_buffer(size_t initial_size) {
    buffer_t *buffer = malloc(sizeof(buffer_t));
    if (!buffer) {
        handle_error(MEMORY_ALLOCATION_ERROR, "Failed to allocate buffer structure");
        return NULL;
    }

    // Allocate the memory for the actual data storage within the buffer.
    buffer->data = (char *)malloc(initial_size);
    if (!buffer->data) {
        free(buffer);  // Clean up the buffer structure if data allocation fails to avoid leaks.
        handle_error(MEMORY_ALLOCATION_ERROR, "Failed to allocate buffer data");
        return NULL;
    }

    buffer->size = initial_size;
    buffer->used = 0;

    return buffer;
}

// Appends data to the buffer, resizing as necessary to accommodate new data.
// Automatic expansion ensures that the buffer can handle any amount of data appended
// in a single operation, which is useful for format-heavy operations like printf that
// may generate output dynamically. This design minimizes reallocation overhead during appending.
void append_to_buffer(buffer_t *buffer, const char *str, size_t len) {
    // Ensure the buffer has enough space. Expanding in chunks reduces the number of reallocations
    // in scenarios with frequent appends, which is common in formatted output.
    if (buffer->used + len > buffer->size) {
        expand_buffer(buffer, len);
    }

    // Copy the provided data to the buffer's current position, updating the usage counter.
    // This is more efficient than direct output for printf, allowing us to handle the
    // entire output sequence at once in `flush_buffer`.
    memcpy(buffer->data + buffer->used, str, len);
    buffer->used += len;
}

// Expands the buffer size to accommodate at least `extra_len` more bytes.
// Typically, doubling the buffer size provides amortized efficiency by minimizing
// the frequency of reallocations as data grows. This is a common pattern in dynamic data structures.
void expand_buffer(buffer_t *buffer, size_t extra_len) {
    // Calculate the new buffer size, generally doubling to allow for exponential growth.
    const size_t new_size = buffer->size * 2 + extra_len;
    char *new_data = (char *)realloc(buffer->data, new_size);
    if (!new_data) {
        handle_error(MEMORY_ALLOCATION_ERROR, "Failed to expand buffer");
        return;  // Future operations may fail if buffer can't expand, but graceful handling helps avoid crashes.
    }

    buffer->data = new_data;
    buffer->size = new_size;
}

// Flushes the buffer content to the specified output stream (e.g., stdout).
// This function is crucial for output efficiency in a printf implementation,
// as it allows batching data and writing it all at once, significantly reducing
// the number of I/O operations compared to unbuffered output. Resets `used` to 0 after flushing.
void flush_buffer(buffer_t *buffer, FILE *stream) {
    // Write the entire contents of the buffer in one operation.
    // Bulk writing optimizes performance, especially for repeated printf operations.
    fwrite(buffer->data, 1, buffer->used, stream);
    buffer->used = 0;  // Clear the buffer for future data appends.
}

// Frees the memory allocated for the buffer, including the internal data.
// Proper cleanup is necessary to prevent memory leaks, especially in implementations
// where dynamic memory allocation is frequent and handling errors consistently is key.
void free_buffer(buffer_t *buffer) {
    if (buffer) {
        free(buffer->data);  // Free the data storage allocated within the buffer.
        free(buffer);        // Free the buffer structure itself.
    }
}
