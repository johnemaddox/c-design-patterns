add_library(interface_lib STATIC
    intf.c
    example/camera_hw.c
    example/camera_sim.c
)

target_include_directories(interface_lib PUBLIC
    .
    example
)
