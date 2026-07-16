include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.tar.gz
)
FetchContent_MakeAvailable(googletest)

# Disable clang-tidy on GoogleTest targets
if(CLANG_TIDY_EXE)
    set_target_properties(gtest gtest_main gmock gmock_main PROPERTIES
        C_CLANG_TIDY ""
        CXX_CLANG_TIDY ""
    )
endif()

add_executable(${MY_OUTPUT}
    ${MY_PROG}-test.cpp
)

target_link_libraries(${MY_OUTPUT} PRIVATE
    ${MY_PROG}_lib
    gtest_main
)
