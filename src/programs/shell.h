#ifndef SHELL_H
#define SHELL_H

// Maximum command line length
#define CMD_MAX_LENGTH 256

// Maximum number of command arguments
#define CMD_MAX_ARGS 16

// Command function pointer type
typedef int (*cmd_function)(int argc, char** argv);

// Command structure
typedef struct {
    const char* name;
    cmd_function function;
    const char* description;
} command_t;

// Shell initialization
void shell_initialize(void);

// Main shell loop
void shell_run(void);

// Register a new command
int shell_register_command(const char* name, cmd_function function, const char* description);

#endif // SHELL_H
