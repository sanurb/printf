# Custom `printf` Implementation in C

## Project Overview

This project is a custom implementation of the well-known `printf` function in C, aimed at providing an educational exploration of the inner workings of formatted output in C. The goal is to "reinvent the wheel" by building `printf` from scratch, handling format specifiers, variable arguments, and output to various streams.

While `printf` is part of the C standard library, this project allows us to deeply understand:
- The handling of variable-length argument lists (`va_list`).
- Format string parsing (e.g., `%d`, `%s`, `%x`, etc.).
- Custom handling of format specifiers and how they can be extended or altered.
- Buffer management and efficient output handling.

The project uses advanced C techniques, such as function pointers and hashmaps, to maintain modularity, scalability, and efficiency. This implementation adheres strictly to the ISO C standard.

## Features

- **Supports Multiple Format Specifiers**:
    - `%s` - String.
    - `%d`, `%i` - Integer (decimal).
    - `%x`, `%X` - Hexadecimal (lowercase and uppercase).
    - `%o` - Octal.
    - `%c` - Character.
    - `%p` - Pointer.
    - `%%` - Escape for literal `%`.
- **Handles Edge Cases**:
    - Unrecognized specifiers, such as `%z`, are managed gracefully.
    - Supports printing of null pointers (`(null)` output for `NULL`).
    - Dynamic buffer handling ensures efficient memory usage.
- **Modular Design**:
    - Format specifier handlers are dynamically registered in a hashmap.
    - Easy to extend with new format specifiers or custom functionality.

## Purpose of the Project

This project is **educational** in nature. By building a custom `printf`, we aim to:
- Deepen our understanding of C, specifically how formatted output works under the hood.
- Reinforce concepts like pointers, function pointers, and variable argument lists.
- Learn how to design modular, reusable code that is scalable for larger systems.
- Practice efficient memory management through dynamic buffers and error handling.

## Project Structure

```bash
.
├── CMakeLists.txt                   # CMake configuration file for the project.
├── include/                         # Header files for all modules.
│   ├── buffer.h                     # Buffer management functions.
│   ├── error_handling.h             # Error handling functions and constants.
│   ├── format_parser.h              # Functions for parsing format specifiers.
│   ├── hashmap.h                    # Hashmap implementation for storing format handlers.
│   ├── itoa.h                       # Integer to ASCII conversion functions.
│   ├── printf.h                     # Main header for custom `my_printf` implementation.
│   ├── vfprintf.h                   # Declarations for formatted output functions (like `vfprintf`).
├── src/                             # Source files implementing project functionality.
│   ├── buffer.c                     # Buffer management implementation.
│   ├── error_handling.c             # Error handling implementation.
│   ├── format_parser.c              # Parsing and processing format specifiers.
│   ├── hashmap.c                    # Hashmap implementation to store and retrieve handlers.
│   ├── itoa.c                       # Implementation of integer to ASCII conversion (`itoa`).
│   ├── main.c                       # Main entry point for testing `my_printf` functionality.
│   ├── printf.c                     # Implementation of `my_printf` and related functions.
│   ├── vfprintf.c                   # Core logic for formatting and outputting to streams.
├── tests/                           # Unit tests for various modules.
│   ├── test_buffer.c                # Unit tests for buffer management functions.
│   ├── test_format_parser.c         # Unit tests for format specifier parsing.
```


## Requirements

To build and run this project, you need the following tools:

- A **C11-compatible compiler** (e.g., GCC, Clang).
- **CMake** (version 3.10 or higher) for building the project.
- **Make** or any other build system supported by CMake.