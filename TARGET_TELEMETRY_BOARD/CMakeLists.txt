add_library(mbed-telemetry-board INTERFACE)

target_sources(mbed-telemetry-board
    INTERFACE
        PeripheralPins.c
        system_clock.c
)

target_include_directories(mbed-telemetry-board
    INTERFACE
        .
)

target_link_libraries(mbed-telemetry-board INTERFACE mbed-stm32g473xe)

