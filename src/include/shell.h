#ifndef SHELL_H
#define SHELL_H

#include "kernel.h"

// Shell constants
#define SHELL_BUFFER_SIZE 256
#define SHELL_MAX_ARGS 16
#define SHELL_PROMPT "kernel> "

// Shell command structure
struct shell_command {
    const char* name;
    const char* description;
    void (*function)(int argc, char* argv[]);
};

// Shell functions
void shell_initialize(void);
void shell_run(void);
void shell_process_input(char c);
void shell_execute_command(const char* command_line);
void shell_print_prompt(void);

// Built-in commands
void cmd_help(int argc, char* argv[]);
void cmd_clear(int argc, char* argv[]);
void cmd_echo(int argc, char* argv[]);
void cmd_uptime(int argc, char* argv[]);
void cmd_version(int argc, char* argv[]);
void cmd_reboot(int argc, char* argv[]);
void cmd_halt(int argc, char* argv[]);
void cmd_cpuinfo(int argc, char* argv[]);
void cmd_meminfo(int argc, char* argv[]);

#endif