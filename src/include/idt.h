#ifndef IDT_H
#define IDT_H

#include "kernel.h"

// IDT entry structure
struct idt_entry {
    u16 base_lo;        // Lower 16 bits of handler address
    u16 sel;            // Kernel segment selector
    u8  always0;        // Must be zero
    u8  flags;          // Flags
    u16 base_hi;        // Upper 16 bits of handler address
} __attribute__((packed));

// IDT pointer structure
struct idt_ptr {
    u16 limit;          // Size of IDT - 1
    u32 base;           // Address of IDT
} __attribute__((packed));

// Interrupt context structure
struct interrupt_context {
    u32 ds;                                     // Data segment selector
    u32 edi, esi, ebp, esp, ebx, edx, ecx, eax; // Pushed by pusha
    u32 int_no, err_code;                       // Interrupt number and error code
    u32 eip, cs, eflags, useresp, ss;           // Pushed by processor
};

// IDT flags
#define IDT_FLAG_PRESENT   0x80
#define IDT_FLAG_RING0     0x00
#define IDT_FLAG_RING1     0x20
#define IDT_FLAG_RING2     0x40
#define IDT_FLAG_RING3     0x60
#define IDT_FLAG_GATE_32   0x0E
#define IDT_FLAG_GATE_16   0x06

// Exception handlers
void divide_error_handler(struct interrupt_context* ctx);
void debug_handler(struct interrupt_context* ctx);
void nmi_handler(struct interrupt_context* ctx);
void breakpoint_handler(struct interrupt_context* ctx);
void overflow_handler(struct interrupt_context* ctx);
void bound_range_handler(struct interrupt_context* ctx);
void invalid_opcode_handler(struct interrupt_context* ctx);
void device_not_available_handler(struct interrupt_context* ctx);
void double_fault_handler(struct interrupt_context* ctx);
void invalid_tss_handler(struct interrupt_context* ctx);
void segment_not_present_handler(struct interrupt_context* ctx);
void stack_fault_handler(struct interrupt_context* ctx);
void general_protection_handler(struct interrupt_context* ctx);
void page_fault_handler(struct interrupt_context* ctx);
void fpu_error_handler(struct interrupt_context* ctx);
void alignment_check_handler(struct interrupt_context* ctx);
void machine_check_handler(struct interrupt_context* ctx);
void simd_exception_handler(struct interrupt_context* ctx);

// IDT functions
void idt_initialize(void);
void idt_set_gate(u8 num, u32 base, u16 sel, u8 flags);
void interrupt_handler(struct interrupt_context* ctx);

// Assembly interrupt stubs
extern void isr0(void);
extern void isr1(void);
extern void isr2(void);
extern void isr3(void);
extern void isr4(void);
extern void isr5(void);
extern void isr6(void);
extern void isr7(void);
extern void isr8(void);
extern void isr9(void);
extern void isr10(void);
extern void isr11(void);
extern void isr12(void);
extern void isr13(void);
extern void isr14(void);
extern void isr15(void);
extern void isr16(void);
extern void isr17(void);
extern void isr18(void);
extern void isr19(void);
extern void isr20(void);
extern void isr21(void);
extern void isr22(void);
extern void isr23(void);
extern void isr24(void);
extern void isr25(void);
extern void isr26(void);
extern void isr27(void);
extern void isr28(void);
extern void isr29(void);
extern void isr30(void);
extern void isr31(void);

#endif