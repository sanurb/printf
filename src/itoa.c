#include <stdbool.h>
#include "../include/itoa.h"

// Reverses a string in place. This is necessary because digits are
// appended in reverse order during conversion.
static void reverse_string(char *str, int length) {
    int start = 0;
    int end = length - 1;
    while (start < end) {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
    }
}

// Converts an integer 'value' to a string in the specified 'base'.
// Supports base 10 (decimal) and base 16 (hexadecimal).
// Handles negative values only in base 10.
char *itoa(int value, char *str, int base) {
    int i = 0;
    bool is_negative = false;

    // Explicitly handle '0' case as it doesn't enter the loop below.
    if (value == 0) {
        str[i++] = '0';
        str[i] = '\0';
        return str;
    }

    // Only base 10 supports negative values; set flag and convert to positive.
    if (value < 0 && base == 10) {
        is_negative = true;
        value = -value;
    }

    // Convert the number to the specified base by extracting digits.
    while (value != 0) {
        int remainder = value % base;
        str[i++] = (remainder > 9) ? (remainder - 10) + 'a' : remainder + '0';
        value = value / base;
    }

    // Add negative sign for decimal numbers if needed.
    if (is_negative) {
        str[i++] = '-';
    }

    // Null-terminate before reversing the string to get the final result.
    str[i] = '\0';
    reverse_string(str, i);

    return str;
}
