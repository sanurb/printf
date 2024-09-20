#include <stdbool.h>
#include "../include/itoa.h"

// Helper function to reverse a string in place.
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

// Converts an integer 'value' to a string representation in the given 'base'.
// Supports both decimal (base 10) and hexadecimal (base 16) conversion.
char *itoa(int value, char *str, int base) {
    int i = 0;
    bool is_negative = false;

    // Handle 0 explicitly to avoid returning an empty string.
    if (value == 0) {
        str[i++] = '0';
        str[i] = '\0';
        return str;
    }

    // In base 10, handle negative integers.
    if (value < 0 && base == 10) {
        is_negative = true;
        value = -value;
    }

    // Process individual digits.
    while (value != 0) {
        int remainder = value % base;
        str[i++] = (remainder > 9) ? (remainder - 10) + 'a' : remainder + '0';
        value = value / base;
    }

    // If the number was negative, append the minus sign.
    if (is_negative) {
        str[i++] = '-';
    }

    // Null-terminate the string.
    str[i] = '\0';

    // Reverse the string to get the correct result.
    reverse_string(str, i);

    return str;
}