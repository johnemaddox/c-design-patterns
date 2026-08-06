add_library(callback_table_lib STATIC
    callback_table.c
)

target_include_directories(callback_table_lib PUBLIC
    .
)
