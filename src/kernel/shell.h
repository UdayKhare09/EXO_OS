#ifndef SHELL_H
#define SHELL_H

#include "terminal.h"
#include "drivers/keyboard.h"

// Shell constants
#define SHELL_COMMAND_MAX_LEN 256
#define SHELL_PROMPT "EXO_OS> "

// Function declarations
void shell_initialize(void);
void shell_run(void);

#endif // SHELL_H