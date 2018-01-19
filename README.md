# Trollbook
*Homebrew 68040 palmtop computer* (http://h4xxel.org/electronics/trollbook/)

This is a work-in-progress palmtop (ish) sized clamshell laptop that we've been
working on for a while.

In its current state, it boots up into a BIOS menu capable of doing
a few simple things like browsing a FAT formatted SD-card, flashing its bootrom
from SD-card and excuting programs from SD-card.
One of the included demo programs can play .MOD music files from SD card.

The entire setup uses about 4.3 Watts while running, including
display and backlight.

The hardware specs as of now can be summed up as following:

 - Motorola 68040FE33V CPU, running at 33 MHz
 - Altera Flex10KA FPGA for glue logic and peripherals (video, uart, spi, audio etc.)
 - Atmel attiny48 for scanning keyboard matrix and mouse, interfacing to I2C peripherals
 - 512 kB of SRAM (used for frame buffer, sound buffer and stage 2 BIOS code)
 - 64 MB of SDRAM
 - 800x480 resolution 7" TFT display, driven at 256 colors (from a 16-bpp palette)
 - Stereo audio DAC, 8 bit/channel, samplerate of 48 kHz
 - SD card interface over SPI
 - On-board SPI Flash ROM (512 kB) for BIOS code
 - 7.4 V, 2.2 Ah battery pack with custom balancing circuitry (which sucks)

The (blue) mainboard is a 10x10 cm 4-layer board, designed by us and manufactured by a Chinese PCB fab.
The larger upper board is a CNC milled peripheral board sporting the keyboard, mouse, IO ports and power supplies.
We desperately need to make a proper case...

## This repository
This repository contains the following:
 - Hardware schematics and layout (kicad)
   - Current Rev 2 motherboard
   - Peripheral board
 - VHDL code for the glue logic/peripheral controller FPGA
 - Source code
   - Stage 1 boot, resides in a small ROM block in the FPGA
   - Stage 2 bios, resides in the large SPI rom
   - Demo code for feature testing/demonstration

## Companion projects
The following projects have been developed for the system:
 - The m68k-emu system emulator (https://github.com/slaeshjag/m68k-emu)
 - The trollos operating system (https://github.com/h4xxel/trollos)
