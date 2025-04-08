//
// Created by uday on 4/7/25.
//

#ifndef STDIO_H
#define STDIO_H

#include "../kernel/terminal.h"


void printf(const char* format, ...);
void println(const char* format, ...);
void int_to_string(int value, char* str, int base);

#endif //STDIO_H
