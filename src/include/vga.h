#ifndef VGA_H
#define VGA_H

#include "kernel.h"

// VGA text mode constants
#define VGA_WIDTH  80
#define VGA_HEIGHT 25
#define VGA_MEMORY 0xB8000

// VGA colors
typedef enum {
    VGA_COLOR_BLACK = 0,
    VGA_COLOR_BLUE = 1,
    VGA_COLOR_GREEN = 2,
    VGA_COLOR_CYAN = 3,
    VGA_COLOR_RED = 4,
    VGA_COLOR_MAGENTA = 5,
    VGA_COLOR_BROWN = 6,
    VGA_COLOR_LIGHT_GREY = 7,
    VGA_COLOR_DARK_GREY = 8,
    VGA_COLOR_LIGHT_BLUE = 9,
    VGA_COLOR_LIGHT_GREEN = 10,
    VGA_COLOR_LIGHT_CYAN = 11,
    VGA_COLOR_LIGHT_RED = 12,
    VGA_COLOR_LIGHT_MAGENTA = 13,
    VGA_COLOR_LIGHT_BROWN = 14,
    VGA_COLOR_WHITE = 15,
} vga_color_t;

// VGA entry structure
static inline u8 vga_entry_color(vga_color_t fg, vga_color_t bg) {
    return fg | bg << 4;
}

static inline u16 vga_entry(unsigned char uc, u8 color) {
    return (u16)uc | (u16)color << 8;
}

// VGA driver functions
void vga_initialize(void);
void vga_setcolor(u8 color);
void vga_putentryat(char c, u8 color, size_t x, size_t y);
void vga_putchar(char c);
void vga_write(const char* data, size_t size);
void vga_writestring(const char* data);
void vga_clear(void);
void vga_scroll(void);
void vga_set_cursor(size_t x, size_t y);
void vga_enable_cursor(u8 cursor_start, u8 cursor_end);
void vga_disable_cursor(void);
void vga_update_cursor(void);

#endif