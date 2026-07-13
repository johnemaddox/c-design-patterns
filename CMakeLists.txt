cmake_minimum_required(VERSION 3.16.3)

project(common LANGUAGES C CXX)

set(LIB_DIR lib)
set(OUTPUT "test.out")

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 14)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_compile_options(-Wall -Werror -Wextra)

message(STATUS "Building ${MY_PROG} in ${OUTPUT}")

add_subdirectory(${LIB_DIR}/${MY_PROG})
add_subdirectory(tests)
