// kernel/keyboard.h
#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "terminal.h"

// IO port definitions
#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64

// Function declarations
void keyboard_initialize(void);
void keyboard_handler(void);
char keyboard_getchar(void);
void keyboard_read_line(char* buffer, size_t max_size);

#endif // KEYBOARD_H