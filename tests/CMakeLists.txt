include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.tar.gz
)
FetchContent_MakeAvailable(googletest)

add_executable(${OUTPUT}
    ${MY_PROG}-test.cpp
)

target_link_libraries(${OUTPUT} PRIVATE
    ${MY_PROG}_lib
    gtest_main
)
