# Makefile for bootloader and kernel

.PHONY: all clean run

C_SOURCES = $(wildcard kernel/*.c)
C_OBJECTS = $(patsubst kernel/%.c, build/%.o, $(C_SOURCES))
CC = gcc
CFLAGS = -m32 -fno-pie -ffreestanding -fno-builtin -nostdlib -nostdinc -Wall -Wextra

all: build/os-image

build:
	mkdir -p build

# Build the kernel binary
build/kernel.bin: build/kernel_entry.o ${C_OBJECTS}
	ld -m elf_i386 -o $@ -Ttext 0x1000 $^ --oformat binary

# Compile C kernel files
build/%.o: kernel/%.c build
	${CC} ${CFLAGS} -c $< -o $@

# Compile kernel entry assembly file
build/kernel_entry.o: kernel/kernel_entry.asm build
	nasm $< -f elf -o $@

# Build the bootloader
build/boot.bin: bootloader/boot.asm build
	nasm -f bin $< -o $@

# Combine bootloader and kernel into a single disk image
build/os-image: build/boot.bin build/kernel.bin
	cat $^ > $@

run: build/os-image
	qemu-system-i386 -fda $<

clean:
	rm -rf build