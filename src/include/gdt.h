#ifndef GDT_H
#define GDT_H

#include "kernel.h"

// GDT entry structure
struct gdt_entry {
    u16 limit_low;      // Lower 16 bits of limit
    u16 base_low;       // Lower 16 bits of base
    u8  base_middle;    // Next 8 bits of base
    u8  access;         // Access flags
    u8  granularity;    // Granularity and upper 4 bits of limit
    u8  base_high;      // Higher 8 bits of base
} __attribute__((packed));

// GDT pointer structure
struct gdt_ptr {
    u16 limit;          // Size of GDT - 1
    u32 base;           // Address of GDT
} __attribute__((packed));

// Access byte flags
#define GDT_ACCESS_PRESENT    0x80
#define GDT_ACCESS_RING0      0x00
#define GDT_ACCESS_RING1      0x20
#define GDT_ACCESS_RING2      0x40
#define GDT_ACCESS_RING3      0x60
#define GDT_ACCESS_SEGMENT    0x10
#define GDT_ACCESS_EXEC       0x08
#define GDT_ACCESS_DC         0x04
#define GDT_ACCESS_RW         0x02
#define GDT_ACCESS_ACCESSED   0x01

// Granularity byte flags
#define GDT_GRAN_4K           0x80
#define GDT_GRAN_32BIT        0x40
#define GDT_GRAN_16BIT        0x00

// GDT functions
void gdt_initialize(void);
void gdt_set_gate(int num, u32 base, u32 limit, u8 access, u8 gran);
void gdt_flush(u32 gdt_ptr);

#endif