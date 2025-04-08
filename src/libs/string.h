#ifndef STRING_H
#define STRING_H

#include "stddef.h"

int strcmp(const char* str1, const char* str2);
char* strcpy(char* dest, const char* src);
size_t strlen(const char* str);
char* strcat(char* dest, const char* src);

#endif // STRING_H