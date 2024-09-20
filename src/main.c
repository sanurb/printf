#include "../include/printf.h"

int main() {
    initialize_printf();  // Initialize resources if needed.

    // Test multiple format specifiers in a single string.
    my_printf("String: %s\n", "Hello World!");
    my_printf("Char: %c\n", 'A');
    my_printf("Integer: %i\n", 123);
    my_printf("Binary: %b\n", 42);
    my_printf("Hex (lower): %x\n", 255);
    my_printf("Hex (upper): %X\n", 255);
    my_printf("Octal: %o\n", 123);
    my_printf("ROT13: %R\n", "Hello World!");

    cleanup_printf();

    return 0;
}