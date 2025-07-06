# Set minimum CMake version
cmake_minimum_required(VERSION "3.21")

# Include the Pico SDK
include(pico_sdk_import.cmake)

# Configure project and languages
project(c C CXX ASM)

# CMake target for the WavePhoenix USB firmware
set(TARGET wavephoenix-usb)

# Initialize the Pico SDK
pico_sdk_init()

# Create the target and set the sources
add_executable(${TARGET}
    src/main.c
    src/usb_descriptors.c
)

# Specify the include paths
target_include_directories(${TARGET} PUBLIC src)

# Depend on the Pico SDK and TinyUSB
target_link_libraries(${TARGET} PUBLIC pico_stdlib tinyusb_device tinyusb_board)

# Enable printf over USB
pico_enable_stdio_usb(${TARGET} 1)
pico_enable_stdio_uart(${TARGET} 0)

# Generate UF2 file
pico_add_extra_outputs(${TARGET})