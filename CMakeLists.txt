cmake_minimum_required(VERSION 3.16.3)

find_program(CLANG_TIDY_EXE NAMES "clang-tidy")
if(CLANG_TIDY_EXE)
    set(CMAKE_C_CLANG_TIDY ${CLANG_TIDY_EXE})
    set(CMAKE_CXX_CLANG_TIDY ${CLANG_TIDY_EXE})
else()
    message(STATUS "clang-tidy not found; skipping static analysis during compilation.")
endif()

project(common LANGUAGES C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 14)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(CMAKE_BUILD_TYPE Debug)

add_compile_options(-Wall -Werror -Wextra)

# Dynamically find and add all subdirectories in lib/
file(GLOB lib_subdirs RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}/lib lib/*)
foreach(subdir ${lib_subdirs})
    if(IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/lib/${subdir})
        add_subdirectory(lib/${subdir})
    endif()
endforeach()

add_subdirectory(tests)
