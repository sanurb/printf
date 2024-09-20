#include <assert.h>
#include <string.h>
#include "../include/buffer.h"

void test_buffer_initialization() {
    buffer_t *buffer = init_buffer(10);
    assert(buffer != NULL);
    assert(buffer->size == 10);
    assert(buffer->used == 0);
    free_buffer(buffer);
}

void test_append_to_buffer() {
    buffer_t *buffer = init_buffer(5);
    append_to_buffer(buffer, "abc", 3);
    assert(buffer->used == 3);
    assert(strncmp(buffer->data, "abc", 3) == 0);
    free_buffer(buffer);
}

void test_buffer_expansion() {
    buffer_t *buffer = init_buffer(5);
    append_to_buffer(buffer, "abcde", 5);
    append_to_buffer(buffer, "f", 1);
    assert(buffer->used == 6);
    assert(strncmp(buffer->data, "abcdef", 6) == 0);
    free_buffer(buffer);
}

int main() {
    test_buffer_initialization();
    test_append_to_buffer();
    test_buffer_expansion();

    return 0;
}
