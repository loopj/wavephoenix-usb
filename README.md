# WavePhoenix USB

WaveBird controller as a USB gamepad, using a WavePhoenix receiver and a Raspberry Pi Pico. Appears as a USB-HID gamepad with no additional drivers required on Windows, Linux, and macOS, compatible with emulators like Dolphin.

## Requirements

- A WavePhoenix receiver
- JST-SH 4-pin cable
- Raspberry Pi Pico

## Wiring

- Configure your WavePhoenix in UART mode (currently requires beta WavePhoenix firmware with UART support)
- Connect Pin 3 of the WavePhoenix JST-SH connector to Pin 1 (GP0 / UART0 TX) on the Pi Pico
- Connect Pin 4 of the WavePhoenix JST-SH connector to Pin 2 (GP1 / UART0 RX) on the Pi Pico

## Installation

- Download the latest firmware from the [releases page](https://github.com/loopj/wavephoenix-usb/releases)
- Enter bootloader mode on the Raspberry Pi Pico by holding the BOOTSEL button while plugging it into your computer
- Copy the downloaded firmware file to the Raspberry Pi Pico storage
- Reboot the Raspberry Pi Pico
- The WavePhoenix should now be recognized as a USB-HID gamepad

## Using with Dolphin

- Open Dolphin and go to `Controllers`
- Click on `Configure` under `GameCube Controllers`
- Select `SDL/0/WavePhoenix USB Gamepad` from the dropdown menu
- Configure the buttons as desired

You can find a pre-configured Dolphin profile in the ['dolphin' directory](dolphin/) of this repository.

## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details
