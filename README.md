# Advanced Kernel with Keyboard and Screen Support

A modern, feature-rich kernel implementation written in C and Assembly, featuring comprehensive hardware support and advanced system capabilities.

## Features

- **Multiboot2 Compliant**: Compatible with GRUB and other multiboot-compliant bootloaders
- **VGA Text Mode Driver**: Full-featured 80x25 character display with color support
- **PS/2 Keyboard Driver**: Complete keyboard input handling with scancode translation
- **Memory Management**: Physical and virtual memory management with paging
- **Interrupt Handling**: Comprehensive interrupt descriptor table and exception handling
- **Dynamic Memory**: Kernel heap with malloc/free functionality
- **System Calls**: Basic system call interface for kernel services
- **Interactive Shell**: Built-in command-line interface for kernel interaction
- **Timer Support**: Programmable Interval Timer for scheduling and timing

## System Requirements

- **Architecture**: x86 (32-bit)
- **Bootloader**: GRUB or any Multiboot2-compliant bootloader
- **Memory**: Minimum 4MB RAM
- **Hardware**: VGA-compatible graphics, PS/2 keyboard

## Build Requirements

- GCC cross-compiler for i686-elf target
- GNU Make
- NASM assembler
- GRUB utilities (grub-mkrescue)
- xorriso (for ISO creation)

## Building

```bash
# Install dependencies (Ubuntu/Debian)
sudo apt-get install build-essential nasm grub-pc-bin grub-common xorriso

# Build the kernel
make all

# Create bootable ISO
make iso

# Clean build artifacts
make clean
```

## Running

### In QEMU
```bash
# Run with QEMU
qemu-system-i386 -cdrom kernel.iso

# Run with debugging
qemu-system-i386 -cdrom kernel.iso -s -S
```

### In VirtualBox
1. Create a new VM with at least 32MB RAM
2. Mount the `kernel.iso` file as a CD/DVD
3. Boot the VM

## Architecture

```
kernel/
├── src/
│   ├── boot/           # Bootloader and entry point
│   ├── kernel/         # Core kernel functionality
│   ├── drivers/        # Hardware drivers
│   └── include/        # Header files
├── build/              # Build artifacts
└── iso/               # ISO creation workspace
```

## Development

The kernel is structured in a modular way:

- **Boot**: Assembly bootloader stub and kernel entry
- **Kernel**: Core system functionality and main loop
- **Drivers**: Hardware abstraction layer
- **Memory**: Physical and virtual memory management
- **Interrupts**: Exception and interrupt handling
- **Shell**: User interface and command processing

## Contributing

1. Fork the repository
2. Create a feature branch
3. Make your changes
4. Test thoroughly
5. Submit a pull request

## License

This project is licensed under the MIT License - see the LICENSE file for details.

## Acknowledgments

- OSDev Wiki for comprehensive OS development documentation
- Multiboot2 specification
- x86 architecture references