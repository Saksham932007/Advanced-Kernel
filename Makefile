# Compiler settings
CC = gcc
AS = nasm
LD = ld

# Directories
SRC_DIR = src
BUILD_DIR = build
ISO_DIR = iso
INCLUDE_DIR = $(SRC_DIR)/include

# Compiler flags
CFLAGS = -std=gnu99 -ffreestanding -O2 -Wall -Wextra -I$(INCLUDE_DIR) -m32 -fno-pie -fno-stack-protector
ASFLAGS = -f elf32
LDFLAGS = -T linker.ld -m elf_i386 -nostdlib

# Source files
ASM_SOURCES = $(wildcard $(SRC_DIR)/boot/*.asm)
C_SOURCES = $(wildcard $(SRC_DIR)/kernel/*.c) $(wildcard $(SRC_DIR)/drivers/*.c)

# Object files
ASM_OBJECTS = $(ASM_SOURCES:$(SRC_DIR)/%.asm=$(BUILD_DIR)/%.o)
C_OBJECTS = $(C_SOURCES:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o)
OBJECTS = $(ASM_OBJECTS) $(C_OBJECTS)

# Target
KERNEL = $(BUILD_DIR)/kernel.bin
ISO = kernel.iso

# Default target
all: $(KERNEL)

# Create build directories
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)/boot $(BUILD_DIR)/kernel $(BUILD_DIR)/drivers

# Compile assembly files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.asm | $(BUILD_DIR)
	$(AS) $(ASFLAGS) $< -o $@

# Compile C files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Link kernel
$(KERNEL): $(OBJECTS)
	$(LD) $(LDFLAGS) $(OBJECTS) -o $@

# Create ISO
iso: $(KERNEL)
	mkdir -p $(ISO_DIR)/boot/grub
	cp $(KERNEL) $(ISO_DIR)/boot/kernel.bin
	cp grub.cfg $(ISO_DIR)/boot/grub/grub.cfg
	grub-mkrescue -o $(ISO) $(ISO_DIR)

# Run in QEMU
run: $(ISO)
	qemu-system-i386 -cdrom $(ISO)

# Debug in QEMU
debug: $(ISO)
	qemu-system-i386 -cdrom $(ISO) -s -S

# Clean
clean:
	rm -rf $(BUILD_DIR) $(ISO_DIR) $(ISO)

# Rebuild
rebuild: clean all

.PHONY: all iso run debug clean rebuild