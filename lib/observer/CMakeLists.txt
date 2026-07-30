add_library(observer_lib STATIC
    observer.c
)

target_include_directories(observer_lib PUBLIC
    .
)
