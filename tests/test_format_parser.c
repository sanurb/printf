#include <assert.h>
#include "../include/format_parser.h"
#include "../include/buffer.h"

void test_valid_integer_format() {
    initialize_format_specifiers();

    format_info_t info = parse_format("%d");
    assert(info.valid);
    assert(info.specifier == 'd');
    assert(info.length == 2);

    cleanup_format_specifiers();
}

void test_valid_string_format() {
    initialize_format_specifiers();

    format_info_t info = parse_format("%s");
    assert(info.valid);
    assert(info.specifier == 's');
    assert(info.length == 2);

    cleanup_format_specifiers();
}

void test_invalid_format() {
    initialize_format_specifiers();

    format_info_t info = parse_format("%z");
    assert(!info.valid);
    assert(info.length == 1);

    cleanup_format_specifiers();
}

int main() {
    test_valid_integer_format();
    test_valid_string_format();
    test_invalid_format();

    return 0;
}
