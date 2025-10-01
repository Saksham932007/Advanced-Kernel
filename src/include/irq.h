#ifndef IRQ_H
#define IRQ_H

#include "kernel.h"
#include "idt.h"

// PIC (Programmable Interrupt Controller) constants
#define PIC1_COMMAND    0x20
#define PIC1_DATA       0x21
#define PIC2_COMMAND    0xA0
#define PIC2_DATA       0xA1

#define PIC_EOI         0x20

// IRQ numbers
#define IRQ0_TIMER      32
#define IRQ1_KEYBOARD   33
#define IRQ12_MOUSE     44

// IRQ handler function type
typedef void (*irq_handler_t)(struct interrupt_context* ctx);

// IRQ functions
void irq_initialize(void);
void irq_install_handler(int irq, irq_handler_t handler);
void irq_uninstall_handler(int irq);
void irq_handler(struct interrupt_context* ctx);

// Assembly IRQ stubs
extern void irq0(void);
extern void irq1(void);
extern void irq2(void);
extern void irq3(void);
extern void irq4(void);
extern void irq5(void);
extern void irq6(void);
extern void irq7(void);
extern void irq8(void);
extern void irq9(void);
extern void irq10(void);
extern void irq11(void);
extern void irq12(void);
extern void irq13(void);
extern void irq14(void);
extern void irq15(void);

#endif