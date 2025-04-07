// kernel/terminal.c
#include "terminal.h"

// Terminal variables
static size_t terminal_row;
static size_t terminal_column;
static uint8_t terminal_color;
static uint16_t* terminal_buffer;

// Create a color attribute
uint8_t make_color(uint8_t fg, uint8_t bg) {
    return fg | (bg << 4);
}

// Create a VGA entry
uint16_t make_vga_entry(char c, uint8_t color) {
    uint16_t c16 = c;
    uint16_t color16 = color;
    return c16 | (color16 << 8);
}

// Initialize the terminal
void terminal_initialize() {
    terminal_row = 0;
    terminal_column = 0;
    terminal_color = make_color(VGA_COLOR_WHITE, VGA_COLOR_BLUE);
    terminal_buffer = (uint16_t*) VGA_MEMORY;

    // Clear the screen
    for (size_t y = 0; y < VGA_HEIGHT; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            const size_t index = y * VGA_WIDTH + x;
            terminal_buffer[index] = make_vga_entry(' ', terminal_color);
        }
    }
}

// Put a character at a specific position
void terminal_putentryat(char c, uint8_t color, size_t x, size_t y) {
    const size_t index = y * VGA_WIDTH + x;
    terminal_buffer[index] = make_vga_entry(c, color);
}

// Handle newline
void terminal_newline() {
    terminal_column = 0;
    if (++terminal_row == VGA_HEIGHT) {
        terminal_row = 0;  // Simple scrolling - just wrap around for now
    }
}

// Put a character at the current position and advance the cursor
void terminal_putchar(char c) {
    if (c == '\n') {
        terminal_newline();
        return;
    }

    terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
    if (++terminal_column == VGA_WIDTH) {
        terminal_newline();
    }
}

// Write a string to the terminal
void terminal_write(const char* data) {
    while (*data) {
        terminal_putchar(*data++);
    }
}