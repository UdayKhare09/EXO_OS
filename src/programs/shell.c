#include "shell.h"
#include "../libs/stdio.h"
#include "../libs/string.h"
#include "../kernel/drivers/keyboard.h"

// Command registry
#define MAX_COMMANDS 32
static command_t command_registry[MAX_COMMANDS];
static int command_count = 0;

// Built-in commands
static int cmd_help(int argc, char** argv);
static int cmd_echo(int argc, char** argv);
static int cmd_clear(int argc, char** argv);
static int cmd_test(int argc, char** argv);

// Initialize the shell
void shell_initialize(void) {
    // Register built-in commands
    shell_register_command("help", cmd_help, "Display help information");
    shell_register_command("echo", cmd_echo, "Echo text to the terminal");
    shell_register_command("clear", cmd_clear, "Clear the terminal");
    shell_register_command("test", cmd_test, "Run test program");
    
    printf("EXO Shell v0.1\n");
}

// Register a command with the shell
int shell_register_command(const char* name, const cmd_function function, const char* description) {
    if (command_count >= MAX_COMMANDS) {
        return -1; // Command registry full
    }
    
    command_registry[command_count].name = name;
    command_registry[command_count].function = function;
    command_registry[command_count].description = description;
    command_count++;
    
    return 0;
}

// Parse command line into arguments
int parse_command(char* cmdline, char** argv) {
    int argc = 0;
    char* token;
    char* rest = cmdline;
    
    // Extract tokens separated by spaces
    while (((token = strtok_r(rest, " ", &rest))) && argc < CMD_MAX_ARGS) {
        argv[argc++] = token;
    }
    
    argv[argc] = NULL; // NULL-terminate the argument array
    return argc;
}

// Execute a command
int execute_command(const int argc, char** argv) {
    if (argc == 0 || argv[0][0] == '\0') {
        return 0; // Empty command
    }
    
    // Search for command in registry
    for (int i = 0; i < command_count; i++) {
        if (strcmp(argv[0], command_registry[i].name) == 0) {
            return command_registry[i].function(argc, argv);
        }
    }
    
    // Command not found
    printf("Unknown command: %s, type help\n", argv[0]);
    return -1;
}

// Main shell loop
void shell_run(void) {
    while (1) {
        char* argv[CMD_MAX_ARGS + 1];
        char cmdline[CMD_MAX_LENGTH];
        // Display prompt
        printf("EXO> ");
        
        // Read command line
        keyboard_read_line(cmdline, CMD_MAX_LENGTH);
        printf("\n");
        
        // Parse and execute command
        const int argc = parse_command(cmdline, argv);
        execute_command(argc, argv);
    }
}

// Built-in command implementations

// Display help information
static int cmd_help(int argc, char** argv) {
    if (argc > 1) {
        // Help for specific command
        for (int i = 0; i < command_count; i++) {
            if (strcmp(argv[1], command_registry[i].name) == 0) {
                printf("%s - %s\n", command_registry[i].name, command_registry[i].description);
                return 0;
            }
        }
        printf("No help available for '%s'\n", argv[1]);
    } else {
        // List all commands
        printf("Available commands:\n");
        for (int i = 0; i < command_count; i++) {
            printf("  %s - %s\n", command_registry[i].name, command_registry[i].description);
        }
    }
    return 0;
}

// Echo command
static int cmd_echo(const int argc, char** argv) {
    for (int i = 1; i < argc; i++) {
        printf("%s ", argv[i]);
    }
    printf("\n");
    return 0;
}

// Clear terminal
static int cmd_clear(int argc, char** argv) {
    terminal_initialize();
    return 0;
}

// Test program
static int cmd_test(int argc, char** argv) {
    extern void test(void);
    test();
    return 0;
}

// Helper function to split a string by delimiter (strtok replacement)
// The function saves the state between calls in 'saveptr'
char* strtok_r(char* str, const char* delim, char** saveptr) {
    if (str == NULL) {
        str = *saveptr;
    }
    
    // Skip leading delimiters
    str += strspn(str, delim);
    
    if (*str == '\0') {
        *saveptr = str;
        return NULL;
    }
    
    char *token = str;
    
    // Find end of token
    str = strpbrk(str, delim);
    if (str == NULL) {
        *saveptr = strchr(token, '\0');
    } else {
        *str = '\0';
        *saveptr = str + 1;
    }
    
    return token;
}