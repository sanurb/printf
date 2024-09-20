#include "../include/printf.h"

int main() {
    initialize_printf();  // Initialize resources if needed.

    // Test integer output
    my_printf("Integer: %d\n", 123);

    // Test string output
    my_printf("String: %s\n", "Hello, World!");

    // Test hexadecimal output
    my_printf("Hexadecimal: %x\n", 255);

    // Test more complex cases
    //my_printf("Multiple formats: %d, %s, %x\n", 42, "test", 255);

    cleanup_printf();  // Cleanup resources if needed.

    return 0;
}