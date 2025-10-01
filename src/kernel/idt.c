#include "idt.h"
#include "irq.h"
#include "vga.h"

// IDT with 256 entries
static struct idt_entry idt_entries[256];
static struct idt_ptr idt_pointer;

// Exception names for debugging
static const char* exception_messages[] = {
    "Division By Zero",
    "Debug",
    "Non Maskable Interrupt",
    "Breakpoint",
    "Into Detected Overflow",
    "Out of Bounds",
    "Invalid Opcode",
    "No Coprocessor",
    "Double Fault",
    "Coprocessor Segment Overrun",
    "Bad TSS",
    "Segment Not Present",
    "Stack Fault",
    "General Protection Fault",
    "Page Fault",
    "Unknown Interrupt",
    "Coprocessor Fault",
    "Alignment Check",
    "Machine Check",
    "SIMD Floating-Point Exception",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved"
};

void idt_initialize(void) {
    idt_pointer.limit = sizeof(struct idt_entry) * 256 - 1;
    idt_pointer.base = (u32)&idt_entries;

    // Clear IDT
    memset(&idt_entries, 0, sizeof(struct idt_entry) * 256);

    // Set up exception handlers (ISRs 0-31)
    idt_set_gate(0, (u32)isr0, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_GATE_32);
    idt_set_gate(1, (u32)isr1, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_GATE_32);
    idt_set_gate(2, (u32)isr2, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_GATE_32);
    idt_set_gate(3, (u32)isr3, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_GATE_32);
    idt_set_gate(4, (u32)isr4, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_GATE_32);
    idt_set_gate(5, (u32)isr5, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_GATE_32);
    idt_set_gate(6, (u32)isr6, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_GATE_32);
    idt_set_gate(7, (u32)isr7, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_GATE_32);
    idt_set_gate(8, (u32)isr8, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_GATE_32);
    idt_set_gate(9, (u32)isr9, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_GATE_32);
    idt_set_gate(10, (u32)isr10, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_GATE_32);
    idt_set_gate(11, (u32)isr11, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_GATE_32);
    idt_set_gate(12, (u32)isr12, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_GATE_32);
    idt_set_gate(13, (u32)isr13, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_GATE_32);
    idt_set_gate(14, (u32)isr14, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_GATE_32);
    idt_set_gate(15, (u32)isr15, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_GATE_32);
    idt_set_gate(16, (u32)isr16, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_GATE_32);
    idt_set_gate(17, (u32)isr17, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_GATE_32);
    idt_set_gate(18, (u32)isr18, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_GATE_32);
    idt_set_gate(19, (u32)isr19, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_GATE_32);
    idt_set_gate(20, (u32)isr20, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_GATE_32);
    idt_set_gate(21, (u32)isr21, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_GATE_32);
    idt_set_gate(22, (u32)isr22, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_GATE_32);
    idt_set_gate(23, (u32)isr23, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_GATE_32);
    idt_set_gate(24, (u32)isr24, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_GATE_32);
    idt_set_gate(25, (u32)isr25, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_GATE_32);
    idt_set_gate(26, (u32)isr26, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_GATE_32);
    idt_set_gate(27, (u32)isr27, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_GATE_32);
    idt_set_gate(28, (u32)isr28, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_GATE_32);
    idt_set_gate(29, (u32)isr29, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_GATE_32);
    idt_set_gate(30, (u32)isr30, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_GATE_32);
    idt_set_gate(31, (u32)isr31, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_GATE_32);

    // Load IDT
    __asm__ volatile ("lidt %0" : : "m" (idt_pointer));
    
    // Enable interrupts
    __asm__ volatile ("sti");
}

void idt_set_gate(u8 num, u32 base, u16 sel, u8 flags) {
    idt_entries[num].base_lo = base & 0xFFFF;
    idt_entries[num].base_hi = (base >> 16) & 0xFFFF;
    idt_entries[num].sel = sel;
    idt_entries[num].always0 = 0;
    idt_entries[num].flags = flags;
}

void interrupt_handler(struct interrupt_context* ctx) {
    if (ctx->int_no < 32) {
        // Handle exceptions
        vga_setcolor(vga_entry_color(VGA_COLOR_WHITE, VGA_COLOR_RED));
        vga_writestring("\nEXCEPTION: ");
        vga_writestring(exception_messages[ctx->int_no]);
        vga_writestring("\n");
        
        // Display error information
        vga_writestring("Error code: ");
        // Simple hex printing (just for demo)
        vga_putchar('0');
        vga_putchar('x');
        for (int i = 28; i >= 0; i -= 4) {
            u8 nibble = (ctx->err_code >> i) & 0xF;
            if (nibble < 10) {
                vga_putchar('0' + nibble);
            } else {
                vga_putchar('A' + nibble - 10);
            }
        }
        vga_putchar('\n');
        
        vga_writestring("EIP: ");
        vga_putchar('0');
        vga_putchar('x');
        for (int i = 28; i >= 0; i -= 4) {
            u8 nibble = (ctx->eip >> i) & 0xF;
            if (nibble < 10) {
                vga_putchar('0' + nibble);
            } else {
                vga_putchar('A' + nibble - 10);
            }
        }
        vga_putchar('\n');
        
        vga_writestring("System halted.\n");
        __asm__ volatile ("cli; hlt");
    } else if (ctx->int_no >= 32 && ctx->int_no < 48) {
        // Handle IRQs
        irq_handler(ctx);
    }
}