cmake_minimum_required(VERSION 3.16.3)

## Dependencies ##

find_program(CLANG_TIDY_EXE NAMES "clang-tidy")
if(CLANG_TIDY_EXE)
    set(CMAKE_C_CLANG_TIDY ${CLANG_TIDY_EXE})
    set(CMAKE_CXX_CLANG_TIDY ${CLANG_TIDY_EXE})
else()
    message(STATUS "clang-tidy not found; skipping static analysis during compilation.")
endif()

find_program(LLD_EXE NAMES "ld.lld" "lld")
if(LLD_EXE)
    add_link_options("-fuse-ld=lld")
    message(STATUS "Using LLVM Linker (LLD): ${LLD_EXE}")
else()
    message(STATUS "LLD linker not found; using default system linker.")
endif()

find_library(LLVM_LIBC_LIB NAMES llvmlibc PATHS /usr/lib/llvm-18/lib /usr/lib/llvm-19/lib /usr/lib)
if(LLVM_LIBC_LIB)
    message(STATUS "Found LLVM libc: ${LLVM_LIBC_LIB}")
    link_libraries(${LLVM_LIBC_LIB})
else()
    message(WARNING "LLVM libc static library (llvmlibc) not found. System default libc will be used.")
endif()

## Project ##

project(design_patterns LANGUAGES C CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 14)

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
