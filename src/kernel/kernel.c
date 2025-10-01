#include "kernel.h"
#include "vga.h"
#include "gdt.h"
#include "idt.h"
#include "irq.h"
#include "keyboard.h"
#include "timer.h"
#include "shell.h"

// Basic utility functions
void* memset(void* dest, int c, size_t n) {
    unsigned char* ptr = (unsigned char*)dest;
    for (size_t i = 0; i < n; i++) {
        ptr[i] = (unsigned char)c;
    }
    return dest;
}

void* memcpy(void* dest, const void* src, size_t n) {
    const unsigned char* s = (const unsigned char*)src;
    unsigned char* d = (unsigned char*)dest;
    for (size_t i = 0; i < n; i++) {
        d[i] = s[i];
    }
    return dest;
}

int strcmp(const char* s1, const char* s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(unsigned char*)s1 - *(unsigned char*)s2;
}

size_t strlen(const char* str) {
    size_t len = 0;
    while (str[len]) {
        len++;
    }
    return len;
}

void kernel_panic(const char* message) {
    vga_setcolor(vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_RED));
    vga_writestring("\nKERNEL PANIC: ");
    vga_writestring(message);
    vga_writestring("\nSystem halted.");
    __asm__ volatile ("cli; hlt");
}

// Kernel main function - entry point from assembly
void kernel_main(void) {
    // Initialize VGA driver
    vga_initialize();
    
    // Display welcome message
    vga_setcolor(vga_entry_color(VGA_COLOR_LIGHT_CYAN, VGA_COLOR_BLACK));
    vga_writestring("Advanced Kernel v1.0\n");
    vga_writestring("====================\n\n");
    
    vga_setcolor(vga_entry_color(VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK));
    vga_writestring("Initializing kernel subsystems...\n");
    
    // Initialize GDT
    gdt_initialize();
    vga_writestring("GDT: OK\n");
    
    // Initialize IDT
    idt_initialize();
    vga_writestring("IDT: OK\n");
    
    // Initialize IRQs
    irq_initialize();
    vga_writestring("IRQ: OK\n");
    
    // Initialize keyboard
    keyboard_initialize();
    vga_writestring("Keyboard: OK\n");
    
    // Initialize timer (100Hz)
    timer_initialize(100);
    vga_writestring("Timer: OK\n");
    
    vga_writestring("VGA text mode driver: OK\n");
    
    vga_setcolor(vga_entry_color(VGA_COLOR_LIGHT_BROWN, VGA_COLOR_BLACK));
    vga_writestring("\nAll subsystems initialized successfully!\n");
    
    // Initialize and run shell
    shell_initialize();
    shell_run();
}