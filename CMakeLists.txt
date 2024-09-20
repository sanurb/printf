cmake_minimum_required(VERSION 3.29)
project(printf C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Werror")

include_directories(include)

set(SRC_FILES
        src/printf.c
        src/format_parser.c
        src/buffer.c
        src/itoa.c
        src/hashmap.c
        src/error_handling.c
        src/vfprintf.c
)

add_executable(main src/main.c ${SRC_FILES})

add_executable(test_format_parser tests/test_format_parser.c ${SRC_FILES})
add_executable(test_buffer tests/test_buffer.c ${SRC_FILES})

enable_testing()

add_test(NAME TestFormatParser COMMAND test_format_parser)
add_test(NAME TestBuffer COMMAND test_buffer)

add_custom_target(run_tests
        COMMAND ${CMAKE_CTEST_COMMAND} --verbose
        DEPENDS test_format_parser test_buffer
)