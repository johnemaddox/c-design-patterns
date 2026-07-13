add_library(state_machine_lib STATIC
    state_machine.c
    traffic_light.c
)

target_include_directories(state_machine_lib PUBLIC
    .
)
