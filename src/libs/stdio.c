//
// Created by uday on 4/7/25.
//

#include "../kernel/terminal.h"
#include "../kernel/drivers/keyboard.h"

// %c - Character
// %d - Decimal
// %x - Hexadecimal
// %s - String
// %% - Percent sign

// Convert integer to string
void int_to_string(int value, char* str, const int base) {
    // Handle negative numbers
    int i = 0;
    int negative = 0;

    // Handle 0 explicitly
    if (value == 0) {
        str[i++] = '0';
        str[i] = '\0';
        return;
    }

    // Handle negative numbers only for base 10
    if (value < 0 && base == 10) {
        negative = 1;
        value = -value;
    }

    unsigned int temp = value;

    // Process digits
    while (temp != 0) {
        const int remainder = temp % base;
        str[i++] = (remainder < 10) ? remainder + '0' : remainder - 10 + 'a';
        temp = temp / base;
    }

    // Add negative sign if needed
    if (negative) {
        str[i++] = '-';
    }

    // Null terminate string
    str[i] = '\0';

    // Reverse the string
    int start = 0;
    int end = i - 1;
    while (start < end) {
        const char temp1 = str[start];
        str[start] = str[end];
        str[end] = temp1;
        start++;
        end--;
    }
}

// Printf implementation
void printf(const char* format, ...) {
    char buffer[256];
    int i = 0;
    int j = 0;

    __builtin_va_list args;
    __builtin_va_start(args, format);

    while (format[i]) {
        if (format[i] == '%' && format[i+1]) {
            char int_buffer[32];
            i++;
            switch (format[i]) {
                case 'c': {
                    // Character
                    buffer[j++] = (char)__builtin_va_arg(args, int);
                    break;
                }
                case 'd': {
                    // Decimal
                    const int val = __builtin_va_arg(args, int);
                    int_to_string(val, int_buffer, 10);

                    // Copy to main buffer
                    int k = 0;
                    while (int_buffer[k]) {
                        buffer[j++] = int_buffer[k++];
                    }
                    break;
                }
                case 'x': {
                    // Hexadecimal
                    const int val = __builtin_va_arg(args, int);
                    int_to_string(val, int_buffer, 16);

                    // Copy to main buffer
                    int k = 0;
                    while (int_buffer[k]) {
                        buffer[j++] = int_buffer[k++];
                    }
                    break;
                }
                case 's': {
                    // String
                    const char* str = __builtin_va_arg(args, const char*);
                    while (*str) {
                        buffer[j++] = *str++;
                    }
                    break;
                }
                case 'p': {
                    // Pointer
                    const void* ptr = __builtin_va_arg(args, void*);
                    int_to_string((uintptr_t)ptr, int_buffer, 16);

                    // Copy to main buffer
                    int k = 0;
                    while (int_buffer[k]) {
                        buffer[j++] = int_buffer[k++];
                    }
                    break;
                }
                case '%': {
                    // Percent sign
                    buffer[j++] = '%';
                    break;
                }
                default:
                    // Unsupported format specifier
                    buffer[j++] = '%';
                    buffer[j++] = format[i];
                    break;
            }
        } else {
            buffer[j++] = format[i];
        }

        i++;

        // Ensure we don't overflow
        if (j >= 255) {
            buffer[j] = '\0';
            terminal_write(buffer);
            j = 0;
        }
    }

    // Null terminate and print remaining buffer
    buffer[j] = '\0';
    terminal_write(buffer);

    __builtin_va_end(args);
}

// Println implementation (printf with newline)
void println(const char* format, ...) {
    char buffer[256];
    int i = 0;
    int j = 0;

    __builtin_va_list args;
    __builtin_va_start(args, format);

    while (format[i]) {
        if (format[i] == '%' && format[i+1]) {
            char int_buffer[32];
            i++;
            switch (format[i]) {
                case 'c': {
                    // Character
                    buffer[j++] = (char)__builtin_va_arg(args, int);
                    break;
                }
                case 'd': {
                    // Decimal
                    const int val = __builtin_va_arg(args, int);
                    int_to_string(val, int_buffer, 10);

                    // Copy to main buffer
                    int k = 0;
                    while (int_buffer[k]) {
                        buffer[j++] = int_buffer[k++];
                    }
                    break;
                }
                case 'x': {
                    // Hexadecimal
                    const int val = __builtin_va_arg(args, int);
                    int_to_string(val, int_buffer, 16);

                    // Copy to main buffer
                    int k = 0;
                    while (int_buffer[k]) {
                        buffer[j++] = int_buffer[k++];
                    }
                    break;
                }
                case 's': {
                    // String
                    const char* str = __builtin_va_arg(args, const char*);
                    while (*str) {
                        buffer[j++] = *str++;
                    }
                    break;
                }
                case '%': {
                    // Percent sign
                    buffer[j++] = '%';
                    break;
                }
                default:
                    // Unsupported format specifier
                    buffer[j++] = '%';
                    buffer[j++] = format[i];
                    break;
            }
        } else {
            buffer[j++] = format[i];
        }

        i++;

        // Ensure we don't overflow
        if (j >= 255) {
            buffer[j] = '\0';
            terminal_write(buffer);
            j = 0;
        }
    }

    // Null terminate and print remaining buffer
    buffer[j] = '\0';
    terminal_write(buffer);
    terminal_putchar('\n');

    __builtin_va_end(args);
}

void scanf(const char* format, ...) {
    char input_buffer[256];
    keyboard_read_line(input_buffer, sizeof(input_buffer));

    __builtin_va_list args;
    __builtin_va_start(args, format);

    const char* fmt = format;
    char* input = input_buffer;

    while (*fmt) {
        if (*fmt == '%' && *(fmt + 1)) {
            fmt++;
            switch (*fmt) {
                case 'd': { // Integer
                    int* int_ptr = __builtin_va_arg(args, int*);
                    *int_ptr = 0;
                    int sign = 1;

                    if (*input == '-') {
                        sign = -1;
                        input++;
                    }

                    while (*input >= '0' && *input <= '9') {
                        *int_ptr = *int_ptr * 10 + (*input - '0');
                        input++;
                    }
                    *int_ptr *= sign;
                    break;
                }
                case 'c': { // Character
                    char* char_ptr = __builtin_va_arg(args, char*);
                    *char_ptr = *input;
                    input++;
                    break;
                }
                case 's': { // String (read until newline or end of buffer)
                    char* str_ptr = __builtin_va_arg(args, char*);
                    while (*input && *input != '\n') {
                        *str_ptr++ = *input++;
                    }
                    *str_ptr = '\0';
                    break;
                }
                default:
                    break;
            }
        } else if (*fmt == *input) {
            input++;
        }
        fmt++;
    }

    __builtin_va_end(args);
}