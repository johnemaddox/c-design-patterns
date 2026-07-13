add_library(interface_lib STATIC
    intf.c
    camera_one.c
    camera_two.c
)

target_include_directories(interface_lib PUBLIC
    .
)
