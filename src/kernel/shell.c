#include "shell.h"
#include "vga.h"
#include "keyboard.h"
#include "timer.h"

// Shell state
static char shell_buffer[SHELL_BUFFER_SIZE];
static size_t shell_buffer_pos = 0;

// Command table
static struct shell_command commands[] = {
    {"help", "Show available commands", cmd_help},
    {"clear", "Clear the screen", cmd_clear},
    {"echo", "Print text to screen", cmd_echo},
    {"uptime", "Show system uptime", cmd_uptime},
    {"version", "Show kernel version", cmd_version},
    {"reboot", "Restart the system", cmd_reboot},
    {"halt", "Halt the system", cmd_halt},
    {"cpuinfo", "Show CPU information", cmd_cpuinfo},
    {"meminfo", "Show memory information", cmd_meminfo},
    {0, 0, 0}  // Terminator
};

void shell_initialize(void) {
    shell_buffer_pos = 0;
    memset(shell_buffer, 0, SHELL_BUFFER_SIZE);
    
    vga_setcolor(vga_entry_color(VGA_COLOR_LIGHT_CYAN, VGA_COLOR_BLACK));
    vga_writestring("\n=== Advanced Kernel Shell ===\n");
    vga_writestring("Type 'help' for available commands.\n\n");
    
    shell_print_prompt();
}

void shell_run(void) {
    while (1) {
        if (keyboard_haschar()) {
            char c = keyboard_getchar();
            shell_process_input(c);
        }
        __asm__ volatile ("hlt");
    }
}

void shell_process_input(char c) {
    if (c == '\n') {
        // Process command
        vga_putchar('\n');
        shell_buffer[shell_buffer_pos] = '\0';
        
        if (shell_buffer_pos > 0) {
            shell_execute_command(shell_buffer);
        }
        
        // Reset buffer and show prompt
        shell_buffer_pos = 0;
        memset(shell_buffer, 0, SHELL_BUFFER_SIZE);
        shell_print_prompt();
        
    } else if (c == '\b') {
        // Backspace
        if (shell_buffer_pos > 0) {
            shell_buffer_pos--;
            shell_buffer[shell_buffer_pos] = '\0';
            vga_putchar('\b');
        }
        
    } else if (c >= 32 && c <= 126) {
        // Printable character
        if (shell_buffer_pos < SHELL_BUFFER_SIZE - 1) {
            shell_buffer[shell_buffer_pos] = c;
            shell_buffer_pos++;
            vga_putchar(c);
        }
    }
}

void shell_execute_command(const char* command_line) {
    // Simple command parsing
    char* argv[SHELL_MAX_ARGS];
    int argc = 0;
    
    // Copy command line to a writable buffer
    static char cmd_copy[SHELL_BUFFER_SIZE];
    size_t len = strlen(command_line);
    if (len >= SHELL_BUFFER_SIZE) len = SHELL_BUFFER_SIZE - 1;
    memcpy(cmd_copy, command_line, len);
    cmd_copy[len] = '\0';
    
    // Simple tokenization (split by spaces)
    char* token = cmd_copy;
    char* end = cmd_copy + len;
    
    while (token < end && argc < SHELL_MAX_ARGS - 1) {
        // Skip whitespace
        while (token < end && *token == ' ') token++;
        if (token >= end) break;
        
        argv[argc++] = token;
        
        // Find end of token
        while (token < end && *token != ' ' && *token != '\0') token++;
        if (token < end) *token++ = '\0';
    }
    argv[argc] = 0;
    
    if (argc == 0) return;
    
    // Find and execute command
    for (int i = 0; commands[i].name; i++) {
        if (strcmp(argv[0], commands[i].name) == 0) {
            commands[i].function(argc, argv);
            return;
        }
    }
    
    // Command not found
    vga_setcolor(vga_entry_color(VGA_COLOR_RED, VGA_COLOR_BLACK));
    vga_writestring("Unknown command: ");
    vga_writestring(argv[0]);
    vga_writestring("\nType 'help' for available commands.\n");
    vga_setcolor(vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK));
}

void shell_print_prompt(void) {
    vga_setcolor(vga_entry_color(VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK));
    vga_writestring(SHELL_PROMPT);
    vga_setcolor(vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK));
}

// Built-in commands implementation
void cmd_help(int argc, char* argv[]) {
    (void)argc; (void)argv;
    
    vga_setcolor(vga_entry_color(VGA_COLOR_LIGHT_CYAN, VGA_COLOR_BLACK));
    vga_writestring("Available commands:\n");
    vga_setcolor(vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK));
    
    for (int i = 0; commands[i].name; i++) {
        vga_writestring("  ");
        vga_setcolor(vga_entry_color(VGA_COLOR_LIGHT_GREEN, VGA_COLOR_BLACK));
        vga_writestring(commands[i].name);
        vga_setcolor(vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK));
        vga_writestring(" - ");
        vga_writestring(commands[i].description);
        vga_writestring("\n");
    }
}

void cmd_clear(int argc, char* argv[]) {
    (void)argc; (void)argv;
    vga_clear();
}

void cmd_echo(int argc, char* argv[]) {
    for (int i = 1; i < argc; i++) {
        if (i > 1) vga_putchar(' ');
        vga_writestring(argv[i]);
    }
    vga_putchar('\n');
}

void cmd_uptime(int argc, char* argv[]) {
    (void)argc; (void)argv;
    u32 seconds = timer_get_seconds();
    u32 minutes = seconds / 60;
    u32 hours = minutes / 60;
    
    vga_writestring("System uptime: ");
    
    // Simple number printing
    if (hours > 0) {
        // Print hours (simplified)
        if (hours >= 10) vga_putchar('0' + (hours / 10));
        vga_putchar('0' + (hours % 10));
        vga_writestring("h ");
    }
    
    if (minutes > 0 || hours > 0) {
        minutes %= 60;
        if (minutes >= 10) vga_putchar('0' + (minutes / 10));
        vga_putchar('0' + (minutes % 10));
        vga_writestring("m ");
    }
    
    seconds %= 60;
    if (seconds >= 10) vga_putchar('0' + (seconds / 10));
    vga_putchar('0' + (seconds % 10));
    vga_writestring("s\n");
}

void cmd_version(int argc, char* argv[]) {
    (void)argc; (void)argv;
    vga_setcolor(vga_entry_color(VGA_COLOR_LIGHT_CYAN, VGA_COLOR_BLACK));
    vga_writestring("Advanced Kernel v1.0\n");
    vga_writestring("Built with GCC and NASM\n");
    vga_writestring("Features: VGA, Keyboard, Interrupts, Memory Management\n");
    vga_setcolor(vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK));
}

void cmd_reboot(int argc, char* argv[]) {
    (void)argc; (void)argv;
    vga_writestring("Rebooting system...\n");
    // TODO: Implement actual reboot
    vga_writestring("Reboot not implemented yet.\n");
}

void cmd_halt(int argc, char* argv[]) {
    (void)argc; (void)argv;
    vga_writestring("Halting system...\n");
    __asm__ volatile ("cli; hlt");
}

void cmd_cpuinfo(int argc, char* argv[]) {
    (void)argc; (void)argv;
    vga_writestring("CPU: x86 (32-bit)\n");
    vga_writestring("Architecture: i386\n");
    vga_writestring("Mode: Protected Mode\n");
}

void cmd_meminfo(int argc, char* argv[]) {
    (void)argc; (void)argv;
    vga_writestring("Memory information:\n");
    vga_writestring("  Total RAM: Unknown (memory manager not implemented)\n");
    vga_writestring("  Kernel memory: ~1MB\n");
    vga_writestring("  Available: Unknown\n");
}