# Building and Testing Guide

## Prerequisites

### Required Tools

1. **GCC**: C compiler for building kernel code
2. **NASM**: Netwide Assembler for assembly code
3. **GNU Make**: Build automation
4. **LD**: GNU linker for linking object files

### Optional Tools (for testing)

1. **QEMU**: x86 emulator for testing the kernel
2. **VirtualBox**: Virtual machine for testing
3. **GRUB**: For creating bootable ISOs (requires grub-mkrescue)

## Building the Kernel

### Basic Build

```bash
# Clone or download the kernel source code
cd /path/to/kernel

# Build the kernel binary
make all

# The kernel binary will be created as build/kernel.bin
```

### Creating a Bootable ISO (requires GRUB tools)

```bash
# Install GRUB tools (Ubuntu/Debian)
sudo apt-get install grub-pc-bin grub-common xorriso

# Create bootable ISO
make iso

# This creates kernel.iso which can be booted in VMs
```

### Clean Build

```bash
# Remove all build artifacts
make clean

# Rebuild everything from scratch
make rebuild
```

## Testing the Kernel

### Method 1: QEMU (Recommended)

```bash
# Install QEMU (Ubuntu/Debian)
sudo apt-get install qemu-system-x86

# Test kernel binary directly
qemu-system-i386 -kernel build/kernel.bin

# Or boot from ISO
qemu-system-i386 -cdrom kernel.iso

# Debug with GDB
qemu-system-i386 -kernel build/kernel.bin -s -S
```

### Method 2: VirtualBox

1. Create a new VM:
   - Type: Other
   - Version: Other/Unknown
   - Memory: 32MB minimum
   - No hard disk needed

2. Mount the ISO:
   - Go to VM settings
   - Storage tab
   - Add optical drive
   - Select kernel.iso

3. Boot the VM

### Method 3: Real Hardware (Advanced)

**Warning**: Only attempt this if you understand the risks!

```bash
# Write ISO to USB drive (replace /dev/sdX with your USB device)
sudo dd if=kernel.iso of=/dev/sdX bs=4M status=progress sync

# Boot from USB on target machine
```

## What to Expect

When the kernel boots successfully, you should see:

1. **Boot Screen**: Multiboot2 compliant loading
2. **Initialization Messages**: System components loading
3. **Shell Prompt**: Interactive command line interface

### Available Shell Commands

- `help` - Show all available commands
- `clear` - Clear the screen
- `echo <text>` - Print text to screen
- `version` - Show kernel version and features
- `uptime` - Show system uptime
- `cpuinfo` - Display CPU information
- `meminfo` - Show memory information
- `halt` - Halt the system
- `reboot` - Restart (not fully implemented)

### Testing Features

1. **VGA Driver**: Text should display with colors
2. **Keyboard**: Type commands and see responses
3. **Interrupts**: System should respond to keyboard input
4. **Timer**: `uptime` command should show increasing time
5. **Shell**: Commands should execute and show results

## Troubleshooting

### Build Issues

1. **Missing compiler**: Install GCC and development tools
2. **NASM not found**: Install NASM assembler
3. **Linker errors**: Ensure you have GNU binutils

### Runtime Issues

1. **Black screen**: VGA driver may not be working
2. **No keyboard response**: IRQ/keyboard driver issue
3. **System hang**: Check interrupt handling
4. **Boot failure**: Verify multiboot2 compliance

### Common Build Commands

```bash
# Verbose build to see all commands
make V=1 all

# Check if binary was created
ls -la build/kernel.bin

# Check binary size (should be reasonable, not too large)
file build/kernel.bin

# Verify it's a valid x86 binary
objdump -f build/kernel.bin
```

## Development Tips

1. **Start Simple**: Test basic boot first
2. **Use QEMU**: Faster iteration than VMs
3. **Debug Output**: Use VGA output for debugging
4. **Modular Testing**: Test each component separately
5. **Version Control**: Use git to track changes

## Performance Expectations

- **Boot Time**: Should boot in under 1 second in QEMU
- **Memory Usage**: ~1MB kernel size
- **Responsiveness**: Immediate keyboard response
- **Stability**: Should run indefinitely without crashes

This kernel demonstrates fundamental OS concepts and can serve as a foundation for more advanced operating system development.