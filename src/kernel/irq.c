#include "irq.h"
#include "idt.h"

// IRQ handler array
static irq_handler_t irq_handlers[16];

// Port I/O functions
static inline void outb(u16 port, u8 val) {
    __asm__ volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

static inline u8 inb(u16 port) {
    u8 ret;
    __asm__ volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

void irq_initialize(void) {
    // Clear IRQ handlers
    for (int i = 0; i < 16; i++) {
        irq_handlers[i] = 0;
    }

    // Remap PIC interrupts
    // ICW1 - Initialize PICs
    outb(PIC1_COMMAND, 0x11);
    outb(PIC2_COMMAND, 0x11);

    // ICW2 - Set interrupt vectors
    outb(PIC1_DATA, 0x20);  // PIC1 starts at IRQ 32
    outb(PIC2_DATA, 0x28);  // PIC2 starts at IRQ 40

    // ICW3 - Set cascading
    outb(PIC1_DATA, 0x04);  // PIC1 has slave on IRQ2
    outb(PIC2_DATA, 0x02);  // PIC2 is cascade identity

    // ICW4 - Set mode
    outb(PIC1_DATA, 0x01);  // 8086 mode
    outb(PIC2_DATA, 0x01);  // 8086 mode

    // Mask all interrupts except cascade
    outb(PIC1_DATA, 0xFC);  // Enable timer and keyboard
    outb(PIC2_DATA, 0xFF);  // Mask all PIC2 interrupts

    // Install IRQ handlers in IDT
    idt_set_gate(32, (u32)irq0, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_GATE_32);
    idt_set_gate(33, (u32)irq1, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_GATE_32);
    idt_set_gate(34, (u32)irq2, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_GATE_32);
    idt_set_gate(35, (u32)irq3, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_GATE_32);
    idt_set_gate(36, (u32)irq4, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_GATE_32);
    idt_set_gate(37, (u32)irq5, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_GATE_32);
    idt_set_gate(38, (u32)irq6, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_GATE_32);
    idt_set_gate(39, (u32)irq7, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_GATE_32);
    idt_set_gate(40, (u32)irq8, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_GATE_32);
    idt_set_gate(41, (u32)irq9, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_GATE_32);
    idt_set_gate(42, (u32)irq10, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_GATE_32);
    idt_set_gate(43, (u32)irq11, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_GATE_32);
    idt_set_gate(44, (u32)irq12, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_GATE_32);
    idt_set_gate(45, (u32)irq13, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_GATE_32);
    idt_set_gate(46, (u32)irq14, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_GATE_32);
    idt_set_gate(47, (u32)irq15, 0x08, IDT_FLAG_PRESENT | IDT_FLAG_RING0 | IDT_FLAG_GATE_32);
}

void irq_install_handler(int irq, irq_handler_t handler) {
    if (irq >= 0 && irq < 16) {
        irq_handlers[irq] = handler;
    }
}

void irq_uninstall_handler(int irq) {
    if (irq >= 0 && irq < 16) {
        irq_handlers[irq] = 0;
    }
}

void irq_handler(struct interrupt_context* ctx) {
    int irq = ctx->int_no - 32;
    
    // Call handler if one is installed
    if (irq_handlers[irq]) {
        irq_handlers[irq](ctx);
    }
    
    // Send EOI (End of Interrupt) to PICs
    if (irq >= 8) {
        outb(PIC2_COMMAND, PIC_EOI);  // Send EOI to slave PIC
    }
    outb(PIC1_COMMAND, PIC_EOI);      // Send EOI to master PIC
}