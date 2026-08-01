add_library(interface_lib STATIC
    camera_intf.c
    example/camera_hw.c
    example/camera_sim.c
    sensor_intf.c
    example/sensor_hw.c
    example/sensor_sim.c
)

target_include_directories(interface_lib PUBLIC
    .
    example
)
