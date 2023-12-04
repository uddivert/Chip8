# Chip8 Emulator

This is a simple Chip8 emulator written in C using the freeGlut for windowing

## Trip8 Demo
[![Alt text](https://img.youtube.com/vi/T6_-amiJ6_w/0.jpg)](https://www.youtube.com/watch?v=T6_-amiJ6_w)

## Build Instructions

1.  Clone the repository: `git clone <repository_url>`
2.  Navigate to the project directory: `cd chip8-emulator`
3.  Build the emulator: `make`

## Usage

To run the emulator, use the following command:

`./bin/chip8 -t -f <path_to_rom_file>` 

or 

`make test_name`

Options:

-   `-t` or `-T`: Enables the ncurses-based display for the emulator.
-   `-f <path_to_rom_file>` or `-F <path_to_rom_file>`: Load a Chip8 ROM file into the emulator for execution.

Tests:

-   `make corax`
-   `make flags`
-   ...


For example, if the ROM you are running expects the keys to be mapped to the above layout, you can use your keyboard's number keys (0-9) and letter keys (A-F) to interact with the emulator.

## ROMs

You can find Chip8 ROMs online. Multiple test roms are provided

## Issues 

Debugger currently has quirks. Uncomment debPrint and debInit to use.

Keypad doesn't work properly

No sound
