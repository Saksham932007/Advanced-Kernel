#ifndef TIMER_H
#define TIMER_H

#include "kernel.h"
#include "idt.h"

// PIT (Programmable Interval Timer) constants
#define PIT_FREQUENCY   1193180
#define PIT_COMMAND     0x43
#define PIT_DATA0       0x40

// Timer functions
void timer_initialize(u32 frequency);
void timer_handler(struct interrupt_context* ctx);
u32 timer_get_ticks(void);
u32 timer_get_seconds(void);

#endif