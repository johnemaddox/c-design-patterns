add_library(callback_lib STATIC
    callback.c
)

target_include_directories(callback_lib PUBLIC
    .
)
