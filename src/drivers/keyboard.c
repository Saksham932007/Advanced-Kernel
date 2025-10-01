#include "keyboard.h"
#include "irq.h"
#include "vga.h"

// Port I/O functions
static inline void outb(u16 port, u8 val) {
    __asm__ volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

static inline u8 inb(u16 port) {
    u8 ret;
    __asm__ volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

// Keyboard state
static u8 keyboard_modifiers = 0;
static char keyboard_buffer[KEYBOARD_BUFFER_SIZE];
static size_t keyboard_buffer_head = 0;
static size_t keyboard_buffer_tail = 0;

// US QWERTY scancode to ASCII translation table
static const char scancode_to_ascii[] = {
    0,  0, '1', '2', '3', '4', '5', '6',     // 0x00-0x07
    '7', '8', '9', '0', '-', '=', '\b', '\t', // 0x08-0x0F
    'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', // 0x10-0x17
    'o', 'p', '[', ']', '\n', 0, 'a', 's',  // 0x18-0x1F
    'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', // 0x20-0x27
    '\'', '`', 0, '\\', 'z', 'x', 'c', 'v', // 0x28-0x2F
    'b', 'n', 'm', ',', '.', '/', 0, '*',   // 0x30-0x37
    0, ' ', 0, 0, 0, 0, 0, 0,               // 0x38-0x3F
    0, 0, 0, 0, 0, 0, 0, '7',               // 0x40-0x47
    '8', '9', '-', '4', '5', '6', '+', '1', // 0x48-0x4F
    '2', '3', '0', '.', 0, 0, 0, 0,         // 0x50-0x57
    0, 0, 0, 0, 0, 0, 0, 0                  // 0x58-0x5F
};

// Shifted characters
static const char scancode_to_ascii_shift[] = {
    0,  0, '!', '@', '#', '$', '%', '^',     // 0x00-0x07
    '&', '*', '(', ')', '_', '+', '\b', '\t', // 0x08-0x0F
    'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', // 0x10-0x17
    'O', 'P', '{', '}', '\n', 0, 'A', 'S',  // 0x18-0x1F
    'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', // 0x20-0x27
    '"', '~', 0, '|', 'Z', 'X', 'C', 'V',   // 0x28-0x2F
    'B', 'N', 'M', '<', '>', '?', 0, '*',   // 0x30-0x37
    0, ' ', 0, 0, 0, 0, 0, 0,               // 0x38-0x3F
    0, 0, 0, 0, 0, 0, 0, '7',               // 0x40-0x47
    '8', '9', '-', '4', '5', '6', '+', '1', // 0x48-0x4F
    '2', '3', '0', '.', 0, 0, 0, 0,         // 0x50-0x57
    0, 0, 0, 0, 0, 0, 0, 0                  // 0x58-0x5F
};

void keyboard_initialize(void) {
    // Install keyboard interrupt handler
    irq_install_handler(1, keyboard_handler);
    
    // Clear keyboard buffer
    keyboard_buffer_head = 0;
    keyboard_buffer_tail = 0;
    keyboard_modifiers = 0;
}

void keyboard_handler(struct interrupt_context* ctx) {
    (void)ctx; // Suppress unused parameter warning
    
    u8 scancode = inb(KEYBOARD_DATA_PORT);
    
    // Check if this is a key release (bit 7 set)
    bool key_released = (scancode & 0x80) != 0;
    scancode &= 0x7F; // Remove release bit
    
    // Handle modifier keys
    switch (scancode) {
        case KEY_LSHIFT:
            if (key_released) {
                keyboard_modifiers &= ~KEY_MOD_LSHIFT;
            } else {
                keyboard_modifiers |= KEY_MOD_LSHIFT;
            }
            return;
            
        case KEY_RSHIFT:
            if (key_released) {
                keyboard_modifiers &= ~KEY_MOD_RSHIFT;
            } else {
                keyboard_modifiers |= KEY_MOD_RSHIFT;
            }
            return;
            
        case KEY_LCTRL:
            if (key_released) {
                keyboard_modifiers &= ~KEY_MOD_LCTRL;
            } else {
                keyboard_modifiers |= KEY_MOD_LCTRL;
            }
            return;
            
        case KEY_LALT:
            if (key_released) {
                keyboard_modifiers &= ~KEY_MOD_LALT;
            } else {
                keyboard_modifiers |= KEY_MOD_LALT;
            }
            return;
            
        case KEY_CAPS:
            if (!key_released) {
                keyboard_modifiers ^= KEY_MOD_CAPS;
            }
            return;
    }
    
    // Only process key presses, not releases
    if (key_released) {
        return;
    }
    
    // Convert scancode to ASCII
    char ascii = 0;
    if (scancode < sizeof(scancode_to_ascii)) {
        bool shift_pressed = (keyboard_modifiers & (KEY_MOD_LSHIFT | KEY_MOD_RSHIFT)) != 0;
        bool caps_on = (keyboard_modifiers & KEY_MOD_CAPS) != 0;
        
        if (shift_pressed) {
            ascii = scancode_to_ascii_shift[scancode];
        } else {
            ascii = scancode_to_ascii[scancode];
            
            // Apply caps lock to letters
            if (caps_on && ascii >= 'a' && ascii <= 'z') {
                ascii = ascii - 'a' + 'A';
            }
        }
    }
    
    // Add to buffer if it's a valid character
    if (ascii != 0) {
        size_t next_head = (keyboard_buffer_head + 1) % KEYBOARD_BUFFER_SIZE;
        if (next_head != keyboard_buffer_tail) {
            keyboard_buffer[keyboard_buffer_head] = ascii;
            keyboard_buffer_head = next_head;
        }
    }
}

char keyboard_getchar(void) {
    while (!keyboard_haschar()) {
        __asm__ volatile ("hlt"); // Wait for interrupt
    }
    
    char c = keyboard_buffer[keyboard_buffer_tail];
    keyboard_buffer_tail = (keyboard_buffer_tail + 1) % KEYBOARD_BUFFER_SIZE;
    return c;
}

bool keyboard_haschar(void) {
    return keyboard_buffer_head != keyboard_buffer_tail;
}

void keyboard_wait_for_key(void) {
    keyboard_getchar(); // Just consume one character
}