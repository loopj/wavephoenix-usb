# WavePhoenix USB Firmware

## Prerequisites

- Install CMake, Python3, and GCC toolchain for ARM
- Install Pico SDK
- Set `PICO_SDK_PATH` environment variable

## Building

Run the following commands to build the firmware:

```bash
cmake -B build -DPICO_BOARD=pico
cmake --build build
```

Replace `pico` with your board type if you are using a different Raspberry Pi Pico variant, e.g. `pico2` for Raspberry Pi Pico 2.

## Flashing

- Enter bootloader mode on the Pi Pico
- Copy the generated `uf2` file from the `build` directory to the Pi Pico
