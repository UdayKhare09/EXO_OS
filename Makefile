# Makefile for EXO_OS

# Tools
ASM=nasm
CC=gcc
LD=ld
QEMU=qemu-system-x86_64

# Flags
ASMFLAGS=-f elf32
CFLAGS=-m32 -nostdlib -nostdinc -fno-builtin -fno-stack-protector -ffreestanding -Wall -Wextra
LDFLAGS=-m elf_i386 -T link.ld

# Directories
BUILD=build
BOOTLOADER=bootloader
KERNEL=kernel

# Files
BOOT_SRC=$(BOOTLOADER)/boot.asm
KERNEL_C_SRCS=$(wildcard $(KERNEL)/*.c)
KERNEL_OBJ=$(patsubst $(KERNEL)/%.c,$(BUILD)/%.o,$(KERNEL_C_SRCS))

# Output files
BOOTLOADER_BIN=$(BUILD)/boot.bin
KERNEL_BIN=$(BUILD)/kernel.bin
OS_IMAGE=$(BUILD)/exos.img

all: dirs $(OS_IMAGE)

dirs:
	mkdir -p $(BUILD)

# Create the OS image by concatenating boot sector and kernel
$(OS_IMAGE): $(BOOTLOADER_BIN) $(KERNEL_BIN)
	cat $(BOOTLOADER_BIN) $(KERNEL_BIN) > $(OS_IMAGE)
	# Pad the image to make it bootable
	dd if=/dev/zero bs=512 count=2880 >> $(OS_IMAGE)
	dd if=$(OS_IMAGE) of=$(OS_IMAGE) bs=512 count=2880 conv=notrunc

# Compile bootloader
$(BOOTLOADER_BIN): $(BOOT_SRC)
	$(ASM) -f bin -o $@ $<

# Compile kernel object files
$(BUILD)/%.o: $(KERNEL)/%.c
	$(CC) $(CFLAGS) -c -o $@ $<

# Link kernel - update to create a flat binary
$(KERNEL_BIN): $(KERNEL_OBJ)
	$(LD) $(LDFLAGS) -o $(BUILD)/kernel.elf $^
	objcopy -O binary $(BUILD)/kernel.elf $(KERNEL_BIN)

# Run in QEMU
run: $(OS_IMAGE)
	$(QEMU) -drive format=raw,file=$(OS_IMAGE)

clean:
	rm -rf $(BUILD)

.PHONY: all dirs run clean