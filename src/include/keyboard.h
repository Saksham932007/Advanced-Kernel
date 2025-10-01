#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "kernel.h"
#include "idt.h"

// Keyboard ports
#define KEYBOARD_DATA_PORT    0x60
#define KEYBOARD_STATUS_PORT  0x64
#define KEYBOARD_COMMAND_PORT 0x64

// Keyboard commands
#define KEYBOARD_CMD_SET_LEDS     0xED
#define KEYBOARD_CMD_ECHO         0xEE
#define KEYBOARD_CMD_SET_SCANCODE 0xF0
#define KEYBOARD_CMD_IDENTIFY     0xF2
#define KEYBOARD_CMD_SET_TYPEMATIC 0xF3
#define KEYBOARD_CMD_ENABLE       0xF4
#define KEYBOARD_CMD_DISABLE      0xF5
#define KEYBOARD_CMD_RESET        0xFF

// Key modifiers
#define KEY_MOD_LSHIFT    0x01
#define KEY_MOD_RSHIFT    0x02
#define KEY_MOD_LCTRL     0x04
#define KEY_MOD_RCTRL     0x08
#define KEY_MOD_LALT      0x10
#define KEY_MOD_RALT      0x20
#define KEY_MOD_CAPS      0x40
#define KEY_MOD_NUM       0x80

// Special keys
#define KEY_ESC           0x01
#define KEY_BACKSPACE     0x0E
#define KEY_TAB           0x0F
#define KEY_ENTER         0x1C
#define KEY_LCTRL         0x1D
#define KEY_LSHIFT        0x2A
#define KEY_RSHIFT        0x36
#define KEY_LALT          0x38
#define KEY_SPACE         0x39
#define KEY_CAPS          0x3A
#define KEY_F1            0x3B
#define KEY_F2            0x3C
#define KEY_F3            0x3D
#define KEY_F4            0x3E
#define KEY_F5            0x3F
#define KEY_F6            0x40
#define KEY_F7            0x41
#define KEY_F8            0x42
#define KEY_F9            0x43
#define KEY_F10           0x44
#define KEY_NUM           0x45
#define KEY_SCROLL        0x46
#define KEY_F11           0x57
#define KEY_F12           0x58

// Keyboard buffer size
#define KEYBOARD_BUFFER_SIZE 256

// Keyboard functions
void keyboard_initialize(void);
void keyboard_handler(struct interrupt_context* ctx);
char keyboard_getchar(void);
bool keyboard_haschar(void);
void keyboard_wait_for_key(void);

#endif