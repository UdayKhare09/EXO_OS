# EXO_OS 🚀

## Overview
EXO_OS is a simple 32-bit operating system developed from scratch. It features a basic kernel, a bootloader, and essential system functionality.

## Features ✨
- Custom bootloader that switches from 16-bit real mode to 32-bit protected mode
- Terminal with VGA text mode support
- Keyboard input handling
- Basic stdio implementation (printf/println)

## Building 🔨
To build EXO_OS, simply run:
```bash
make
```

This will create the bootable OS image at `build/exos.img`.

## Running 🖥️
You can run EXO_OS in QEMU:
```bash
make run
```

## Project Structure 📁
- `bootloader/` - Contains the bootloader assembly code
- `kernel/` - Contains the C kernel code
- `build/` - Output directory for compiled files
- `Makefile` - Build configuration
- `link.ld` - Linker script

## System Requirements 🔧
- NASM assembler
- GCC compiler (32-bit support)
- QEMU for testing

## Technical Details 🔍
- Boots into 32-bit protected mode
- Uses a flat memory model
- Implements basic I/O through VGA text buffer and PS/2 keyboard

## Future Improvements 🔮
- Memory management system
- Process scheduling
- File system support
- Network stack
- Graphics mode support

## License 📝
This project is open source and available for learning purposes.