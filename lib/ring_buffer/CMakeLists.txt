add_library(ring_buffer_lib STATIC
    ring_buffer.c
)

target_include_directories(ring_buffer_lib PUBLIC
    .
)
