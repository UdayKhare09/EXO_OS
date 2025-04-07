#ifndef TERMINAL_H
#define TERMINAL_H

// Define standard types
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int size_t;
// define NULL
#define NULL ((void*)0)

// Constants for VGA text mode
#define VGA_MEMORY 0xB8000
#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_COLOR_BLACK 0
#define VGA_COLOR_LIGHT_GREY 7
#define VGA_COLOR_WHITE 15
#define VGA_COLOR_BLUE 1

// Function declarations
uint8_t make_color(uint8_t fg, uint8_t bg);
uint16_t make_vga_entry(char c, uint8_t color);
void terminal_initialize(void);
void terminal_putentryat(char c, uint8_t color, size_t x, size_t y);
void terminal_newline(void);
void terminal_putchar(char c);
void terminal_write(const char* data);

#endif // TERMINAL_H