#include "timer.h"
#include "irq.h"

// Timer state
static u32 timer_ticks = 0;
static u32 timer_frequency = 0;

// Port I/O functions
static inline void outb(u16 port, u8 val) {
    __asm__ volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

void timer_initialize(u32 frequency) {
    timer_frequency = frequency;
    timer_ticks = 0;
    
    // Install timer interrupt handler
    irq_install_handler(0, timer_handler);
    
    // Calculate divisor
    u32 divisor = PIT_FREQUENCY / frequency;
    
    // Send command byte
    outb(PIT_COMMAND, 0x36);  // Channel 0, lobyte/hibyte, rate generator
    
    // Send frequency divisor
    outb(PIT_DATA0, divisor & 0xFF);        // Low byte
    outb(PIT_DATA0, (divisor >> 8) & 0xFF); // High byte
}

void timer_handler(struct interrupt_context* ctx) {
    (void)ctx; // Suppress unused parameter warning
    timer_ticks++;
}

u32 timer_get_ticks(void) {
    return timer_ticks;
}

u32 timer_get_seconds(void) {
    return timer_ticks / timer_frequency;
}