add_library(callback_queue_lib STATIC
    callback_queue.c
)

target_include_directories(callback_queue_lib PUBLIC
    .
    ../ring_buffer
)

target_link_libraries(callback_queue_lib PUBLIC
    ring_buffer_lib
)
