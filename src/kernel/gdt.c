#include "gdt.h"

// GDT with 5 entries: null, kernel code, kernel data, user code, user data
static struct gdt_entry gdt_entries[5];
static struct gdt_ptr gdt_pointer;

// Assembly function to flush GDT
extern void gdt_flush_asm(u32 gdt_ptr);

void gdt_initialize(void) {
    gdt_pointer.limit = (sizeof(struct gdt_entry) * 5) - 1;
    gdt_pointer.base = (u32)&gdt_entries;

    // NULL descriptor
    gdt_set_gate(0, 0, 0, 0, 0);
    
    // Kernel code segment
    gdt_set_gate(1, 0, 0xFFFFFFFF, 
                 GDT_ACCESS_PRESENT | GDT_ACCESS_RING0 | GDT_ACCESS_SEGMENT | GDT_ACCESS_EXEC | GDT_ACCESS_RW,
                 GDT_GRAN_4K | GDT_GRAN_32BIT | 0x0F);
    
    // Kernel data segment
    gdt_set_gate(2, 0, 0xFFFFFFFF,
                 GDT_ACCESS_PRESENT | GDT_ACCESS_RING0 | GDT_ACCESS_SEGMENT | GDT_ACCESS_RW,
                 GDT_GRAN_4K | GDT_GRAN_32BIT | 0x0F);
    
    // User code segment
    gdt_set_gate(3, 0, 0xFFFFFFFF,
                 GDT_ACCESS_PRESENT | GDT_ACCESS_RING3 | GDT_ACCESS_SEGMENT | GDT_ACCESS_EXEC | GDT_ACCESS_RW,
                 GDT_GRAN_4K | GDT_GRAN_32BIT | 0x0F);
    
    // User data segment
    gdt_set_gate(4, 0, 0xFFFFFFFF,
                 GDT_ACCESS_PRESENT | GDT_ACCESS_RING3 | GDT_ACCESS_SEGMENT | GDT_ACCESS_RW,
                 GDT_GRAN_4K | GDT_GRAN_32BIT | 0x0F);

    gdt_flush((u32)&gdt_pointer);
}

void gdt_set_gate(int num, u32 base, u32 limit, u8 access, u8 gran) {
    gdt_entries[num].base_low = (base & 0xFFFF);
    gdt_entries[num].base_middle = (base >> 16) & 0xFF;
    gdt_entries[num].base_high = (base >> 24) & 0xFF;

    gdt_entries[num].limit_low = (limit & 0xFFFF);
    gdt_entries[num].granularity = (limit >> 16) & 0x0F;

    gdt_entries[num].granularity |= gran & 0xF0;
    gdt_entries[num].access = access;
}

void gdt_flush(u32 gdt_ptr) {
    gdt_flush_asm(gdt_ptr);
}