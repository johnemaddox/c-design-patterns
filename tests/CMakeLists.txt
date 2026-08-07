if(POLICY CMP0135)
  cmake_policy(SET CMP0135 NEW)
endif()

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

# Dynamically add test executables if the test source file exists
file(GLOB lib_subdirs RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}/../lib ../lib/*)
foreach(subdir ${lib_subdirs})
    if(IS_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/../lib/${subdir})
        if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${subdir}-test.cpp)
            add_executable(${subdir} ${subdir}-test.cpp)
            target_link_libraries(${subdir} PRIVATE ${subdir}_lib gtest_main)
        endif()
    endif()
endforeach()
