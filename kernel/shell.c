// kernel/shell.c
#include "shell.h"
#include "terminal.h"
#include "io.h"
#include "stdio.h"

// Command handler function type
typedef void (*command_handler_t)(char* args);

// Command structure
typedef struct {
    const char* name;
    command_handler_t handler;
    const char* help;
} command_t;

// Command handlers
void cmd_help(char* args);
void cmd_clear(char* args);
void cmd_echo(char* args);
void cmd_info(char* args);
void cmd_reboot(char* args);

// Command table
static const command_t commands[] = {
    {"help", cmd_help, "Display available commands"},
    {"clear", cmd_clear, "Clear the screen"},
    {"echo", cmd_echo, "Display a message"},
    {"info", cmd_info, "Display system information"},
    {"reboot", cmd_reboot, "Reboot the system"},
};

static const int num_commands = sizeof(commands) / sizeof(command_t);

// String comparison
static int str_compare(const char* s1, const char* s2) {
    while (*s1 && *s2 && *s1 == *s2) {
        s1++;
        s2++;
    }
    return *s1 - *s2;
}

// String length
static size_t str_length(const char* s) {
    size_t len = 0;
    while (*s++) len++;
    return len;
}

// Split command into command name and arguments
static void split_command(char* input, char** cmd, char** args) {
    *cmd = input;

    // Find first space
    while (*input && *input != ' ') {
        input++;
    }

    // If space found, null-terminate command and set args
    if (*input) {
        *input = '\0';
        *args = input + 1;
    } else {
        *args = input; // Empty args
    }
}

// Initialize shell
void shell_initialize(void) {
    keyboard_initialize();
    terminal_initialize();
    terminal_write("EXO_OS Shell v0.1\n");
    terminal_write("Type 'help' for a list of commands\n");
}

// Run shell
void shell_run(void) {
    char input[SHELL_COMMAND_MAX_LEN];
    char* cmd;
    char* args;
    int i;
    int found;

    while (1) {
        // Display prompt
        terminal_write(SHELL_PROMPT);

        // Read command
        keyboard_read_line(input, SHELL_COMMAND_MAX_LEN);
        terminal_putchar('\n');

        // Skip empty commands
        if (!str_length(input)) {
            continue;
        }

        // Parse command
        split_command(input, &cmd, &args);

        // Find and execute command
        found = 0;
        for (i = 0; i < num_commands; i++) {
            if (str_compare(cmd, commands[i].name) == 0) {
                commands[i].handler(args);
                found = 1;
                break;
            }
        }

        if (!found) {
            terminal_write("Unknown command: ");
            terminal_write(cmd);
            terminal_putchar('\n');
        }
    }
}

// Command implementations
void cmd_help(char* args) {
    int i;
    terminal_write("Available commands:\n");
    for (i = 0; i < num_commands; i++) {
        terminal_write("  ");
        terminal_write(commands[i].name);
        terminal_write(" - ");
        terminal_write(commands[i].help);
        terminal_putchar('\n');
    }
    (void)args; // Suppress unused parameter warning
}

void cmd_clear(char* args) {
    terminal_initialize();
    (void)args; // Suppress unused parameter warning
}

void cmd_echo(char* args) {
    terminal_write(args);
    terminal_putchar('\n');
}

void cmd_info(char* args) {
    terminal_write("EXO_OS - A simple 32-bit operating system\n");
    terminal_write("Developed by: UdayKhare09\n");
    terminal_write("Architecture: x86 (32-bit)\n");
    println("Hi test for println %c %d %x %s %%", 'A', -123, 0x7B, "Hello World");
    (void)args; // Suppress unused parameter warning
}

void cmd_reboot(char* args) {
    terminal_write("Rebooting system...\n");

    // Wait for keypress before reboot
    terminal_write("Press any key to continue...\n");
    keyboard_getchar();

    // Use the 8042 keyboard controller to trigger a system reset
    uint8_t temp;
    do {
        temp = inb(KEYBOARD_STATUS_PORT);
        if (temp & 1) inb(KEYBOARD_DATA_PORT); // Empty keyboard buffer
    } while (temp & 2);

    outb(KEYBOARD_STATUS_PORT, 0xFE); // Reset CPU
    (void)args; // Suppress unused parameter warning
}