#include "vga.h"

// VGA state
static size_t vga_row;
static size_t vga_column;
static u8 vga_color;
static u16* vga_buffer;

// Port I/O functions
static inline void outb(u16 port, u8 val) {
    __asm__ volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

static inline u8 inb(u16 port) {
    u8 ret;
    __asm__ volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

void vga_initialize(void) {
    vga_row = 0;
    vga_column = 0;
    vga_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
    vga_buffer = (u16*)VGA_MEMORY;
    
    vga_clear();
    vga_enable_cursor(14, 15);
    vga_set_cursor(0, 0);
}

void vga_setcolor(u8 color) {
    vga_color = color;
}

void vga_putentryat(char c, u8 color, size_t x, size_t y) {
    const size_t index = y * VGA_WIDTH + x;
    vga_buffer[index] = vga_entry(c, color);
}

void vga_scroll(void) {
    // Move all lines up by one
    for (size_t y = 0; y < VGA_HEIGHT - 1; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            vga_buffer[y * VGA_WIDTH + x] = vga_buffer[(y + 1) * VGA_WIDTH + x];
        }
    }
    
    // Clear the last line
    for (size_t x = 0; x < VGA_WIDTH; x++) {
        vga_putentryat(' ', vga_color, x, VGA_HEIGHT - 1);
    }
}

void vga_putchar(char c) {
    if (c == '\n') {
        vga_column = 0;
        if (++vga_row == VGA_HEIGHT) {
            vga_scroll();
            vga_row = VGA_HEIGHT - 1;
        }
    } else if (c == '\r') {
        vga_column = 0;
    } else if (c == '\t') {
        vga_column = (vga_column + 8) & ~(8 - 1);
        if (vga_column >= VGA_WIDTH) {
            vga_column = 0;
            if (++vga_row == VGA_HEIGHT) {
                vga_scroll();
                vga_row = VGA_HEIGHT - 1;
            }
        }
    } else if (c == '\b') {
        if (vga_column > 0) {
            vga_column--;
            vga_putentryat(' ', vga_color, vga_column, vga_row);
        }
    } else {
        vga_putentryat(c, vga_color, vga_column, vga_row);
        if (++vga_column == VGA_WIDTH) {
            vga_column = 0;
            if (++vga_row == VGA_HEIGHT) {
                vga_scroll();
                vga_row = VGA_HEIGHT - 1;
            }
        }
    }
    
    vga_update_cursor();
}

void vga_write(const char* data, size_t size) {
    for (size_t i = 0; i < size; i++) {
        vga_putchar(data[i]);
    }
}

void vga_writestring(const char* data) {
    vga_write(data, strlen(data));
}

void vga_clear(void) {
    for (size_t y = 0; y < VGA_HEIGHT; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            vga_putentryat(' ', vga_color, x, y);
        }
    }
    vga_row = 0;
    vga_column = 0;
    vga_update_cursor();
}

void vga_set_cursor(size_t x, size_t y) {
    vga_column = x;
    vga_row = y;
    vga_update_cursor();
}

void vga_enable_cursor(u8 cursor_start, u8 cursor_end) {
    outb(0x3D4, 0x0A);
    outb(0x3D5, (inb(0x3D5) & 0xC0) | cursor_start);
    
    outb(0x3D4, 0x0B);
    outb(0x3D5, (inb(0x3D5) & 0xE0) | cursor_end);
}

void vga_disable_cursor(void) {
    outb(0x3D4, 0x0A);
    outb(0x3D5, 0x20);
}

void vga_update_cursor(void) {
    u16 pos = vga_row * VGA_WIDTH + vga_column;
    
    outb(0x3D4, 0x0F);
    outb(0x3D5, (u8)(pos & 0xFF));
    outb(0x3D4, 0x0E);
    outb(0x3D5, (u8)((pos >> 8) & 0xFF));
}