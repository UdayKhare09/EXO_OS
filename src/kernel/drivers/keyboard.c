#include "keyboard.h"
#include "io.h"

// US keyboard layout map (scancode set 1)
static const char scancode_to_ascii[] = {
    0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b',
    '\t', 'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',
    0, 'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`',
    0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0,
    '*', 0, ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    '-', 0, 0, 0, '+', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

static const char scancode_to_ascii_shift[] = {
    0, 0, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', '\b',
    '\t', 'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n',
    0, 'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~',
    0, '|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M', '<', '>', '?', 0,
    '*', 0, ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    '-', 0, 0, 0, '+', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

// Buffer for keyboard input
#define KEYBOARD_BUFFER_SIZE 256
static char keyboard_buffer[KEYBOARD_BUFFER_SIZE];
static size_t buffer_read_pos = 0;
static size_t buffer_write_pos = 0;
static uint8_t previous_scancode = 0;
static uint8_t key_release_received = 1; // Start with 1 to allow first key press

// Initialize the keyboard
void keyboard_initialize(void) {
    // Reset buffer positions
    buffer_read_pos = 0;
    buffer_write_pos = 0;
    previous_scancode = 0;
    key_release_received = 1;
}

// Get a character from the keyboard buffer (non-blocking)
char keyboard_getchar(void) {
    if (buffer_read_pos == buffer_write_pos) {
        return 0; // No character available
    }

    const char c = keyboard_buffer[buffer_read_pos];
    buffer_read_pos = (buffer_read_pos + 1) % KEYBOARD_BUFFER_SIZE;
    return c;
}

void keyboard_read_line(char* buffer, const size_t max_size) {
    size_t i = 0;
    int scanning = 1;
    int shift_pressed = 0;

    // Clear input buffer
    buffer_read_pos = buffer_write_pos = 0;

    while (scanning && i < max_size - 1) {
        // Wait for a key press
        const uint8_t scancode = inb(KEYBOARD_DATA_PORT);

        // Check if it's a key release (bit 7 set)
        if (scancode & 0x80) {
            if (scancode == 0xAA || scancode == 0xB6) { // Left or Right Shift release
                shift_pressed = 0;
            }
            key_release_received = 1;
            continue;
        }

        // Only process if this is a new keypress or a different key
        if (key_release_received || scancode != previous_scancode) {
            previous_scancode = scancode;
            key_release_received = 0;

            // Check for shift key press
            if (scancode == 0x2A || scancode == 0x36) { // Left or Right Shift press
                shift_pressed = 1;
                continue;
            }

            // Convert scancode to ASCII
            if (scancode < sizeof(scancode_to_ascii)) {
                const char key = shift_pressed ? scancode_to_ascii_shift[scancode] : scancode_to_ascii[scancode];

                // Process valid keys only
                if (key) {
                    if (key == '\b') {  // Backspace
                        if (i > 0) {
                            i--;  // Move back in our buffer
                            terminal_putchar('\b');
                            terminal_putchar(' ');
                            terminal_putchar('\b');
                        }
                    }
                    else if (key == '\n') {  // Enter
                        buffer[i] = '\0';
                        scanning = 0;  // End input loop
                    }
                    else {  // Regular character
                        buffer[i] = key;
                        terminal_putchar(key);
                        i++;
                    }
                }
            }
        }

        // Wait for key release before accepting another keypress
        // This improves keyboard responsiveness
        while ((inb(KEYBOARD_STATUS_PORT) & 1) == 0) {
            // Small delay to avoid hammering the port
            for (volatile int j = 0; j < 1000; j++);
        }
    }

    buffer[i] = '\0';  // Ensure null-termination
}

// Handle keyboard input (to be called by interrupt handler)
void keyboard_handler(void) {
    const uint8_t scancode = inb(KEYBOARD_DATA_PORT);

    // Check if it's a key press (not a key release)
    if (!(scancode & 0x80)) {
        // Convert scancode to ASCII
        if (scancode < sizeof(scancode_to_ascii)) {
            const char c = scancode_to_ascii[scancode];
            if (c) {
                // Add to buffer if there's space
                const size_t next_pos = (buffer_write_pos + 1) % KEYBOARD_BUFFER_SIZE;
                if (next_pos != buffer_read_pos) {
                    keyboard_buffer[buffer_write_pos] = c;
                    buffer_write_pos = next_pos;
                }
            }
        }
    }
}